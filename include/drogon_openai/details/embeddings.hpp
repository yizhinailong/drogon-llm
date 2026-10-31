/**
 * @file embeddings.hpp
 * @brief embeddings API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/embeddings.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK embeddings resource.
     */
    class Embeddings {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Embeddings(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /embeddings.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::embeddings::EmbeddingCreateParams params = {}
        ) const -> drogon::Task<Result<types::embeddings::CreateEmbeddingResponse>>;

        /**
         * @brief POST /embeddings.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::embeddings::EmbeddingCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::embeddings::CreateEmbeddingResponse>>>;
    };
} // namespace drogon_openai::details
