/**
 * @file openai_client.hpp
 * @brief Declare the `OpenAI` client and the drogon plugin that holds its endpoint settings.
 */
#pragma once

#include <string>

#include <drogon/plugins/Plugin.h>

#include "drogon_openai/details/chat.hpp"

namespace drogon_openai {

    /**
     * @brief Endpoint settings of an `OpenAI` client; the plugin reads them from its `config` object.
     */
    struct OpenAIConfig {
        std::string base_url{ "https://api.openai.com/v1" }; ///< API base URL; OpenAI's by default.
        std::string api_key;                                 ///< Bearer token from the `api_key_env` variable; empty without it.
        std::string model;                                   ///< Model sent when a request leaves `model` unset.
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
    public:
        /**
         * @brief Create a client for the endpoint in `config`.
         * @param config Endpoint settings; `api_key` may be empty for endpoints without auth.
         */
        explicit OpenAI(OpenAIConfig config);

        details::Chat chat; ///< Chat API group.
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
     * without auth.
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
         * or if `api_key_env` names an unset or empty environment variable.
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
