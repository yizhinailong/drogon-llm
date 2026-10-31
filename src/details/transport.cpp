/**
 * @file transport.cpp
 * @brief Implement request defaults, retry policy, and JSON/error metadata decoding.
 */
#include "details/transport.hpp"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <charconv>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <random>
#include <utility>

#include <drogon/HttpAppFramework.h>
#include <drogon/utils/Utilities.h>
#include <json/reader.h>
#include <json/writer.h>

namespace drogon_openai::details {

    namespace {

        /**
         * @brief Merge headers using case-insensitive names.
         * @param destination Lowercase headers.
         * @param source Headers overriding existing values.
         */
        auto merge_headers(Headers& destination, Headers const& source) -> void {
            for (auto const& [name, value] : source) {
                std::string key{ name };
                std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
                destination[key] = value;
            }
        }

        /**
         * @brief Serialize an individual query value.
         * @param value Query scalar.
         * @return Scalar text; null becomes an empty value.
         */
        auto query_scalar(Json::Value const& value) -> std::string {
            if (value.isNull()) {
                return {};
            }
            return value.asString();
        }

        /**
         * @brief Append nested query values in the SDK's brackets format.
         * @param target Path and existing query.
         * @param name Query key.
         * @param value Scalar, list, or object.
         */
        auto append_query(std::string& target, std::string const& name, Json::Value const& value) -> void {
            if (value.isArray()) {
                for (auto const& item : value) {
                    append_query(target, name + "[]", item);
                }
            } else if (value.isObject()) {
                for (auto const& key : value.getMemberNames()) {
                    append_query(target, name + "[" + key + "]", value[key]);
                }
            } else {
                target += target.find('?') == std::string::npos ? '?' : '&';
                target += encode_path_segment(name) + "=" + encode_path_segment(query_scalar(value));
            }
        }

        /**
         * @brief Parse server-directed retry delays in milliseconds, seconds, or HTTP date form.
         * @param headers Response headers.
         * @return Requested delay when recognized.
         */
        auto retry_after(Headers const& headers) -> std::optional<double> {
            for (auto const& [name, divisor] : { std::pair{ "retry-after-ms", 1000.0 }, std::pair{ "retry-after", 1.0 } }) {
                auto const it{ headers.find(name) };
                if (it == headers.end()) {
                    continue;
                }
                auto const& text{ it->second };
                char* end{};
                errno = 0;
                double const value{ std::strtod(text.c_str(), &end) };
                if (end != text.c_str() && *end == '\0') {
                    if (errno == ERANGE && value > 0) {
                        return 121.0;
                    }
                    return value / divisor;
                }
            }
            if (auto const it{ headers.find("retry-after") }; it != headers.end()) {
                auto const date{ drogon::utils::getHttpDate(it->second) };
                if (date.microSecondsSinceEpoch() > 0 && date.microSecondsSinceEpoch() != std::numeric_limits<std::int64_t>::max()) {
                    return static_cast<double>(date.microSecondsSinceEpoch() - trantor::Date::now().microSecondsSinceEpoch()) / 1e6;
                }
            }
            return {};
        }

        /**
         * @brief Apply the SDK status retry policy, including server overrides.
         * @param status HTTP status.
         * @param headers Response headers.
         * @return Whether the response is retryable.
         */
        auto should_retry(int status, Headers const& headers) -> bool {
            auto const delay{ retry_after(headers) };
            if (delay && std::isfinite(*delay) && *delay > 120) {
                return false;
            }
            if (auto const it{ headers.find("x-should-retry") }; it != headers.end()) {
                if (it->second == "true") {
                    return true;
                }
                if (it->second == "false") {
                    return false;
                }
            }
            return status == 408 || status == 409 || status == 429 || status >= 500;
        }

        /**
         * @brief Calculate bounded exponential backoff with jitter.
         * @param attempt Number of earlier retries.
         * @param headers Response headers when available.
         * @return Delay in seconds.
         */
        auto retry_delay(int attempt, Headers const& headers) -> double {
            if (auto delay{ retry_after(headers) }; delay && std::isfinite(*delay) && *delay > 0 && *delay <= 120) {
                return *delay;
            }
            thread_local std::mt19937 generator{ std::random_device{}() };
            auto const jitter{ std::uniform_real_distribution<double>{ 0.75, 1.0 }(generator) };
            return std::min(0.5 * std::pow(2.0, std::min(attempt, 4)), 8.0) * jitter;
        }

        /**
         * @brief Decode an API status error without discarding response metadata.
         * @param response Unsuccessful HTTP response.
         * @return Structured status error.
         */
        auto status_error(RawResponse<std::string> const& response) -> Error {
            Error error{
                .kind = ErrorKind::Status,
                .message = response.body,
                .status_code = response.status_code,
                .body = response.body,
                .request_id = response.request_id,
                .headers = response.headers,
            };
            auto root{ decode_json(response.body) };
            if (root && root->isObject()) {
                auto const& item{ (*root)["error"] };
                if (item.isObject()) {
                    if (item["message"].isString()) {
                        error.message = item["message"].asString();
                    }
                    if (item["code"].isNumeric() || item["code"].isBool()) {
                        error.code = item["code"].asString();
                    }
                    for (auto const& [name, field] : { std::pair{ "type", &error.type }, { "code", &error.code }, { "param", &error.param } }) {
                        if (item[name].isString()) {
                            *field = item[name].asString();
                        }
                    }
                } else if (item.isString()) {
                    error.message = item.asString();
                }
            }
            error.message = "HTTP " + std::to_string(response.status_code) + ": " + error.message;
            return error;
        }

