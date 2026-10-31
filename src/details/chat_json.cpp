/**
 * @file chat_json.cpp
 * @brief Serialize and decode chat messages, completions, and stream chunks.
 */
#include "details/chat_json.hpp"

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <utility>

#include "details/transport.hpp"

namespace {

    /**
     * @brief Check the wire representation of a signed 64-bit integer.
     * @param value JSON field.
     * @return Whether the value is an integral number representable by the public API.
     */
    auto is_integer(Json::Value const& value) -> bool {
        return value.isIntegral() && value.isInt64();
    }

    /**
     * @brief Validate present, non-null object fields without inspecting vendor fields.
     * @tparam Predicate Field validator.
     * @param value Object containing the fields.
     * @param names Known field names.
     * @param predicate Validator for each present value.
     * @return Whether every present field has the expected shape.
     */
    template <typename Predicate>
    auto fields_match(
        Json::Value const& value,
        std::initializer_list<char const*> names,
        Predicate predicate
    ) -> bool {
        return std::ranges::all_of(names, [&](auto name) {
            return value[name].isNull() || std::invoke(predicate, value[name]);
        });
    }

    /**
     * @brief Validate a nullable array and each of its entries.
     * @tparam Predicate Entry validator.
     * @param value Optional array.
     * @param predicate Validator for each entry.
     * @return Whether the array is null or contains only valid entries.
     */
    template <typename Predicate>
    auto array_matches(Json::Value const& value, Predicate predicate) -> bool {
        return value.isNull() || (value.isArray() && std::ranges::all_of(value, predicate));
    }

    /**
     * @brief Validate a tool invocation before reading nested objects.
     * @param value Tool invocation.
     * @return Whether its known fields are usable; future tool types retain their raw JSON.
     */
    auto valid_tool_call(Json::Value const& value) -> bool {
        if (!value.isObject() || !value["id"].isString() || !value["type"].isString()) {
            return false;
        }
        if (value["type"] == "function") {
            return value["function"].isObject() && value["function"]["name"].isString() && value["function"]["arguments"].isString();
        }
        if (value["type"] == "custom") {
            return value["custom"].isObject() && value["custom"]["name"].isString() && value["custom"]["input"].isString();
        }
        return true;
    }

    /**
     * @brief Validate token probability records and their byte arrays.
     * @param value Token probability object.
     * @return Whether the token's known fields have the expected types.
     */
    auto valid_token(Json::Value const& value) -> bool {
        return value.isObject() && value["token"].isString() && value["logprob"].isNumeric() && array_matches(value["bytes"], [](auto const& byte) {
                   return byte.isInt() && byte.asInt() >= 0 && byte.asInt() <= 255;
               });
    }

    /**
     * @brief Validate optional candidate log probabilities.
     * @param value Nullable probability object.
     * @return Whether token records and their alternatives can be decoded.
     */
    auto valid_logprobs(Json::Value const& value) -> bool {
        return value.isNull() || (value.isObject() && fields_match(value, { "content", "refusal" }, [](auto const& tokens) {
                                      return array_matches(tokens, [](auto const& token) {
                                          return valid_token(token) && array_matches(token["top_logprobs"], valid_token);
                                      });
                                  }));
    }

    /**
     * @brief Validate token counts while retaining additional provider counters.
     * @param value Nullable usage object.
     * @return Whether all known counters and breakdowns are integers.
     */
    auto valid_usage(Json::Value const& value) -> bool {
        return value.isNull() || (value.isObject() && fields_match(value, { "completion_tokens", "prompt_tokens", "total_tokens" }, is_integer) && fields_match(value, { "completion_tokens_details" }, [](auto const& detail) {
                                      return detail.isObject() && fields_match(detail, { "reasoning_tokens", "accepted_prediction_tokens", "rejected_prediction_tokens", "audio_tokens", "text_tokens" }, is_integer);
                                  }) &&
                                  fields_match(value, { "prompt_tokens_details" }, [](auto const& detail) {
                                      return detail.isObject() && fields_match(detail, { "cached_tokens", "cache_write_tokens", "audio_tokens", "image_tokens", "text_tokens" }, is_integer);
                                  }));
    }

