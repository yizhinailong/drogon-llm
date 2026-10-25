/**
 * @file transport.hpp
 * @brief Declare the HTTP transport that the API groups of one `Llm` share.
 */
#pragma once

#include <string>

#include <drogon/utils/coroutine.h>
#include <json/value.h>

#include "drogon_llm/error.hpp"
#include "drogon_llm/llm_client.hpp"

namespace drogon_llm::details {

    /**
     * @brief Send authenticated JSON requests to one OpenAI-format endpoint.
     *
     * Each request gets its own HTTP client on the calling thread's event loop. drogon
     * allows one in-flight request per client, so a shared client would serialize them;
     * a client on the caller's loop also makes the coroutine resume on the caller's thread.
     */
    class Transport {
    public:
        /**
         * @brief Store the endpoint settings and split `config.base_url`.
         * @param config Endpoint settings; `api_key` may be empty for endpoints without auth.
         */
        explicit Transport(LlmConfig config);

        /**
         * @brief Return the endpoint settings.
         * @return Settings passed to the constructor.
         */
        [[nodiscard]] auto Config() const noexcept -> LlmConfig const&;

        /**
         * @brief POST a JSON body and await the JSON response.
         * @param path Path below `base_url`, e.g. `/chat/completions`.
         * @param body Request body.
         * @param timeout Seconds to wait for the response; 0 waits forever.
         * @return Task yielding the parsed body of a 2xx response, or why the request failed.
         * @note The caller keeps this object alive until the task completes.
         */
        [[nodiscard]] auto Post(
            std::string path,
            Json::Value body,
            double timeout
        ) const -> drogon::Task<Result<Json::Value>>;

    private:
        LlmConfig m_config;        ///< Endpoint settings.
        std::string m_host;        ///< Scheme, host, and port of `base_url`, for `HttpClient::newHttpClient()`.
        std::string m_path_prefix; ///< Path part of `base_url`, e.g. `/v1`; empty without one.
    };

} // namespace drogon_llm::details
