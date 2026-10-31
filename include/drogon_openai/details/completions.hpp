/**
 * @file completions.hpp
 * @brief Declare creation, parsing, streaming, and management of chat completions.
 */
#pragma once

#include <functional>
#include <memory>
#include <string>

#include <drogon/utils/coroutine.h>

#include "drogon_openai/error.hpp"
#include "drogon_openai/types/chat.hpp"

namespace drogon_openai {

    /**
     * @brief Consume a stream chunk; false closes the response and returns the current snapshot.
     */
    using ChatChunkCallback = std::function<Result<bool>(ChatCompletionChunk const&)>;

    /**
     * @brief Consume an aggregated event; false closes the response and returns the current snapshot.
     */
    using ChatEventCallback = std::function<Result<bool>(ChatCompletionStreamEvent const&)>;

} // namespace drogon_openai

namespace drogon_openai::details {

    class Transport;

    /**
     * @brief Chat completions API group, reached through `client.chat.completions`.
     */
    class Completions {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind the group to an endpoint.
         * @param transport Connection shared by the API groups of one `OpenAI` client.
         */
        explicit Completions(std::shared_ptr<Transport> transport);

        /**
         * @brief Generate a reply to a conversation.
         * @param params Owned request fields and HTTP overrides.
         * @return Completion or a request/response error.
         * @note Requests and continuations use the awaiting thread's loop. Use `sync_wait`
         * only on a thread without an event loop. Pass client handles to coroutines by value.
         */
        [[nodiscard]] auto Create(
            ChatCompletionCreateParams params
        ) const -> drogon::Task<Result<ChatCompletion>>;

        /**
         * @brief Generate a reply and retain HTTP metadata and original response bytes.
         * @param params Owned request fields and HTTP overrides.
         * @return Raw response with a typed completion or an error with response metadata.
         */
        [[nodiscard]] auto CreateRaw(
            ChatCompletionCreateParams params
        ) const -> drogon::Task<Result<RawResponse<ChatCompletion>>>;

        /**
         * @brief Generate structured JSON output and decode strict function tool arguments.
         * @param params Owned request fields, schema, strict tools, and HTTP overrides.
         * @return Parsed completion, or validation, finish-reason, or request error.
         */
        [[nodiscard]] auto Parse(
            ChatCompletionCreateParams params
        ) const -> drogon::Task<Result<ChatCompletion>>;

        /**
         * @brief Stream SSE chunks and high-level events, returning the final completion.
         * @param params Owned request fields; this method sets `stream` to true.
         * @param on_chunk Optional per-chunk consumer.
         * @param on_event Optional `chunk`, text, refusal, tool, reasoning, and logprobs events.
         * @return Final completion, current snapshot after callback cancellation, or an error.
         * @note Callbacks run on the calling loop and must not block. A started stream is
         * never retried. Callback false stops immediately without synthesizing done events.
         */
        [[nodiscard]] auto Stream(
            ChatCompletionCreateParams params,
            ChatChunkCallback on_chunk = {},
            ChatEventCallback on_event = {}
        ) const -> drogon::Task<Result<ChatCompletion>>;
    };

} // namespace drogon_openai::details
