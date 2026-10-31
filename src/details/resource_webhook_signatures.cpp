/**
 * @file resource_webhook_signatures.cpp
 * @brief Authenticate webhook payloads with HMAC-SHA256 and a bounded replay window.
 */
#include <array>
#include <cctype>
#include <charconv>
#include <chrono>
#include <climits>
#include <cstdint>
#include <exception>
#include <sstream>
#include <utility>

#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>

#include "details/transport.hpp"
#include "drogon_openai/details/webhooks.hpp"

namespace drogon_openai::details {
    namespace {
        /**
         * @brief Find a header without depending on its original casing.
         * @param headers Original webhook headers.
         * @param name Lowercase header name.
         * @return Header value or an empty string.
         */
        auto webhook_header(Headers const& headers, std::string const& name) -> std::string {
            for (auto const& [key, value] : headers) {
                std::string normalized{ key };
                for (auto& c : normalized) {
                    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                }
                if (normalized == name) {
                    return value;
                }
            }
            return {};
        }

        /**
         * @brief Decode a signing key encoded using standard base64.
         * @param text Key after the whsec_ prefix.
         * @return Decoded key or invalid-signature error.
         */
        auto decode_signing_key(std::string const& text) -> Result<std::string> {
            if (text.empty() || text.size() % 4 != 0 || text.size() > INT_MAX) {
                return std::unexpected{ Error{ .kind = ErrorKind::InvalidWebhookSignature, .message = "invalid base64 webhook secret" } };
            }
            auto const padding{ text.ends_with("==") ? 2u : text.ends_with('=') ? 1u :
                                                                                  0u };
            for (std::size_t i{}; i < text.size() - padding; ++i) {
                unsigned char const c{ static_cast<unsigned char>(text[i]) };
                if (!std::isalnum(c) && c != '+' && c != '/') {
                    return std::unexpected{ Error{ .kind = ErrorKind::InvalidWebhookSignature, .message = "invalid base64 webhook secret" } };
                }
            }
            std::string bytes(text.size() / 4 * 3, '\0');
            int const count{ EVP_DecodeBlock(reinterpret_cast<unsigned char*>(bytes.data()), reinterpret_cast<unsigned char const*>(text.data()), static_cast<int>(text.size())) };
            if (count < 0) {
                return std::unexpected{ Error{ .kind = ErrorKind::InvalidWebhookSignature, .message = "invalid base64 webhook secret" } };
            }
            bytes.resize(static_cast<std::size_t>(count) - padding);
            return bytes;
        }
    } // namespace

    /**
     * @brief Verify the webhook HMAC and replay window before consuming its body.
     * @param payload Owned operation parameters.
     * @param headers Owned operation parameters.
     * @param secret Owned operation parameters.
     * @param tolerance Owned operation parameters.
     * @return Operation result or an error.
     */
    auto Webhooks::VerifySignature(
        std::string payload,
        Headers headers,
        std::optional<std::string> secret,
        int tolerance
    ) const -> Result<void> {

        auto fail{
            [](std::string message) -> Result<void> {
                return std::unexpected{ Error{ .kind = ErrorKind::InvalidWebhookSignature, .message = std::move(message) } };
            }
        };
        try {
            auto key{ secret.value_or(m_transport->Config().webhook_secret) };
            if (key.empty()) {
                return fail("webhook signing secret is empty");
            }
            if (tolerance < 0) {
                return fail("webhook tolerance must be nonnegative");
            }
            auto const id{ webhook_header(headers, "webhook-id") };
            auto const timestamp{ webhook_header(headers, "webhook-timestamp") };
            auto const supplied{ webhook_header(headers, "webhook-signature") };
            if (id.empty() || timestamp.empty() || supplied.empty()) {
                return fail("required webhook headers are missing");
            }
            std::int64_t seconds{};
            auto const parsed{ std::from_chars(timestamp.data(), timestamp.data() + timestamp.size(), seconds) };
            if (parsed.ec != std::errc{} || parsed.ptr != timestamp.data() + timestamp.size()) {
                return fail("invalid webhook timestamp");
            }
            auto const now{ std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() };
            if (seconds < now - tolerance || seconds > now + tolerance) {
                return fail("webhook timestamp is outside the replay window");
            }
            if (key.starts_with("whsec_")) {
                auto decoded{ decode_signing_key(key.substr(6)) };
                if (!decoded) {
                    return std::unexpected{ std::move(decoded.error()) };
                }
                key = std::move(*decoded);
            }
            if (key.size() > INT_MAX) {
                return fail("webhook signing secret is too long");
            }
            auto const message{ id + "." + timestamp + "." + payload };
            std::array<unsigned char, EVP_MAX_MD_SIZE> digest{};
            unsigned int size{};
            if (HMAC(EVP_sha256(), key.data(), static_cast<int>(key.size()), reinterpret_cast<unsigned char const*>(message.data()), message.size(), digest.data(), &size) == nullptr) {
                return fail("could not compute webhook signature");
            }
            std::string expected(4 * ((size + 2) / 3), '\0');
            EVP_EncodeBlock(reinterpret_cast<unsigned char*>(expected.data()), digest.data(), static_cast<int>(size));
            std::istringstream signatures{ supplied };
            std::string candidate;
            bool matches{};
            while (signatures >> candidate) {
                if (candidate.starts_with("v1,")) {
                    candidate.erase(0, 3);
                }
                if (candidate.size() == expected.size()) {
                    matches = (CRYPTO_memcmp(candidate.data(), expected.data(), expected.size()) == 0) || matches;
                }
            }
            return matches ? Result<void>{} : fail("webhook signature does not match");
        } catch (std::exception const& error) {
            return fail(error.what());
        } catch (...) {
            return fail("webhook verification failed");
        }
    }

    /**
     * @brief Verify a webhook signature and decode the original JSON event.
     * @param payload Owned operation parameters.
     * @param headers Owned operation parameters.
     * @param secret Owned operation parameters.
     * @param tolerance Owned operation parameters.
     * @return Operation result or an error.
     */
    auto Webhooks::Unwrap(
        std::string payload,
        Headers headers,
        std::optional<std::string> secret,
        int tolerance
    ) const -> Result<Json::Value> {

        auto valid{ VerifySignature(payload, std::move(headers), std::move(secret), tolerance) };
        if (!valid) {
            return std::unexpected{ std::move(valid.error()) };
        }
        auto parsed{ decode_json(payload) };
        if (!parsed) {
            return parsed;
        }
        if (!parsed->isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "webhook event is not a JSON object" } };
        }
        return parsed;
    }
} // namespace drogon_openai::details