    /**
     * @brief Validate a full assistant message before decoding its known fields.
     * @param value Message object.
     * @return Whether text, tools, citations, and audio have usable representations.
     */
    auto valid_message(Json::Value const& value) -> bool {
        return value.isObject() && fields_match(value, { "role", "content", "reasoning_content", "refusal" }, &Json::Value::isString) && array_matches(value["tool_calls"], valid_tool_call) && array_matches(value["annotations"], [](auto const& annotation) {
                   return annotation.isObject() && fields_match(annotation, { "type" }, &Json::Value::isString) && fields_match(annotation, { "url_citation" }, [](auto const& citation) {
                              return citation.isObject() && fields_match(citation, { "url", "title" }, &Json::Value::isString) && fields_match(citation, { "start_index", "end_index" }, is_integer);
                          });
               }) &&
            fields_match(value, { "audio" }, [](auto const& audio) {
                   return audio.isObject() && fields_match(audio, { "id", "data", "transcript" }, &Json::Value::isString) && fields_match(audio, { "expires_at" }, is_integer);
               });
    }

    /**
     * @brief Validate the common completion and chunk envelope.
     * @param value Response object.
     * @return Whether its known scalar fields, choices, and usage have usable types.
     */
    auto valid_envelope(Json::Value const& value) -> bool {
        return value.isObject() && value["choices"].isArray() && fields_match(value, { "id", "model", "object", "system_fingerprint", "service_tier" }, &Json::Value::isString) && fields_match(value, { "created" }, is_integer) && valid_usage(value["usage"]);
    }

    /**
     * @brief Build a response validation error retaining the complete response.
     * @param root Original response.
     * @param description Invalid response shape.
     * @return Error suitable for propagation through Result.
     */
    auto invalid_response(Json::Value const& root, std::string description) -> drogon_openai::Error {
        return {
            .kind = drogon_openai::ErrorKind::ResponseValidation,
            .message = std::move(description),
            .body = root.toStyledString(),
        };
    }

    /**
     * @brief Read an optional string.
     * @param value JSON field.
     * @return String, or no value for missing/null/non-string fields.
     */
    auto text(Json::Value const& value) -> std::optional<std::string> {
        return value.isString() ? std::optional{ value.asString() } : std::nullopt;
    }

    /**
     * @brief Read an optional integer.
     * @param value JSON field.
     * @return Integer, or no value.
     */
    auto integer(Json::Value const& value) -> std::optional<std::int64_t> {
        return is_integer(value) ? std::optional{ value.asInt64() } : std::nullopt;
    }

    /**
     * @brief Read an optional JSON field.
     * @param value JSON field.
     * @return Field, or no value when null/missing.
     */
    auto json_if(Json::Value const& value) -> std::optional<Json::Value> {
        return value.isNull() ? std::nullopt : std::optional{ value };
    }

    /**
     * @brief Serialize a vector of JSON-compatible scalar values.
     * @tparam T Element type.
     * @param values Elements.
     * @return JSON array.
     */
    template <typename T>
    auto to_json(std::vector<T> const& values) -> Json::Value {
        Json::Value result{ Json::arrayValue };
        for (auto const& value : values) {
            result.append(value);
        }
        return result;
    }

    /**
     * @brief Serialize a tool invocation.
     * @param call Invocation.
     * @return Complete JSON object.
     */
    auto to_json(drogon_openai::ChatCompletionMessageToolCall const& call) -> Json::Value {
        Json::Value result{ call.raw.isObject() ? call.raw : Json::Value{ Json::objectValue } };
        result["id"] = call.id;
        result["type"] = call.type;
        if (call.type == "function") {
            result.removeMember("custom");
            if (!result["function"].isObject()) {
                result["function"] = Json::objectValue;
            }
            result["function"]["name"] = call.function.name;
            result["function"]["arguments"] = call.function.arguments;
        } else if (call.type == "custom") {
            result.removeMember("function");
            if (!result["custom"].isObject()) {
                result["custom"] = Json::objectValue;
            }
            result["custom"]["name"] = call.custom.name;
            result["custom"]["input"] = call.custom.input;
        }
        return result;
    }