        /**
         * @brief Read the OPENAI_LOG setting without exposing environment contents.
         * @return Whether request and retry logging is enabled.
         */
        auto logging_enabled() -> bool {
#ifdef _WIN32
            char* raw{};
            std::size_t size{};
            if (_dupenv_s(&raw, &size, "OPENAI_LOG") != 0 || raw == nullptr) {
                return false;
            }
            std::unique_ptr<char, decltype(&std::free)> owner{ raw, &std::free };
            std::string const value{ raw };
#else
            char const* raw{ std::getenv("OPENAI_LOG") };
            std::string const value{ raw != nullptr ? raw : "" };
#endif
            return value == "debug" || value == "info";
        }

        /**
         * @brief Validate a phase timeout.
         * @param value Timeout in seconds.
         * @return Whether it is a finite, nonnegative duration.
         */
        auto valid_timeout(double value) -> bool {
            return std::isfinite(value) && value >= 0;
        }

    } // namespace

    /**
     * @brief Find the calling event loop, falling back to the application's main loop.
     * @return Loop used for requests and asynchronous delays.
     */
    auto current_loop() -> trantor::EventLoop* {
        auto* loop{ trantor::EventLoop::getEventLoopOfCurrentThread() };
        return loop != nullptr ? loop : drogon::app().getLoop();
    }

