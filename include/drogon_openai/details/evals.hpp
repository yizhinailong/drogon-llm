/**
 * @file evals.hpp
 * @brief evals API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/evals.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK evals.runs.output_items resource.
     */
    class EvalsRunsOutputItems {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit EvalsRunsOutputItems(std::shared_ptr<Transport> transport);

        /**
         * @brief GET /evals/{eval_id}/runs/{run_id}/output_items/{output_item_id}.
         * @param output_item_id Owned path identifier, encoded as one segment.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param run_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string output_item_id,
            std::string eval_id,
            std::string run_id,
            types::evals::EvalsRunsOutputItemsRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::evals::OutputItemRetrieveResponse>>;

        /**
         * @brief GET /evals/{eval_id}/runs/{run_id}/output_items/{output_item_id}.
         * @param output_item_id Owned path identifier, encoded as one segment.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param run_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string output_item_id,
            std::string eval_id,
            std::string run_id,
            types::evals::EvalsRunsOutputItemsRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::evals::OutputItemRetrieveResponse>>>;

        /**
         * @brief GET /evals/{eval_id}/runs/{run_id}/output_items.
         * @param run_id Owned path identifier, encoded as one segment.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            std::string run_id,
            std::string eval_id,
            types::evals::OutputItemListParams params = {}
        ) const -> drogon::Task<Result<Page<types::evals::OutputItemListResponse>>>;
    };

    /**
     * @brief Value handle for the SDK evals.runs resource.
     */
    class EvalsRuns {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit EvalsRuns(std::shared_ptr<Transport> transport);
        EvalsRunsOutputItems output_items; ///< Nested SDK resource output_items.

        /**
         * @brief POST /evals/{eval_id}/runs.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            std::string eval_id,
            types::evals::RunCreateParams params = {}
        ) const -> drogon::Task<Result<types::evals::RunCreateResponse>>;

        /**
         * @brief POST /evals/{eval_id}/runs.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            std::string eval_id,
            types::evals::RunCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::evals::RunCreateResponse>>>;

        /**
         * @brief GET /evals/{eval_id}/runs/{run_id}.
         * @param run_id Owned path identifier, encoded as one segment.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string run_id,
            std::string eval_id,
            types::evals::EvalsRunsRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::evals::RunRetrieveResponse>>;

        /**
         * @brief GET /evals/{eval_id}/runs/{run_id}.
         * @param run_id Owned path identifier, encoded as one segment.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string run_id,
            std::string eval_id,
            types::evals::EvalsRunsRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::evals::RunRetrieveResponse>>>;

        /**
         * @brief GET /evals/{eval_id}/runs.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            std::string eval_id,
            types::evals::RunListParams params = {}
        ) const -> drogon::Task<Result<Page<types::evals::RunListResponse>>>;

        /**
         * @brief DELETE /evals/{eval_id}/runs/{run_id}.
         * @param run_id Owned path identifier, encoded as one segment.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string run_id,
            std::string eval_id,
            types::evals::EvalsRunsDeleteParams params = {}
        ) const -> drogon::Task<Result<types::evals::RunDeleteResponse>>;

        /**
         * @brief DELETE /evals/{eval_id}/runs/{run_id}.
         * @param run_id Owned path identifier, encoded as one segment.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string run_id,
            std::string eval_id,
            types::evals::EvalsRunsDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::evals::RunDeleteResponse>>>;

        /**
         * @brief POST /evals/{eval_id}/runs/{run_id}/cancel.
         * @param run_id Owned path identifier, encoded as one segment.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Cancel(
            std::string run_id,
            std::string eval_id,
            types::evals::EvalsRunsCancelParams params = {}
        ) const -> drogon::Task<Result<types::evals::RunCancelResponse>>;

        /**
         * @brief POST /evals/{eval_id}/runs/{run_id}/cancel.
         * @param run_id Owned path identifier, encoded as one segment.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CancelRaw(
            std::string run_id,
            std::string eval_id,
            types::evals::EvalsRunsCancelParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::evals::RunCancelResponse>>>;
    };

    /**
     * @brief Value handle for the SDK evals resource.
     */
    class Evals {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Evals(std::shared_ptr<Transport> transport);
        EvalsRuns runs; ///< Nested SDK resource runs.

        /**
         * @brief POST /evals.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::evals::EvalCreateParams params = {}
        ) const -> drogon::Task<Result<types::evals::EvalCreateResponse>>;

        /**
         * @brief POST /evals.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::evals::EvalCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::evals::EvalCreateResponse>>>;

        /**
         * @brief GET /evals/{eval_id}.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string eval_id,
            types::evals::EvalsRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::evals::EvalRetrieveResponse>>;

        /**
         * @brief GET /evals/{eval_id}.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string eval_id,
            types::evals::EvalsRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::evals::EvalRetrieveResponse>>>;

        /**
         * @brief POST /evals/{eval_id}.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Update(
            std::string eval_id,
            types::evals::EvalUpdateParams params = {}
        ) const -> drogon::Task<Result<types::evals::EvalUpdateResponse>>;

        /**
         * @brief POST /evals/{eval_id}.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto UpdateRaw(
            std::string eval_id,
            types::evals::EvalUpdateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::evals::EvalUpdateResponse>>>;

        /**
         * @brief GET /evals.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            types::evals::EvalListParams params = {}
        ) const -> drogon::Task<Result<Page<types::evals::EvalListResponse>>>;

        /**
         * @brief DELETE /evals/{eval_id}.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string eval_id,
            types::evals::EvalsDeleteParams params = {}
        ) const -> drogon::Task<Result<types::evals::EvalDeleteResponse>>;

        /**
         * @brief DELETE /evals/{eval_id}.
         * @param eval_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string eval_id,
            types::evals::EvalsDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::evals::EvalDeleteResponse>>>;
    };
} // namespace drogon_openai::details
