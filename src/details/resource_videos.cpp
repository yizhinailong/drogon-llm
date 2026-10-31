/**
 * @file resource_videos.cpp
 * @brief Implement videos resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/videos.hpp"

namespace drogon_openai::types::videos {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto Video::FromJson(Json::Value const& value) -> Result<Video> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        Video result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Video.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("completed_at") && !value["completed_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["completed_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Video.completed_at", .body = value.toStyledString() } };
            }
            result.completed_at = std::move(field);
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Video.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("error") && !value["error"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["error"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Video.error", .body = value.toStyledString() } };
            }
            result.error = std::move(field);
        }
        if (value.isMember("expires_at") && !value["expires_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["expires_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Video.expires_at", .body = value.toStyledString() } };
            }
            result.expires_at = std::move(field);
        }
        if (value.isMember("model")) {
            if (!details::resource_read(value["model"], result.model)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Video.model", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Video.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("progress")) {
            if (!details::resource_read(value["progress"], result.progress)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Video.progress", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("prompt") && !value["prompt"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["prompt"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Video.prompt", .body = value.toStyledString() } };
            }
            result.prompt = std::move(field);
        }
        if (value.isMember("remixed_from_video_id") && !value["remixed_from_video_id"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["remixed_from_video_id"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Video.remixed_from_video_id", .body = value.toStyledString() } };
            }
            result.remixed_from_video_id = std::move(field);
        }
        if (value.isMember("seconds")) {
            if (!details::resource_read(value["seconds"], result.seconds)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Video.seconds", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("size")) {
            if (!details::resource_read(value["size"], result.size)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Video.size", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Video.status", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto VideoDeleteResponse::FromJson(Json::Value const& value) -> Result<VideoDeleteResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        VideoDeleteResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VideoDeleteResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("deleted")) {
            if (!details::resource_read(value["deleted"], result.deleted)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VideoDeleteResponse.deleted", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VideoDeleteResponse.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto VideoCreateCharacterResponse::FromJson(Json::Value const& value) -> Result<VideoCreateCharacterResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        VideoCreateCharacterResponse result;
        result.raw = value;
        if (value.isMember("id") && !value["id"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["id"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VideoCreateCharacterResponse.id", .body = value.toStyledString() } };
            }
            result.id = std::move(field);
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VideoCreateCharacterResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("name") && !value["name"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["name"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VideoCreateCharacterResponse.name", .body = value.toStyledString() } };
            }
            result.name = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto VideoGetCharacterResponse::FromJson(Json::Value const& value) -> Result<VideoGetCharacterResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        VideoGetCharacterResponse result;
        result.raw = value;
        if (value.isMember("id") && !value["id"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["id"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VideoGetCharacterResponse.id", .body = value.toStyledString() } };
            }
            result.id = std::move(field);
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VideoGetCharacterResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("name") && !value["name"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["name"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VideoGetCharacterResponse.name", .body = value.toStyledString() } };
            }
            result.name = std::move(field);
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VideoCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["prompt"] = details::resource_json(prompt);
        if (input_reference) {
            details::resource_file(payload, "input_reference", *input_reference);
        }
        if (model) {
            payload.body["model"] = details::resource_json(*model);
        }
        if (seconds) {
            payload.body["seconds"] = details::resource_json(*seconds);
        }
        if (size) {
            payload.body["size"] = details::resource_json(*size);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VideoCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VideosRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VideosRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VideoListParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (after) {
            payload.query["after"] = details::resource_json(*after);
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
    auto VideoListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VideosDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VideosDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VideoCreateCharacterParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["name"] = details::resource_json(name);
        details::resource_file(payload, "video", video);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VideoCreateCharacterParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VideoDownloadContentParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (variant) {
            payload.query["variant"] = details::resource_json(*variant);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VideoDownloadContentParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VideoEditParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["prompt"] = details::resource_json(prompt);
        details::resource_file(payload, "video", video);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VideoEditParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VideoExtendParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["prompt"] = details::resource_json(prompt);
        payload.body["seconds"] = details::resource_json(seconds);
        details::resource_file(payload, "video", video);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VideoExtendParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VideosGetCharacterParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VideosGetCharacterParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VideoRemixParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["prompt"] = details::resource_json(prompt);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VideoRemixParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::videos

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Videos::Videos(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /videos.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::Create(
        types::videos::VideoCreateParams params
    ) const -> drogon::Task<Result<types::videos::Video>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/videos", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::Video>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /videos.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::CreateRaw(
        types::videos::VideoCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::videos::Video>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/videos", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::Video>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /videos/{video_id}.
     * @param video_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::Retrieve(
        std::string video_id,
        types::videos::VideosRetrieveParams params
    ) const -> drogon::Task<Result<types::videos::Video>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/videos/{video_id}", { { "video_id", video_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::Video>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /videos/{video_id}.
     * @param video_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::RetrieveRaw(
        std::string video_id,
        types::videos::VideosRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::videos::Video>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/videos/{video_id}", { { "video_id", video_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::Video>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /videos.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::List(
        types::videos::VideoListParams params
    ) const -> drogon::Task<Result<Page<types::videos::Video>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/videos", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::videos::Video>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true, "last_id");
    }

    /**
     * @brief DELETE /videos/{video_id}.
     * @param video_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::Delete(
        std::string video_id,
        types::videos::VideosDeleteParams params
    ) const -> drogon::Task<Result<types::videos::VideoDeleteResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/videos/{video_id}", { { "video_id", video_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::VideoDeleteResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief DELETE /videos/{video_id}.
     * @param video_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::DeleteRaw(
        std::string video_id,
        types::videos::VideosDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<types::videos::VideoDeleteResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/videos/{video_id}", { { "video_id", video_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::VideoDeleteResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /videos/characters.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::CreateCharacter(
        types::videos::VideoCreateCharacterParams params
    ) const -> drogon::Task<Result<types::videos::VideoCreateCharacterResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/videos/characters", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::VideoCreateCharacterResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /videos/characters.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::CreateCharacterRaw(
        types::videos::VideoCreateCharacterParams params
    ) const -> drogon::Task<Result<RawResponse<types::videos::VideoCreateCharacterResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/videos/characters", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::VideoCreateCharacterResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /videos/{video_id}/content.
     * @param video_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::DownloadContent(
        std::string video_id,
        types::videos::VideoDownloadContentParams params
    ) const -> drogon::Task<Result<RawResponse<std::string>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/videos/{video_id}/content", { { "video_id", video_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_bytes(transport, "GET", std::move(*path), std::move(payload), params.Options(), { { "Accept", "application/binary" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        co_return std::move(*response);
    }

    /**
     * @brief POST /videos/edits.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::Edit(
        types::videos::VideoEditParams params
    ) const -> drogon::Task<Result<types::videos::Video>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/videos/edits", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::Video>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /videos/edits.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::EditRaw(
        types::videos::VideoEditParams params
    ) const -> drogon::Task<Result<RawResponse<types::videos::Video>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/videos/edits", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::Video>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /videos/extensions.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::Extend(
        types::videos::VideoExtendParams params
    ) const -> drogon::Task<Result<types::videos::Video>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/videos/extensions", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::Video>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /videos/extensions.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::ExtendRaw(
        types::videos::VideoExtendParams params
    ) const -> drogon::Task<Result<RawResponse<types::videos::Video>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/videos/extensions", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::Video>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /videos/characters/{character_id}.
     * @param character_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::GetCharacter(
        std::string character_id,
        types::videos::VideosGetCharacterParams params
    ) const -> drogon::Task<Result<types::videos::VideoGetCharacterResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/videos/characters/{character_id}", { { "character_id", character_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::VideoGetCharacterResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /videos/characters/{character_id}.
     * @param character_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::GetCharacterRaw(
        std::string character_id,
        types::videos::VideosGetCharacterParams params
    ) const -> drogon::Task<Result<RawResponse<types::videos::VideoGetCharacterResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/videos/characters/{character_id}", { { "character_id", character_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::VideoGetCharacterResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /videos/{video_id}/remix.
     * @param video_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::Remix(
        std::string video_id,
        types::videos::VideoRemixParams params
    ) const -> drogon::Task<Result<types::videos::Video>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/videos/{video_id}/remix", { { "video_id", video_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::Video>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /videos/{video_id}/remix.
     * @param video_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Videos::RemixRaw(
        std::string video_id,
        types::videos::VideoRemixParams params
    ) const -> drogon::Task<Result<RawResponse<types::videos::Video>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/videos/{video_id}/remix", { { "video_id", video_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::videos::Video>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }
} // namespace drogon_openai::details