    /**
     * @brief Serialize a collection of tool invocations.
     * @param calls Invocations.
     * @return JSON array.
     */
    auto to_json(
        std::vector<drogon_openai::ChatCompletionMessageToolCall> const& calls
    ) -> Json::Value {
        Json::Value result{ Json::arrayValue };
        for (auto const& call : calls) {
            result.append(to_json(call));
        }
        return result;
    }

    /**
     * @brief Set one optional JSON-compatible request field.
     * @tparam T Value type.
     * @param body Destination.
     * @param key Field name.
     * @param value Optional field.
     */
    template <typename T>
    auto set_if(Json::Value& body, char const* key, std::optional<T> const& value) -> void {
        if (value) {
            if constexpr (requires { to_json(*value); }) {
                body[key] = to_json(*value);
            } else {
                body[key] = *value;
            }
        }
    }

    /**
     * @brief Merge JSON object fields, replacing existing members.
     * @param body Destination object.
     * @param extra Fields to merge.
     */
    auto merge(Json::Value& body, Json::Value const& extra) -> void {
        if (extra.isObject()) {
            for (auto const& key : extra.getMemberNames()) {
                body[key] = extra[key];
            }
        }
    }

    /**
     * @brief Decode a tool invocation.
     * @param value Tool call object.
     * @return Function or custom invocation, retaining unknown fields.
     */
    auto tool_call(Json::Value const& value) -> drogon_openai::ChatCompletionMessageToolCall {
        drogon_openai::ChatCompletionMessageToolCall result{
            .id = text(value["id"]).value_or(""),
            .type = text(value["type"]).value_or(""),
            .raw = value,
        };
        if (result.type == "function") {
            result.function.name = value["function"]["name"].asString();
            result.function.arguments = value["function"]["arguments"].asString();
        } else if (result.type == "custom") {
            result.custom.name = value["custom"]["name"].asString();
            result.custom.input = value["custom"]["input"].asString();
        }
        return result;
    }

    /**
     * @brief Decode optional UTF-8 bytes.
     * @param value Byte array.
     * @return Bytes or no value.
     */
    auto token_bytes(Json::Value const& value) -> std::optional<std::vector<int>> {
        if (!value.isArray()) {
            return std::nullopt;
        }
        std::vector<int> result;
        for (auto const& item : value) {
            if (item.isInt()) {
                result.push_back(item.asInt());
            }
        }
        return result;
    }

    /**
     * @brief Decode a token probability array.
     * @param value Token array.
     * @return Token records or no value.
     */
    auto token_logprobs(
        Json::Value const& value
    ) -> std::optional<std::vector<drogon_openai::ChatCompletionTokenLogprob>> {
        if (!value.isArray()) {
            return std::nullopt;
        }
        std::vector<drogon_openai::ChatCompletionTokenLogprob> result;
        for (auto const& item : value) {
            drogon_openai::ChatCompletionTokenLogprob token{
                .token = text(item["token"]).value_or(""),
                .logprob = item["logprob"].isNumeric() ? item["logprob"].asDouble() : 0.0,
                .bytes = token_bytes(item["bytes"]),
                .raw = item,
            };
            if (item["top_logprobs"].isArray()) {
                for (auto const& top : item["top_logprobs"]) {
                    token.top_logprobs.push_back({
                        .token = text(top["token"]).value_or(""),
                        .logprob = top["logprob"].isNumeric() ? top["logprob"].asDouble() : 0.0,
                        .bytes = token_bytes(top["bytes"]),
                        .raw = top,
                    });
                }
            }
            result.push_back(std::move(token));
        }
        return result;
    }

    /**
     * @brief Decode candidate token probabilities.
     * @param value Probability object.
     * @return Decoded probabilities or no value.
     */
    auto logprobs(Json::Value const& value) -> std::optional<drogon_openai::ChatCompletionLogprobs> {
        if (!value.isObject()) {
            return std::nullopt;
        }
        return drogon_openai::ChatCompletionLogprobs{
            .content = token_logprobs(value["content"]),
            .refusal = token_logprobs(value["refusal"]),
            .raw = value,
        };
    }

