/**
 * @file models.hpp
 * @brief models API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/models.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK models resource.
     */
    class Models {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Models(std::shared_ptr<Transport> transport);

        /**
         * @brief GET /models/{model}.
         * @param model Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string model,
            types::models::ModelsRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::models::Model>>;

        /**
         * @brief GET /models/{model}.
         * @param model Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string model,
            types::models::ModelsRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::models::Model>>>;

        /**
         * @brief GET /models.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            types::models::ModelsListParams params = {}
        ) const -> drogon::Task<Result<Page<types::models::Model>>>;

        /**
         * @brief DELETE /models/{model}.
         * @param model Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string model,
            types::models::ModelsDeleteParams params = {}
        ) const -> drogon::Task<Result<types::models::ModelDeleted>>;

        /**
         * @brief DELETE /models/{model}.
         * @param model Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string model,
            types::models::ModelsDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::models::ModelDeleted>>>;
    };
} // namespace drogon_openai::details