    /**
     * @brief Encode a single path segment or query component using UTF-8 percent escapes.
     * @param value Unescaped component.
     * @return Encoded component.
     */
    auto encode_path_segment(std::string const& value) -> std::string {
        constexpr char HEX[]{ "0123456789ABCDEF" };
        std::string result;
        for (unsigned char c : value) {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~') {
                result += static_cast<char>(c);
            } else {
                result += '%';
                result += HEX[c >> 4];
                result += HEX[c & 15];
            }
        }
        return result;
    }

    /**
     * @brief Decode JSON without logging response bodies.
     * @param body Response bytes; empty means null.
     * @return Parsed JSON or a response validation error.
     */
    auto decode_json(std::string const& body) -> Result<Json::Value> {
        if (body.empty()) {
            return Json::Value{};
        }
        Json::CharReaderBuilder builder;
        builder["collectComments"] = false;
        builder["allowComments"] = false;
        builder["allowTrailingCommas"] = false;
        builder["strictRoot"] = false;
        builder["failIfExtra"] = true;
        std::unique_ptr<Json::CharReader> reader{ builder.newCharReader() };
        Json::Value value;
        std::string errors;
        if (!reader->parse(body.data(), body.data() + body.size(), &value, &errors)) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "response body is not JSON: " + errors, .body = body } };
        }
        return value;
    }

    /**
     * @brief Store immutable client settings.
     * @param config Endpoint settings.
     */
    Transport::Transport(OpenAIConfig config) : m_config{ std::move(config) } {}

    /**
     * @brief Return the endpoint settings.
     * @return Settings passed to the constructor.
     */
    auto Transport::Config() const noexcept -> OpenAIConfig const& {
        return m_config;
    }

    /**
     * @brief Send JSON and retain response metadata.
     * @param method HTTP method.
     * @param path Path relative to the base URL.
     * @param body JSON body; null omits it.
     * @param options Request overrides.
     * @param deadline Optional total deadline across retries and phases.
     * @return Parsed JSON with headers, or an error.
     */
    auto Transport::Request(
        std::string method,
        std::string path,
        Json::Value body,
        RequestOptions options,
        std::optional<std::chrono::steady_clock::time_point> deadline
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        Json::StreamWriterBuilder writer;
        writer["indentation"] = "";
        auto response{ co_await Send(std::move(method), std::move(path), body.isNull() ? "" : Json::writeString(writer, body), body.isNull() ? "" : "application/json", std::move(options), {}, deadline) };
        if (!response) {
            co_return std::unexpected{ response.error() };
        }
        auto parsed{ decode_json(response->body) };
        if (!parsed) {
            auto error{ parsed.error() };
            error.request_id = response->request_id;
            error.headers = response->headers;
            error.status_code = response->status_code;
            co_return std::unexpected{ std::move(error) };
        }
        co_return RawResponse<Json::Value>{ response->status_code, std::move(response->headers), std::move(response->body), std::move(*parsed), std::move(response->request_id) };
    }

    /**
     * @brief Send bytes with retries before successful streaming body delivery begins.
     * @param method HTTP method.
     * @param path Path relative to the base URL.
     * @param body Owned request bytes.
     * @param content_type Request media type; empty omits it.
     * @param options Request overrides.
     * @param on_data Optional incremental response consumer.
     * @param deadline Optional total deadline across retries and phases.
     * @return Response bytes and metadata, or an error.
     */
    auto Transport::Send(
        std::string method,
        std::string path,
        std::string body,
        std::string content_type,
        RequestOptions options,
        BodyCallback on_data,
        std::optional<std::chrono::steady_clock::time_point> deadline
    ) const -> drogon::Task<Result<RawResponse<std::string>>> {
        auto endpoint{ parse_endpoint(m_config.base_url) };
        if (!endpoint) {
            co_return std::unexpected{ endpoint.error() };
        }
        int const retries{ options.max_retries.value_or(m_config.max_retries) };
        auto const timeout{ options.timeout.value_or(m_config.timeout) };
        if (retries < 0 || !valid_timeout(timeout.connect) || !valid_timeout(timeout.read) || !valid_timeout(timeout.write)) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "max_retries and timeouts must be nonnegative and timeouts finite" } };
        }
        static std::vector<std::string> const METHODS{ "GET", "POST", "PUT", "PATCH", "DELETE", "HEAD", "OPTIONS" };
        if (std::find(METHODS.begin(), METHODS.end(), method) == METHODS.end() || path.find_first_of("\r\n #") != std::string::npos || path.starts_with("http:") || path.starts_with("https:") || path.starts_with("//")) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "invalid HTTP method or relative request path" } };
        }
        Headers headers{ { "accept", "application/json" } };
        if (!content_type.empty()) {
            headers["content-type"] = std::move(content_type);
        }
        if (!m_config.api_key.empty()) {
            headers["authorization"] = "Bearer " + m_config.api_key;
        }
        if (!m_config.organization.empty()) {
            headers["openai-organization"] = m_config.organization;
        }
        if (!m_config.project.empty()) {
            headers["openai-project"] = m_config.project;
        }
        merge_headers(headers, m_config.default_headers);
        merge_headers(headers, options.extra_headers);
        for (auto const& [name, value] : headers) {
            if (name.empty() || name.find_first_not_of("!#$%&'*+-.^_`|~0123456789abcdefghijklmnopqrstuvwxyz") != std::string::npos || value.find_first_of("\r\n") != std::string::npos || value.find('\0') != std::string::npos) {
                co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "invalid HTTP header" } };
            }
        }
        Json::Value query{ m_config.default_query };
        if ((!query.isNull() && !query.isObject()) || (!options.extra_query.isNull() && !options.extra_query.isObject())) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "query parameters must be a JSON object" } };
        }
        for (auto const& key : options.extra_query.getMemberNames()) {
            query[key] = options.extra_query[key];
        }
        std::string target{ endpoint->prefix + (path.starts_with('/') ? "" : "/") + path };
        for (auto const& key : query.getMemberNames()) {
            append_query(target, key, query[key]);
        }
        bool started{};
        BodyCallback consumer;
        if (on_data) {
            consumer = [&started, &on_data](std::string_view bytes, Headers const& fields) -> Result<bool> {
                started = true;
                try {
                    return on_data(bytes, fields);
                } catch (std::exception const& exception) {
                    return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = std::string{ "stream callback failed: " } + exception.what() } };
                } catch (...) {
                    return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "stream callback failed" } };
                }
            };
        }
        for (int attempt{};; ++attempt) {
            if (deadline && std::chrono::steady_clock::now() >= *deadline) {
                co_return std::unexpected{ Error{ .kind = ErrorKind::Timeout, .message = "polling deadline exceeded" } };
            }
            if (logging_enabled()) {
                LOG_INFO << "OpenAI " << method << " " << endpoint->host << " attempt " << attempt + 1;
            }
            auto attempt_headers{ headers };
            if (!attempt_headers.contains("x-stainless-retry-count")) {
                attempt_headers["x-stainless-retry-count"] = std::to_string(attempt);
            }
            auto result{ co_await exchange(WireRequest{ *endpoint, method, target, std::move(attempt_headers), body, timeout, consumer, deadline }) };
            bool retryable{};
            Headers fields;
            if (result) {
                if (result->status_code >= 200 && result->status_code < 300) {
                    co_return result;
                }
                fields = result->headers;
                retryable = should_retry(result->status_code, fields);
                result = std::unexpected{ status_error(*result) };
            } else {
                retryable = result.error().kind == ErrorKind::Connection || result.error().kind == ErrorKind::Timeout;
            }
            if (started || !retryable || attempt >= retries) {
                co_return result;
            }
            double delay{ retry_delay(attempt, fields) };
            if (deadline) {
                delay = std::min(delay, std::max(0.0, std::chrono::duration<double>(*deadline - std::chrono::steady_clock::now()).count()));
            }
            if (logging_enabled()) {
                LOG_INFO << "OpenAI retry " << attempt + 1 << " after " << delay << " seconds";
            }
            co_await drogon::sleepCoro(current_loop(), delay);
        }
    }

} // namespace drogon_openai::details
