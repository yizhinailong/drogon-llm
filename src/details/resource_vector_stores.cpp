/**
 * @file resource_vector_stores.cpp
 * @brief Implement vector_stores resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/vector_stores.hpp"

namespace drogon_openai::types::vector_stores {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto VectorStoreFileBatch::FromJson(Json::Value const& value) -> Result<VectorStoreFileBatch> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        VectorStoreFileBatch result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFileBatch.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFileBatch.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("file_counts")) {
            if (!details::resource_read(value["file_counts"], result.file_counts)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFileBatch.file_counts", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFileBatch.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFileBatch.status", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("vector_store_id")) {
            if (!details::resource_read(value["vector_store_id"], result.vector_store_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFileBatch.vector_store_id", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto VectorStoreFile::FromJson(Json::Value const& value) -> Result<VectorStoreFile> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        VectorStoreFile result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFile.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFile.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("last_error") && !value["last_error"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["last_error"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFile.last_error", .body = value.toStyledString() } };
            }
            result.last_error = std::move(field);
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFile.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFile.status", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("usage_bytes")) {
            if (!details::resource_read(value["usage_bytes"], result.usage_bytes)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFile.usage_bytes", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("vector_store_id")) {
            if (!details::resource_read(value["vector_store_id"], result.vector_store_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFile.vector_store_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("attributes") && !value["attributes"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["attributes"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFile.attributes", .body = value.toStyledString() } };
            }
            result.attributes = std::move(field);
        }
        if (value.isMember("chunking_strategy") && !value["chunking_strategy"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["chunking_strategy"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFile.chunking_strategy", .body = value.toStyledString() } };
            }
            result.chunking_strategy = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto VectorStoreFileDeleted::FromJson(Json::Value const& value) -> Result<VectorStoreFileDeleted> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        VectorStoreFileDeleted result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFileDeleted.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("deleted")) {
            if (!details::resource_read(value["deleted"], result.deleted)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFileDeleted.deleted", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreFileDeleted.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto FileContentResponse::FromJson(Json::Value const& value) -> Result<FileContentResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        FileContentResponse result;
        result.raw = value;
        if (value.isMember("text") && !value["text"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["text"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileContentResponse.text", .body = value.toStyledString() } };
            }
            result.text = std::move(field);
        }
        if (value.isMember("type") && !value["type"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["type"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FileContentResponse.type", .body = value.toStyledString() } };
            }
            result.type = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto VectorStore::FromJson(Json::Value const& value) -> Result<VectorStore> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        VectorStore result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStore.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStore.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("file_counts")) {
            if (!details::resource_read(value["file_counts"], result.file_counts)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStore.file_counts", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("last_active_at") && !value["last_active_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["last_active_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStore.last_active_at", .body = value.toStyledString() } };
            }
            result.last_active_at = std::move(field);
        }
        if (value.isMember("metadata") && !value["metadata"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["metadata"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStore.metadata", .body = value.toStyledString() } };
            }
            result.metadata = std::move(field);
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStore.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStore.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStore.status", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("usage_bytes")) {
            if (!details::resource_read(value["usage_bytes"], result.usage_bytes)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStore.usage_bytes", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("expires_after") && !value["expires_after"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["expires_after"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStore.expires_after", .body = value.toStyledString() } };
            }
            result.expires_after = std::move(field);
        }
        if (value.isMember("expires_at") && !value["expires_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["expires_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStore.expires_at", .body = value.toStyledString() } };
            }
            result.expires_at = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto VectorStoreDeleted::FromJson(Json::Value const& value) -> Result<VectorStoreDeleted> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        VectorStoreDeleted result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreDeleted.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("deleted")) {
            if (!details::resource_read(value["deleted"], result.deleted)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreDeleted.deleted", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreDeleted.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto VectorStoreSearchResponse::FromJson(Json::Value const& value) -> Result<VectorStoreSearchResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        VectorStoreSearchResponse result;
        result.raw = value;
        if (value.isMember("attributes") && !value["attributes"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["attributes"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreSearchResponse.attributes", .body = value.toStyledString() } };
            }
            result.attributes = std::move(field);
        }
        if (value.isMember("content")) {
            if (!details::resource_read(value["content"], result.content)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreSearchResponse.content", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("file_id")) {
            if (!details::resource_read(value["file_id"], result.file_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreSearchResponse.file_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("filename")) {
            if (!details::resource_read(value["filename"], result.filename)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreSearchResponse.filename", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("score")) {
            if (!details::resource_read(value["score"], result.score)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VectorStoreSearchResponse.score", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FileBatchCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (attributes) {
            payload.body["attributes"] = details::resource_json(*attributes);
        }
        if (chunking_strategy) {
            payload.body["chunking_strategy"] = details::resource_json(*chunking_strategy);
        }
        if (file_ids) {
            payload.body["file_ids"] = details::resource_json(*file_ids);
        }
        if (files) {
            payload.body["files"] = details::resource_json(*files);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto FileBatchCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VectorStoresFileBatchesRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VectorStoresFileBatchesRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VectorStoresFileBatchesCancelParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VectorStoresFileBatchesCancelParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FileBatchListFilesParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (after) {
            payload.query["after"] = details::resource_json(*after);
        }
        if (before) {
            payload.query["before"] = details::resource_json(*before);
        }
        if (filter) {
            payload.query["filter"] = details::resource_json(*filter);
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
    auto FileBatchListFilesParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FileCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["file_id"] = details::resource_json(file_id);
        if (attributes) {
            payload.body["attributes"] = details::resource_json(*attributes);
        }
        if (chunking_strategy) {
            payload.body["chunking_strategy"] = details::resource_json(*chunking_strategy);
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
    auto VectorStoresFilesRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VectorStoresFilesRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FileUpdateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["attributes"] = details::resource_json(attributes);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto FileUpdateParams::Options() const -> RequestOptions {
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
        if (before) {
            payload.query["before"] = details::resource_json(*before);
        }
        if (filter) {
            payload.query["filter"] = details::resource_json(*filter);
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
    auto VectorStoresFilesDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VectorStoresFilesDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VectorStoresFilesContentParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VectorStoresFilesContentParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VectorStoreCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (chunking_strategy) {
            payload.body["chunking_strategy"] = details::resource_json(*chunking_strategy);
        }
        if (description) {
            payload.body["description"] = details::resource_json(*description);
        }
        if (expires_after) {
            payload.body["expires_after"] = details::resource_json(*expires_after);
        }
        if (file_ids) {
            payload.body["file_ids"] = details::resource_json(*file_ids);
        }
        if (metadata) {
            payload.body["metadata"] = details::resource_json(*metadata);
        }
        if (name) {
            payload.body["name"] = details::resource_json(*name);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VectorStoreCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VectorStoresRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VectorStoresRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VectorStoreUpdateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (expires_after) {
            payload.body["expires_after"] = details::resource_json(*expires_after);
        }
        if (metadata) {
            payload.body["metadata"] = details::resource_json(*metadata);
        }
        if (name) {
            payload.body["name"] = details::resource_json(*name);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VectorStoreUpdateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VectorStoreListParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (after) {
            payload.query["after"] = details::resource_json(*after);
        }
        if (before) {
            payload.query["before"] = details::resource_json(*before);
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
    auto VectorStoreListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VectorStoresDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VectorStoresDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VectorStoreSearchParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["query"] = details::resource_json(query);
        if (filters) {
            payload.body["filters"] = details::resource_json(*filters);
        }
        if (max_num_results) {
            payload.body["max_num_results"] = details::resource_json(*max_num_results);
        }
        if (ranking_options) {
            payload.body["ranking_options"] = details::resource_json(*ranking_options);
        }
        if (rewrite_query) {
            payload.body["rewrite_query"] = details::resource_json(*rewrite_query);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VectorStoreSearchParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::vector_stores

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    VectorStoresFileBatches::VectorStoresFileBatches(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /vector_stores/{vector_store_id}/file_batches.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFileBatches::Create(
        std::string vector_store_id,
        types::vector_stores::FileBatchCreateParams params
    ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFileBatch>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/file_batches", { { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreFileBatch>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /vector_stores/{vector_store_id}/file_batches.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFileBatches::CreateRaw(
        std::string vector_store_id,
        types::vector_stores::FileBatchCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreFileBatch>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/file_batches", { { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreFileBatch>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /vector_stores/{vector_store_id}/file_batches/{batch_id}.
     * @param batch_id Owned path identifier, encoded as one segment.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFileBatches::Retrieve(
        std::string batch_id,
        std::string vector_store_id,
        types::vector_stores::VectorStoresFileBatchesRetrieveParams params
    ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFileBatch>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/file_batches/{batch_id}", { { "batch_id", batch_id }, { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreFileBatch>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /vector_stores/{vector_store_id}/file_batches/{batch_id}.
     * @param batch_id Owned path identifier, encoded as one segment.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFileBatches::RetrieveRaw(
        std::string batch_id,
        std::string vector_store_id,
        types::vector_stores::VectorStoresFileBatchesRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreFileBatch>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/file_batches/{batch_id}", { { "batch_id", batch_id }, { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreFileBatch>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /vector_stores/{vector_store_id}/file_batches/{batch_id}/cancel.
     * @param batch_id Owned path identifier, encoded as one segment.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFileBatches::Cancel(
        std::string batch_id,
        std::string vector_store_id,
        types::vector_stores::VectorStoresFileBatchesCancelParams params
    ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFileBatch>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/file_batches/{batch_id}/cancel", { { "batch_id", batch_id }, { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreFileBatch>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /vector_stores/{vector_store_id}/file_batches/{batch_id}/cancel.
     * @param batch_id Owned path identifier, encoded as one segment.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFileBatches::CancelRaw(
        std::string batch_id,
        std::string vector_store_id,
        types::vector_stores::VectorStoresFileBatchesCancelParams params
    ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreFileBatch>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/file_batches/{batch_id}/cancel", { { "batch_id", batch_id }, { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreFileBatch>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /vector_stores/{vector_store_id}/file_batches/{batch_id}/files.
     * @param batch_id Owned path identifier, encoded as one segment.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFileBatches::ListFiles(
        std::string batch_id,
        std::string vector_store_id,
        types::vector_stores::FileBatchListFilesParams params
    ) const -> drogon::Task<Result<Page<types::vector_stores::VectorStoreFile>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/file_batches/{batch_id}/files", { { "batch_id", batch_id }, { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::vector_stores::VectorStoreFile>(transport, "GET", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }, true);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    VectorStoresFiles::VectorStoresFiles(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /vector_stores/{vector_store_id}/files.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFiles::Create(
        std::string vector_store_id,
        types::vector_stores::FileCreateParams params
    ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFile>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/files", { { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreFile>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /vector_stores/{vector_store_id}/files.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFiles::CreateRaw(
        std::string vector_store_id,
        types::vector_stores::FileCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreFile>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/files", { { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreFile>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /vector_stores/{vector_store_id}/files/{file_id}.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFiles::Retrieve(
        std::string file_id,
        std::string vector_store_id,
        types::vector_stores::VectorStoresFilesRetrieveParams params
    ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFile>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/files/{file_id}", { { "file_id", file_id }, { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreFile>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /vector_stores/{vector_store_id}/files/{file_id}.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFiles::RetrieveRaw(
        std::string file_id,
        std::string vector_store_id,
        types::vector_stores::VectorStoresFilesRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreFile>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/files/{file_id}", { { "file_id", file_id }, { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreFile>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /vector_stores/{vector_store_id}/files/{file_id}.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFiles::Update(
        std::string file_id,
        std::string vector_store_id,
        types::vector_stores::FileUpdateParams params
    ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFile>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/files/{file_id}", { { "file_id", file_id }, { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreFile>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /vector_stores/{vector_store_id}/files/{file_id}.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFiles::UpdateRaw(
        std::string file_id,
        std::string vector_store_id,
        types::vector_stores::FileUpdateParams params
    ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreFile>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/files/{file_id}", { { "file_id", file_id }, { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreFile>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /vector_stores/{vector_store_id}/files.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFiles::List(
        std::string vector_store_id,
        types::vector_stores::FileListParams params
    ) const -> drogon::Task<Result<Page<types::vector_stores::VectorStoreFile>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/files", { { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::vector_stores::VectorStoreFile>(transport, "GET", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }, true);
    }

    /**
     * @brief DELETE /vector_stores/{vector_store_id}/files/{file_id}.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFiles::Delete(
        std::string file_id,
        std::string vector_store_id,
        types::vector_stores::VectorStoresFilesDeleteParams params
    ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFileDeleted>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/files/{file_id}", { { "file_id", file_id }, { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreFileDeleted>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief DELETE /vector_stores/{vector_store_id}/files/{file_id}.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFiles::DeleteRaw(
        std::string file_id,
        std::string vector_store_id,
        types::vector_stores::VectorStoresFilesDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreFileDeleted>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/files/{file_id}", { { "file_id", file_id }, { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreFileDeleted>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /vector_stores/{vector_store_id}/files/{file_id}/content.
     * @param file_id Owned path identifier, encoded as one segment.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStoresFiles::Content(
        std::string file_id,
        std::string vector_store_id,
        types::vector_stores::VectorStoresFilesContentParams params
    ) const -> drogon::Task<Result<Page<types::vector_stores::FileContentResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/files/{file_id}/content", { { "file_id", file_id }, { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::vector_stores::FileContentResponse>(transport, "GET", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }, false);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    VectorStores::VectorStores(std::shared_ptr<Transport> transport) : m_transport{ transport }, files{ transport }, file_batches{ transport } {}

    /**
     * @brief POST /vector_stores.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStores::Create(
        types::vector_stores::VectorStoreCreateParams params
    ) const -> drogon::Task<Result<types::vector_stores::VectorStore>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStore>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /vector_stores.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStores::CreateRaw(
        types::vector_stores::VectorStoreCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStore>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStore>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /vector_stores/{vector_store_id}.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStores::Retrieve(
        std::string vector_store_id,
        types::vector_stores::VectorStoresRetrieveParams params
    ) const -> drogon::Task<Result<types::vector_stores::VectorStore>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}", { { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStore>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /vector_stores/{vector_store_id}.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStores::RetrieveRaw(
        std::string vector_store_id,
        types::vector_stores::VectorStoresRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStore>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}", { { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStore>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /vector_stores/{vector_store_id}.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStores::Update(
        std::string vector_store_id,
        types::vector_stores::VectorStoreUpdateParams params
    ) const -> drogon::Task<Result<types::vector_stores::VectorStore>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}", { { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStore>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /vector_stores/{vector_store_id}.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStores::UpdateRaw(
        std::string vector_store_id,
        types::vector_stores::VectorStoreUpdateParams params
    ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStore>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}", { { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStore>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /vector_stores.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStores::List(
        types::vector_stores::VectorStoreListParams params
    ) const -> drogon::Task<Result<Page<types::vector_stores::VectorStore>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::vector_stores::VectorStore>(transport, "GET", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }, true);
    }

    /**
     * @brief DELETE /vector_stores/{vector_store_id}.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStores::Delete(
        std::string vector_store_id,
        types::vector_stores::VectorStoresDeleteParams params
    ) const -> drogon::Task<Result<types::vector_stores::VectorStoreDeleted>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}", { { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreDeleted>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief DELETE /vector_stores/{vector_store_id}.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStores::DeleteRaw(
        std::string vector_store_id,
        types::vector_stores::VectorStoresDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreDeleted>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}", { { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::vector_stores::VectorStoreDeleted>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /vector_stores/{vector_store_id}/search.
     * @param vector_store_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto VectorStores::Search(
        std::string vector_store_id,
        types::vector_stores::VectorStoreSearchParams params
    ) const -> drogon::Task<Result<Page<types::vector_stores::VectorStoreSearchResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/vector_stores/{vector_store_id}/search", { { "vector_store_id", vector_store_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::vector_stores::VectorStoreSearchResponse>(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "OpenAI-Beta", "assistants=v2" } }, false);
    }
} // namespace drogon_openai::details
