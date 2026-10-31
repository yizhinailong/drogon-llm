/**
 * @file resource_live.cpp
 * @brief Implement live resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/live.hpp"

namespace drogon_openai::types::live {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto LiveCreateResponse::FromJson(Json::Value const& value) -> Result<LiveCreateResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        LiveCreateResponse result;
        result.raw = value;
        if (value.isMember("session")) {
            if (!details::resource_read(value["session"], result.session)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid LiveCreateResponse.session", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("transport")) {
            if (!details::resource_read(value["transport"], result.transport)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid LiveCreateResponse.transport", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto SessionForkResponse::FromJson(Json::Value const& value) -> Result<SessionForkResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        SessionForkResponse result;
        result.raw = value;
        if (value.isMember("session")) {
            if (!details::resource_read(value["session"], result.session)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SessionForkResponse.session", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("transport")) {
            if (!details::resource_read(value["transport"], result.transport)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid SessionForkResponse.transport", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto LiveCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["session"] = details::resource_json(session);
        payload.body["transport"] = details::resource_json(transport);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto LiveCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto SessionAcceptParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["session"] = details::resource_json(session);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto SessionAcceptParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto LiveSessionsDownloadRecordingParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto LiveSessionsDownloadRecordingParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto SessionForkParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["transport"] = details::resource_json(transport);
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
    auto SessionForkParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto LiveSessionsHangupParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto LiveSessionsHangupParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto SessionReferParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["target_uri"] = details::resource_json(target_uri);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto SessionReferParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto SessionRejectParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["status_code"] = details::resource_json(status_code);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto SessionRejectParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::live

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    LiveForks::LiveForks(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    LiveSessions::LiveSessions(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /live/sessions/{session_id}/accept.
     * @param session_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto LiveSessions::Accept(
        std::string session_id,
        types::live::SessionAcceptParams params
    ) const -> drogon::Task<Result<void>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/live/sessions/{session_id}/accept", { { "session_id", session_id } }) };
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
     * @brief POST /live/sessions/{session_id}/accept.
     * @param session_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto LiveSessions::AcceptRaw(
        std::string session_id,
        types::live::SessionAcceptParams params
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/live/sessions/{session_id}/accept", { { "session_id", session_id } }) };
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
     * @brief GET /live/sessions/{session_id}/content.
     * @param session_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto LiveSessions::DownloadRecording(
        std::string session_id,
        types::live::LiveSessionsDownloadRecordingParams params
    ) const -> drogon::Task<Result<RawResponse<std::string>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/live/sessions/{session_id}/content", { { "session_id", session_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_bytes(transport, "GET", std::move(*path), std::move(payload), params.Options(), { { "Accept", "application/binary" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        co_return std::move(*response);
    }

    /**
     * @brief POST /live/sessions/{session_id}/fork.
     * @param session_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto LiveSessions::Fork(
        std::string session_id,
        types::live::SessionForkParams params
    ) const -> drogon::Task<Result<types::live::SessionForkResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/live/sessions/{session_id}/fork", { { "session_id", session_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::live::SessionForkResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /live/sessions/{session_id}/fork.
     * @param session_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto LiveSessions::ForkRaw(
        std::string session_id,
        types::live::SessionForkParams params
    ) const -> drogon::Task<Result<RawResponse<types::live::SessionForkResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/live/sessions/{session_id}/fork", { { "session_id", session_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::live::SessionForkResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /live/sessions/{session_id}/hangup.
     * @param session_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto LiveSessions::Hangup(
        std::string session_id,
        types::live::LiveSessionsHangupParams params
    ) const -> drogon::Task<Result<void>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/live/sessions/{session_id}/hangup", { { "session_id", session_id } }) };
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
     * @brief POST /live/sessions/{session_id}/hangup.
     * @param session_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto LiveSessions::HangupRaw(
        std::string session_id,
        types::live::LiveSessionsHangupParams params
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/live/sessions/{session_id}/hangup", { { "session_id", session_id } }) };
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
     * @brief POST /live/sessions/{session_id}/refer.
     * @param session_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto LiveSessions::Refer(
        std::string session_id,
        types::live::SessionReferParams params
    ) const -> drogon::Task<Result<void>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/live/sessions/{session_id}/refer", { { "session_id", session_id } }) };
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
     * @brief POST /live/sessions/{session_id}/refer.
     * @param session_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto LiveSessions::ReferRaw(
        std::string session_id,
        types::live::SessionReferParams params
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/live/sessions/{session_id}/refer", { { "session_id", session_id } }) };
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
     * @brief POST /live/sessions/{session_id}/reject.
     * @param session_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto LiveSessions::Reject(
        std::string session_id,
        types::live::SessionRejectParams params
    ) const -> drogon::Task<Result<void>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/live/sessions/{session_id}/reject", { { "session_id", session_id } }) };
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
     * @brief POST /live/sessions/{session_id}/reject.
     * @param session_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto LiveSessions::RejectRaw(
        std::string session_id,
        types::live::SessionRejectParams params
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/live/sessions/{session_id}/reject", { { "session_id", session_id } }) };
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
    LiveSideband::LiveSideband(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Live::Live(std::shared_ptr<Transport> transport) : m_transport{ transport }, sideband{ transport }, forks{ transport }, sessions{ transport } {}

    /**
     * @brief POST /live/sessions.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Live::Create(
        types::live::LiveCreateParams params
    ) const -> drogon::Task<Result<types::live::LiveCreateResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/live/sessions", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::live::LiveCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /live/sessions.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Live::CreateRaw(
        types::live::LiveCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::live::LiveCreateResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/live/sessions", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::live::LiveCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }
} // namespace drogon_openai::details
