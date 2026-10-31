/**
 * @file resource_containers.cpp
 * @brief Implement containers resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/containers.hpp"

namespace drogon_openai::types::containers {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto ContainerCreateResponse::FromJson(Json::Value const& value) -> Result<ContainerCreateResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        ContainerCreateResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerCreateResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerCreateResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerCreateResponse.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerCreateResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerCreateResponse.status", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("expires_after") && !value["expires_after"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["expires_after"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerCreateResponse.expires_after", .body = value.toStyledString() } };
            }
            result.expires_after = std::move(field);
        }
        if (value.isMember("last_active_at") && !value["last_active_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["last_active_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerCreateResponse.last_active_at", .body = value.toStyledString() } };
            }
            result.last_active_at = std::move(field);
        }
        if (value.isMember("memory_limit") && !value["memory_limit"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["memory_limit"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerCreateResponse.memory_limit", .body = value.toStyledString() } };
            }
            result.memory_limit = std::move(field);
        }
        if (value.isMember("network_policy") && !value["network_policy"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["network_policy"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerCreateResponse.network_policy", .body = value.toStyledString() } };
            }
            result.network_policy = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto ContainerRetrieveResponse::FromJson(Json::Value const& value) -> Result<ContainerRetrieveResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        ContainerRetrieveResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerRetrieveResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerRetrieveResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerRetrieveResponse.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerRetrieveResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerRetrieveResponse.status", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("expires_after") && !value["expires_after"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["expires_after"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerRetrieveResponse.expires_after", .body = value.toStyledString() } };
            }
            result.expires_after = std::move(field);
        }
        if (value.isMember("last_active_at") && !value["last_active_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["last_active_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerRetrieveResponse.last_active_at", .body = value.toStyledString() } };
            }
            result.last_active_at = std::move(field);
        }
        if (value.isMember("memory_limit") && !value["memory_limit"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["memory_limit"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerRetrieveResponse.memory_limit", .body = value.toStyledString() } };
            }
            result.memory_limit = std::move(field);
        }
        if (value.isMember("network_policy") && !value["network_policy"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["network_policy"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerRetrieveResponse.network_policy", .body = value.toStyledString() } };
            }
            result.network_policy = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto ContainerListResponse::FromJson(Json::Value const& value) -> Result<ContainerListResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        ContainerListResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerListResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerListResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerListResponse.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerListResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerListResponse.status", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("expires_after") && !value["expires_after"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["expires_after"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerListResponse.expires_after", .body = value.toStyledString() } };
            }
            result.expires_after = std::move(field);
        }
        if (value.isMember("last_active_at") && !value["last_active_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["last_active_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerListResponse.last_active_at", .body = value.toStyledString() } };
            }
            result.last_active_at = std::move(field);
        }
        if (value.isMember("memory_limit") && !value["memory_limit"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["memory_limit"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerListResponse.memory_limit", .body = value.toStyledString() } };
            }
            result.memory_limit = std::move(field);
        }
        if (value.isMember("network_policy") && !value["network_policy"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["network_policy"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ContainerListResponse.network_policy", .body = value.toStyledString() } };
            }
            result.network_policy = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto FileCreateResponse::FromJson(Json::Value const& value) -> Result<FileCreateResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        FileCreateResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileCreateResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("bytes")) {
            if (!details::resource_read(value["bytes"], result.bytes)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileCreateResponse.bytes", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("container_id")) {
            if (!details::resource_read(value["container_id"], result.container_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileCreateResponse.container_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileCreateResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileCreateResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("path")) {
            if (!details::resource_read(value["path"], result.path)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileCreateResponse.path", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("source")) {
            if (!details::resource_read(value["source"], result.source)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileCreateResponse.source", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto FileRetrieveResponse::FromJson(Json::Value const& value) -> Result<FileRetrieveResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        FileRetrieveResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileRetrieveResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("bytes")) {
            if (!details::resource_read(value["bytes"], result.bytes)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileRetrieveResponse.bytes", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("container_id")) {
            if (!details::resource_read(value["container_id"], result.container_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileRetrieveResponse.container_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileRetrieveResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileRetrieveResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("path")) {
            if (!details::resource_read(value["path"], result.path)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileRetrieveResponse.path", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("source")) {
            if (!details::resource_read(value["source"], result.source)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileRetrieveResponse.source", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto FileListResponse::FromJson(Json::Value const& value) -> Result<FileListResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        FileListResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileListResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("bytes")) {
            if (!details::resource_read(value["bytes"], result.bytes)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileListResponse.bytes", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("container_id")) {
            if (!details::resource_read(value["container_id"], result.container_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileListResponse.container_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileListResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileListResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("path")) {
            if (!details::resource_read(value["path"], result.path)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileListResponse.path", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("source")) {
            if (!details::resource_read(value["source"], result.source)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileListResponse.source", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ContainerCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["name"] = details::resource_json(name);
        if (expires_after) {
            payload.body["expires_after"] = details::resource_json(*expires_after);
        }
        if (file_ids) {
            payload.body["file_ids"] = details::resource_json(*file_ids);
        }
        if (memory_limit) {
            payload.body["memory_limit"] = details::resource_json(*memory_limit);
        }
        if (network_policy) {
            payload.body["network_policy"] = details::resource_json(*network_policy);
        }
        if (skills) {
            payload.body["skills"] = details::resource_json(*skills);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ContainerCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ContainersRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ContainersRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ContainerListParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (after) {
            payload.query["after"] = details::resource_json(*after);
        }
        if (limit) {
            payload.query["limit"] = details::resource_json(*limit);
        }
        if (name) {
            payload.query["name"] = details::resource_json(*name);
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
    auto ContainerListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ContainersDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ContainersDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ContainersFilesContentRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ContainersFilesContentRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FileCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (file) {
            details::resource_file(payload, "file", *file);
        }
        if (file_id) {
            payload.body["file_id"] = details::resource_json(*file_id);
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
    auto ContainersFilesRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ContainersFilesRetrieveParams::Options() const -> RequestOptions {
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
    auto ContainersFilesDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ContainersFilesDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::containers

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    ContainersFilesContent::ContainersFilesContent(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief GET /containers/{container_id}/files/{file_id}/content.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param container_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ContainersFilesContent::Retrieve(
        std::string file_id,
        std::string container_id,
        types::containers::ContainersFilesContentRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<std::string>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/containers/{container_id}/files/{file_id}/content", { { "file_id", file_id }, { "container_id", container_id } }) };
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
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    ContainersFiles::ContainersFiles(std::shared_ptr<Transport> transport) : m_transport{ transport }, content{ transport } {}

    /**
     * @brief POST /containers/{container_id}/files.
     * @param container_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ContainersFiles::Create(
        std::string container_id,
        types::containers::FileCreateParams params
    ) const -> drogon::Task<Result<types::containers::FileCreateResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/containers/{container_id}/files", { { "container_id", container_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::containers::FileCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /containers/{container_id}/files.
     * @param container_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ContainersFiles::CreateRaw(
        std::string container_id,
        types::containers::FileCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::containers::FileCreateResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/containers/{container_id}/files", { { "container_id", container_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::containers::FileCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /containers/{container_id}/files/{file_id}.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param container_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ContainersFiles::Retrieve(
        std::string file_id,
        std::string container_id,
        types::containers::ContainersFilesRetrieveParams params
    ) const -> drogon::Task<Result<types::containers::FileRetrieveResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/containers/{container_id}/files/{file_id}", { { "file_id", file_id }, { "container_id", container_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::containers::FileRetrieveResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /containers/{container_id}/files/{file_id}.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param container_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ContainersFiles::RetrieveRaw(
        std::string file_id,
        std::string container_id,
        types::containers::ContainersFilesRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::containers::FileRetrieveResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/containers/{container_id}/files/{file_id}", { { "file_id", file_id }, { "container_id", container_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::containers::FileRetrieveResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /containers/{container_id}/files.
     * @param container_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ContainersFiles::List(
        std::string container_id,
        types::containers::FileListParams params
    ) const -> drogon::Task<Result<Page<types::containers::FileListResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/containers/{container_id}/files", { { "container_id", container_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::containers::FileListResponse>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true);
    }

    /**
     * @brief DELETE /containers/{container_id}/files/{file_id}.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param container_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ContainersFiles::Delete(
        std::string file_id,
        std::string container_id,
        types::containers::ContainersFilesDeleteParams params
    ) const -> drogon::Task<Result<void>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/containers/{container_id}/files/{file_id}", { { "file_id", file_id }, { "container_id", container_id } }) };
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
     * @brief DELETE /containers/{container_id}/files/{file_id}.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param container_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ContainersFiles::DeleteRaw(
        std::string file_id,
        std::string container_id,
        types::containers::ContainersFilesDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/containers/{container_id}/files/{file_id}", { { "file_id", file_id }, { "container_id", container_id } }) };
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
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Containers::Containers(std::shared_ptr<Transport> transport) : m_transport{ transport }, files{ transport } {}

    /**
     * @brief POST /containers.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Containers::Create(
        types::containers::ContainerCreateParams params
    ) const -> drogon::Task<Result<types::containers::ContainerCreateResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/containers", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::containers::ContainerCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /containers.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Containers::CreateRaw(
        types::containers::ContainerCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::containers::ContainerCreateResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/containers", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::containers::ContainerCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /containers/{container_id}.
     * @param container_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Containers::Retrieve(
        std::string container_id,
        types::containers::ContainersRetrieveParams params
    ) const -> drogon::Task<Result<types::containers::ContainerRetrieveResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/containers/{container_id}", { { "container_id", container_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::containers::ContainerRetrieveResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /containers/{container_id}.
     * @param container_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Containers::RetrieveRaw(
        std::string container_id,
        types::containers::ContainersRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::containers::ContainerRetrieveResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/containers/{container_id}", { { "container_id", container_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::containers::ContainerRetrieveResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /containers.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Containers::List(
        types::containers::ContainerListParams params
    ) const -> drogon::Task<Result<Page<types::containers::ContainerListResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/containers", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::containers::ContainerListResponse>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true);
    }

    /**
     * @brief DELETE /containers/{container_id}.
     * @param container_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Containers::Delete(
        std::string container_id,
        types::containers::ContainersDeleteParams params
    ) const -> drogon::Task<Result<void>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/containers/{container_id}", { { "container_id", container_id } }) };
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
     * @brief DELETE /containers/{container_id}.
     * @param container_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Containers::DeleteRaw(
        std::string container_id,
        types::containers::ContainersDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/containers/{container_id}", { { "container_id", container_id } }) };
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
} // namespace drogon_openai::details
