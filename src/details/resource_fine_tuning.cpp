/**
 * @file resource_fine_tuning.cpp
 * @brief Implement fine_tuning resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/fine_tuning.hpp"

namespace drogon_openai::types::fine_tuning {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto GraderRunResponse::FromJson(Json::Value const& value) -> Result<GraderRunResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        GraderRunResponse result;
        result.raw = value;
        if (value.isMember("metadata")) {
            if (!details::resource_read(value["metadata"], result.metadata)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid GraderRunResponse.metadata", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("api_model_grader_token_usage_per_model")) {
            if (!details::resource_read(value["api_model_grader_token_usage_per_model"], result.api_model_grader_token_usage_per_model)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid GraderRunResponse.api_model_grader_token_usage_per_model", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("reward")) {
            if (!details::resource_read(value["reward"], result.reward)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid GraderRunResponse.reward", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("sub_rewards")) {
            if (!details::resource_read(value["sub_rewards"], result.sub_rewards)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid GraderRunResponse.sub_rewards", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto GraderValidateResponse::FromJson(Json::Value const& value) -> Result<GraderValidateResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        GraderValidateResponse result;
        result.raw = value;
        if (value.isMember("grader") && !value["grader"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["grader"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid GraderValidateResponse.grader", .body = value.toStyledString() } };
            }
            result.grader = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto PermissionCreateResponse::FromJson(Json::Value const& value) -> Result<PermissionCreateResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        PermissionCreateResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionCreateResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionCreateResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionCreateResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("project_id")) {
            if (!details::resource_read(value["project_id"], result.project_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionCreateResponse.project_id", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto PermissionRetrieveResponse::FromJson(Json::Value const& value) -> Result<PermissionRetrieveResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        PermissionRetrieveResponse result;
        result.raw = value;
        if (value.isMember("data")) {
            if (!details::resource_read(value["data"], result.data)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionRetrieveResponse.data", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("has_more")) {
            if (!details::resource_read(value["has_more"], result.has_more)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionRetrieveResponse.has_more", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionRetrieveResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("first_id") && !value["first_id"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["first_id"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionRetrieveResponse.first_id", .body = value.toStyledString() } };
            }
            result.first_id = std::move(field);
        }
        if (value.isMember("last_id") && !value["last_id"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["last_id"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionRetrieveResponse.last_id", .body = value.toStyledString() } };
            }
            result.last_id = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto PermissionListResponse::FromJson(Json::Value const& value) -> Result<PermissionListResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        PermissionListResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionListResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionListResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionListResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("project_id")) {
            if (!details::resource_read(value["project_id"], result.project_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionListResponse.project_id", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto PermissionDeleteResponse::FromJson(Json::Value const& value) -> Result<PermissionDeleteResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        PermissionDeleteResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionDeleteResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("deleted")) {
            if (!details::resource_read(value["deleted"], result.deleted)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionDeleteResponse.deleted", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid PermissionDeleteResponse.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto FineTuningJobCheckpoint::FromJson(Json::Value const& value) -> Result<FineTuningJobCheckpoint> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        FineTuningJobCheckpoint result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJobCheckpoint.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJobCheckpoint.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("fine_tuned_model_checkpoint")) {
            if (!details::resource_read(value["fine_tuned_model_checkpoint"], result.fine_tuned_model_checkpoint)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJobCheckpoint.fine_tuned_model_checkpoint", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("fine_tuning_job_id")) {
            if (!details::resource_read(value["fine_tuning_job_id"], result.fine_tuning_job_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJobCheckpoint.fine_tuning_job_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("metrics")) {
            if (!details::resource_read(value["metrics"], result.metrics)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJobCheckpoint.metrics", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJobCheckpoint.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("step_number")) {
            if (!details::resource_read(value["step_number"], result.step_number)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJobCheckpoint.step_number", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto FineTuningJob::FromJson(Json::Value const& value) -> Result<FineTuningJob> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        FineTuningJob result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("error") && !value["error"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["error"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.error", .body = value.toStyledString() } };
            }
            result.error = std::move(field);
        }
        if (value.isMember("fine_tuned_model") && !value["fine_tuned_model"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["fine_tuned_model"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.fine_tuned_model", .body = value.toStyledString() } };
            }
            result.fine_tuned_model = std::move(field);
        }
        if (value.isMember("finished_at") && !value["finished_at"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["finished_at"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.finished_at", .body = value.toStyledString() } };
            }
            result.finished_at = std::move(field);
        }
        if (value.isMember("hyperparameters")) {
            if (!details::resource_read(value["hyperparameters"], result.hyperparameters)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.hyperparameters", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("model")) {
            if (!details::resource_read(value["model"], result.model)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.model", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("organization_id")) {
            if (!details::resource_read(value["organization_id"], result.organization_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.organization_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("result_files")) {
            if (!details::resource_read(value["result_files"], result.result_files)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.result_files", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("seed")) {
            if (!details::resource_read(value["seed"], result.seed)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.seed", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.status", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("trained_tokens") && !value["trained_tokens"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["trained_tokens"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.trained_tokens", .body = value.toStyledString() } };
            }
            result.trained_tokens = std::move(field);
        }
        if (value.isMember("training_file")) {
            if (!details::resource_read(value["training_file"], result.training_file)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.training_file", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("validation_file") && !value["validation_file"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["validation_file"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.validation_file", .body = value.toStyledString() } };
            }
            result.validation_file = std::move(field);
        }
        if (value.isMember("estimated_finish") && !value["estimated_finish"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["estimated_finish"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.estimated_finish", .body = value.toStyledString() } };
            }
            result.estimated_finish = std::move(field);
        }
        if (value.isMember("integrations") && !value["integrations"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["integrations"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.integrations", .body = value.toStyledString() } };
            }
            result.integrations = std::move(field);
        }
        if (value.isMember("metadata") && !value["metadata"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["metadata"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.metadata", .body = value.toStyledString() } };
            }
            result.metadata = std::move(field);
        }
        if (value.isMember("method") && !value["method"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["method"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJob.method", .body = value.toStyledString() } };
            }
            result.method = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto FineTuningJobEvent::FromJson(Json::Value const& value) -> Result<FineTuningJobEvent> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        FineTuningJobEvent result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJobEvent.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJobEvent.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("level")) {
            if (!details::resource_read(value["level"], result.level)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJobEvent.level", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("message")) {
            if (!details::resource_read(value["message"], result.message)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJobEvent.message", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJobEvent.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("data") && !value["data"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["data"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJobEvent.data", .body = value.toStyledString() } };
            }
            result.data = std::move(field);
        }
        if (value.isMember("type") && !value["type"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["type"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid FineTuningJobEvent.type", .body = value.toStyledString() } };
            }
            result.type = std::move(field);
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto GraderRunParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["grader"] = details::resource_json(grader);
        payload.body["model_sample"] = details::resource_json(model_sample);
        if (item) {
            payload.body["item"] = details::resource_json(*item);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto GraderRunParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto GraderValidateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["grader"] = details::resource_json(grader);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto GraderValidateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto PermissionCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["project_ids"] = details::resource_json(project_ids);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto PermissionCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto PermissionRetrieveParams::Payload() const -> ResourcePayload {
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
        if (project_id) {
            payload.query["project_id"] = details::resource_json(*project_id);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto PermissionRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto PermissionListParams::Payload() const -> ResourcePayload {
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
        if (project_id) {
            payload.query["project_id"] = details::resource_json(*project_id);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto PermissionListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FineTuningCheckpointsPermissionsDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto FineTuningCheckpointsPermissionsDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto CheckpointListParams::Payload() const -> ResourcePayload {
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
    auto CheckpointListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto JobCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["model"] = details::resource_json(model);
        payload.body["training_file"] = details::resource_json(training_file);
        if (hyperparameters) {
            payload.body["hyperparameters"] = details::resource_json(*hyperparameters);
        }
        if (integrations) {
            payload.body["integrations"] = details::resource_json(*integrations);
        }
        if (metadata) {
            payload.body["metadata"] = details::resource_json(*metadata);
        }
        if (method) {
            payload.body["method"] = details::resource_json(*method);
        }
        if (seed) {
            payload.body["seed"] = details::resource_json(*seed);
        }
        if (suffix) {
            payload.body["suffix"] = details::resource_json(*suffix);
        }
        if (validation_file) {
            payload.body["validation_file"] = details::resource_json(*validation_file);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto JobCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FineTuningJobsRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto FineTuningJobsRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto JobListParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (after) {
            payload.query["after"] = details::resource_json(*after);
        }
        if (limit) {
            payload.query["limit"] = details::resource_json(*limit);
        }
        if (metadata) {
            payload.query["metadata"] = details::resource_json(*metadata);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto JobListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FineTuningJobsCancelParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto FineTuningJobsCancelParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto JobListEventsParams::Payload() const -> ResourcePayload {
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
    auto JobListEventsParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FineTuningJobsPauseParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto FineTuningJobsPauseParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto FineTuningJobsResumeParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto FineTuningJobsResumeParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::fine_tuning

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    FineTuningAlphaGraders::FineTuningAlphaGraders(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /fine_tuning/alpha/graders/run.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningAlphaGraders::Run(
        types::fine_tuning::GraderRunParams params
    ) const -> drogon::Task<Result<types::fine_tuning::GraderRunResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/alpha/graders/run", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::GraderRunResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /fine_tuning/alpha/graders/run.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningAlphaGraders::RunRaw(
        types::fine_tuning::GraderRunParams params
    ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::GraderRunResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/alpha/graders/run", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::GraderRunResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /fine_tuning/alpha/graders/validate.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningAlphaGraders::Validate(
        types::fine_tuning::GraderValidateParams params
    ) const -> drogon::Task<Result<types::fine_tuning::GraderValidateResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/alpha/graders/validate", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::GraderValidateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /fine_tuning/alpha/graders/validate.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningAlphaGraders::ValidateRaw(
        types::fine_tuning::GraderValidateParams params
    ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::GraderValidateResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/alpha/graders/validate", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::GraderValidateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    FineTuningCheckpointsPermissions::FineTuningCheckpointsPermissions(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions.
     * @param fine_tuned_model_checkpoint Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningCheckpointsPermissions::Create(
        std::string fine_tuned_model_checkpoint,
        types::fine_tuning::PermissionCreateParams params
    ) const -> drogon::Task<Result<Page<types::fine_tuning::PermissionCreateResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions", { { "fine_tuned_model_checkpoint", fine_tuned_model_checkpoint } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::fine_tuning::PermissionCreateResponse>(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}, false);
    }

    /**
     * @brief GET /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions.
     * @param fine_tuned_model_checkpoint Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningCheckpointsPermissions::Retrieve(
        std::string fine_tuned_model_checkpoint,
        types::fine_tuning::PermissionRetrieveParams params
    ) const -> drogon::Task<Result<types::fine_tuning::PermissionRetrieveResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions", { { "fine_tuned_model_checkpoint", fine_tuned_model_checkpoint } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::PermissionRetrieveResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions.
     * @param fine_tuned_model_checkpoint Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningCheckpointsPermissions::RetrieveRaw(
        std::string fine_tuned_model_checkpoint,
        types::fine_tuning::PermissionRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::PermissionRetrieveResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions", { { "fine_tuned_model_checkpoint", fine_tuned_model_checkpoint } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::PermissionRetrieveResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions.
     * @param fine_tuned_model_checkpoint Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningCheckpointsPermissions::List(
        std::string fine_tuned_model_checkpoint,
        types::fine_tuning::PermissionListParams params
    ) const -> drogon::Task<Result<Page<types::fine_tuning::PermissionListResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions", { { "fine_tuned_model_checkpoint", fine_tuned_model_checkpoint } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::fine_tuning::PermissionListResponse>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true, "last_id");
    }

    /**
     * @brief DELETE /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions/{permission_id}.
     * @param permission_id Owned path identifier, encoded as one segment.
     * @param fine_tuned_model_checkpoint Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningCheckpointsPermissions::Delete(
        std::string permission_id,
        std::string fine_tuned_model_checkpoint,
        types::fine_tuning::FineTuningCheckpointsPermissionsDeleteParams params
    ) const -> drogon::Task<Result<types::fine_tuning::PermissionDeleteResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions/{permission_id}", { { "permission_id", permission_id }, { "fine_tuned_model_checkpoint", fine_tuned_model_checkpoint } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::PermissionDeleteResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief DELETE /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions/{permission_id}.
     * @param permission_id Owned path identifier, encoded as one segment.
     * @param fine_tuned_model_checkpoint Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningCheckpointsPermissions::DeleteRaw(
        std::string permission_id,
        std::string fine_tuned_model_checkpoint,
        types::fine_tuning::FineTuningCheckpointsPermissionsDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::PermissionDeleteResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions/{permission_id}", { { "permission_id", permission_id }, { "fine_tuned_model_checkpoint", fine_tuned_model_checkpoint } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::PermissionDeleteResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    FineTuningJobsCheckpoints::FineTuningJobsCheckpoints(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief GET /fine_tuning/jobs/{fine_tuning_job_id}/checkpoints.
     * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningJobsCheckpoints::List(
        std::string fine_tuning_job_id,
        types::fine_tuning::CheckpointListParams params
    ) const -> drogon::Task<Result<Page<types::fine_tuning::FineTuningJobCheckpoint>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/jobs/{fine_tuning_job_id}/checkpoints", { { "fine_tuning_job_id", fine_tuning_job_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::fine_tuning::FineTuningJobCheckpoint>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    FineTuningAlpha::FineTuningAlpha(std::shared_ptr<Transport> transport) : m_transport{ transport }, graders{ transport } {}

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    FineTuningCheckpoints::FineTuningCheckpoints(std::shared_ptr<Transport> transport) : m_transport{ transport }, permissions{ transport } {}

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    FineTuningJobs::FineTuningJobs(std::shared_ptr<Transport> transport) : m_transport{ transport }, checkpoints{ transport } {}

    /**
     * @brief POST /fine_tuning/jobs.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningJobs::Create(
        types::fine_tuning::JobCreateParams params
    ) const -> drogon::Task<Result<types::fine_tuning::FineTuningJob>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/jobs", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::FineTuningJob>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /fine_tuning/jobs.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningJobs::CreateRaw(
        types::fine_tuning::JobCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::FineTuningJob>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/jobs", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::FineTuningJob>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /fine_tuning/jobs/{fine_tuning_job_id}.
     * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningJobs::Retrieve(
        std::string fine_tuning_job_id,
        types::fine_tuning::FineTuningJobsRetrieveParams params
    ) const -> drogon::Task<Result<types::fine_tuning::FineTuningJob>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/jobs/{fine_tuning_job_id}", { { "fine_tuning_job_id", fine_tuning_job_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::FineTuningJob>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /fine_tuning/jobs/{fine_tuning_job_id}.
     * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningJobs::RetrieveRaw(
        std::string fine_tuning_job_id,
        types::fine_tuning::FineTuningJobsRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::FineTuningJob>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/jobs/{fine_tuning_job_id}", { { "fine_tuning_job_id", fine_tuning_job_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::FineTuningJob>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /fine_tuning/jobs.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningJobs::List(
        types::fine_tuning::JobListParams params
    ) const -> drogon::Task<Result<Page<types::fine_tuning::FineTuningJob>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/jobs", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::fine_tuning::FineTuningJob>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true);
    }

    /**
     * @brief POST /fine_tuning/jobs/{fine_tuning_job_id}/cancel.
     * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningJobs::Cancel(
        std::string fine_tuning_job_id,
        types::fine_tuning::FineTuningJobsCancelParams params
    ) const -> drogon::Task<Result<types::fine_tuning::FineTuningJob>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/jobs/{fine_tuning_job_id}/cancel", { { "fine_tuning_job_id", fine_tuning_job_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::FineTuningJob>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /fine_tuning/jobs/{fine_tuning_job_id}/cancel.
     * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningJobs::CancelRaw(
        std::string fine_tuning_job_id,
        types::fine_tuning::FineTuningJobsCancelParams params
    ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::FineTuningJob>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/jobs/{fine_tuning_job_id}/cancel", { { "fine_tuning_job_id", fine_tuning_job_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::FineTuningJob>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /fine_tuning/jobs/{fine_tuning_job_id}/events.
     * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningJobs::ListEvents(
        std::string fine_tuning_job_id,
        types::fine_tuning::JobListEventsParams params
    ) const -> drogon::Task<Result<Page<types::fine_tuning::FineTuningJobEvent>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/jobs/{fine_tuning_job_id}/events", { { "fine_tuning_job_id", fine_tuning_job_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::fine_tuning::FineTuningJobEvent>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true);
    }

    /**
     * @brief POST /fine_tuning/jobs/{fine_tuning_job_id}/pause.
     * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningJobs::Pause(
        std::string fine_tuning_job_id,
        types::fine_tuning::FineTuningJobsPauseParams params
    ) const -> drogon::Task<Result<types::fine_tuning::FineTuningJob>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/jobs/{fine_tuning_job_id}/pause", { { "fine_tuning_job_id", fine_tuning_job_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::FineTuningJob>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /fine_tuning/jobs/{fine_tuning_job_id}/pause.
     * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningJobs::PauseRaw(
        std::string fine_tuning_job_id,
        types::fine_tuning::FineTuningJobsPauseParams params
    ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::FineTuningJob>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/jobs/{fine_tuning_job_id}/pause", { { "fine_tuning_job_id", fine_tuning_job_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::FineTuningJob>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /fine_tuning/jobs/{fine_tuning_job_id}/resume.
     * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningJobs::Resume(
        std::string fine_tuning_job_id,
        types::fine_tuning::FineTuningJobsResumeParams params
    ) const -> drogon::Task<Result<types::fine_tuning::FineTuningJob>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/jobs/{fine_tuning_job_id}/resume", { { "fine_tuning_job_id", fine_tuning_job_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::FineTuningJob>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /fine_tuning/jobs/{fine_tuning_job_id}/resume.
     * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto FineTuningJobs::ResumeRaw(
        std::string fine_tuning_job_id,
        types::fine_tuning::FineTuningJobsResumeParams params
    ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::FineTuningJob>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/fine_tuning/jobs/{fine_tuning_job_id}/resume", { { "fine_tuning_job_id", fine_tuning_job_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::fine_tuning::FineTuningJob>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    FineTuning::FineTuning(std::shared_ptr<Transport> transport) : m_transport{ transport }, jobs{ transport }, checkpoints{ transport }, alpha{ transport } {}
} // namespace drogon_openai::details