    /**
     * @brief Decode token usage and optional detailed counters.
     * @param value Usage object.
     * @return Usage or no value.
     */
    auto usage(Json::Value const& value) -> std::optional<drogon_openai::CompletionUsage> {
        if (!value.isObject()) {
            return std::nullopt;
        }
        drogon_openai::CompletionUsage result{
            .completion_tokens = integer(value["completion_tokens"]).value_or(0),
            .prompt_tokens = integer(value["prompt_tokens"]).value_or(0),
            .total_tokens = integer(value["total_tokens"]).value_or(0),
            .raw = value,
        };
        if (auto const& detail{ value["completion_tokens_details"] }; detail.isObject()) {
            result.completion_tokens_details = drogon_openai::CompletionTokensDetails{
                .reasoning_tokens = integer(detail["reasoning_tokens"]),
                .accepted_prediction_tokens = integer(detail["accepted_prediction_tokens"]),
                .rejected_prediction_tokens = integer(detail["rejected_prediction_tokens"]),
                .audio_tokens = integer(detail["audio_tokens"]),
                .text_tokens = integer(detail["text_tokens"]),
                .raw = detail,
            };
        }
        if (auto const& detail{ value["prompt_tokens_details"] }; detail.isObject()) {
            result.prompt_tokens_details = drogon_openai::PromptTokensDetails{
                .cached_tokens = integer(detail["cached_tokens"]),
                .cache_write_tokens = integer(detail["cache_write_tokens"]),
                .audio_tokens = integer(detail["audio_tokens"]),
                .image_tokens = integer(detail["image_tokens"]),
                .text_tokens = integer(detail["text_tokens"]),
                .raw = detail,
            };
        }
        return result;
    }

    /**
     * @brief Decode an assistant message.
     * @param value Message object.
     * @return Typed message, including its raw JSON.
     */
    auto message(Json::Value const& value) -> drogon_openai::ChatCompletionMessage {
        drogon_openai::ChatCompletionMessage result{
            .role = text(value["role"]).value_or(""),
            .content = text(value["content"]),
            .reasoning_content = text(value["reasoning_content"]),
            .refusal = text(value["refusal"]),
            .raw = value,
        };
        if (value["tool_calls"].isArray()) {
            result.tool_calls.emplace();
            for (auto const& item : value["tool_calls"]) {
                result.tool_calls->push_back(tool_call(item));
            }
        }
        if (value["annotations"].isArray()) {
            result.annotations.emplace();
            for (auto const& item : value["annotations"]) {
                result.annotations->push_back({
                    .type = text(item["type"]).value_or(""),
                    .url_citation = item["url_citation"],
                    .raw = item,
                });
            }
        }
        if (auto const& audio{ value["audio"] }; audio.isObject()) {
            result.audio = drogon_openai::ChatCompletionAudio{
                .id = text(audio["id"]).value_or(""),
                .expires_at = integer(audio["expires_at"]).value_or(0),
                .data = text(audio["data"]).value_or(""),
                .transcript = text(audio["transcript"]).value_or(""),
                .raw = audio,
            };
        }
        return result;
    }

} // namespace

namespace drogon_openai {

    /**
     * @brief Preserve this reply as the next assistant request message.
     * @return Conversation message retaining tool calls and vendor fields.
     */
    auto ChatCompletionMessage::ToParam() const -> ChatCompletionMessageParam {
        ChatCompletionMessageParam result{
            .role = role,
            .content = content ? Json::Value{ *content } : Json::Value{},
            .tool_calls = tool_calls,
            .refusal = refusal,
            .reasoning_content = reasoning_content,
            .extra_body = raw,
        };
        if (!result.extra_body.isObject()) {
            result.extra_body = Json::objectValue;
        }
        for (auto const* key : { "role", "content", "tool_calls", "refusal", "reasoning_content", "audio" }) {
            result.extra_body.removeMember(key);
        }
        if (audio) {
            Json::Value reference{ Json::objectValue };
            reference["id"] = audio->id;
            result.audio = reference;
        }
        return result;
    }

} // namespace drogon_openai

