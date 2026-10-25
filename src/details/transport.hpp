/**
 * @file transport.hpp
 * @brief Declare the HTTP connection that the API groups of one `Llm` share.
 */
#pragma once

#include <string>
#include <string_view>

#include <drogon/HttpClient.h>
#include <json/value.h>

#include "drogon_llm/error.hpp"
#include "drogon_llm/llm_client.hpp"

namespace drogon_llm::details {

    /**
     * @brief Send authenticated JSON requests to one OpenAI-format endpoint.
     *
     * The HTTP client runs on the app's main event loop.
     */
    class Transport {
    public:
        /**
         * @brief Create an HTTP client for the scheme, host, and port of `config.base_url`.
         * @param config Endpoint settings; `api_key` may be empty for endpoints without auth.
         */
        explicit Transport(LlmConfig config);

        /**
         * @brief Return the endpoint settings.
         * @return Settings passed to the constructor.
         */
        [[nodiscard]] auto Config() const noexcept -> LlmConfig const&;

        /**
         * @brief POST a JSON body and wait for the JSON response.
         * @param path Path below `base_url`, e.g. `/chat/completions`.
         * @param body Request body.
         * @param timeout Seconds to wait for the response; 0 waits forever.
         * @return The parsed body of a 2xx response, or why the request failed.
         * @note Blocks the calling thread; never call it on the main event loop's thread.
         */
        [[nodiscard]] auto Post(
            std::string_view path,
            Json::Value const& body,
            double timeout
        ) -> Result<Json::Value>;

    private:
        LlmConfig m_config;                  ///< Endpoint settings.
        std::string m_path_prefix;           ///< Path part of `base_url`, e.g. `/v1`; empty without one.
        drogon::HttpClientPtr m_http_client; ///< Client for the scheme, host, and port of `base_url`.
    };

} // namespace drogon_llm::details
