/**
 * @file llm_client.hpp
 * @brief Declare the `Llm` client and the drogon plugin that holds its endpoint settings.
 */
#pragma once

#include <memory>
#include <string>

#include <drogon/plugins/Plugin.h>

#include "drogon_llm/details/chat.hpp"

namespace drogon_llm {

    /**
     * @brief Endpoint settings of an `Llm`; the plugin reads them from its `config` object.
     */
    struct LlmConfig {
        std::string base_url{ "https://api.openai.com/v1" }; ///< API base URL; OpenAI's by default.
        std::string api_key;                                 ///< Bearer token from the `api_key_env` variable; empty without it.
        std::string model;                                   ///< Model sent when a request leaves `model` unset.
    };

    /**
     * @brief Client for one OpenAI-format endpoint, laid out like the OpenAI Python SDK's `AsyncOpenAI`.
     *
     * @code
     * auto const completion{ co_await llm->chat->completions->Create({
     *     .messages = { { .role = "user", .content = "Hello!" } },
     * }) };
     * if (completion) {
     *     std::println("{}", completion->choices.front().message.content.value_or(""));
     * }
     * @endcode
     */
    class Llm final {
    public:
        /**
         * @brief Create a client for the endpoint in `config`.
         * @param config Endpoint settings; `api_key` may be empty for endpoints without auth.
         */
        explicit Llm(LlmConfig config);

        std::unique_ptr<details::Chat> chat{}; ///< Chat API group.
    };

    /**
     * @brief Drogon plugin that owns the LLM endpoint settings.
     *
     * Register it in the application's config.json:
     * @code{.json}
     * "plugins": [{
     *     "name": "drogon_llm::LlmClient",
     *     "config": { "base_url": "https://api.deepseek.com", "api_key_env": "DEEPSEEK_API_KEY", "model": "deepseek-flash" }
     * }]
     * @endcode
     *
     * The API key is never read from config.json: `api_key_env` names the
     * environment variable that holds it. Omit `api_key_env` for endpoints
     * without auth.
     *
     * Then fetch it once `app().run()` has started, e.g. from a beginning advice
     * or a handler: `drogon::app().getPlugin<drogon_llm::LlmClient>()`, and
     * call `NewLlmClient()` for an `Llm` bound to these settings.
     *
     * @note With a static build, the plugin registers itself only if the
     * application references this class (calling `getPlugin<LlmClient>()` is
     * enough). Otherwise the linker drops it and drogon cannot find the name.
     */
    class LlmClient : public drogon::Plugin<LlmClient> {
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
        [[nodiscard]] auto Config() const noexcept -> LlmConfig const&;

        /**
         * @brief Create an `Llm` client for the configured endpoint.
         * @return A new client; call only after drogon has initialized the plugin.
         */
        [[nodiscard]] auto NewLlmClient() -> std::shared_ptr<Llm>;

    private:
        LlmConfig m_config; ///< Settings parsed in `initAndStart()`.
    };

} // namespace drogon_llm
