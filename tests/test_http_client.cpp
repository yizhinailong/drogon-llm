/**
 * @file test_http_client.cpp
 * @brief Send one chat completion request through `Llm::chat->completions` to the configured endpoint.
 */
#include <print>
#include <string>
#include <string_view>
#include <thread>

#include <drogon/drogon.h>

#include <drogon_llm/llm_client.hpp>

namespace {

    constexpr std::string_view PROMPT{ "Introduce yourself." }; ///< User message to send.
    constexpr double REQUEST_TIMEOUT{ 120.0 };                  ///< Seconds; thinking mode can be slow.

    /**
     * @brief Send `PROMPT` and print the reply.
     * @param llm Client created by the LlmClient plugin.
     * @return True if the request succeeded and returned at least one choice.
     */
    auto send_chat(drogon_llm::Llm& llm) -> bool {
        auto const completion{ llm.chat->completions->Create({
            .messages = { { .role = "user", .content = std::string{ PROMPT } } },
            .timeout = REQUEST_TIMEOUT,
        }) };
        if (!completion) {
            std::println(stderr, "request failed: {}", completion.error().message);
            return false;
        }
        if (completion->choices.empty()) {
            std::println(stderr, "response has no choices");
            return false;
        }

        std::println("model: {}", completion->model);
        auto const& message{ completion->choices.front().message };
        if (message.reasoning_content) {
            std::println("reasoning: {}", *message.reasoning_content);
        }
        std::println("content: {}", message.content.value_or(""));
        if (completion->usage) {
            std::println(
                "tokens: prompt {} + completion {} = {}",
                completion->usage->prompt_tokens,
                completion->usage->completion_tokens,
                completion->usage->total_tokens
            );
        }
        return true;
    }

} // namespace

auto main() -> int {
    int exit_code{ 1 };
    std::thread worker{};
    drogon::app()
        .loadConfigFile("config.json")
        .registerBeginningAdvice([&exit_code, &worker] {
            // Create() blocks, and the HTTP client runs on this (the main) loop, so call it from another thread.
            worker = std::thread{
                [&exit_code, llm = drogon::app().getPlugin<drogon_llm::LlmClient>()->NewLlmClient()] {
                    exit_code = send_chat(*llm) ? 0 : 1;
                    drogon::app().quit();
                }
            };
        })
        .run();
    worker.join();
    return exit_code;
}
