/**
 * @file llm_client.hpp
 * @brief Declare the drogon plugin that holds the LLM endpoint settings.
 */
#pragma once

#include <string>

#include <drogon/plugins/Plugin.h>

namespace drogon_llm {

    /**
     * @brief Endpoint settings read from the plugin's `config` object.
     */
    struct LlmConfig {
        std::string base_url; ///< API base URL, e.g. `https://api.openai.com/v1`.
        std::string api_key;  ///< Bearer token from the `api_key_env` variable; empty without it.
        std::string model;    ///< Default model name sent with each request.
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
     * or a handler: `drogon::app().getPlugin<drogon_llm::LlmClient>()`.
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

    private:
        LlmConfig m_config; ///< Settings parsed in `initAndStart()`.
    };

} // namespace drogon_llm
