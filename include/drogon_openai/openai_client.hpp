/**
 * @file openai_client.hpp
 * @brief Declare the `OpenAI` client and the drogon plugin that holds its endpoint settings.
 */
#pragma once

#include <memory>
#include <optional>
#include <string>

#include <drogon/plugins/Plugin.h>

#include "drogon_openai/details/chat.hpp"
#include "drogon_openai/details/embeddings.hpp"
#include "drogon_openai/details/models.hpp"
#include "drogon_openai/details/responses.hpp"
#include "drogon_openai/details/text_completions.hpp"
#include "drogon_openai/error.hpp"
#include "drogon_openai/pagination.hpp"
#include "drogon_openai/request_options.hpp"

namespace drogon_openai {

    /**
     * @brief Endpoint settings of an `OpenAI` client; the plugin reads them from its `config` object.
     */
    struct OpenAIConfig {
        std::string base_url{ "https://api.openai.com/v1" }; ///< API base URL; OpenAI's by default.
        std::string api_key;                                 ///< Bearer token from the `api_key_env` variable; empty without it.
        std::string model;                                   ///< Model sent when a request leaves `model` unset.
        int max_retries{ 2 };                                ///< Maximum retries after the initial attempt.
        Timeout timeout{};                                   ///< Client-level phase timeouts.
        Headers default_headers{};                           ///< Headers applied to every request.
        Json::Value default_query{};                         ///< Query parameters applied to every request.
        std::string organization;                            ///< Optional OpenAI organization.
        std::string project;                                 ///< Optional OpenAI project.
    };

    /**
     * @brief Overrides used by `OpenAI::WithOptions` and `OpenAI::Copy`.
     */
    struct OpenAIOptions {
        std::optional<std::string> base_url{};      ///< Endpoint override.
        std::optional<std::string> api_key{};       ///< Bearer token override.
        std::optional<std::string> model{};         ///< Default model override.
        std::optional<int> max_retries{};           ///< Retry count override.
        std::optional<Timeout> timeout{};           ///< Phase timeout override.
        std::optional<Headers> default_headers{};   ///< Headers merged with the original headers.
        std::optional<Json::Value> default_query{}; ///< Query parameters merged with the originals.
        std::optional<std::string> organization{};  ///< Organization override; empty clears it.
        std::optional<std::string> project{};       ///< Project override; empty clears it.
    };

    /**
     * @brief Client for one OpenAI-format endpoint, laid out like the OpenAI Python SDK's `AsyncOpenAI`.
     *
     * @code
     * auto const completion{ co_await client.chat.completions.Create({
     *     .messages = { { .role = "user", .content = "Hello!" } },
     * }) };
     * if (completion) {
     *     std::println("{}", completion->choices.front().message.content.value_or(""));
     * }
     * @endcode
     *
     * A cheap handle: copies share one transport, which is safe to use from any thread.
     * Pass it to coroutines by value.
     */
    class OpenAI final {
    private:
        std::shared_ptr<details::Transport> m_transport; ///< Immutable settings shared by all API groups.

    public:
        /**
         * @brief Create a client for the endpoint in `config`.
         * @param config Endpoint settings; `api_key` may be empty for endpoints without auth.
         */
        explicit OpenAI(OpenAIConfig config);

        details::Chat chat;                   ///< Chat API group.
        details::Models models;               ///< SDK models API group.
        details::Embeddings embeddings;       ///< SDK embeddings API group.
        details::TextCompletions completions; ///< SDK completions API group.
        details::Responses responses;         ///< SDK responses API group.

        /**
         * @brief Copy this client with selected settings overridden.
         * @param options Settings to override.
         * @return Independent value handle; the original settings remain valid.
         */
        [[nodiscard]] auto WithOptions(OpenAIOptions options = {}) const -> OpenAI;

        /**
         * @brief Copy this client with selected settings overridden.
         * @param options Settings to override.
         * @return Independent value handle.
         */
        [[nodiscard]] auto Copy(OpenAIOptions options = {}) const -> OpenAI;