namespace drogon_openai::details {

    /**
     * @brief Serialize a chat request, merging vendor fields last.
     * @param params Request fields.
     * @param default_model Configured default model.
     * @return JSON request body.
     */
    auto to_request_body(
        ChatCompletionCreateParams const& params,
        std::string const& default_model
    ) -> Json::Value {
        Json::Value body{ Json::objectValue };
        body["model"] = params.model.value_or(default_model);
        body["messages"] = Json::arrayValue;
        for (auto const& item : params.messages) {
            Json::Value value{ Json::objectValue };
            value["role"] = item.role;
            value["content"] = item.content;
            set_if(value, "name", item.name);
            set_if(value, "tool_call_id", item.tool_call_id);
            set_if(value, "tool_calls", item.tool_calls);
            set_if(value, "refusal", item.refusal);
            set_if(value, "audio", item.audio);
            set_if(value, "reasoning_content", item.reasoning_content);
            merge(value, item.extra_body);
            body["messages"].append(std::move(value));
        }
        set_if(body, "frequency_penalty", params.frequency_penalty);
        set_if(body, "max_tokens", params.max_tokens);
        set_if(body, "presence_penalty", params.presence_penalty);
        set_if(body, "temperature", params.temperature);
        set_if(body, "top_p", params.top_p);
        if (params.tools) {
            body["tools"] = Json::arrayValue;
            for (auto const& tool : *params.tools) {
                Json::Value value{ Json::objectValue };
                value["type"] = tool.type;
                value[tool.type] = tool.type == "custom" ? tool.custom : tool.function;
                body["tools"].append(std::move(value));
            }
        }
        set_if(body, "tool_choice", params.tool_choice);
        set_if(body, "parallel_tool_calls", params.parallel_tool_calls);
        set_if(body, "response_format", params.response_format);
        set_if(body, "stop", params.stop);
        set_if(body, "max_completion_tokens", params.max_completion_tokens);
        set_if(body, "n", params.n);
        set_if(body, "seed", params.seed);
        set_if(body, "logprobs", params.logprobs);
        set_if(body, "top_logprobs", params.top_logprobs);
        set_if(body, "logit_bias", params.logit_bias);
        set_if(body, "reasoning_effort", params.reasoning_effort);
        set_if(body, "verbosity", params.verbosity);
        set_if(body, "safety_identifier", params.safety_identifier);
        set_if(body, "prompt_cache_key", params.prompt_cache_key);
        set_if(body, "user", params.user);
        set_if(body, "metadata", params.metadata);
        set_if(body, "store", params.store);
        set_if(body, "service_tier", params.service_tier);
        set_if(body, "modalities", params.modalities);
        set_if(body, "audio", params.audio);
        set_if(body, "prediction", params.prediction);
        set_if(body, "web_search_options", params.web_search_options);
        set_if(body, "prompt_cache_retention", params.prompt_cache_retention);
        set_if(body, "prompt_cache_options", params.prompt_cache_options);
        set_if(body, "moderation", params.moderation);
        set_if(body, "stream_options", params.stream_options);
        merge(body, params.extra_body);
        return body;
    }

