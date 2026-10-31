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
#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/chat.hpp"
#include "drogon_openai/types/chat_management.hpp"

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
     * @brief Messages API group below stored chat completions.
     */
    class CompletionMessages {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind the messages group to an endpoint.
         * @param transport Shared HTTP transport.
         */
        explicit CompletionMessages(std::shared_ptr<Transport> transport);

        /**
         * @brief List the original messages of a stored completion.
         * @param completion_id Stored completion ID, encoded as one path segment.
         * @param params Cursor, page size, and order.
         * @param options Per-request HTTP overrides, retained across pages.
         * @return Typed page supporting manual and automatic traversal.
         */
        [[nodiscard]] auto List(
            std::string completion_id,
            ChatCompletionMessageListParams params = {},
            RequestOptions options = {}
        ) const -> drogon::Task<Result<Page<ChatCompletionStoreMessage>>>;
    };

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

        CompletionMessages messages; ///< Messages belonging to stored completions.

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

        /**
         * @brief Retrieve a completion created with `store: true`.
         * @param completion_id Stored completion ID, encoded as one path segment.
         * @param options Per-request HTTP overrides.
         * @return Stored completion or a request/response error.
         */
        [[nodiscard]] auto Retrieve(
            std::string completion_id,
            RequestOptions options = {}
        ) const -> drogon::Task<Result<ChatCompletion>>;

        /**
         * @brief Update the metadata of a stored completion.
         * @param completion_id Stored completion ID, encoded as one path segment.
         * @param params Metadata and additional body fields.
         * @param options Per-request HTTP overrides.
         * @return Updated completion or a request/response error.
         */
        [[nodiscard]] auto Update(
            std::string completion_id,
            ChatCompletionUpdateParams params,
            RequestOptions options = {}
        ) const -> drogon::Task<Result<ChatCompletion>>;

        /**
         * @brief Delete a stored completion and its stored messages.
         * @param completion_id Stored completion ID, encoded as one path segment.
         * @param options Per-request HTTP overrides.
         * @return Deletion confirmation or a request/response error.
         */
        [[nodiscard]] auto Delete(
            std::string completion_id,
            RequestOptions options = {}
        ) const -> drogon::Task<Result<ChatCompletionDeleted>>;

        /**
         * @brief List stored completions with optional filters.
         * @param params Cursor, page size, metadata, model, and sort order.
         * @param options Per-request HTTP overrides, retained across pages.
         * @return Typed page supporting manual and automatic traversal.
         */
        [[nodiscard]] auto List(
            ChatCompletionListParams params = {},
            RequestOptions options = {}
        ) const -> drogon::Task<Result<Page<ChatCompletion>>>;
    };

} // namespace drogon_openai::details