        /**
         * @brief Return the immutable client configuration.
         * @return Current endpoint and request defaults.
         */
        [[nodiscard]] auto Config() const noexcept -> OpenAIConfig const&;

        /**
         * @brief Request an unwrapped API path and parse its JSON response.
         * @param method HTTP method, e.g. `GET` or `POST`.
         * @param path Path relative to base_url.
         * @param body JSON body; null omits the body.
         * @param options Request overrides.
         * @return JSON value or a request error; an empty successful body yields null.
         */
        [[nodiscard]] auto Request(
            std::string method,
            std::string path,
            Json::Value body = {},
            RequestOptions options = {}
        ) const -> drogon::Task<Result<Json::Value>>;

        /**
         * @brief GET an unwrapped JSON path.
         * @param path Path below base_url.
         * @param options Request overrides.
         * @return Parsed JSON or an error.
         */
        [[nodiscard]] auto Get(std::string path, RequestOptions options = {}) const -> drogon::Task<Result<Json::Value>>;

        /**
         * @brief POST an unwrapped JSON path.
         * @param path Path below base_url.
         * @param body JSON request body.
         * @param options Request overrides.
         * @return Parsed JSON or an error.
         */
        [[nodiscard]] auto Post(
            std::string path,
            Json::Value body,
            RequestOptions options = {}
        ) const -> drogon::Task<Result<Json::Value>>;

        /**
         * @brief PUT an unwrapped JSON path.
         * @param path Path below base_url.
         * @param body JSON request body.
         * @param options Request overrides.
         * @return Parsed JSON or an error.
         */
        [[nodiscard]] auto Put(
            std::string path,
            Json::Value body,
            RequestOptions options = {}
        ) const -> drogon::Task<Result<Json::Value>>;

        /**
         * @brief PATCH an unwrapped JSON path.
         * @param path Path below base_url.
         * @param body JSON request body.
         * @param options Request overrides.
         * @return Parsed JSON or an error.
         */
        [[nodiscard]] auto Patch(
            std::string path,
            Json::Value body,
            RequestOptions options = {}
        ) const -> drogon::Task<Result<Json::Value>>;

        /**
         * @brief DELETE an unwrapped JSON path.
         * @param path Path below base_url.
         * @param options Request overrides.
         * @return Parsed JSON or an error.
         */
        [[nodiscard]] auto Delete(std::string path, RequestOptions options = {}) const -> drogon::Task<Result<Json::Value>>;

