/**
 * @file transport.hpp
 * @brief Declare shared request construction, retries, and response decoding.
 */
#pragma once

#include "details/http_exchange.hpp"
#include "drogon_openai/openai_client.hpp"

namespace drogon_openai::details {

    /**
     * @brief Find the calling event loop, falling back to the application's main loop.
     * @return Loop used for requests and asynchronous delays.
     */
    auto current_loop() -> trantor::EventLoop*;

    /**
     * @brief Encode a single path segment or query component using UTF-8 percent escapes.
     * @param value Unescaped component.
     * @return Encoded component.
     */
    auto encode_path_segment(std::string const& value) -> std::string;

    /**
     * @brief Decode JSON without logging response bodies.
     * @param body Response bytes; empty means null.
     * @return Parsed JSON or a response validation error.
     */
    auto decode_json(std::string const& body) -> Result<Json::Value>;

    /**
     * @brief Immutable client settings with a new asynchronous connection per attempt.
     */
    class Transport {
    private:
        OpenAIConfig m_config; ///< Endpoint and client defaults.

    public:
        /**
         * @brief Store immutable client settings.
         * @param config Endpoint settings.
         */
        explicit Transport(OpenAIConfig config);

        /**
         * @brief Return the endpoint settings.
         * @return Settings passed to the constructor.
         */
        [[nodiscard]] auto Config() const noexcept -> OpenAIConfig const&;

        /**
         * @brief Send JSON and retain response metadata.
         * @param method HTTP method.
         * @param path Path relative to the base URL.
         * @param body JSON body; null omits it.
         * @param options Request overrides.
         * @param deadline Optional total deadline across retries and phases.
         * @return Parsed JSON with headers, or an error.
         */
        [[nodiscard]] auto Request(
            std::string method,
            std::string path,
            Json::Value body = {},
            RequestOptions options = {},
            std::optional<std::chrono::steady_clock::time_point> deadline = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;

        /**
         * @brief Send bytes with retries before successful streaming body delivery begins.
         * @param method HTTP method.
         * @param path Path relative to the base URL.
         * @param body Owned request bytes.
         * @param content_type Request media type; empty omits it.
         * @param options Request overrides.
         * @param on_data Optional incremental response consumer.
         * @param deadline Optional total deadline across retries and phases.
         * @return Response bytes and metadata, or an error.
         */
        [[nodiscard]] auto Send(
            std::string method,
            std::string path,
            std::string body,
            std::string content_type,
            RequestOptions options = {},
            BodyCallback on_data = {},
            std::optional<std::chrono::steady_clock::time_point> deadline = {}
        ) const -> drogon::Task<Result<RawResponse<std::string>>>;
    };

} // namespace drogon_openai::details
