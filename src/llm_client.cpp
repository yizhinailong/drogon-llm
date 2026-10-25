/**
 * @file llm_client.cpp
 * @brief Implement the `Llm` client and the drogon plugin that holds its endpoint settings.
 */
#include "drogon_llm/llm_client.hpp"

#include <cstdlib>
#include <format>
#include <stdexcept>
#include <utility>

#include "details/transport.hpp"

namespace {

    /**
     * @brief Read the API key from the named environment variable.
     * @param env_name Variable name from `api_key_env`; empty when the endpoint needs no key.
     * @return The variable's value, or an empty string when `env_name` is empty.
     * @throws std::invalid_argument If the variable is unset or empty.
     */
    auto read_api_key(std::string const& env_name) -> std::string {
        if (env_name.empty()) {
            return {};
        }
        char const* value{ std::getenv(env_name.c_str()) };
        if (value == nullptr || *value == '\0') {
            throw std::invalid_argument{ std::format("drogon_llm::LlmClient: environment variable {} is unset or empty", env_name) };
        }
        return value;
    }

} // namespace

namespace drogon_llm {

    /**
     * @brief Create a client for the endpoint in `config`.
     * @param config Endpoint settings; `api_key` may be empty for endpoints without auth.
     */
    Llm::Llm(LlmConfig config)
        : chat{ std::make_unique<details::Chat>(std::make_shared<details::Transport>(std::move(config))) } {}

    /**
     * @brief Read the endpoint settings from the plugin's `config` object.
     * @param config The plugin's `config` object from config.json.
     * @throws std::invalid_argument If `base_url` or `model` is missing or empty,
     * or if `api_key_env` names an unset or empty environment variable.
     */
    auto LlmClient::initAndStart(Json::Value const& config) -> void {
        m_config = LlmConfig{
            .base_url = config.get("base_url", "").asString(),
            .api_key = read_api_key(config.get("api_key_env", "").asString()),
            .model = config.get("model", "").asString(),
        };
        if (m_config.base_url.empty() || m_config.model.empty()) {
            throw std::invalid_argument{ "drogon_llm::LlmClient: config needs non-empty base_url and model" };
        }
    }

    /**
     * @brief Release resources on application shutdown.
     */
    auto LlmClient::shutdown() -> void {}

    /**
     * @brief Return the settings parsed from config.json.
     * @return Settings valid after drogon has initialized the plugin.
     */
    auto LlmClient::Config() const noexcept -> LlmConfig const& {
        return m_config;
    }

    /**
     * @brief Create an `Llm` client for the configured endpoint.
     * @return A new client; call only after drogon has initialized the plugin.
     */
    auto LlmClient::NewLlmClient() -> std::shared_ptr<Llm> {
        return std::make_shared<Llm>(m_config);
    }

} // namespace drogon_llm
