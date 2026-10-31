/**
 * @file resource_files.cpp
 * @brief Implement files resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/files.hpp"

namespace drogon_openai::types::files {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto FileObject::FromJson(Json::Value const& value) -> Result<FileObject> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        FileObject result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileObject.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("bytes")) {
            if (!details::resource_read(value["bytes"], result.bytes)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileObject.bytes", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileObject.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("filename")) {
            if (!details::resource_read(value["filename"], result.filename)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileObject.filename", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileObject.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("purpose")) {
            if (!details::resource_read(value["purpose"], result.purpose)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileObject.purpose", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileObject.status", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("expires_at") && !value["expires_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["expires_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileObject.expires_at", .body = value.toStyledString() } };
            }
            result.expires_at = std::move(field);
        }
        if (value.isMember("status_details") && !value["status_details"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["status_details"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileObject.status_details", .body = value.toStyledString() } };
            }
            result.status_details = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto FileDeleted::FromJson(Json::Value const& value) -> Result<FileDeleted> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        FileDeleted result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileDeleted.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("deleted")) {
            if (!details::resource_read(value["deleted"], result.deleted)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileDeleted.deleted", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileDeleted.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FileCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        details::resource_file(payload, "file", file);
        payload.body["purpose"] = details::resource_json(purpose);
        if (expires_after) {
            payload.body["expires_after"] = details::resource_json(*expires_after);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto FileCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FilesRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto FilesRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FileListParams::Payload() const -> ResourcePayload {
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
        if (purpose) {
            payload.query["purpose"] = details::resource_json(*purpose);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto FileListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FilesDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto FilesDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FilesContentParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto FilesContentParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FilesRetrieveContentParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto FilesRetrieveContentParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::files

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Files::Files(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /files.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Files::Create(
        types::files::FileCreateParams params
    ) const -> drogon::Task<Result<types::files::FileObject>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/files", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::files::FileObject>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /files.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Files::CreateRaw(
        types::files::FileCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::files::FileObject>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/files", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::files::FileObject>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /files/{file_id}.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Files::Retrieve(
        std::string file_id,
        types::files::FilesRetrieveParams params
    ) const -> drogon::Task<Result<types::files::FileObject>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/files/{file_id}", { { "file_id", file_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::files::FileObject>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /files/{file_id}.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Files::RetrieveRaw(
        std::string file_id,
        types::files::FilesRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::files::FileObject>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/files/{file_id}", { { "file_id", file_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::files::FileObject>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /files.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Files::List(
        types::files::FileListParams params
    ) const -> drogon::Task<Result<Page<types::files::FileObject>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/files", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::files::FileObject>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true);
    }

    /**
     * @brief DELETE /files/{file_id}.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Files::Delete(
        std::string file_id,
        types::files::FilesDeleteParams params
    ) const -> drogon::Task<Result<types::files::FileDeleted>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/files/{file_id}", { { "file_id", file_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::files::FileDeleted>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief DELETE /files/{file_id}.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Files::DeleteRaw(
        std::string file_id,
        types::files::FilesDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<types::files::FileDeleted>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/files/{file_id}", { { "file_id", file_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::files::FileDeleted>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /files/{file_id}/content.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Files::Content(
        std::string file_id,
        types::files::FilesContentParams params
    ) const -> drogon::Task<Result<RawResponse<std::string>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/files/{file_id}/content", { { "file_id", file_id } }) };
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
     * @brief GET /files/{file_id}/content.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Files::RetrieveContent(
        std::string file_id,
        types::files::FilesRetrieveContentParams params
    ) const -> drogon::Task<Result<std::string>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/files/{file_id}/content", { { "file_id", file_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_bytes(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        co_return std::move(response->body);
    }

    /**
     * @brief GET /files/{file_id}/content.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Files::RetrieveContentRaw(
        std::string file_id,
        types::files::FilesRetrieveContentParams params
    ) const -> drogon::Task<Result<RawResponse<std::string>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/files/{file_id}/content", { { "file_id", file_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_bytes(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        co_return std::move(*response);
    }
} // namespace drogon_openai::details
