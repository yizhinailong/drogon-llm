/**
 * @file resource_safety.cpp
 * @brief Implement safety resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/safety.hpp"

namespace drogon_openai::types::safety {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto SafetyAlert::FromJson(Json::Value const& value) -> Result<SafetyAlert> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        SafetyAlert result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyAlert.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyAlert.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("error_type")) {
            if (!details::resource_read(value["error_type"], result.error_type)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyAlert.error_type", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("model")) {
            if (!details::resource_read(value["model"], result.model)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyAlert.model", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyAlert.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("reason") && !value["reason"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["reason"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyAlert.reason", .body = value.toStyledString() } };
            }
            result.reason = std::move(field);
        }
        if (value.isMember("request_id")) {
            if (!details::resource_read(value["request_id"], result.request_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyAlert.request_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("request_paused")) {
            if (!details::resource_read(value["request_paused"], result.request_paused)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyAlert.request_paused", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("response_id")) {
            if (!details::resource_read(value["response_id"], result.response_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyAlert.response_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("detailed_explanation") && !value["detailed_explanation"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["detailed_explanation"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyAlert.detailed_explanation", .body = value.toStyledString() } };
            }
            result.detailed_explanation = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto SafetyCase::FromJson(Json::Value const& value) -> Result<SafetyCase> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        SafetyCase result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyCase.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyCase.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("entity_identifier")) {
            if (!details::resource_read(value["entity_identifier"], result.entity_identifier)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyCase.entity_identifier", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("notice")) {
            if (!details::resource_read(value["notice"], result.notice)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyCase.notice", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyCase.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("reason") && !value["reason"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["reason"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SafetyCase.reason", .body = value.toStyledString() } };
            }
            result.reason = std::move(field);
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto SafetyAlertsRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto SafetyAlertsRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto SafetyCasesRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto SafetyCasesRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::safety

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    SafetyAlerts::SafetyAlerts(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief GET /safety/alerts/{id}.
     * @param id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto SafetyAlerts::Retrieve(
        std::string id,
        types::safety::SafetyAlertsRetrieveParams params
    ) const -> drogon::Task<Result<types::safety::SafetyAlert>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/safety/alerts/{id}", { { "id", id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::safety::SafetyAlert>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /safety/alerts/{id}.
     * @param id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto SafetyAlerts::RetrieveRaw(
        std::string id,
        types::safety::SafetyAlertsRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::safety::SafetyAlert>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/safety/alerts/{id}", { { "id", id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::safety::SafetyAlert>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    SafetyCases::SafetyCases(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief GET /safety/cases/{id}.
     * @param id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto SafetyCases::Retrieve(
        std::string id,
        types::safety::SafetyCasesRetrieveParams params
    ) const -> drogon::Task<Result<types::safety::SafetyCase>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/safety/cases/{id}", { { "id", id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::safety::SafetyCase>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /safety/cases/{id}.
     * @param id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto SafetyCases::RetrieveRaw(
        std::string id,
        types::safety::SafetyCasesRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::safety::SafetyCase>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/safety/cases/{id}", { { "id", id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::safety::SafetyCase>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Safety::Safety(std::shared_ptr<Transport> transport) : m_transport{ transport }, cases{ transport }, alerts{ transport } {}
} // namespace drogon_openai::details
