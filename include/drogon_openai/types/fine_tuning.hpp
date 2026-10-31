/**
 * @file fine_tuning.hpp
 * @brief Request and response types for fine_tuning; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::fine_tuning {

    /**
     * @brief Response GraderRunResponse, retaining SDK union fields as JSON.
     */
    struct GraderRunResponse {
        Json::Value metadata{};                               ///< Wire field metadata.
        Json::Value api_model_grader_token_usage_per_model{}; ///< Wire field api_model_grader_token_usage_per_model.
        double reward{};                                      ///< Wire field reward.
        Json::Value sub_rewards{};                            ///< Wire field sub_rewards.
        Json::Value raw{};                                    ///< Complete JSON including provider extensions.
        std::string _request_id;                              ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<GraderRunResponse>;
    };

    /**
     * @brief Response GraderValidateResponse, retaining SDK union fields as JSON.
     */
    struct GraderValidateResponse {
        std::optional<Json::Value> grader{}; ///< Wire field grader.
        Json::Value raw{};                   ///< Complete JSON including provider extensions.
        std::string _request_id;             ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<GraderValidateResponse>;
    };

    /**
     * @brief Response PermissionCreateResponse, retaining SDK union fields as JSON.
     */
    struct PermissionCreateResponse {
        std::string id{};          ///< Wire field id.
        std::int64_t created_at{}; ///< Wire field created_at.
        std::string object{};      ///< Wire field object.
        std::string project_id{};  ///< Wire field project_id.
        Json::Value raw{};         ///< Complete JSON including provider extensions.
        std::string _request_id;   ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<PermissionCreateResponse>;
    };

    /**
     * @brief Response PermissionRetrieveResponse, retaining SDK union fields as JSON.
     */
    struct PermissionRetrieveResponse {
        Json::Value data{};                    ///< Wire field data.
        bool has_more{};                       ///< Wire field has_more.
        std::string object{};                  ///< Wire field object.
        std::optional<Json::Value> first_id{}; ///< Wire field first_id.
        std::optional<Json::Value> last_id{};  ///< Wire field last_id.
        Json::Value raw{};                     ///< Complete JSON including provider extensions.
        std::string _request_id;               ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<PermissionRetrieveResponse>;
    };

    /**
     * @brief Response PermissionListResponse, retaining SDK union fields as JSON.
     */
    struct PermissionListResponse {
        std::string id{};          ///< Wire field id.
        std::int64_t created_at{}; ///< Wire field created_at.
        std::string object{};      ///< Wire field object.
        std::string project_id{};  ///< Wire field project_id.
        Json::Value raw{};         ///< Complete JSON including provider extensions.
        std::string _request_id;   ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<PermissionListResponse>;
    };

    /**
     * @brief Response PermissionDeleteResponse, retaining SDK union fields as JSON.
     */
    struct PermissionDeleteResponse {
        std::string id{};        ///< Wire field id.
        bool deleted{};          ///< Wire field deleted.
        std::string object{};    ///< Wire field object.
        Json::Value raw{};       ///< Complete JSON including provider extensions.
        std::string _request_id; ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<PermissionDeleteResponse>;
    };

    /**
     * @brief Response FineTuningJobCheckpoint, retaining SDK union fields as JSON.
     */
    struct FineTuningJobCheckpoint {
        std::string id{};                          ///< Wire field id.
        std::int64_t created_at{};                 ///< Wire field created_at.
        std::string fine_tuned_model_checkpoint{}; ///< Wire field fine_tuned_model_checkpoint.
        std::string fine_tuning_job_id{};          ///< Wire field fine_tuning_job_id.
        Json::Value metrics{};                     ///< Wire field metrics.
        std::string object{};                      ///< Wire field object.
        std::int64_t step_number{};                ///< Wire field step_number.
        Json::Value raw{};                         ///< Complete JSON including provider extensions.
        std::string _request_id;                   ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<FineTuningJobCheckpoint>;
    };

    /**
     * @brief Response FineTuningJob, retaining SDK union fields as JSON.
     */
    struct FineTuningJob {
        std::string id{};                              ///< Wire field id.
        std::int64_t created_at{};                     ///< Wire field created_at.
        std::optional<Json::Value> error{};            ///< Wire field error.
        std::optional<Json::Value> fine_tuned_model{}; ///< Wire field fine_tuned_model.
        std::optional<Json::Value> finished_at{};      ///< Wire field finished_at.
        Json::Value hyperparameters{};                 ///< Wire field hyperparameters.
        std::string model{};                           ///< Wire field model.
        std::string object{};                          ///< Wire field object.
        std::string organization_id{};                 ///< Wire field organization_id.
        std::vector<std::string> result_files{};       ///< Wire field result_files.
        std::int64_t seed{};                           ///< Wire field seed.
        std::string status{};                          ///< Wire field status.
        std::optional<Json::Value> trained_tokens{};   ///< Wire field trained_tokens.
        std::string training_file{};                   ///< Wire field training_file.
        std::optional<Json::Value> validation_file{};  ///< Wire field validation_file.
        std::optional<Json::Value> estimated_finish{}; ///< Wire field estimated_finish.
        std::optional<Json::Value> integrations{};     ///< Wire field integrations.
        std::optional<Json::Value> metadata{};         ///< Wire field metadata.
        std::optional<Json::Value> method{};           ///< Wire field method.
        Json::Value raw{};                             ///< Complete JSON including provider extensions.
        std::string _request_id;                       ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<FineTuningJob>;
    };

    /**
     * @brief Response FineTuningJobEvent, retaining SDK union fields as JSON.
     */
    struct FineTuningJobEvent {
        std::string id{};                  ///< Wire field id.
        std::int64_t created_at{};         ///< Wire field created_at.
        std::string level{};               ///< Wire field level.
        std::string message{};             ///< Wire field message.
        std::string object{};              ///< Wire field object.
        std::optional<Json::Value> data{}; ///< Wire field data.
        std::optional<Json::Value> type{}; ///< Wire field type.
        Json::Value raw{};                 ///< Complete JSON including provider extensions.
        std::string _request_id;           ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<FineTuningJobEvent>;
    };

    /**
     * @brief Parameters for GraderRunParams; omitted optionals are absent on the wire.
     */
    struct GraderRunParams {
        Json::Value grader{};              ///< Required SDK field grader.
        std::string model_sample{};        ///< Required SDK field model_sample.
        std::optional<Json::Value> item{}; ///< Optional SDK field item.
        Json::Value extra_body{};          ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};  ///< Override phase timeouts.
        std::optional<int> max_retries{};  ///< Override retries.
        Headers extra_headers{};           ///< Override request headers.
        Json::Value extra_query{};         ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for GraderValidateParams; omitted optionals are absent on the wire.
     */
    struct GraderValidateParams {
        Json::Value grader{};             ///< Required SDK field grader.
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for PermissionCreateParams; omitted optionals are absent on the wire.
     */
    struct PermissionCreateParams {
        std::vector<std::string> project_ids{}; ///< Required SDK field project_ids.
        Json::Value extra_body{};               ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};       ///< Override phase timeouts.
        std::optional<int> max_retries{};       ///< Override retries.
        Headers extra_headers{};                ///< Override request headers.
        Json::Value extra_query{};              ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for PermissionRetrieveParams; omitted optionals are absent on the wire.
     */
    struct PermissionRetrieveParams {
        std::optional<std::string> after{};      ///< Optional SDK field after.
        std::optional<std::int64_t> limit{};     ///< Optional SDK field limit.
        std::optional<std::string> order{};      ///< Optional SDK field order.
        std::optional<std::string> project_id{}; ///< Optional SDK field project_id.
        Json::Value extra_body{};                ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};        ///< Override phase timeouts.
        std::optional<int> max_retries{};        ///< Override retries.
        Headers extra_headers{};                 ///< Override request headers.
        Json::Value extra_query{};               ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for PermissionListParams; omitted optionals are absent on the wire.
     */
    struct PermissionListParams {
        std::optional<std::string> after{};      ///< Optional SDK field after.
        std::optional<std::int64_t> limit{};     ///< Optional SDK field limit.
        std::optional<std::string> order{};      ///< Optional SDK field order.
        std::optional<std::string> project_id{}; ///< Optional SDK field project_id.
        Json::Value extra_body{};                ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};        ///< Override phase timeouts.
        std::optional<int> max_retries{};        ///< Override retries.
        Headers extra_headers{};                 ///< Override request headers.
        Json::Value extra_query{};               ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for FineTuningCheckpointsPermissionsDeleteParams; omitted optionals are absent on the wire.
     */
    struct FineTuningCheckpointsPermissionsDeleteParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for CheckpointListParams; omitted optionals are absent on the wire.
     */
    struct CheckpointListParams {
        std::optional<std::string> after{};  ///< Optional SDK field after.
        std::optional<std::int64_t> limit{}; ///< Optional SDK field limit.
        Json::Value extra_body{};            ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};    ///< Override phase timeouts.
        std::optional<int> max_retries{};    ///< Override retries.
        Headers extra_headers{};             ///< Override request headers.
        Json::Value extra_query{};           ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for JobCreateParams; omitted optionals are absent on the wire.
     */
    struct JobCreateParams {
        std::string model{};                          ///< Required SDK field model.
        std::string training_file{};                  ///< Required SDK field training_file.
        std::optional<Json::Value> hyperparameters{}; ///< Optional SDK field hyperparameters.
        std::optional<Json::Value> integrations{};    ///< Optional SDK field integrations.
        std::optional<Json::Value> metadata{};        ///< Optional SDK field metadata.
        std::optional<Json::Value> method{};          ///< Optional SDK field method.
        std::optional<Json::Value> seed{};            ///< Optional SDK field seed.
        std::optional<Json::Value> suffix{};          ///< Optional SDK field suffix.
        std::optional<Json::Value> validation_file{}; ///< Optional SDK field validation_file.
        Json::Value extra_body{};                     ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};             ///< Override phase timeouts.
        std::optional<int> max_retries{};             ///< Override retries.
        Headers extra_headers{};                      ///< Override request headers.
        Json::Value extra_query{};                    ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for FineTuningJobsRetrieveParams; omitted optionals are absent on the wire.
     */
    struct FineTuningJobsRetrieveParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for JobListParams; omitted optionals are absent on the wire.
     */
    struct JobListParams {
        std::optional<std::string> after{};    ///< Optional SDK field after.
        std::optional<std::int64_t> limit{};   ///< Optional SDK field limit.
        std::optional<Json::Value> metadata{}; ///< Optional SDK field metadata.
        Json::Value extra_body{};              ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};      ///< Override phase timeouts.
        std::optional<int> max_retries{};      ///< Override retries.
        Headers extra_headers{};               ///< Override request headers.
        Json::Value extra_query{};             ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for FineTuningJobsCancelParams; omitted optionals are absent on the wire.
     */
    struct FineTuningJobsCancelParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for JobListEventsParams; omitted optionals are absent on the wire.
     */
    struct JobListEventsParams {
        std::optional<std::string> after{};  ///< Optional SDK field after.
        std::optional<std::int64_t> limit{}; ///< Optional SDK field limit.
        Json::Value extra_body{};            ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};    ///< Override phase timeouts.
        std::optional<int> max_retries{};    ///< Override retries.
        Headers extra_headers{};             ///< Override request headers.
        Json::Value extra_query{};           ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for FineTuningJobsPauseParams; omitted optionals are absent on the wire.
     */
    struct FineTuningJobsPauseParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for FineTuningJobsResumeParams; omitted optionals are absent on the wire.
     */
    struct FineTuningJobsResumeParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };
} // namespace drogon_openai::types::fine_tuning