        /**
         * @brief Send JSON and retain response headers and raw bytes.
         * @param method HTTP method.
         * @param path API path.
         * @param body JSON request body.
         * @param options Request overrides.
         * @return Raw response with parsed JSON or an error.
         */
        [[nodiscard]] auto RequestRaw(
            std::string method,
            std::string path,
            Json::Value body = {},
            RequestOptions options = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;

        /**
         * @brief List an unwrapped collection with automatic or manual pagination.
         * @param path API collection path.
         * @param options Query and request overrides retained across pages.
         * @param cursor False for a single SDK-style page without cursor traversal.
         * @return Owned JSON page or an error.
         */
        [[nodiscard]] auto List(
            std::string path,
            RequestOptions options = {},
            bool cursor = true
        ) const -> drogon::Task<Result<JsonPage>>;

        /**
         * @brief GET binary content without interpreting it as JSON.
         * @param path API path.
         * @param options Request overrides.
         * @return Response bytes and metadata or an error.
         */
        [[nodiscard]] auto GetBinary(
            std::string path,
            RequestOptions options = {}
        ) const -> drogon::Task<Result<RawResponse<std::string>>>;

        /**
         * @brief POST multipart form fields and files.
         * @param path API path.
         * @param parts Form fields and files, in order.
         * @param options Request overrides.
         * @return Parsed JSON response or an error.
         */
        [[nodiscard]] auto PostMultipart(
            std::string path,
            std::vector<MultipartPart> parts,
            RequestOptions options = {}
        ) const -> drogon::Task<Result<Json::Value>>;

        /**
         * @brief Create an unwrapped resource and poll its collection/id path to a terminal state.
         * @param path Collection POST path.
         * @param body Creation JSON body.
         * @param poll Polling options; the total deadline starts after creation.
         * @param options Request overrides for creation and polling.
         * @return Terminal resource or an error.
         */
        [[nodiscard]] auto PostAndPoll(
            std::string path,
            Json::Value body,
            PollOptions poll = {},
            RequestOptions options = {}
        ) const -> drogon::Task<Result<Json::Value>>;

        /**
         * @brief Upload an unwrapped resource and poll its collection/id path to a terminal state.
         * @param path Collection upload path.
         * @param parts Multipart fields and files.
         * @param poll Polling options; the total deadline starts after creation.
         * @param options Request overrides for upload and polling.
         * @return Terminal resource or an error.
         */
        [[nodiscard]] auto UploadAndPoll(
            std::string path,
            std::vector<MultipartPart> parts,
            PollOptions poll = {},
            RequestOptions options = {}
        ) const -> drogon::Task<Result<Json::Value>>;

        /**
         * @brief Poll an unwrapped resource until it reaches a terminal state.
         * @param path Resource GET path.
         * @param poll Polling interval, deadline, and terminal states.
         * @param options Request overrides, applied to each poll.
         * @return Terminal resource or an error.
         */
        [[nodiscard]] auto Poll(
            std::string path,
            PollOptions poll = {},
            RequestOptions options = {}
        ) const -> drogon::Task<Result<Json::Value>>;

        /**
         * @brief Poll a resource and preserve the final HTTP response metadata.
         * @param path Resource GET path.
         * @param poll Polling interval, deadline, and terminal states.
         * @param options Request overrides, applied to each poll.
         * @return Terminal raw resource or an error.
         */
        [[nodiscard]] auto PollRaw(
            std::string path,
            PollOptions poll = {},
            RequestOptions options = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;
    };

    /**
     * @brief Drogon plugin that owns the settings of one OpenAI-format endpoint.
     *
     * Register it in the application's config.json:
     * @code{.json}
     * "plugins": [{
     *     "name": "drogon_openai::OpenAIClient",
     *     "config": { "base_url": "https://api.deepseek.com", "api_key_env": "DEEPSEEK_API_KEY", "model": "deepseek-flash" }
     * }]
     * @endcode
     *
     * The API key is never read from config.json: `api_key_env` names the
     * environment variable that holds it. Omit `api_key_env` for endpoints
     * without auth. Optional settings are `max_retries` (2), `timeout`
     * (600 seconds, connect 5), `default_headers`, `default_query`,
     * `organization`, and `project`. `timeout` may also be an object with
     * `connect`, `read`, and `write` seconds; zero disables a phase timeout.
     *
     * Then fetch it once `app().run()` has started, e.g. from a beginning advice
     * or a handler: `drogon::app().getPlugin<drogon_openai::OpenAIClient>()`, and
     * call `NewOpenAIClient()` for an `OpenAI` client bound to these settings.
     *
     * @note With a static build, the plugin registers itself only if the
     * application references this class (calling `getPlugin<OpenAIClient>()` is
     * enough). Otherwise the linker drops it and drogon cannot find the name.
     */
    class OpenAIClient : public drogon::Plugin<OpenAIClient> {
    public:
        /**
         * @brief Read the endpoint settings from the plugin's `config` object.
         * @param config The plugin's `config` object from config.json.
         * @throws std::invalid_argument If `base_url` or `model` is missing or empty,
         * environment variable, or request defaults are invalid.
         */
        auto initAndStart(Json::Value const& config) -> void override;

        /**
         * @brief Release resources on application shutdown.
         */
        auto shutdown() -> void override;

        /**
         * @brief Return the settings parsed from config.json.
         * @return Settings valid after drogon has initialized the plugin.
         */
        [[nodiscard]] auto Config() const noexcept -> OpenAIConfig const&;

        /**
         * @brief Create an `OpenAI` client for the configured endpoint.
         * @return A new client; call only after drogon has initialized the plugin.
         */
        [[nodiscard]] auto NewOpenAIClient() const -> OpenAI;

    private:
        OpenAIConfig m_config; ///< Settings parsed in `initAndStart()`.
    };

} // namespace drogon_openai
