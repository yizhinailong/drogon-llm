/**
 * @file resource_embeddings.cpp
 * @brief Implement embeddings resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/embeddings.hpp"

namespace drogon_openai::types::embeddings {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto CreateEmbeddingResponse::FromJson(Json::Value const& value) -> Result<CreateEmbeddingResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        CreateEmbeddingResponse result;
        result.raw = value;
        if (value.isMember("data")) {
            if (!details::resource_read(value["data"], result.data)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid CreateEmbeddingResponse.data", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("model")) {
            if (!details::resource_read(value["model"], result.model)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid CreateEmbeddingResponse.model", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid CreateEmbeddingResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("usage")) {
            if (!details::resource_read(value["usage"], result.usage)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid CreateEmbeddingResponse.usage", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto EmbeddingCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["input"] = details::resource_json(input);
        payload.body["model"] = details::resource_json(model);
        if (dimensions) {
            payload.body["dimensions"] = details::resource_json(*dimensions);
        }
        if (encoding_format) {
            payload.body["encoding_format"] = details::resource_json(*encoding_format);
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
    auto EmbeddingCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::embeddings

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Embeddings::Embeddings(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /embeddings.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Embeddings::Create(
        types::embeddings::EmbeddingCreateParams params
    ) const -> drogon::Task<Result<types::embeddings::CreateEmbeddingResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/embeddings", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::embeddings::CreateEmbeddingResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /embeddings.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Embeddings::CreateRaw(
        types::embeddings::EmbeddingCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::embeddings::CreateEmbeddingResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/embeddings", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::embeddings::CreateEmbeddingResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }
} // namespace drogon_openai::details