    /**
     * @brief Decode a chat completion, retaining its original JSON.
     * @param root Response body.
     * @return Completion or a response validation error.
     */
    auto parse_chat_completion(Json::Value const& root) -> Result<ChatCompletion> {
        if (!root.isObject() || !root["choices"].isArray()) {
            return std::unexpected{ invalid_response(root, "chat completion has no choices array") };
        }
        if (!valid_envelope(root)) {
            return std::unexpected{ invalid_response(root, "chat completion has invalid envelope fields") };
        }
        ChatCompletion result{
            .id = text(root["id"]).value_or(""),
            .created = integer(root["created"]).value_or(0),
            .model = text(root["model"]).value_or(""),
            .object = text(root["object"]).value_or(""),
            .usage = usage(root["usage"]),
            .system_fingerprint = text(root["system_fingerprint"]),
            .service_tier = text(root["service_tier"]),
            .metadata = json_if(root["metadata"]),
            .moderation = json_if(root["moderation"]),
            .raw = root,
        };
        for (auto const& choice : root["choices"]) {
            if (!choice.isObject() || !choice["message"].isObject()) {
                return std::unexpected{ invalid_response(root, "chat completion has a choice without a message object") };
            }
            if (!valid_message(choice["message"]) || !fields_match(choice, { "finish_reason" }, &Json::Value::isString) || !fields_match(choice, { "index" }, is_integer) || !valid_logprobs(choice["logprobs"])) {
                return std::unexpected{ invalid_response(root, "chat completion has invalid choice, message, or logprobs fields") };
            }
            result.choices.push_back({
                .finish_reason = text(choice["finish_reason"]).value_or(""),
                .index = integer(choice["index"]).value_or(0),
                .message = message(choice["message"]),
                .logprobs = logprobs(choice["logprobs"]),
                .raw = choice,
            });
        }
        return result;
    }

    /**
     * @brief Decode one streaming chat chunk.
     * @param root Chunk body.
     * @return Chunk or a response validation error.
     */
    auto parse_chat_chunk(Json::Value const& root) -> Result<ChatCompletionChunk> {
        if (!valid_envelope(root)) {
            return std::unexpected{ invalid_response(root, "chat chunk has invalid envelope fields or no choices array") };
        }
        ChatCompletionChunk result{
            .id = text(root["id"]).value_or(""),
            .created = integer(root["created"]).value_or(0),
            .model = text(root["model"]).value_or(""),
            .object = text(root["object"]).value_or(""),
            .usage = usage(root["usage"]),
            .raw = root,
        };
        for (auto const& choice : root["choices"]) {
            if (!choice.isObject() || !choice["delta"].isObject() || !is_integer(choice["index"]) || !fields_match(choice, { "finish_reason" }, &Json::Value::isString) || !valid_logprobs(choice["logprobs"])) {
                return std::unexpected{ invalid_response(root, "chat chunk choice needs a valid index, delta, finish_reason, and logprobs") };
            }
            auto const& delta{ choice["delta"] };
            if (!fields_match(delta, { "role", "content", "refusal", "reasoning_content" }, &Json::Value::isString) || !array_matches(delta["tool_calls"], [](auto const& tool) {
                    return tool.isObject() && is_integer(tool["index"]) && fields_match(tool, { "id", "type" }, &Json::Value::isString) && fields_match(tool, { "function" }, [](auto const& function) {
                               return function.isObject() && fields_match(function, { "name", "arguments" }, &Json::Value::isString);
                           }) &&
                        fields_match(tool, { "custom" }, [](auto const& custom) {
                               return custom.isObject() && fields_match(custom, { "name", "input" }, &Json::Value::isString);
                           });
                })) {
                return std::unexpected{ invalid_response(root, "chat chunk has invalid message or tool delta fields") };
            }
            ChatCompletionChunkChoice item{
                .index = choice["index"].asInt64(),
                .delta = {
                    .role = text(delta["role"]),
                    .content = text(delta["content"]),
                    .refusal = text(delta["refusal"]),
                    .reasoning_content = text(delta["reasoning_content"]),
                    .raw = delta,
                },
                .finish_reason = text(choice["finish_reason"]),
                .logprobs = logprobs(choice["logprobs"]),
                .raw = choice,
            };
            if (delta["tool_calls"].isArray()) {
                item.delta.tool_calls.emplace();
                for (auto const& tool : delta["tool_calls"]) {
                    item.delta.tool_calls->push_back({
                        .index = tool["index"].asInt64(),
                        .id = text(tool["id"]),
                        .type = text(tool["type"]),
                        .function = json_if(tool["function"]),
                        .custom = json_if(tool["custom"]),
                        .raw = tool,
                    });
                }
            }
            result.choices.push_back(std::move(item));
        }
        return result;
    }

