/**
 * @file resource_completions.cpp
 * @brief Implement completions resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/text_completions.hpp"

namespace drogon_openai::types::completions {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto Completion::FromJson(Json::Value const& value) -> Result<Completion> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        Completion result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Completion.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("choices")) {
            if (!details::resource_read(value["choices"], result.choices)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Completion.choices", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created")) {
            if (!details::resource_read(value["created"], result.created)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Completion.created", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("model")) {
            if (!details::resource_read(value["model"], result.model)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Completion.model", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Completion.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("system_fingerprint") && !value["system_fingerprint"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["system_fingerprint"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Completion.system_fingerprint", .body = value.toStyledString() } };
            }
            result.system_fingerprint = std::move(field);
        }
        if (value.isMember("usage") && !value["usage"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["usage"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Completion.usage", .body = value.toStyledString() } };
            }
            result.usage = std::move(field);
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto CompletionsCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["model"] = details::resource_json(model);
        payload.body["prompt"] = details::resource_json(prompt);
        if (best_of) {
            payload.body["best_of"] = details::resource_json(*best_of);
        }
        if (echo) {
            payload.body["echo"] = details::resource_json(*echo);
        }
        if (frequency_penalty) {
            payload.body["frequency_penalty"] = details::resource_json(*frequency_penalty);
        }
        if (logit_bias) {
            payload.body["logit_bias"] = details::resource_json(*logit_bias);
        }
        if (logprobs) {
            payload.body["logprobs"] = details::resource_json(*logprobs);
        }
        if (max_tokens) {
            payload.body["max_tokens"] = details::resource_json(*max_tokens);
        }
        if (n) {
            payload.body["n"] = details::resource_json(*n);
        }
        if (presence_penalty) {
            payload.body["presence_penalty"] = details::resource_json(*presence_penalty);
        }
        if (seed) {
            payload.body["seed"] = details::resource_json(*seed);
        }
        if (stop) {
            payload.body["stop"] = details::resource_json(*stop);
        }
        if (stream) {
            payload.body["stream"] = details::resource_json(*stream);
        }
        if (stream_options) {
            payload.body["stream_options"] = details::resource_json(*stream_options);
        }
        if (suffix) {
            payload.body["suffix"] = details::resource_json(*suffix);
        }
        if (temperature) {
            payload.body["temperature"] = details::resource_json(*temperature);
        }
        if (top_p) {
            payload.body["top_p"] = details::resource_json(*top_p);
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
    auto CompletionsCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::completions

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    TextCompletions::TextCompletions(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /completions.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto TextCompletions::Create(
        types::completions::CompletionsCreateParams params
    ) const -> drogon::Task<Result<types::completions::Completion>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/completions", {}) };
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
        auto parsed{ resource_decode_response<types::completions::Completion>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /completions.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto TextCompletions::CreateRaw(
        types::completions::CompletionsCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::completions::Completion>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/completions", {}) };
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
        auto parsed{ resource_decode_response<types::completions::Completion>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Stream SSE from POST /completions.
     * @param params Owned SDK parameters.
     * @param callback Event consumer; false stops delivery.
     * @return Final event and stream metadata or an error.
     */
    auto TextCompletions::CreateStream(
        types::completions::CompletionsCreateParams params,
        ApiEventCallback callback
    ) const -> drogon::Task<Result<ApiStreamResult>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.body["stream"] = true;
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/completions", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_stream(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}, std::move(callback));
    }
} // namespace drogon_openai::details
