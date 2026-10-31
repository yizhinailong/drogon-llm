/**
 * @file resource_realtime.cpp
 * @brief Implement realtime resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/realtime.hpp"

namespace drogon_openai::types::realtime {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto ClientSecretCreateResponse::FromJson(Json::Value const& value) -> Result<ClientSecretCreateResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        ClientSecretCreateResponse result;
        result.raw = value;
        if (value.isMember("expires_at")) {
            if (!details::resource_read(value["expires_at"], result.expires_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ClientSecretCreateResponse.expires_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("session")) {
            if (!details::resource_read(value["session"], result.session)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ClientSecretCreateResponse.session", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("value")) {
            if (!details::resource_read(value["value"], result.value)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ClientSecretCreateResponse.value", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto RealtimeTranslationClientSecretCreateResponse::FromJson(Json::Value const& value) -> Result<RealtimeTranslationClientSecretCreateResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        RealtimeTranslationClientSecretCreateResponse result;
        result.raw = value;
        if (value.isMember("expires_at")) {
            if (!details::resource_read(value["expires_at"], result.expires_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RealtimeTranslationClientSecretCreateResponse.expires_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("session")) {
            if (!details::resource_read(value["session"], result.session)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RealtimeTranslationClientSecretCreateResponse.session", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("value")) {
            if (!details::resource_read(value["value"], result.value)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RealtimeTranslationClientSecretCreateResponse.value", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto CallCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["sdp"] = details::resource_json(sdp);
        if (session) {
            payload.body["session"] = details::resource_json(*session);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto CallCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto CallAcceptParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["type"] = details::resource_json(type);
        if (audio) {
            payload.body["audio"] = details::resource_json(*audio);
        }
        if (include) {
            payload.body["include"] = details::resource_json(*include);
        }
        if (instructions) {
            payload.body["instructions"] = details::resource_json(*instructions);
        }
        if (max_output_tokens) {
            payload.body["max_output_tokens"] = details::resource_json(*max_output_tokens);
        }
        if (model) {
            payload.body["model"] = details::resource_json(*model);
        }
        if (output_modalities) {
            payload.body["output_modalities"] = details::resource_json(*output_modalities);
        }
        if (parallel_tool_calls) {
            payload.body["parallel_tool_calls"] = details::resource_json(*parallel_tool_calls);
        }
        if (prompt) {
            payload.body["prompt"] = details::resource_json(*prompt);
        }
        if (reasoning) {
            payload.body["reasoning"] = details::resource_json(*reasoning);
        }
        if (tool_choice) {
            payload.body["tool_choice"] = details::resource_json(*tool_choice);
        }
        if (tools) {
            payload.body["tools"] = details::resource_json(*tools);
        }
        if (tracing) {
            payload.body["tracing"] = details::resource_json(*tracing);
        }
        if (truncation) {
            payload.body["truncation"] = details::resource_json(*truncation);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto CallAcceptParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto RealtimeCallsHangupParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto RealtimeCallsHangupParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto CallReferParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["target_uri"] = details::resource_json(target_uri);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto CallReferParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto CallRejectParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (status_code) {
            payload.body["status_code"] = details::resource_json(*status_code);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto CallRejectParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ClientSecretCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (expires_after) {
            payload.body["expires_after"] = details::resource_json(*expires_after);
        }
        if (session) {
            payload.body["session"] = details::resource_json(*session);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ClientSecretCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto RealtimeTranslationsClientSecretsCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["session"] = details::resource_json(session);
        if (expires_after) {
            payload.body["expires_after"] = details::resource_json(*expires_after);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto RealtimeTranslationsClientSecretsCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::realtime

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    RealtimeTranslationsClientSecrets::RealtimeTranslationsClientSecrets(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /realtime/translations/client_secrets.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto RealtimeTranslationsClientSecrets::Create(
        types::realtime::RealtimeTranslationsClientSecretsCreateParams params
    ) const -> drogon::Task<Result<types::realtime::RealtimeTranslationClientSecretCreateResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/realtime/translations/client_secrets", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::realtime::RealtimeTranslationClientSecretCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /realtime/translations/client_secrets.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto RealtimeTranslationsClientSecrets::CreateRaw(
        types::realtime::RealtimeTranslationsClientSecretsCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::realtime::RealtimeTranslationClientSecretCreateResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/realtime/translations/client_secrets", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::realtime::RealtimeTranslationClientSecretCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    RealtimeCalls::RealtimeCalls(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /realtime/calls.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto RealtimeCalls::Create(
        types::realtime::CallCreateParams params
    ) const -> drogon::Task<Result<RawResponse<std::string>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/realtime/calls", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        payload.multipart_encodings["sdp"] = { "application/sdp", false };
        payload.multipart_encodings["session"] = { "application/json", true };
        payload.raw_body_field = "sdp";
        auto response{ co_await resource_bytes(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "Accept", "application/sdp" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        co_return std::move(*response);
    }

    /**
     * @brief POST /realtime/calls/{call_id}/accept.
     * @param call_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto RealtimeCalls::Accept(
        std::string call_id,
        types::realtime::CallAcceptParams params
    ) const -> drogon::Task<Result<void>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/realtime/calls/{call_id}/accept", { { "call_id", call_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "Accept", "*/*" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        co_return {};
    }

    /**
     * @brief POST /realtime/calls/{call_id}/accept.
     * @param call_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto RealtimeCalls::AcceptRaw(
        std::string call_id,
        types::realtime::CallAcceptParams params
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/realtime/calls/{call_id}/accept", { { "call_id", call_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "Accept", "*/*" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<Json::Value>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /realtime/calls/{call_id}/hangup.
     * @param call_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto RealtimeCalls::Hangup(
        std::string call_id,
        types::realtime::RealtimeCallsHangupParams params
    ) const -> drogon::Task<Result<void>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/realtime/calls/{call_id}/hangup", { { "call_id", call_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "Accept", "*/*" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        co_return {};
    }

    /**
     * @brief POST /realtime/calls/{call_id}/hangup.
     * @param call_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto RealtimeCalls::HangupRaw(
        std::string call_id,
        types::realtime::RealtimeCallsHangupParams params
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/realtime/calls/{call_id}/hangup", { { "call_id", call_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "Accept", "*/*" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<Json::Value>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /realtime/calls/{call_id}/refer.
     * @param call_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto RealtimeCalls::Refer(
        std::string call_id,
        types::realtime::CallReferParams params
    ) const -> drogon::Task<Result<void>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/realtime/calls/{call_id}/refer", { { "call_id", call_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "Accept", "*/*" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        co_return {};
    }

    /**
     * @brief POST /realtime/calls/{call_id}/refer.
     * @param call_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto RealtimeCalls::ReferRaw(
        std::string call_id,
        types::realtime::CallReferParams params
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/realtime/calls/{call_id}/refer", { { "call_id", call_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "Accept", "*/*" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<Json::Value>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /realtime/calls/{call_id}/reject.
     * @param call_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto RealtimeCalls::Reject(
        std::string call_id,
        types::realtime::CallRejectParams params
    ) const -> drogon::Task<Result<void>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/realtime/calls/{call_id}/reject", { { "call_id", call_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "Accept", "*/*" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        co_return {};
    }

    /**
     * @brief POST /realtime/calls/{call_id}/reject.
     * @param call_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto RealtimeCalls::RejectRaw(
        std::string call_id,
        types::realtime::CallRejectParams params
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/realtime/calls/{call_id}/reject", { { "call_id", call_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "Accept", "*/*" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<Json::Value>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    RealtimeClientSecrets::RealtimeClientSecrets(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /realtime/client_secrets.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto RealtimeClientSecrets::Create(
        types::realtime::ClientSecretCreateParams params
    ) const -> drogon::Task<Result<types::realtime::ClientSecretCreateResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/realtime/client_secrets", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::realtime::ClientSecretCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /realtime/client_secrets.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto RealtimeClientSecrets::CreateRaw(
        types::realtime::ClientSecretCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::realtime::ClientSecretCreateResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/realtime/client_secrets", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::realtime::ClientSecretCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    RealtimeTranslations::RealtimeTranslations(std::shared_ptr<Transport> transport) : m_transport{ transport }, client_secrets{ transport } {}

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Realtime::Realtime(std::shared_ptr<Transport> transport) : m_transport{ transport }, client_secrets{ transport }, calls{ transport }, translations{ transport } {}
} // namespace drogon_openai::details
