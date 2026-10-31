/**
 * @file http_exchange.hpp
 * @brief Declare a per-request HTTP/1.1 connection supporting incremental body delivery.
 */
#pragma once

#include <chrono>
#include <functional>
#include <string>
#include <string_view>

#include <drogon/utils/coroutine.h>

#include "drogon_openai/error.hpp"

namespace drogon_openai::details {

    /**
     * @brief Consume response bytes; return false to finish the stream early.
     * @note Runs on the exchange's loop for successful responses only. Views expire on return.
     * HTTP transfer framing is removed; SSE event boundaries are the consumer's responsibility.
     */
    using BodyCallback = std::function<Result<bool>(std::string_view, Headers const&)>;

    /**
     * @brief Parsed HTTP endpoint and request target.
     */
    struct Endpoint {
        std::string host;      ///< DNS name or unbracketed IP address.
        std::string authority; ///< Host header, including the port when specified.
        unsigned short port{}; ///< TCP port.
        bool tls{};            ///< Establish TLS before sending HTTP.
        std::string prefix;    ///< Base URL path prefix.
    };

    /**
     * @brief Parse an HTTP or HTTPS base URL.
     * @param url Base URL without a query or fragment.
     * @return Endpoint or a local configuration error.
     */
    auto parse_endpoint(std::string const& url) -> Result<Endpoint>;

    /**
     * @brief One complete request ready for transmission.
     */
    struct WireRequest {
        Endpoint endpoint;                                               ///< Destination.
        std::string method;                                              ///< HTTP method.
        std::string target;                                              ///< Encoded path and query.
        Headers headers;                                                 ///< Request headers; names are normalized before sending.
        std::string body;                                                ///< JSON, multipart, or raw request bytes.
        Timeout timeout;                                                 ///< Phase timeouts.
        BodyCallback on_data;                                            ///< Optional incremental successful response consumer.
        std::optional<std::chrono::steady_clock::time_point> deadline{}; ///< Optional total deadline across phases.
    };

    /**
     * @brief Send a single request using a new connection on the calling event loop.
     * @param request Owned request data.
     * @return Response metadata and bytes or a connection/protocol error.
     * @note TLS verifies the hostname and certificate chain against the system trust store.
     * A callback consumes successful bodies without retaining them. Other bodies are buffered.
     * Non-loop callers use the main app loop; that loop must already be running.
     */
    auto exchange(WireRequest request) -> drogon::Task<Result<RawResponse<std::string>>>;

} // namespace drogon_openai::details
