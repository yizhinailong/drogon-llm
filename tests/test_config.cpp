/**
 * @file test_config.cpp
 * @brief Load the LlmClient plugin from config.json, print its settings, then exit.
 */
#include <print>

#include <drogon/drogon.h>

#include <drogon_llm/llm_client.hpp>

namespace {

    /**
     * @brief Print the plugin's settings and stop the app.
     *
     * Drogon initializes plugins inside `app().run()`, so call this from a
     * beginning advice rather than before `run()`.
     */
    auto print_llm_config() -> void {
        auto const& config{ drogon::app().getPlugin<drogon_llm::LlmClient>()->Config() };
        std::println("base_url: {}", config.base_url);
        std::println("model: {}", config.model);
        std::println("api_key: {}", config.api_key.empty() ? "(empty)" : "(set)");
        drogon::app().quit();
    }

} // namespace

auto main() -> int {
    drogon::app()
        .loadConfigFile("config.json")
        .registerBeginningAdvice(print_llm_config)
        .run();
    return 0;
}
