/**
 * @file test_http_client.cpp
 * @brief Send one chat completion request through `Llm::chat->completions` to the configured endpoint.
 */
#include <memory>
#include <print>
#include <string>
#include <string_view>
#include <utility>

#include <drogon/drogon.h>
#include <drogon/utils/coroutine.h>

#include <drogon_llm/llm_client.hpp>

namespace {

    constexpr std::string_view PROMPT{ "Introduce yourself." }; ///< User message to send.
    constexpr double REQUEST_TIMEOUT{ 120.0 };                  ///< Seconds; thinking mode can be slow.

    /**
     * @brief Send `PROMPT` and print the reply.
     * @param llm Client created by the LlmClient plugin, taken by value to outlive the caller's frame.
     * @return True if the request succeeded and returned at least one choice.
     */
    auto send_chat(std::shared_ptr<drogon_llm::Llm> llm) -> drogon::Task<bool> {
        auto const completion{ co_await llm->chat->completions->Create({
            .messages = { { .role = "user", .content = std::string{ PROMPT } } },
            .timeout = REQUEST_TIMEOUT,
        }) };
        if (!completion) {
            std::println(stderr, "request failed: {}", completion.error().message);
            co_return false;
        }
        if (completion->choices.empty()) {
            std::println(stderr, "response has no choices");
            co_return false;
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
        co_return true;
    }

} // namespace

auto main() -> int {
    int exit_code{ 1 };
    drogon::app()
        .loadConfigFile("config.json")
        .registerBeginningAdvice([&exit_code] {
            drogon::async_run([&exit_code]() -> drogon::Task<> {
                auto llm{ drogon::app().getPlugin<drogon_llm::LlmClient>()->NewLlmClient() };
                bool const ok{ co_await send_chat(std::move(llm)) };
                exit_code = ok ? 0 : 1;
                drogon::app().quit();
            });
        })
        .run();
    return exit_code;
}
