/**
 * @file resource_batches.cpp
 * @brief Implement batches resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/batches.hpp"

namespace drogon_openai::types::batches {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto Batch::FromJson(Json::Value const& value) -> Result<Batch> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        Batch result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("completion_window")) {
            if (!details::resource_read(value["completion_window"], result.completion_window)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.completion_window", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("endpoint")) {
            if (!details::resource_read(value["endpoint"], result.endpoint)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.endpoint", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("input_file_id")) {
            if (!details::resource_read(value["input_file_id"], result.input_file_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.input_file_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.status", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("cancelled_at") && !value["cancelled_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["cancelled_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.cancelled_at", .body = value.toStyledString() } };
            }
            result.cancelled_at = std::move(field);
        }
        if (value.isMember("cancelling_at") && !value["cancelling_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["cancelling_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.cancelling_at", .body = value.toStyledString() } };
            }
            result.cancelling_at = std::move(field);
        }
        if (value.isMember("completed_at") && !value["completed_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["completed_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.completed_at", .body = value.toStyledString() } };
            }
            result.completed_at = std::move(field);
        }
        if (value.isMember("error_file_id") && !value["error_file_id"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["error_file_id"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.error_file_id", .body = value.toStyledString() } };
            }
            result.error_file_id = std::move(field);
        }
        if (value.isMember("errors") && !value["errors"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["errors"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.errors", .body = value.toStyledString() } };
            }
            result.errors = std::move(field);
        }
        if (value.isMember("expired_at") && !value["expired_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["expired_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.expired_at", .body = value.toStyledString() } };
            }
            result.expired_at = std::move(field);
        }
        if (value.isMember("expires_at") && !value["expires_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["expires_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.expires_at", .body = value.toStyledString() } };
            }
            result.expires_at = std::move(field);
        }
        if (value.isMember("failed_at") && !value["failed_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["failed_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.failed_at", .body = value.toStyledString() } };
            }
            result.failed_at = std::move(field);
        }
        if (value.isMember("finalizing_at") && !value["finalizing_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["finalizing_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.finalizing_at", .body = value.toStyledString() } };
            }
            result.finalizing_at = std::move(field);
        }
        if (value.isMember("in_progress_at") && !value["in_progress_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["in_progress_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.in_progress_at", .body = value.toStyledString() } };
            }
            result.in_progress_at = std::move(field);
        }
        if (value.isMember("metadata") && !value["metadata"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["metadata"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.metadata", .body = value.toStyledString() } };
            }
            result.metadata = std::move(field);
        }
        if (value.isMember("model") && !value["model"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["model"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.model", .body = value.toStyledString() } };
            }
            result.model = std::move(field);
        }
        if (value.isMember("output_file_id") && !value["output_file_id"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["output_file_id"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.output_file_id", .body = value.toStyledString() } };
            }
            result.output_file_id = std::move(field);
        }
        if (value.isMember("request_counts") && !value["request_counts"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["request_counts"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.request_counts", .body = value.toStyledString() } };
            }
            result.request_counts = std::move(field);
        }
        if (value.isMember("usage") && !value["usage"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["usage"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Batch.usage", .body = value.toStyledString() } };
            }
            result.usage = std::move(field);
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto BatchCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["completion_window"] = details::resource_json(completion_window);
        payload.body["endpoint"] = details::resource_json(endpoint);
        payload.body["input_file_id"] = details::resource_json(input_file_id);
        if (metadata) {
            payload.body["metadata"] = details::resource_json(*metadata);
        }
        if (output_expires_after) {
            payload.body["output_expires_after"] = details::resource_json(*output_expires_after);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto BatchCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto BatchesRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto BatchesRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto BatchListParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (after) {
            payload.query["after"] = details::resource_json(*after);
        }
        if (limit) {
            payload.query["limit"] = details::resource_json(*limit);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto BatchListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto BatchesCancelParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto BatchesCancelParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::batches

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Batches::Batches(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /batches.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Batches::Create(
        types::batches::BatchCreateParams params
    ) const -> drogon::Task<Result<types::batches::Batch>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/batches", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::batches::Batch>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /batches.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Batches::CreateRaw(
        types::batches::BatchCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::batches::Batch>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/batches", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::batches::Batch>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /batches/{batch_id}.
     * @param batch_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Batches::Retrieve(
        std::string batch_id,
        types::batches::BatchesRetrieveParams params
    ) const -> drogon::Task<Result<types::batches::Batch>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/batches/{batch_id}", { { "batch_id", batch_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::batches::Batch>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /batches/{batch_id}.
     * @param batch_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Batches::RetrieveRaw(
        std::string batch_id,
        types::batches::BatchesRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::batches::Batch>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/batches/{batch_id}", { { "batch_id", batch_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::batches::Batch>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /batches.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Batches::List(
        types::batches::BatchListParams params
    ) const -> drogon::Task<Result<Page<types::batches::Batch>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/batches", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::batches::Batch>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true);
    }

    /**
     * @brief POST /batches/{batch_id}/cancel.
     * @param batch_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Batches::Cancel(
        std::string batch_id,
        types::batches::BatchesCancelParams params
    ) const -> drogon::Task<Result<types::batches::Batch>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/batches/{batch_id}/cancel", { { "batch_id", batch_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::batches::Batch>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /batches/{batch_id}/cancel.
     * @param batch_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Batches::CancelRaw(
        std::string batch_id,
        types::batches::BatchesCancelParams params
    ) const -> drogon::Task<Result<RawResponse<types::batches::Batch>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/batches/{batch_id}/cancel", { { "batch_id", batch_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::batches::Batch>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }
} // namespace drogon_openai::details
