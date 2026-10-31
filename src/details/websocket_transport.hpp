/**
 * @file websocket_transport.hpp
 * @brief Open authenticated WebSocket sessions on the caller's event loop.
 */
#pragma once
#include "details/transport.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    /**
     * @brief Open a verified TLS or local plain WebSocket connection.
     * @param transport Shared endpoint settings.
     * @param path Relative WebSocket API path.
     * @param options Handshake headers, query and connect timeout.
     * @return Owned session or connection/handshake error.
     */
    auto websocket_connect(
        std::shared_ptr<Transport> transport,
        std::string path,
        RequestOptions options
    ) -> drogon::Task<Result<WebSocketSession>>;
} // namespace drogon_openai::details
