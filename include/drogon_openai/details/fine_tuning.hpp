/**
 * @file fine_tuning.hpp
 * @brief fine_tuning API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/fine_tuning.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK fine_tuning.alpha.graders resource.
     */
    class FineTuningAlphaGraders {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit FineTuningAlphaGraders(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /fine_tuning/alpha/graders/run.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Run(
            types::fine_tuning::GraderRunParams params = {}
        ) const -> drogon::Task<Result<types::fine_tuning::GraderRunResponse>>;

        /**
         * @brief POST /fine_tuning/alpha/graders/run.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RunRaw(
            types::fine_tuning::GraderRunParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::GraderRunResponse>>>;

        /**
         * @brief POST /fine_tuning/alpha/graders/validate.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Validate(
            types::fine_tuning::GraderValidateParams params = {}
        ) const -> drogon::Task<Result<types::fine_tuning::GraderValidateResponse>>;

        /**
         * @brief POST /fine_tuning/alpha/graders/validate.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto ValidateRaw(
            types::fine_tuning::GraderValidateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::GraderValidateResponse>>>;
    };

    /**
     * @brief Value handle for the SDK fine_tuning.checkpoints.permissions resource.
     */
    class FineTuningCheckpointsPermissions {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit FineTuningCheckpointsPermissions(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions.
         * @param fine_tuned_model_checkpoint Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            std::string fine_tuned_model_checkpoint,
            types::fine_tuning::PermissionCreateParams params = {}
        ) const -> drogon::Task<Result<Page<types::fine_tuning::PermissionCreateResponse>>>;

        /**
         * @brief GET /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions.
         * @param fine_tuned_model_checkpoint Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string fine_tuned_model_checkpoint,
            types::fine_tuning::PermissionRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::fine_tuning::PermissionRetrieveResponse>>;

        /**
         * @brief GET /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions.
         * @param fine_tuned_model_checkpoint Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string fine_tuned_model_checkpoint,
            types::fine_tuning::PermissionRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::PermissionRetrieveResponse>>>;

        /**
         * @brief GET /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions.
         * @param fine_tuned_model_checkpoint Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            std::string fine_tuned_model_checkpoint,
            types::fine_tuning::PermissionListParams params = {}
        ) const -> drogon::Task<Result<Page<types::fine_tuning::PermissionListResponse>>>;

        /**
         * @brief DELETE /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions/{permission_id}.
         * @param permission_id Owned path identifier, encoded as one segment.
         * @param fine_tuned_model_checkpoint Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string permission_id,
            std::string fine_tuned_model_checkpoint,
            types::fine_tuning::FineTuningCheckpointsPermissionsDeleteParams params = {}
        ) const -> drogon::Task<Result<types::fine_tuning::PermissionDeleteResponse>>;

        /**
         * @brief DELETE /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions/{permission_id}.
         * @param permission_id Owned path identifier, encoded as one segment.
         * @param fine_tuned_model_checkpoint Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string permission_id,
            std::string fine_tuned_model_checkpoint,
            types::fine_tuning::FineTuningCheckpointsPermissionsDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::PermissionDeleteResponse>>>;
    };

    /**
     * @brief Value handle for the SDK fine_tuning.jobs.checkpoints resource.
     */
    class FineTuningJobsCheckpoints {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit FineTuningJobsCheckpoints(std::shared_ptr<Transport> transport);

        /**
         * @brief GET /fine_tuning/jobs/{fine_tuning_job_id}/checkpoints.
         * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            std::string fine_tuning_job_id,
            types::fine_tuning::CheckpointListParams params = {}
        ) const -> drogon::Task<Result<Page<types::fine_tuning::FineTuningJobCheckpoint>>>;
    };

    /**
     * @brief Value handle for the SDK fine_tuning.alpha resource.
     */
    class FineTuningAlpha {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit FineTuningAlpha(std::shared_ptr<Transport> transport);
        FineTuningAlphaGraders graders; ///< Nested SDK resource graders.
    };

    /**
     * @brief Value handle for the SDK fine_tuning.checkpoints resource.
     */
    class FineTuningCheckpoints {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit FineTuningCheckpoints(std::shared_ptr<Transport> transport);
        FineTuningCheckpointsPermissions permissions; ///< Nested SDK resource permissions.
    };

    /**
     * @brief Value handle for the SDK fine_tuning.jobs resource.
     */
    class FineTuningJobs {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit FineTuningJobs(std::shared_ptr<Transport> transport);
        FineTuningJobsCheckpoints checkpoints; ///< Nested SDK resource checkpoints.

        /**
         * @brief POST /fine_tuning/jobs.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::fine_tuning::JobCreateParams params = {}
        ) const -> drogon::Task<Result<types::fine_tuning::FineTuningJob>>;

        /**
         * @brief POST /fine_tuning/jobs.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::fine_tuning::JobCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::FineTuningJob>>>;

        /**
         * @brief GET /fine_tuning/jobs/{fine_tuning_job_id}.
         * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string fine_tuning_job_id,
            types::fine_tuning::FineTuningJobsRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::fine_tuning::FineTuningJob>>;

        /**
         * @brief GET /fine_tuning/jobs/{fine_tuning_job_id}.
         * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string fine_tuning_job_id,
            types::fine_tuning::FineTuningJobsRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::FineTuningJob>>>;

        /**
         * @brief GET /fine_tuning/jobs.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            types::fine_tuning::JobListParams params = {}
        ) const -> drogon::Task<Result<Page<types::fine_tuning::FineTuningJob>>>;

        /**
         * @brief POST /fine_tuning/jobs/{fine_tuning_job_id}/cancel.
         * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Cancel(
            std::string fine_tuning_job_id,
            types::fine_tuning::FineTuningJobsCancelParams params = {}
        ) const -> drogon::Task<Result<types::fine_tuning::FineTuningJob>>;

        /**
         * @brief POST /fine_tuning/jobs/{fine_tuning_job_id}/cancel.
         * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CancelRaw(
            std::string fine_tuning_job_id,
            types::fine_tuning::FineTuningJobsCancelParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::FineTuningJob>>>;

        /**
         * @brief GET /fine_tuning/jobs/{fine_tuning_job_id}/events.
         * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto ListEvents(
            std::string fine_tuning_job_id,
            types::fine_tuning::JobListEventsParams params = {}
        ) const -> drogon::Task<Result<Page<types::fine_tuning::FineTuningJobEvent>>>;

        /**
         * @brief POST /fine_tuning/jobs/{fine_tuning_job_id}/pause.
         * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Pause(
            std::string fine_tuning_job_id,
            types::fine_tuning::FineTuningJobsPauseParams params = {}
        ) const -> drogon::Task<Result<types::fine_tuning::FineTuningJob>>;

        /**
         * @brief POST /fine_tuning/jobs/{fine_tuning_job_id}/pause.
         * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto PauseRaw(
            std::string fine_tuning_job_id,
            types::fine_tuning::FineTuningJobsPauseParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::FineTuningJob>>>;

        /**
         * @brief POST /fine_tuning/jobs/{fine_tuning_job_id}/resume.
         * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Resume(
            std::string fine_tuning_job_id,
            types::fine_tuning::FineTuningJobsResumeParams params = {}
        ) const -> drogon::Task<Result<types::fine_tuning::FineTuningJob>>;

        /**
         * @brief POST /fine_tuning/jobs/{fine_tuning_job_id}/resume.
         * @param fine_tuning_job_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto ResumeRaw(
            std::string fine_tuning_job_id,
            types::fine_tuning::FineTuningJobsResumeParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::fine_tuning::FineTuningJob>>>;
    };

    /**
     * @brief Value handle for the SDK fine_tuning resource.
     */
    class FineTuning {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit FineTuning(std::shared_ptr<Transport> transport);
        FineTuningJobs jobs;               ///< Nested SDK resource jobs.
        FineTuningCheckpoints checkpoints; ///< Nested SDK resource checkpoints.
        FineTuningAlpha alpha;             ///< Nested SDK resource alpha.
    };
} // namespace drogon_openai::details
