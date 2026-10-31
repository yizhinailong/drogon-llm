/**
 * @file resource_webhooks.cpp
 * @brief Implement webhooks resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/webhooks.hpp"

namespace drogon_openai::types::webhooks {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto WebhookEventTypeList::FromJson(Json::Value const& value) -> Result<WebhookEventTypeList> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        WebhookEventTypeList result;
        result.raw = value;
        if (value.isMember("data")) {
            if (!details::resource_read(value["data"], result.data)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEventTypeList.data", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEventTypeList.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto WebhookEndpointWithSecret::FromJson(Json::Value const& value) -> Result<WebhookEndpointWithSecret> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        WebhookEndpointWithSecret result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpointWithSecret.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpointWithSecret.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("event_types")) {
            if (!details::resource_read(value["event_types"], result.event_types)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpointWithSecret.event_types", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpointWithSecret.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpointWithSecret.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("signing_secret")) {
            if (!details::resource_read(value["signing_secret"], result.signing_secret)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpointWithSecret.signing_secret", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("signing_secret_hint") && !value["signing_secret_hint"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["signing_secret_hint"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpointWithSecret.signing_secret_hint", .body = value.toStyledString() } };
            }
            result.signing_secret_hint = std::move(field);
        }
        if (value.isMember("url")) {
            if (!details::resource_read(value["url"], result.url)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpointWithSecret.url", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("updated_at") && !value["updated_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["updated_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpointWithSecret.updated_at", .body = value.toStyledString() } };
            }
            result.updated_at = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto WebhookEndpoint::FromJson(Json::Value const& value) -> Result<WebhookEndpoint> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        WebhookEndpoint result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpoint.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpoint.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("event_types")) {
            if (!details::resource_read(value["event_types"], result.event_types)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpoint.event_types", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpoint.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpoint.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("signing_secret_hint") && !value["signing_secret_hint"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["signing_secret_hint"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpoint.signing_secret_hint", .body = value.toStyledString() } };
            }
            result.signing_secret_hint = std::move(field);
        }
        if (value.isMember("url")) {
            if (!details::resource_read(value["url"], result.url)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpoint.url", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("updated_at") && !value["updated_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["updated_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpoint.updated_at", .body = value.toStyledString() } };
            }
            result.updated_at = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto DeletedWebhookEndpoint::FromJson(Json::Value const& value) -> Result<DeletedWebhookEndpoint> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        DeletedWebhookEndpoint result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid DeletedWebhookEndpoint.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("deleted")) {
            if (!details::resource_read(value["deleted"], result.deleted)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid DeletedWebhookEndpoint.deleted", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid DeletedWebhookEndpoint.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto WebhookEndpointTestResult::FromJson(Json::Value const& value) -> Result<WebhookEndpointTestResult> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        WebhookEndpointTestResult result;
        result.raw = value;
        if (value.isMember("event_type")) {
            if (!details::resource_read(value["event_type"], result.event_type)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpointTestResult.event_type", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpointTestResult.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status_code")) {
            if (!details::resource_read(value["status_code"], result.status_code)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpointTestResult.status_code", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("success")) {
            if (!details::resource_read(value["success"], result.success)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpointTestResult.success", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("webhook_endpoint_id")) {
            if (!details::resource_read(value["webhook_endpoint_id"], result.webhook_endpoint_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid WebhookEndpointTestResult.webhook_endpoint_id", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto WebhooksEventTypesListParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto WebhooksEventTypesListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto WebhookCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["event_types"] = details::resource_json(event_types);
        payload.body["name"] = details::resource_json(name);
        payload.body["url"] = details::resource_json(url);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto WebhookCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto WebhooksRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto WebhooksRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto WebhookUpdateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (event_types) {
            payload.body["event_types"] = details::resource_json(*event_types);
        }
        if (name) {
            payload.body["name"] = details::resource_json(*name);
        }
        if (url) {
            payload.body["url"] = details::resource_json(*url);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto WebhookUpdateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto WebhookListParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (after) {
            payload.query["after"] = details::resource_json(*after);
        }
        if (limit) {
            payload.query["limit"] = details::resource_json(*limit);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto WebhookListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto WebhooksDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto WebhooksDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto WebhookRotateSecretParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (keep_old_secret_active_for_24_hours) {
            payload.body["keep_old_secret_active_for_24_hours"] = details::resource_json(*keep_old_secret_active_for_24_hours);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto WebhookRotateSecretParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto WebhookTestParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["event_type"] = details::resource_json(event_type);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto WebhookTestParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::webhooks

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    WebhooksEventTypes::WebhooksEventTypes(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief GET /webhook_event_types.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto WebhooksEventTypes::List(
        types::webhooks::WebhooksEventTypesListParams params
    ) const -> drogon::Task<Result<types::webhooks::WebhookEventTypeList>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_event_types", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::webhooks::WebhookEventTypeList>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /webhook_event_types.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto WebhooksEventTypes::ListRaw(
        types::webhooks::WebhooksEventTypesListParams params
    ) const -> drogon::Task<Result<RawResponse<types::webhooks::WebhookEventTypeList>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_event_types", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::webhooks::WebhookEventTypeList>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Webhooks::Webhooks(std::shared_ptr<Transport> transport) : m_transport{ transport }, event_types{ transport } {}

    /**
     * @brief POST /webhook_endpoints.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Webhooks::Create(
        types::webhooks::WebhookCreateParams params
    ) const -> drogon::Task<Result<types::webhooks::WebhookEndpointWithSecret>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_endpoints", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::webhooks::WebhookEndpointWithSecret>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /webhook_endpoints.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Webhooks::CreateRaw(
        types::webhooks::WebhookCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::webhooks::WebhookEndpointWithSecret>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_endpoints", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::webhooks::WebhookEndpointWithSecret>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /webhook_endpoints/{webhook_endpoint_id}.
     * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Webhooks::Retrieve(
        std::string webhook_endpoint_id,
        types::webhooks::WebhooksRetrieveParams params
    ) const -> drogon::Task<Result<types::webhooks::WebhookEndpoint>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_endpoints/{webhook_endpoint_id}", { { "webhook_endpoint_id", webhook_endpoint_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::webhooks::WebhookEndpoint>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /webhook_endpoints/{webhook_endpoint_id}.
     * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Webhooks::RetrieveRaw(
        std::string webhook_endpoint_id,
        types::webhooks::WebhooksRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::webhooks::WebhookEndpoint>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_endpoints/{webhook_endpoint_id}", { { "webhook_endpoint_id", webhook_endpoint_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::webhooks::WebhookEndpoint>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /webhook_endpoints/{webhook_endpoint_id}.
     * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Webhooks::Update(
        std::string webhook_endpoint_id,
        types::webhooks::WebhookUpdateParams params
    ) const -> drogon::Task<Result<types::webhooks::WebhookEndpoint>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_endpoints/{webhook_endpoint_id}", { { "webhook_endpoint_id", webhook_endpoint_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::webhooks::WebhookEndpoint>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /webhook_endpoints/{webhook_endpoint_id}.
     * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Webhooks::UpdateRaw(
        std::string webhook_endpoint_id,
        types::webhooks::WebhookUpdateParams params
    ) const -> drogon::Task<Result<RawResponse<types::webhooks::WebhookEndpoint>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_endpoints/{webhook_endpoint_id}", { { "webhook_endpoint_id", webhook_endpoint_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::webhooks::WebhookEndpoint>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /webhook_endpoints.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Webhooks::List(
        types::webhooks::WebhookListParams params
    ) const -> drogon::Task<Result<Page<types::webhooks::WebhookEndpoint>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_endpoints", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::webhooks::WebhookEndpoint>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true);
    }

    /**
     * @brief DELETE /webhook_endpoints/{webhook_endpoint_id}.
     * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Webhooks::Delete(
        std::string webhook_endpoint_id,
        types::webhooks::WebhooksDeleteParams params
    ) const -> drogon::Task<Result<types::webhooks::DeletedWebhookEndpoint>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_endpoints/{webhook_endpoint_id}", { { "webhook_endpoint_id", webhook_endpoint_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::webhooks::DeletedWebhookEndpoint>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief DELETE /webhook_endpoints/{webhook_endpoint_id}.
     * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Webhooks::DeleteRaw(
        std::string webhook_endpoint_id,
        types::webhooks::WebhooksDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<types::webhooks::DeletedWebhookEndpoint>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_endpoints/{webhook_endpoint_id}", { { "webhook_endpoint_id", webhook_endpoint_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::webhooks::DeletedWebhookEndpoint>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /webhook_endpoints/{webhook_endpoint_id}/rotate_secret.
     * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Webhooks::RotateSecret(
        std::string webhook_endpoint_id,
        types::webhooks::WebhookRotateSecretParams params
    ) const -> drogon::Task<Result<types::webhooks::WebhookEndpointWithSecret>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_endpoints/{webhook_endpoint_id}/rotate_secret", { { "webhook_endpoint_id", webhook_endpoint_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::webhooks::WebhookEndpointWithSecret>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /webhook_endpoints/{webhook_endpoint_id}/rotate_secret.
     * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Webhooks::RotateSecretRaw(
        std::string webhook_endpoint_id,
        types::webhooks::WebhookRotateSecretParams params
    ) const -> drogon::Task<Result<RawResponse<types::webhooks::WebhookEndpointWithSecret>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_endpoints/{webhook_endpoint_id}/rotate_secret", { { "webhook_endpoint_id", webhook_endpoint_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::webhooks::WebhookEndpointWithSecret>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /webhook_endpoints/{webhook_endpoint_id}/test.
     * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Webhooks::Test(
        std::string webhook_endpoint_id,
        types::webhooks::WebhookTestParams params
    ) const -> drogon::Task<Result<types::webhooks::WebhookEndpointTestResult>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_endpoints/{webhook_endpoint_id}/test", { { "webhook_endpoint_id", webhook_endpoint_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::webhooks::WebhookEndpointTestResult>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /webhook_endpoints/{webhook_endpoint_id}/test.
     * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Webhooks::TestRaw(
        std::string webhook_endpoint_id,
        types::webhooks::WebhookTestParams params
    ) const -> drogon::Task<Result<RawResponse<types::webhooks::WebhookEndpointTestResult>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/webhook_endpoints/{webhook_endpoint_id}/test", { { "webhook_endpoint_id", webhook_endpoint_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::webhooks::WebhookEndpointTestResult>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }
} // namespace drogon_openai::details
