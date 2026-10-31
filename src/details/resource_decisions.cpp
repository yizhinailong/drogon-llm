/**
 * @file resource_decisions.cpp
 * @brief Implement decisions resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/decisions.hpp"

namespace drogon_openai::types::decisions {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto Decision::FromJson(Json::Value const& value) -> Result<Decision> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        Decision result;
        result.raw = value;
        if (value.isMember("answers")) {
            if (!details::resource_read(value["answers"], result.answers)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Decision.answers", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("model")) {
            if (!details::resource_read(value["model"], result.model)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Decision.model", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("usage")) {
            if (!details::resource_read(value["usage"], result.usage)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Decision.usage", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto DecisionCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["input"] = details::resource_json(input);
        payload.body["model"] = details::resource_json(model);
        payload.body["questions"] = details::resource_json(questions);
        if (prompt_cache_key) {
            payload.body["prompt_cache_key"] = details::resource_json(*prompt_cache_key);
        }
        if (prompt_cache_options) {
            payload.body["prompt_cache_options"] = details::resource_json(*prompt_cache_options);
        }
        if (safety_identifier) {
            payload.body["safety_identifier"] = details::resource_json(*safety_identifier);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto DecisionCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::decisions

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Decisions::Decisions(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /decisions.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Decisions::Create(
        types::decisions::DecisionCreateParams params
    ) const -> drogon::Task<Result<types::decisions::Decision>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/decisions", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::decisions::Decision>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /decisions.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Decisions::CreateRaw(
        types::decisions::DecisionCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::decisions::Decision>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/decisions", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::decisions::Decision>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }
} // namespace drogon_openai::details
