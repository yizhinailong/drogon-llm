/**
 * @file resource_moderations.cpp
 * @brief Implement moderations resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/moderations.hpp"

namespace drogon_openai::types::moderations {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto ModerationCreateResponse::FromJson(Json::Value const& value) -> Result<ModerationCreateResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        ModerationCreateResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ModerationCreateResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("model")) {
            if (!details::resource_read(value["model"], result.model)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ModerationCreateResponse.model", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("results")) {
            if (!details::resource_read(value["results"], result.results)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ModerationCreateResponse.results", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ModerationCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["input"] = details::resource_json(input);
        if (model) {
            payload.body["model"] = details::resource_json(*model);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ModerationCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::moderations

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Moderations::Moderations(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /moderations.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Moderations::Create(
        types::moderations::ModerationCreateParams params
    ) const -> drogon::Task<Result<types::moderations::ModerationCreateResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/moderations", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::moderations::ModerationCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /moderations.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Moderations::CreateRaw(
        types::moderations::ModerationCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::moderations::ModerationCreateResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/moderations", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::moderations::ModerationCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }
} // namespace drogon_openai::details
