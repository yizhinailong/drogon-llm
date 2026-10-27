/**
 * @file test_deepseek.cpp
 * @brief Send real chat completion requests to DeepSeek.
 *
 * Needs network access and a valid key in `DEEPSEEK_API_KEY`, spends a few tokens, and
 * takes a few seconds. Without the variable it exits with `SKIP_RETURN_CODE`, which CTest
 * reports as skipped, so CI never calls DeepSeek.
 */
#include <cstdlib>
#include <print>
#include <string>
#include <utility>

#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include <drogon/utils/coroutine.h>

#include <drogon_openai/openai_client.hpp>

#include "support.hpp"

namespace {

    constexpr char const* API_KEY_ENV{ "DEEPSEEK_API_KEY" };      ///< Variable that holds the DeepSeek key.
    constexpr char const* BASE_URL{ "https://api.deepseek.com" }; ///< DeepSeek's OpenAI-format endpoint.
    constexpr char const* MODEL{ "deepseek-flash" };              ///< Model to call; it thinks by default.
    constexpr char const* PROMPT{ "Introduce yourself." };        ///< User message to send.
    constexpr double REQUEST_TIMEOUT{ 120.0 };                    ///< Seconds; thinking mode can be slow.
    constexpr int SKIP_RETURN_CODE{ 77 };                         ///< Exit code that `tests/CMakeLists.txt` reports as skipped.

    /**
     * @brief Read the DeepSeek key.
     * @return The key, or empty when `API_KEY_ENV` is unset.
     */
    auto read_api_key() -> std::string {
        char const* value{ std::getenv(API_KEY_ENV) };
        return value != nullptr ? value : "";
    }

    /**
     * @brief Build a client for DeepSeek, bypassing the plugin, whose config.json points at the fake API.
     * @param api_key Key to send.
     * @return Client for `BASE_URL` and `MODEL`.
     */
    auto deepseek_client(std::string api_key) -> drogon_openai::OpenAI {
        return drogon_openai::OpenAI{ drogon_openai::OpenAIConfig{
            .base_url = BASE_URL,
            .api_key = std::move(api_key),
            .model = MODEL,
        } };
    }

    /**
     * @brief Build the request that both tests send.
     * @return Params with `PROMPT` as the user message and `REQUEST_TIMEOUT`.
     */
    auto introduce_yourself() -> drogon_openai::ChatCompletionCreateParams {
        return {
            .messages = { { .role = "user", .content = PROMPT } },
            .timeout = REQUEST_TIMEOUT,
        };
    }

} // namespace

DROGON_TEST(CreateReturnsReply) {
    auto const completion{ drogon::sync_wait(deepseek_client(read_api_key()).chat.completions.Create(introduce_yourself())) };
    if (!completion) {
        std::println(stderr, "request failed: {}", completion.error().message);
    }
    REQUIRE(completion.has_value());
    REQUIRE(!completion->choices.empty());
    auto const& message{ completion->choices.front().message };
    CHECK(message.content.has_value());
    CHECK(completion->usage.has_value());

    std::println("model: {}", completion->model);
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
}

DROGON_TEST(CreateRejectsInvalidKey) {
    auto const completion{ drogon::sync_wait(deepseek_client("sk-drogon-openai-invalid").chat.completions.Create(introduce_yourself())) };
    REQUIRE(!completion.has_value());
    CHECK(completion.error().kind == drogon_openai::ErrorKind::Status);
    CHECK(completion.error().status_code == 401);
    std::println("invalid key: {}", completion.error().message);
}

auto main(int argc, char** argv) -> int {
    if (read_api_key().empty()) {
        std::println("{} is unset; skipping the DeepSeek tests", API_KEY_ENV);
        return SKIP_RETURN_CODE;
    }
    // config.json keeps drogon's upload directories under /tmp; its plugin needs the test key.
    test_support::set_env(test_support::API_KEY_ENV, test_support::API_KEY);
    drogon::app().loadConfigFile("config.json");
    return test_support::run_tests_with_app(argc, argv);
}
