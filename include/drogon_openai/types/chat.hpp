/**
 * @file chat.hpp
 * @brief Declare the request and response types of the chat completions API.
 *
 * Names and fields follow the OpenAI Python SDK's `openai.types.chat`.
 */
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include <json/value.h>

namespace drogon_openai {

    /**
     * @brief One message of the conversation sent to the model (`ChatCompletionMessageParam`).
     */
    struct ChatCompletionMessageParam {
        std::string role;    ///< Author: `system`, `developer`, `user`, or `assistant`.
        std::string content; ///< Message text.
    };

    /**
     * @brief Arguments of `Completions::Create()`, named after `client.chat.completions.create()`.
     *
     * Unset optional fields are left out of the request body, so the endpoint's defaults apply.
     */
    struct ChatCompletionCreateParams {
        std::vector<ChatCompletionMessageParam> messages; ///< Conversation so far, oldest first.
        std::optional<std::string> model{};               ///< Model name; unset uses `OpenAIConfig::model`.
        std::optional<double> frequency_penalty{};        ///< -2.0 to 2.0; positive values discourage repeating tokens.
        std::optional<std::int64_t> max_tokens{};         ///< Upper bound on generated tokens.
        std::optional<double> presence_penalty{};         ///< -2.0 to 2.0; positive values encourage new topics.
        std::optional<double> temperature{};              ///< Sampling temperature, 0 to 2.
        std::optional<double> top_p{};                    ///< Nucleus sampling probability mass.
        Json::Value extra_body{};                         ///< Object whose members are added to the body, overriding the fields above.
        double timeout{ 600.0 };                          ///< Seconds to wait for the response; 0 waits forever.
    };

    /**
     * @brief Message the model generated (`ChatCompletionMessage`).
     */
    struct ChatCompletionMessage {
        std::string role;                               ///< Always `assistant`.
        std::optional<std::string> content{};           ///< Reply text; unset when the response has none.
        std::optional<std::string> reasoning_content{}; ///< Thinking text from reasoning models such as DeepSeek's; unset otherwise.
    };

    /**
     * @brief One generated alternative (`chat_completion.Choice`).
     */
    struct ChatCompletionChoice {
        std::string finish_reason;     ///< Why generation stopped, e.g. `stop` or `length`.
        std::int64_t index{};          ///< Position in `ChatCompletion::choices`.
        ChatCompletionMessage message; ///< Generated message.
    };

    /**
     * @brief Token counts of one request (`CompletionUsage`).
     */
    struct CompletionUsage {
        std::int64_t completion_tokens{}; ///< Generated tokens, reasoning included.
        std::int64_t prompt_tokens{};     ///< Tokens in the prompt.
        std::int64_t total_tokens{};      ///< Sum of the two.
    };

    /**
     * @brief Response of `Completions::Create()` (`ChatCompletion`).
     */
    struct ChatCompletion {
        std::string id;                            ///< Unique completion ID.
        std::vector<ChatCompletionChoice> choices; ///< Generated alternatives, usually one.
        std::int64_t created{};                    ///< Unix timestamp in seconds.
        std::string model;                         ///< Model that produced the completion.
        std::string object;                        ///< Always `chat.completion`.
        std::optional<CompletionUsage> usage{};    ///< Token counts; unset if the endpoint sent none.
    };

} // namespace drogon_openai
