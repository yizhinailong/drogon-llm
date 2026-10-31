/**
 * @file resource_images.cpp
 * @brief Implement images resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/images.hpp"

namespace drogon_openai::types::images {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto ImagesResponse::FromJson(Json::Value const& value) -> Result<ImagesResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        ImagesResponse result;
        result.raw = value;
        if (value.isMember("created")) {
            if (!details::resource_read(value["created"], result.created)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ImagesResponse.created", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("background") && !value["background"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["background"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ImagesResponse.background", .body = value.toStyledString() } };
            }
            result.background = std::move(field);
        }
        if (value.isMember("data") && !value["data"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["data"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ImagesResponse.data", .body = value.toStyledString() } };
            }
            result.data = std::move(field);
        }
        if (value.isMember("output_format") && !value["output_format"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["output_format"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ImagesResponse.output_format", .body = value.toStyledString() } };
            }
            result.output_format = std::move(field);
        }
        if (value.isMember("quality") && !value["quality"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["quality"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ImagesResponse.quality", .body = value.toStyledString() } };
            }
            result.quality = std::move(field);
        }
        if (value.isMember("safety_identifier") && !value["safety_identifier"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["safety_identifier"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ImagesResponse.safety_identifier", .body = value.toStyledString() } };
            }
            result.safety_identifier = std::move(field);
        }
        if (value.isMember("size") && !value["size"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["size"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ImagesResponse.size", .body = value.toStyledString() } };
            }
            result.size = std::move(field);
        }
        if (value.isMember("usage") && !value["usage"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["usage"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ImagesResponse.usage", .body = value.toStyledString() } };
            }
            result.usage = std::move(field);
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ImageCreateVariationParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        details::resource_file(payload, "image", image);
        if (model) {
            payload.body["model"] = details::resource_json(*model);
        }
        if (n) {
            payload.body["n"] = details::resource_json(*n);
        }
        if (response_format) {
            payload.body["response_format"] = details::resource_json(*response_format);
        }
        if (size) {
            payload.body["size"] = details::resource_json(*size);
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
    auto ImageCreateVariationParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ImagesEditParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        details::resource_file(payload, "image", image);
        payload.body["prompt"] = details::resource_json(prompt);
        if (background) {
            payload.body["background"] = details::resource_json(*background);
        }
        if (input_fidelity) {
            payload.body["input_fidelity"] = details::resource_json(*input_fidelity);
        }
        if (markup) {
            details::resource_file(payload, "markup", *markup);
        }
        if (mask) {
            details::resource_file(payload, "mask", *mask);
        }
        if (model) {
            payload.body["model"] = details::resource_json(*model);
        }
        if (n) {
            payload.body["n"] = details::resource_json(*n);
        }
        if (output_compression) {
            payload.body["output_compression"] = details::resource_json(*output_compression);
        }
        if (output_format) {
            payload.body["output_format"] = details::resource_json(*output_format);
        }
        if (partial_images) {
            payload.body["partial_images"] = details::resource_json(*partial_images);
        }
        if (quality) {
            payload.body["quality"] = details::resource_json(*quality);
        }
        if (response_format) {
            payload.body["response_format"] = details::resource_json(*response_format);
        }
        if (safety_identifier) {
            payload.body["safety_identifier"] = details::resource_json(*safety_identifier);
        }
        if (size) {
            payload.body["size"] = details::resource_json(*size);
        }
        if (stream) {
            payload.body["stream"] = details::resource_json(*stream);
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
    auto ImagesEditParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ImagesGenerateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["prompt"] = details::resource_json(prompt);
        if (background) {
            payload.body["background"] = details::resource_json(*background);
        }
        if (model) {
            payload.body["model"] = details::resource_json(*model);
        }
        if (moderation) {
            payload.body["moderation"] = details::resource_json(*moderation);
        }
        if (n) {
            payload.body["n"] = details::resource_json(*n);
        }
        if (output_compression) {
            payload.body["output_compression"] = details::resource_json(*output_compression);
        }
        if (output_format) {
            payload.body["output_format"] = details::resource_json(*output_format);
        }
        if (partial_images) {
            payload.body["partial_images"] = details::resource_json(*partial_images);
        }
        if (quality) {
            payload.body["quality"] = details::resource_json(*quality);
        }
        if (response_format) {
            payload.body["response_format"] = details::resource_json(*response_format);
        }
        if (safety_identifier) {
            payload.body["safety_identifier"] = details::resource_json(*safety_identifier);
        }
        if (size) {
            payload.body["size"] = details::resource_json(*size);
        }
        if (stream) {
            payload.body["stream"] = details::resource_json(*stream);
        }
        if (style) {
            payload.body["style"] = details::resource_json(*style);
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
    auto ImagesGenerateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::images

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Images::Images(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /images/variations.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Images::CreateVariation(
        types::images::ImageCreateVariationParams params
    ) const -> drogon::Task<Result<types::images::ImagesResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/images/variations", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::images::ImagesResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /images/variations.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Images::CreateVariationRaw(
        types::images::ImageCreateVariationParams params
    ) const -> drogon::Task<Result<RawResponse<types::images::ImagesResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/images/variations", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::images::ImagesResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /images/edits.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Images::Edit(
        types::images::ImagesEditParams params
    ) const -> drogon::Task<Result<types::images::ImagesResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/images/edits", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        if ((payload.extra_body.isObject() ? payload.extra_body.get("stream", payload.body.get("stream", false)) : payload.body.get("stream", false)) == true) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "use EditStream for SSE delivery" } };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::images::ImagesResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /images/edits.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Images::EditRaw(
        types::images::ImagesEditParams params
    ) const -> drogon::Task<Result<RawResponse<types::images::ImagesResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/images/edits", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        if ((payload.extra_body.isObject() ? payload.extra_body.get("stream", payload.body.get("stream", false)) : payload.body.get("stream", false)) == true) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "use EditStream for SSE delivery" } };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::images::ImagesResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Stream SSE from POST /images/edits.
     * @param params Owned SDK parameters.
     * @param callback Event consumer; false stops delivery.
     * @return Final event and stream metadata or an error.
     */
    auto Images::EditStream(
        types::images::ImagesEditParams params,
        ApiEventCallback callback
    ) const -> drogon::Task<Result<ApiStreamResult>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.body["stream"] = true;
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/images/edits", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_stream(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}, std::move(callback));
    }

    /**
     * @brief POST /images/generations.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Images::Generate(
        types::images::ImagesGenerateParams params
    ) const -> drogon::Task<Result<types::images::ImagesResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/images/generations", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        if ((payload.extra_body.isObject() ? payload.extra_body.get("stream", payload.body.get("stream", false)) : payload.body.get("stream", false)) == true) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "use GenerateStream for SSE delivery" } };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::images::ImagesResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /images/generations.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Images::GenerateRaw(
        types::images::ImagesGenerateParams params
    ) const -> drogon::Task<Result<RawResponse<types::images::ImagesResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/images/generations", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        if ((payload.extra_body.isObject() ? payload.extra_body.get("stream", payload.body.get("stream", false)) : payload.body.get("stream", false)) == true) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "use GenerateStream for SSE delivery" } };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::images::ImagesResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Stream SSE from POST /images/generations.
     * @param params Owned SDK parameters.
     * @param callback Event consumer; false stops delivery.
     * @return Final event and stream metadata or an error.
     */
    auto Images::GenerateStream(
        types::images::ImagesGenerateParams params,
        ApiEventCallback callback
    ) const -> drogon::Task<Result<ApiStreamResult>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.body["stream"] = true;
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/images/generations", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_stream(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}, std::move(callback));
    }
} // namespace drogon_openai::details
