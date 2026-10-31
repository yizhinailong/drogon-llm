/**
 * @file chat_stream.cpp
 * @brief Implement SSE framing, indexed delta accumulation, and chat stream events.
 */
#include "details/chat_stream.hpp"

#include <utility>

#include "details/chat_json.hpp"
#include "details/transport.hpp"

namespace drogon_openai::details {

    namespace {
        /**
         * @brief Append text or JSON arrays while retaining other vendor fields.
         * @param destination Accumulated JSON.
         * @param fragment Incremental JSON.
         */
        auto merge_delta(Json::Value& destination, Json::Value const& fragment) -> void {
            if (fragment.isString()) {
                destination = (destination.isString() ? destination.asString() : "") + fragment.asString();
            } else if (fragment.isArray()) {
                if (!destination.isArray()) {
                    destination = Json::arrayValue;
                }
                for (auto const& item : fragment) {
                    destination.append(item);
                }
            } else if (fragment.isObject()) {
                if (!destination.isObject()) {
                    destination = Json::objectValue;
                }
                for (auto const& key : fragment.getMemberNames()) {
                    merge_delta(destination[key], fragment[key]);
                }
            } else if (!fragment.isNull()) {
                destination = fragment;
            }
        }

        /**
         * @brief Construct a stream protocol error.
         * @param message Failure description.
         * @return Validation error.
         */
        auto invalid_stream(std::string message) -> std::unexpected<Error> {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = std::move(message) } };
        }
    } // namespace

    /**
     * @brief Bind owned request and stream consumers.
     * @param params Request and optional parsing configuration.
     * @param on_chunk Chunk consumer; empty means no callback.
     * @param on_event High-level event consumer; empty means no callback.
     */
    ChatStream::ChatStream(
        ChatCompletionCreateParams params,
        ChatChunkCallback on_chunk,
        ChatEventCallback on_event
    ) : m_params{ std::move(params) }, m_on_chunk{ std::move(on_chunk) }, m_on_event{ std::move(on_event) } {
        m_root["choices"] = Json::arrayValue;
        m_root["object"] = "chat.completion";
    }

    /**
     * @brief Decode newly received SSE bytes on the awaiting loop.
     * @param bytes Body fragment, possibly splitting UTF-8, lines, or events.
     * @param headers Original response headers.
     * @return Continue reading, consumer cancellation, or a decoding error.
     */
    auto ChatStream::Feed(std::string_view bytes, Headers const& headers) -> Result<bool> {
        if (m_done || m_cancelled) {
            return false;
        }
        if (auto it{ headers.find("x-request-id") }; it != headers.end()) {
            m_request_id = it->second;
        }
        for (char c : bytes) {
            if (m_skip_lf) {
                m_skip_lf = false;
                if (c == '\n') {
                    continue;
                }
            }
            if (c == '\r' || c == '\n') {
                m_skip_lf = c == '\r';
                auto result{ line(std::exchange(m_buffer, {})) };
                if (!result || !*result) {
                    return result;
                }
            } else {
                m_buffer += c;
            }
        }
        return true;
    }

    /**
     * @brief Process one SSE line.
     * @param line Line excluding its delimiter.
     * @return Whether to continue reading.
     */
    auto ChatStream::line(std::string line) -> Result<bool> {
        if (m_first_line) {
            m_first_line = false;
            if (line.starts_with("\xef\xbb\xbf")) {
                line.erase(0, 3);
            }
        }
        if (line.empty()) {
            return dispatch();
        }
        if (line.front() == ':') {
            return true;
        }
        auto const colon{ line.find(':') };
        auto const name{ line.substr(0, colon) };
        auto value{ colon == std::string::npos ? "" : line.substr(colon + 1) };
        if (value.starts_with(' ')) {
            value.erase(0, 1);
        }
        if (name == "data") {
            m_data += value + '\n';
        } else if (name == "event") {
            m_event = std::move(value);
        }
        return true;
    }

    /**
     * @brief Decode and dispatch the current SSE event.
     * @return Whether to continue reading.
     */
    auto ChatStream::dispatch() -> Result<bool> {
        auto const event{ std::exchange(m_event, {}) };
        auto data{ std::exchange(m_data, {}) };
        if (data.empty()) {
            return true;
        }
        data.pop_back();
        if (data == "[DONE]") {
            m_done = true;
            return false;
        }
        auto json{ decode_json(data) };
        if (!json) {
            return std::unexpected{ json.error() };
        }
        if (event == "error" || (json->isObject() && json->isMember("error"))) {
            auto const& error{ json->isMember("error") ? (*json)["error"] : *json };
            Error failure{ .kind = ErrorKind::Status, .message = "API stream error", .body = data, .request_id = m_request_id };
            if (error.isObject()) {
                if (error["message"].isString()) {
                    failure.message = error["message"].asString();
                }
                if (error["type"].isString()) {
                    failure.type = error["type"].asString();
                }
                if (error["code"].isString()) {
                    failure.code = error["code"].asString();
                }
                if (error["param"].isString()) {
                    failure.param = error["param"].asString();
                }
            }
            return std::unexpected{ std::move(failure) };
        }
        auto chunk{ parse_chat_chunk(*json) };
        if (!chunk) {
            return std::unexpected{ chunk.error() };
        }
        chunk->_request_id = m_request_id;
        return accumulate(std::move(*chunk));
    }

    /**
     * @brief Rebuild the JSON choices array from indexed accumulated alternatives.
     */
    auto ChatStream::rebuild() -> void {
        m_root["choices"] = Json::arrayValue;
        for (auto& [index, candidate] : m_candidates) {
            if (!candidate.tools.empty()) {
                candidate.value["message"]["tool_calls"] = Json::arrayValue;
                for (auto const& [tool_index, tool] : candidate.tools) {
                    candidate.value["message"]["tool_calls"].append(tool);
                }
            }
            m_root["choices"].append(candidate.value);
        }
    }

    /**
     * @brief Emit one event with a full current snapshot.
     * @param type Event name.
     * @param choice Choice index.
     * @param delta New text or the completed text.
     * @param data Event-specific JSON.
     * @param tool Tool index, when applicable.
     * @return Whether the consumer wants to continue.
     */
    auto ChatStream::emit(
        std::string type,
        std::int64_t choice,
        std::string delta,
        Json::Value data,
        std::optional<std::int64_t> tool
    ) -> Result<bool> {
        if (!m_on_event) {
            return true;
        }
        rebuild();
        auto snapshot{ parse_chat_completion(m_root) };
        if (!snapshot) {
            return std::unexpected{ snapshot.error() };
        }
        snapshot->_request_id = m_request_id;
        auto result{ m_on_event(ChatCompletionStreamEvent{ std::move(type), choice, tool, std::move(delta), std::move(data), std::move(*snapshot) }) };
        if (result && !*result) {
            m_cancelled = true;
        }
        return result;
    }

    /**
     * @brief Emit done events for a tool exactly once.
     * @param choice Choice index.
     * @param index Tool index.
     * @return Whether the consumer wants to continue.
     */
    auto ChatStream::toolDone(std::int64_t choice, std::int64_t index) -> Result<bool> {
        auto& candidate{ m_candidates.at(choice) };
        if (!candidate.tools_done.insert(index).second) {
            return true;
        }
        auto const& tool{ candidate.tools.at(index) };
        auto const custom{ tool["type"] == "custom" };
        auto const& value{ tool[custom ? "custom" : "function"][custom ? "input" : "arguments"] };
        return emit(custom ? "tool_calls.custom.input.done" : "tool_calls.function.arguments.done", choice, value.isString() ? value.asString() : "", tool, index);
    }

    /**
     * @brief Merge a chunk and emit associated events.
     * @param chunk Decoded chunk.
     * @return Whether to continue reading.
     */
    auto ChatStream::accumulate(ChatCompletionChunk chunk) -> Result<bool> {
        m_received = true;
        for (auto const& key : chunk.raw.getMemberNames()) {
            if (key != "choices" && key != "object" && !chunk.raw[key].isNull()) {
                m_root[key] = chunk.raw[key];
            }
        }
        // Accumulate all candidates before callbacks so every event snapshot is coherent.
        for (auto const& choice : chunk.choices) {
            if (choice.index < 0) {
                return invalid_stream("negative stream choice index");
            }
            auto& candidate{ m_candidates[choice.index] };
            auto& value{ candidate.value };
            value["index"] = Json::Int64{ choice.index };
            if (!value["message"].isObject()) {
                value["message"]["role"] = "assistant";
                value["message"]["content"] = Json::nullValue;
            }
            auto& message{ value["message"] };
            for (auto const& key : choice.delta.raw.getMemberNames()) {
                if (key != "tool_calls") {
                    merge_delta(message[key], choice.delta.raw[key]);
                }
            }
            if (choice.delta.tool_calls) {
                for (auto const& tool : *choice.delta.tool_calls) {
                    if (tool.index < 0) {
                        return invalid_stream("negative stream tool index");
                    }
                    auto fragment{ tool.raw };
                    fragment.removeMember("index");
                    auto& accumulated{ candidate.tools[tool.index] };
                    merge_delta(accumulated, fragment);
                    if (!accumulated["id"].isString()) {
                        accumulated["id"] = "";
                    }
                    if (!accumulated["type"].isString()) {
                        accumulated["type"] = "";
                    }
                    if (accumulated["type"] == "function" || accumulated["type"] == "custom") {
                        bool const custom{ accumulated["type"] == "custom" };
                        auto& payload{ accumulated[custom ? "custom" : "function"] };
                        if (!payload["name"].isString()) {
                            payload["name"] = "";
                        }
                        if (!payload[custom ? "input" : "arguments"].isString()) {
                            payload[custom ? "input" : "arguments"] = "";
                        }
                    }
                }
            }
            if (choice.logprobs) {
                merge_delta(value["logprobs"], choice.logprobs->raw);
            }
            if (choice.finish_reason) {
                value["finish_reason"] = *choice.finish_reason;
            }
        }
        rebuild();
        if (m_on_chunk) {
            auto result{ m_on_chunk(chunk) };
            if (!result) {
                return result;
            }
            if (!*result) {
                m_cancelled = true;
                return false;
            }
        }
        auto event{ emit("chunk", 0, {}, chunk.raw) };
        if (!event || !*event) {
            return event;
        }
        for (auto const& choice : chunk.choices) {
            auto& candidate{ m_candidates.at(choice.index) };
            auto const& message{ candidate.value["message"] };
            for (auto const* field : { "content", "refusal", "reasoning_content" }) {
                if (choice.delta.raw[field].isString()) {
                    event = emit(std::string{ field } + ".delta", choice.index, choice.delta.raw[field].asString(), message[field]);
                    if (!event || !*event) {
                        return event;
                    }
                }
            }
            if (choice.delta.tool_calls) {
                for (auto const& tool : *choice.delta.tool_calls) {
                    bool const custom{ candidate.tools.at(tool.index)["type"] == "custom" };
                    auto const& fragment{ tool.raw[custom ? "custom" : "function"][custom ? "input" : "arguments"] };
                    if (fragment.isString()) {
                        event = emit(custom ? "tool_calls.custom.input.delta" : "tool_calls.function.arguments.delta", choice.index, fragment.asString(), candidate.tools.at(tool.index), tool.index);
                        if (!event || !*event) {
                            return event;
                        }
                    }
                }
            }
            if (choice.logprobs) {
                for (auto const* field : { "content", "refusal" }) {
                    if (choice.logprobs->raw[field].isArray()) {
                        event = emit(std::string{ "logprobs." } + field + ".delta", choice.index, {}, choice.logprobs->raw[field]);
                        if (!event || !*event) {
                            return event;
                        }
                    }
                }
            }
            if (choice.finish_reason && !candidate.done) {
                candidate.done = true;
                for (auto const* field : { "content", "refusal", "reasoning_content" }) {
                    if (message[field].isString()) {
                        event = emit(std::string{ field } + ".done", choice.index, message[field].asString(), message[field]);
                        if (!event || !*event) {
                            return event;
                        }
                    }
                }
                for (auto const& [index, value] : candidate.tools) {
                    event = toolDone(choice.index, index);
                    if (!event || !*event) {
                        return event;
                    }
                }
                for (auto const* field : { "content", "refusal" }) {
                    auto const& probs{ candidate.value["logprobs"][field] };
                    if (probs.isArray()) {
                        event = emit(std::string{ "logprobs." } + field + ".done", choice.index, {}, probs);
                        if (!event || !*event) {
                            return event;
                        }
                    }
                }
            }
        }
        return true;
    }

    /**
     * @brief Return the final completion after DONE, or an explicitly cancelled snapshot.
     * @return Accumulated completion or a truncated/invalid stream error.
     */
    auto ChatStream::Finish() -> Result<ChatCompletion> {
        if (!m_cancelled && (!m_done || !m_received)) {
            return invalid_stream("chat stream ended before a completion and [DONE]");
        }
        rebuild();
        auto completion{ parse_chat_completion(m_root) };
        if (!completion) {
            return completion;
        }
        completion->_request_id = m_request_id;
        auto params{ m_params };
        if (params.response_format && !params.response_format->isNull() && !params.response_format->isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "response_format must be an object or null" } };
        }
        params.tools = std::vector<ChatCompletionToolParam>{};
        if (m_params.tools) {
            for (auto const& tool : *m_params.tools) {
                if (tool.type == "function" && tool.function.isObject() && tool.function["strict"] == true) {
                    params.tools->push_back(tool);
                }
            }
        }
        bool const structured{ params.response_format && (*params.response_format)["type"] == "json_schema" };
        if (!m_cancelled && (structured || !params.tools->empty())) {
            auto parsed{ parse_structured(*completion, params) };
            if (!parsed) {
                return std::unexpected{ parsed.error() };
            }
        }
        return completion;
    }

} // namespace drogon_openai::details
