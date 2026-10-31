/**
 * @file resource_responses.cpp
 * @brief Implement responses resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/responses.hpp"

namespace drogon_openai::types::responses {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto InputTokenCountResponse::FromJson(Json::Value const& value) -> Result<InputTokenCountResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        InputTokenCountResponse result;
        result.raw = value;
        if (value.isMember("input_tokens")) {
            if (!details::resource_read(value["input_tokens"], result.input_tokens)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid InputTokenCountResponse.input_tokens", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid InputTokenCountResponse.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto Response::FromJson(Json::Value const& value) -> Result<Response> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        Response result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("access_programs") && !value["access_programs"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["access_programs"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.access_programs", .body = value.toStyledString() } };
            }
            result.access_programs = std::move(field);
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("error") && !value["error"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["error"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.error", .body = value.toStyledString() } };
            }
            result.error = std::move(field);
        }
        if (value.isMember("incomplete_details") && !value["incomplete_details"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["incomplete_details"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.incomplete_details", .body = value.toStyledString() } };
            }
            result.incomplete_details = std::move(field);
        }
        if (value.isMember("instructions") && !value["instructions"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["instructions"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.instructions", .body = value.toStyledString() } };
            }
            result.instructions = std::move(field);
        }
        if (value.isMember("metadata") && !value["metadata"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["metadata"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.metadata", .body = value.toStyledString() } };
            }
            result.metadata = std::move(field);
        }
        if (value.isMember("model")) {
            if (!details::resource_read(value["model"], result.model)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.model", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("output")) {
            if (!details::resource_read(value["output"], result.output)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.output", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("parallel_tool_calls")) {
            if (!details::resource_read(value["parallel_tool_calls"], result.parallel_tool_calls)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.parallel_tool_calls", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("temperature") && !value["temperature"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["temperature"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.temperature", .body = value.toStyledString() } };
            }
            result.temperature = std::move(field);
        }
        if (value.isMember("tool_choice")) {
            if (!details::resource_read(value["tool_choice"], result.tool_choice)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.tool_choice", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("tools")) {
            if (!details::resource_read(value["tools"], result.tools)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.tools", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("top_p") && !value["top_p"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["top_p"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.top_p", .body = value.toStyledString() } };
            }
            result.top_p = std::move(field);
        }
        if (value.isMember("background") && !value["background"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["background"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.background", .body = value.toStyledString() } };
            }
            result.background = std::move(field);
        }
        if (value.isMember("completed_at") && !value["completed_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["completed_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.completed_at", .body = value.toStyledString() } };
            }
            result.completed_at = std::move(field);
        }
        if (value.isMember("conversation") && !value["conversation"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["conversation"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.conversation", .body = value.toStyledString() } };
            }
            result.conversation = std::move(field);
        }
        if (value.isMember("max_output_tokens") && !value["max_output_tokens"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["max_output_tokens"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.max_output_tokens", .body = value.toStyledString() } };
            }
            result.max_output_tokens = std::move(field);
        }
        if (value.isMember("max_tool_calls") && !value["max_tool_calls"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["max_tool_calls"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.max_tool_calls", .body = value.toStyledString() } };
            }
            result.max_tool_calls = std::move(field);
        }
        if (value.isMember("moderation") && !value["moderation"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["moderation"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.moderation", .body = value.toStyledString() } };
            }
            result.moderation = std::move(field);
        }
        if (value.isMember("previous_response_id") && !value["previous_response_id"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["previous_response_id"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.previous_response_id", .body = value.toStyledString() } };
            }
            result.previous_response_id = std::move(field);
        }
        if (value.isMember("prompt") && !value["prompt"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["prompt"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.prompt", .body = value.toStyledString() } };
            }
            result.prompt = std::move(field);
        }
        if (value.isMember("prompt_cache_diagnostics") && !value["prompt_cache_diagnostics"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["prompt_cache_diagnostics"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.prompt_cache_diagnostics", .body = value.toStyledString() } };
            }
            result.prompt_cache_diagnostics = std::move(field);
        }
        if (value.isMember("prompt_cache_key") && !value["prompt_cache_key"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["prompt_cache_key"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.prompt_cache_key", .body = value.toStyledString() } };
            }
            result.prompt_cache_key = std::move(field);
        }
        if (value.isMember("prompt_cache_options") && !value["prompt_cache_options"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["prompt_cache_options"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.prompt_cache_options", .body = value.toStyledString() } };
            }
            result.prompt_cache_options = std::move(field);
        }
        if (value.isMember("prompt_cache_retention") && !value["prompt_cache_retention"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["prompt_cache_retention"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.prompt_cache_retention", .body = value.toStyledString() } };
            }
            result.prompt_cache_retention = std::move(field);
        }
        if (value.isMember("reasoning") && !value["reasoning"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["reasoning"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.reasoning", .body = value.toStyledString() } };
            }
            result.reasoning = std::move(field);
        }
        if (value.isMember("safety_identifier") && !value["safety_identifier"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["safety_identifier"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.safety_identifier", .body = value.toStyledString() } };
            }
            result.safety_identifier = std::move(field);
        }
        if (value.isMember("service_tier") && !value["service_tier"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["service_tier"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.service_tier", .body = value.toStyledString() } };
            }
            result.service_tier = std::move(field);
        }
        if (value.isMember("status") && !value["status"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["status"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.status", .body = value.toStyledString() } };
            }
            result.status = std::move(field);
        }
        if (value.isMember("text") && !value["text"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["text"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.text", .body = value.toStyledString() } };
            }
            result.text = std::move(field);
        }
        if (value.isMember("tool_usage") && !value["tool_usage"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["tool_usage"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.tool_usage", .body = value.toStyledString() } };
            }
            result.tool_usage = std::move(field);
        }
        if (value.isMember("top_logprobs") && !value["top_logprobs"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["top_logprobs"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.top_logprobs", .body = value.toStyledString() } };
            }
            result.top_logprobs = std::move(field);
        }
        if (value.isMember("truncation") && !value["truncation"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["truncation"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.truncation", .body = value.toStyledString() } };
            }
            result.truncation = std::move(field);
        }
        if (value.isMember("usage") && !value["usage"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["usage"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.usage", .body = value.toStyledString() } };
            }
            result.usage = std::move(field);
        }
        if (value.isMember("user") && !value["user"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["user"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Response.user", .body = value.toStyledString() } };
            }
            result.user = std::move(field);
        }
        if (result.output.isArray()) {
            for (auto const& item : result.output) {
                if (item.isObject() && item["type"] == "message" && item["content"].isArray()) {
                    for (auto const& part : item["content"]) {
                        if (part.isObject() && part["type"] == "output_text" && part["text"].isString()) {
                            result.output_text += part["text"].asString();
                        }
                    }
                }
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto CompactedResponse::FromJson(Json::Value const& value) -> Result<CompactedResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        CompactedResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid CompactedResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid CompactedResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid CompactedResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("output")) {
            if (!details::resource_read(value["output"], result.output)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid CompactedResponse.output", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("usage")) {
            if (!details::resource_read(value["usage"], result.usage)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid CompactedResponse.usage", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto InputItemListParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (after) {
            payload.query["after"] = details::resource_json(*after);
        }
        if (include) {
            payload.query["include"] = details::resource_json(*include);
        }
        if (limit) {
            payload.query["limit"] = details::resource_json(*limit);
        }
        if (order) {
            payload.query["order"] = details::resource_json(*order);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto InputItemListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto InputTokenCountParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (conversation) {
            payload.body["conversation"] = details::resource_json(*conversation);
        }
        if (input) {
            payload.body["input"] = details::resource_json(*input);
        }
        if (instructions) {
            payload.body["instructions"] = details::resource_json(*instructions);
        }
        if (model) {
            payload.body["model"] = details::resource_json(*model);
        }
        if (parallel_tool_calls) {
            payload.body["parallel_tool_calls"] = details::resource_json(*parallel_tool_calls);
        }
        if (personality) {
            payload.body["personality"] = details::resource_json(*personality);
        }
        if (previous_response_id) {
            payload.body["previous_response_id"] = details::resource_json(*previous_response_id);
        }
        if (reasoning) {
            payload.body["reasoning"] = details::resource_json(*reasoning);
        }
        if (text) {
            payload.body["text"] = details::resource_json(*text);
        }
        if (tool_choice) {
            payload.body["tool_choice"] = details::resource_json(*tool_choice);
        }
        if (tools) {
            payload.body["tools"] = details::resource_json(*tools);
        }
        if (truncation) {
            payload.body["truncation"] = details::resource_json(*truncation);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto InputTokenCountParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ResponsesCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (access_programs) {
            payload.body["access_programs"] = details::resource_json(*access_programs);
        }
        if (background) {
            payload.body["background"] = details::resource_json(*background);
        }
        if (context_management) {
            payload.body["context_management"] = details::resource_json(*context_management);
        }
        if (conversation) {
            payload.body["conversation"] = details::resource_json(*conversation);
        }
        if (include) {
            payload.body["include"] = details::resource_json(*include);
        }
        if (input) {
            payload.body["input"] = details::resource_json(*input);
        }
        if (instructions) {
            payload.body["instructions"] = details::resource_json(*instructions);
        }
        if (max_output_tokens) {
            payload.body["max_output_tokens"] = details::resource_json(*max_output_tokens);
        }
        if (max_tool_calls) {
            payload.body["max_tool_calls"] = details::resource_json(*max_tool_calls);
        }
        if (metadata) {
            payload.body["metadata"] = details::resource_json(*metadata);
        }
        if (model) {
            payload.body["model"] = details::resource_json(*model);
        }
        if (moderation) {
            payload.body["moderation"] = details::resource_json(*moderation);
        }
        if (parallel_tool_calls) {
            payload.body["parallel_tool_calls"] = details::resource_json(*parallel_tool_calls);
        }
        if (previous_response_id) {
            payload.body["previous_response_id"] = details::resource_json(*previous_response_id);
        }
        if (prompt) {
            payload.body["prompt"] = details::resource_json(*prompt);
        }
        if (prompt_cache_key) {
            payload.body["prompt_cache_key"] = details::resource_json(*prompt_cache_key);
        }
        if (prompt_cache_options) {
            payload.body["prompt_cache_options"] = details::resource_json(*prompt_cache_options);
        }
        if (prompt_cache_retention) {
            payload.body["prompt_cache_retention"] = details::resource_json(*prompt_cache_retention);
        }
        if (reasoning) {
            payload.body["reasoning"] = details::resource_json(*reasoning);
        }
        if (safety_identifier) {
            payload.body["safety_identifier"] = details::resource_json(*safety_identifier);
        }
        if (service_tier) {
            payload.body["service_tier"] = details::resource_json(*service_tier);
        }
        if (store) {
            payload.body["store"] = details::resource_json(*store);
        }
        if (stream) {
            payload.body["stream"] = details::resource_json(*stream);
        }
        if (stream_options) {
            payload.body["stream_options"] = details::resource_json(*stream_options);
        }
        if (temperature) {
            payload.body["temperature"] = details::resource_json(*temperature);
        }
        if (text) {
            payload.body["text"] = details::resource_json(*text);
        }
        if (tool_choice) {
            payload.body["tool_choice"] = details::resource_json(*tool_choice);
        }
        if (tools) {
            payload.body["tools"] = details::resource_json(*tools);
        }
        if (top_logprobs) {
            payload.body["top_logprobs"] = details::resource_json(*top_logprobs);
        }
        if (top_p) {
            payload.body["top_p"] = details::resource_json(*top_p);
        }
        if (truncation) {
            payload.body["truncation"] = details::resource_json(*truncation);
        }
        if (user) {
            payload.body["user"] = details::resource_json(*user);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ResponsesCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ResponseRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (include) {
            payload.query["include"] = details::resource_json(*include);
        }
        if (include_obfuscation) {
            payload.query["include_obfuscation"] = details::resource_json(*include_obfuscation);
        }
        if (starting_after) {
            payload.query["starting_after"] = details::resource_json(*starting_after);
        }
        if (stream) {
            payload.query["stream"] = details::resource_json(*stream);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ResponseRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ResponsesDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ResponsesDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ResponsesCancelParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ResponsesCancelParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ResponseCompactParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["model"] = details::resource_json(model);
        if (input) {
            payload.body["input"] = details::resource_json(*input);
        }
        if (instructions) {
            payload.body["instructions"] = details::resource_json(*instructions);
        }
        if (previous_response_id) {
            payload.body["previous_response_id"] = details::resource_json(*previous_response_id);
        }
        if (prompt_cache_key) {
            payload.body["prompt_cache_key"] = details::resource_json(*prompt_cache_key);
        }
        if (prompt_cache_options) {
            payload.body["prompt_cache_options"] = details::resource_json(*prompt_cache_options);
        }
        if (prompt_cache_retention) {
            payload.body["prompt_cache_retention"] = details::resource_json(*prompt_cache_retention);
        }
        if (service_tier) {
            payload.body["service_tier"] = details::resource_json(*service_tier);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ResponseCompactParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::responses

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    ResponsesInputItems::ResponsesInputItems(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief GET /responses/{response_id}/input_items.
     * @param response_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ResponsesInputItems::List(
        std::string response_id,
        types::responses::InputItemListParams params
    ) const -> drogon::Task<Result<Page<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses/{response_id}/input_items", { { "response_id", response_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<Json::Value>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    ResponsesInputTokens::ResponsesInputTokens(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /responses/input_tokens.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ResponsesInputTokens::Count(
        types::responses::InputTokenCountParams params
    ) const -> drogon::Task<Result<types::responses::InputTokenCountResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses/input_tokens", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::responses::InputTokenCountResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /responses/input_tokens.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ResponsesInputTokens::CountRaw(
        types::responses::InputTokenCountParams params
    ) const -> drogon::Task<Result<RawResponse<types::responses::InputTokenCountResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses/input_tokens", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::responses::InputTokenCountResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Responses::Responses(std::shared_ptr<Transport> transport) : m_transport{ transport }, input_items{ transport }, input_tokens{ transport } {}

    /**
     * @brief POST /responses.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Responses::Create(
        types::responses::ResponsesCreateParams params
    ) const -> drogon::Task<Result<types::responses::Response>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        if ((payload.extra_body.isObject() ? payload.extra_body.get("stream", payload.body.get("stream", false)) : payload.body.get("stream", false)) == true) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "use CreateStream for SSE delivery" } };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::responses::Response>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /responses.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Responses::CreateRaw(
        types::responses::ResponsesCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::responses::Response>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        if ((payload.extra_body.isObject() ? payload.extra_body.get("stream", payload.body.get("stream", false)) : payload.body.get("stream", false)) == true) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "use CreateStream for SSE delivery" } };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::responses::Response>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Stream SSE from POST /responses.
     * @param params Owned SDK parameters.
     * @param callback Event consumer; false stops delivery.
     * @return Final event and stream metadata or an error.
     */
    auto Responses::CreateStream(
        types::responses::ResponsesCreateParams params,
        ApiEventCallback callback
    ) const -> drogon::Task<Result<ApiStreamResult>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.body["stream"] = true;
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_stream(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}, std::move(callback));
    }

    /**
     * @brief GET /responses/{response_id}.
     * @param response_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Responses::Retrieve(
        std::string response_id,
        types::responses::ResponseRetrieveParams params
    ) const -> drogon::Task<Result<types::responses::Response>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses/{response_id}", { { "response_id", response_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        if (payload.query.get("stream", false) == true) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "use RetrieveStream for SSE delivery" } };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::responses::Response>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /responses/{response_id}.
     * @param response_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Responses::RetrieveRaw(
        std::string response_id,
        types::responses::ResponseRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::responses::Response>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses/{response_id}", { { "response_id", response_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        if (payload.query.get("stream", false) == true) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "use RetrieveStream for SSE delivery" } };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::responses::Response>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Stream SSE from GET /responses/{response_id}.
     * @param response_id Owned path identifier.
     * @param params Owned SDK parameters.
     * @param callback Event consumer; false stops delivery.
     * @return Final event and stream metadata or an error.
     */
    auto Responses::RetrieveStream(
        std::string response_id,
        types::responses::ResponseRetrieveParams params,
        ApiEventCallback callback
    ) const -> drogon::Task<Result<ApiStreamResult>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.query["stream"] = true;
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses/{response_id}", { { "response_id", response_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_stream(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, std::move(callback));
    }

    /**
     * @brief DELETE /responses/{response_id}.
     * @param response_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Responses::Delete(
        std::string response_id,
        types::responses::ResponsesDeleteParams params
    ) const -> drogon::Task<Result<void>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses/{response_id}", { { "response_id", response_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), { { "Accept", "*/*" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        co_return {};
    }

    /**
     * @brief DELETE /responses/{response_id}.
     * @param response_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Responses::DeleteRaw(
        std::string response_id,
        types::responses::ResponsesDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses/{response_id}", { { "response_id", response_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), { { "Accept", "*/*" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<Json::Value>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /responses/{response_id}/cancel.
     * @param response_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Responses::Cancel(
        std::string response_id,
        types::responses::ResponsesCancelParams params
    ) const -> drogon::Task<Result<types::responses::Response>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses/{response_id}/cancel", { { "response_id", response_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::responses::Response>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /responses/{response_id}/cancel.
     * @param response_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Responses::CancelRaw(
        std::string response_id,
        types::responses::ResponsesCancelParams params
    ) const -> drogon::Task<Result<RawResponse<types::responses::Response>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses/{response_id}/cancel", { { "response_id", response_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::responses::Response>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /responses/compact.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Responses::Compact(
        types::responses::ResponseCompactParams params
    ) const -> drogon::Task<Result<types::responses::CompactedResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses/compact", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::responses::CompactedResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /responses/compact.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Responses::CompactRaw(
        types::responses::ResponseCompactParams params
    ) const -> drogon::Task<Result<RawResponse<types::responses::CompactedResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/responses/compact", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::responses::CompactedResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }
} // namespace drogon_openai::details
