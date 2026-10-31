/**
 * @file chat_stream.hpp
 * @brief Decode incremental SSE bytes and accumulate chat completion snapshots.
 */
#pragma once

#include <map>
#include <set>
#include <string_view>

#include "drogon_openai/details/completions.hpp"

namespace drogon_openai::details {

    /**
     * @brief Own SSE framing and high-level chat stream event state.
     */
    class ChatStream {
    private:
        /**
         * @brief Accumulate one candidate and indexed tool calls.
         */
        struct Candidate {
            Json::Value value{ Json::objectValue };    ///< Full choice snapshot.
            std::map<std::int64_t, Json::Value> tools; ///< Tool calls keyed by delta index.
            std::set<std::int64_t> tools_done;         ///< Tools whose done event was emitted.
            bool done{};                               ///< Candidate finish events already emitted.
        };

        ChatCompletionCreateParams m_params;            ///< Parsing configuration owned by the stream.
        ChatChunkCallback m_on_chunk;                   ///< Chunk consumer.
        ChatEventCallback m_on_event;                   ///< Aggregated event consumer.
        std::string m_buffer;                           ///< Incomplete SSE line.
        std::string m_data;                             ///< Current event data, with multiline separators.
        std::string m_event;                            ///< SSE event name, when supplied.
        std::string m_request_id;                       ///< Original response request ID.
        Json::Value m_root{ Json::objectValue };        ///< Full completion snapshot.
        std::map<std::int64_t, Candidate> m_candidates; ///< Alternatives keyed by choice index.
        bool m_first_line{ true };                      ///< Strip a UTF-8 BOM on the first line.
        bool m_skip_lf{};                               ///< Previous line ended in CR; ignore an optional LF.
        bool m_done{};                                  ///< A DONE marker was received.
        bool m_cancelled{};                             ///< Consumer requested early termination.
        bool m_received{};                              ///< At least one valid chunk was received.

    public:
        /**
         * @brief Bind owned request and stream consumers.
         * @param params Request and optional parsing configuration.
         * @param on_chunk Chunk consumer; empty means no callback.
         * @param on_event High-level event consumer; empty means no callback.
         */
        ChatStream(
            ChatCompletionCreateParams params,
            ChatChunkCallback on_chunk,
            ChatEventCallback on_event
        );

        /**
         * @brief Decode newly received SSE bytes on the awaiting loop.
         * @param bytes Body fragment, possibly splitting UTF-8, lines, or events.
         * @param headers Original response headers.
         * @return Continue reading, consumer cancellation, or a decoding error.
         */
        auto Feed(std::string_view bytes, Headers const& headers) -> Result<bool>;

        /**
         * @brief Return the final completion after DONE, or an explicitly cancelled snapshot.
         * @return Accumulated completion or a truncated/invalid stream error.
         */
        auto Finish() -> Result<ChatCompletion>;

    private:
        /**
         * @brief Process one SSE line.
         * @param line Line excluding its delimiter.
         * @return Whether to continue reading.
         */
        auto line(std::string line) -> Result<bool>;
        /**
         * @brief Decode and dispatch the current SSE event.
         * @return Whether to continue reading.
         */
        auto dispatch() -> Result<bool>;
        /**
         * @brief Merge a chunk and emit associated events.
         * @param chunk Decoded chunk.
         * @return Whether to continue reading.
         */
        auto accumulate(ChatCompletionChunk chunk) -> Result<bool>;
        /**
         * @brief Emit one event with a full current snapshot.
         * @param type Event name.
         * @param choice Choice index.
         * @param delta New text or the completed text.
         * @param data Event-specific JSON.
         * @param tool Tool index, when applicable.
         * @return Whether the consumer wants to continue.
         */
        auto emit(
            std::string type,
            std::int64_t choice,
            std::string delta = {},
            Json::Value data = {},
            std::optional<std::int64_t> tool = {}
        ) -> Result<bool>;
        /**
         * @brief Emit done events for a tool exactly once.
         * @param choice Choice index.
         * @param index Tool index.
         * @return Whether the consumer wants to continue.
         */
        auto toolDone(std::int64_t choice, std::int64_t index) -> Result<bool>;
        /**
         * @brief Rebuild the JSON choices array from indexed accumulated alternatives.
         */
        auto rebuild() -> void;
    };

} // namespace drogon_openai::details