    /**
     * @brief Check that automatic parsing uses only strict function tools.
     * @param params Request whose tools will be parsed.
     * @return Success or an invalid-request error, before sending the request.
     */
    auto validate_parse_tools(ChatCompletionCreateParams const& params) -> Result<void> {
        if (params.tools) {
            for (auto const& tool : *params.tools) {
                if (tool.type != "function" || !tool.function.isObject() || !tool.function["strict"].isBool() || !tool.function["strict"].asBool()) {
                    return std::unexpected{ Error{
                        .kind = ErrorKind::InvalidRequest,
                        .message = "Parse supports only function tools with strict: true",
                    } };
                }
            }
        }
        return {};
    }

    /**
     * @brief Decode structured response text and strict function arguments.
     * @param completion Completion to enrich.
     * @param params Original request, including schema and tools.
     * @return Success or a parsing/finish-reason error.
     */
    auto parse_structured(
        ChatCompletion& completion,
        ChatCompletionCreateParams const& params
    ) -> Result<void> {
        if (auto validation{ validate_parse_tools(params) }; !validation) {
            return validation;
        }
        if (params.response_format && !params.response_format->isNull() && !params.response_format->isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "response_format must be an object or null" } };
        }
        bool const structured{ params.response_format && (*params.response_format)["type"] == "json_schema" };
        Json::Value schema;
        if (params.schema_validator) {
            if (!structured || !(*params.response_format)["json_schema"].isObject()) {
                return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "schema_validator requires response_format.json_schema.schema" } };
            }
            schema = (*params.response_format)["json_schema"]["schema"];
            if (!schema.isObject() && !schema.isBool()) {
                return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "schema_validator requires an object or boolean JSON Schema" } };
            }
        }
        for (auto& choice : completion.choices) {
            if (choice.finish_reason == "length" || choice.finish_reason == "content_filter") {
                return std::unexpected{ Error{
                    .kind = choice.finish_reason == "length" ? ErrorKind::LengthFinishReason : ErrorKind::ContentFilterFinishReason,
                    .message = "cannot parse completion with finish_reason=" + choice.finish_reason,
                    .body = completion.raw.toStyledString(),
                } };
            }
            choice.message.parsed.reset();
            if (structured && choice.message.content && !choice.message.content->empty() && (!choice.message.refusal || choice.message.refusal->empty())) {
                auto parsed{ decode_json(*choice.message.content) };
                if (!parsed) {
                    return std::unexpected{ parsed.error() };
                }
                if (params.schema_validator) {
                    try {
                        auto validation{ params.schema_validator(*parsed, schema) };
                        if (!validation) {
                            return validation;
                        }
                    } catch (std::invalid_argument const& error) {
                        return std::unexpected{ Error{
                            .kind = ErrorKind::InvalidRequest,
                            .message = "schema validator: " + std::string{ error.what() },
                        } };
                    } catch (std::exception const& error) {
                        return std::unexpected{ invalid_response(completion.raw, "schema validator: " + std::string{ error.what() }) };
                    } catch (...) {
                        return std::unexpected{ invalid_response(completion.raw, "schema validator threw a non-standard exception") };
                    }
                }
                choice.message.parsed = std::move(*parsed);
            }
            if (choice.message.tool_calls) {
                for (auto& call : *choice.message.tool_calls) {
                    call.function.parsed_arguments.reset();
                    if (call.type != "function" || !params.tools) {
                        continue;
                    }
                    for (auto const& tool : *params.tools) {
                        if (tool.function["name"] == call.function.name) {
                            if (call.function.arguments.empty()) {
                                return std::unexpected{ invalid_response(completion.raw, "strict function arguments are empty JSON") };
                            }
                            auto parsed{ decode_json(call.function.arguments) };
                            if (!parsed) {
                                return std::unexpected{ parsed.error() };
                            }
                            call.function.parsed_arguments = std::move(*parsed);
                            break;
                        }
                    }
                }
            }
        }
        return {};
    }

} // namespace drogon_openai::details
