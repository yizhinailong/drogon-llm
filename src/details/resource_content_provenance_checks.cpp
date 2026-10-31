/**
 * @file resource_content_provenance_checks.cpp
 * @brief Implement content_provenance_checks resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/content_provenance_checks.hpp"

namespace drogon_openai::types::content_provenance_checks {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto ContentProvenanceCheck::FromJson(Json::Value const& value) -> Result<ContentProvenanceCheck> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        ContentProvenanceCheck result;
        result.raw = value;
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContentProvenanceCheck.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContentProvenanceCheck.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("results")) {
            if (!details::resource_read(value["results"], result.results)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContentProvenanceCheck.results", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ContentProvenanceCheckCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        details::resource_file(payload, "file", file);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ContentProvenanceCheckCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::content_provenance_checks

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    ContentProvenanceChecks::ContentProvenanceChecks(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /content_provenance_checks.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ContentProvenanceChecks::Create(
        types::content_provenance_checks::ContentProvenanceCheckCreateParams params
    ) const -> drogon::Task<Result<types::content_provenance_checks::ContentProvenanceCheck>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/content_provenance_checks", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::content_provenance_checks::ContentProvenanceCheck>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /content_provenance_checks.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ContentProvenanceChecks::CreateRaw(
        types::content_provenance_checks::ContentProvenanceCheckCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::content_provenance_checks::ContentProvenanceCheck>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/content_provenance_checks", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::content_provenance_checks::ContentProvenanceCheck>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }
} // namespace drogon_openai::details
