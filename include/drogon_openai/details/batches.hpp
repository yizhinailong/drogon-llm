/**
 * @file batches.hpp
 * @brief batches API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/batches.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK batches resource.
     */
    class Batches {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Batches(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /batches.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::batches::BatchCreateParams params = {}
        ) const -> drogon::Task<Result<types::batches::Batch>>;

        /**
         * @brief POST /batches.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::batches::BatchCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::batches::Batch>>>;

        /**
         * @brief GET /batches/{batch_id}.
         * @param batch_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string batch_id,
            types::batches::BatchesRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::batches::Batch>>;

        /**
         * @brief GET /batches/{batch_id}.
         * @param batch_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string batch_id,
            types::batches::BatchesRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::batches::Batch>>>;

        /**
         * @brief GET /batches.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            types::batches::BatchListParams params = {}
        ) const -> drogon::Task<Result<Page<types::batches::Batch>>>;

        /**
         * @brief POST /batches/{batch_id}/cancel.
         * @param batch_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Cancel(
            std::string batch_id,
            types::batches::BatchesCancelParams params = {}
        ) const -> drogon::Task<Result<types::batches::Batch>>;

        /**
         * @brief POST /batches/{batch_id}/cancel.
         * @param batch_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CancelRaw(
            std::string batch_id,
            types::batches::BatchesCancelParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::batches::Batch>>>;
    };
} // namespace drogon_openai::details
