/**
 * @file resource_connections.cpp
 * @brief Bind Responses, Realtime and Live resources to persistent WebSocket sessions.
 */
#include "details/resource_transport.hpp"
#include "details/websocket_transport.hpp"
#include "drogon_openai/openai_client.hpp"

namespace drogon_openai::details {

    /**
     * @brief Open an authenticated WebSocket session for this resource.
     * @param options Owned operation parameters.
     * @return Operation result or an error.
     */
    auto Responses::Connect(
        RequestOptions options
    ) const -> drogon::Task<Result<WebSocketSession>> {
        auto transport{ m_transport };
        co_return co_await websocket_connect(std::move(transport), "/responses", std::move(options));
    }

    /**
     * @brief Open an authenticated WebSocket session for this resource.
     * @param model Owned operation parameters.
     * @param options Owned operation parameters.
     * @return Operation result or an error.
     */
    auto Realtime::Connect(
        std::string model,
        RequestOptions options
    ) const -> drogon::Task<Result<WebSocketSession>> {
        auto transport{ m_transport };
        if (!options.extra_query.isNull() && !options.extra_query.isObject()) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "WebSocket query must be an object" } };
        }
        if (model.empty() && (!options.extra_query.isObject() || options.extra_query["call_id"].isNull())) {
            model = transport->Config().model;
        }
        if (!model.empty() && !options.extra_query.isMember("model")) {
            options.extra_query["model"] = std::move(model);
        }
        co_return co_await websocket_connect(std::move(transport), "/realtime", std::move(options));
    }

    /**
     * @brief Open an authenticated WebSocket session for this resource.
     * @param model Owned operation parameters.
     * @param options Owned operation parameters.
     * @return Operation result or an error.
     */
    auto RealtimeTranslations::Connect(
        std::string model,
        RequestOptions options
    ) const -> drogon::Task<Result<WebSocketSession>> {
        auto transport{ m_transport };
        if (!options.extra_query.isNull() && !options.extra_query.isObject()) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "WebSocket query must be an object" } };
        }
        if (model.empty() && (!options.extra_query.isObject() || options.extra_query["call_id"].isNull())) {
            model = transport->Config().model;
        }
        if (!model.empty() && !options.extra_query.isMember("model")) {
            options.extra_query["model"] = std::move(model);
        }
        co_return co_await websocket_connect(std::move(transport), "/realtime/translations", std::move(options));
    }

    /**
     * @brief Open an authenticated WebSocket session for this resource.
     * @param options Owned operation parameters.
     * @return Operation result or an error.
     */
    auto Live::Connect(RequestOptions options) const -> drogon::Task<Result<WebSocketSession>> {
        auto transport{ m_transport };
        co_return co_await websocket_connect(std::move(transport), "/live/sessions", std::move(options));
    }

    /**
     * @brief Open an authenticated WebSocket session for this resource.
     * @param session_id Owned operation parameters.
     * @param options Owned operation parameters.
     * @return Operation result or an error.
     */
    auto LiveSideband::Connect(
        std::string session_id,
        RequestOptions options
    ) const -> drogon::Task<Result<WebSocketSession>> {
        auto transport{ m_transport };
        auto path{ resource_path("/live/sessions/{session_id}/attach", { { "session_id", session_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await websocket_connect(std::move(transport), std::move(*path), std::move(options));
    }

    /**
     * @brief Open an authenticated WebSocket session for this resource.
     * @param session_id Owned operation parameters.
     * @param options Owned operation parameters.
     * @return Operation result or an error.
     */
    auto LiveForks::Connect(
        std::string session_id,
        RequestOptions options
    ) const -> drogon::Task<Result<WebSocketSession>> {
        auto transport{ m_transport };
        auto path{ resource_path("/live/sessions/{session_id}/fork", { { "session_id", session_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await websocket_connect(std::move(transport), std::move(*path), std::move(options));
    }
} // namespace drogon_openai::details
