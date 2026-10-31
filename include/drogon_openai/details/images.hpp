/**
 * @file images.hpp
 * @brief images API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/images.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK images resource.
     */
    class Images {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Images(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /images/variations.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateVariation(
            types::images::ImageCreateVariationParams params = {}
        ) const -> drogon::Task<Result<types::images::ImagesResponse>>;

        /**
         * @brief POST /images/variations.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateVariationRaw(
            types::images::ImageCreateVariationParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::images::ImagesResponse>>>;

        /**
         * @brief POST /images/edits.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Edit(
            types::images::ImagesEditParams params = {}
        ) const -> drogon::Task<Result<types::images::ImagesResponse>>;

        /**
         * @brief POST /images/edits.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto EditRaw(
            types::images::ImagesEditParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::images::ImagesResponse>>>;

        /**
         * @brief Stream SSE from POST /images/edits.
         * @param params Owned SDK parameters.
         * @param callback Event consumer; false stops delivery.
         * @return Final event and stream metadata or an error.
         */
        [[nodiscard]] auto EditStream(
            types::images::ImagesEditParams params,
            ApiEventCallback callback = {}
        ) const -> drogon::Task<Result<ApiStreamResult>>;

        /**
         * @brief POST /images/generations.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Generate(
            types::images::ImagesGenerateParams params = {}
        ) const -> drogon::Task<Result<types::images::ImagesResponse>>;

        /**
         * @brief POST /images/generations.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto GenerateRaw(
            types::images::ImagesGenerateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::images::ImagesResponse>>>;

        /**
         * @brief Stream SSE from POST /images/generations.
         * @param params Owned SDK parameters.
         * @param callback Event consumer; false stops delivery.
         * @return Final event and stream metadata or an error.
         */
        [[nodiscard]] auto GenerateStream(
            types::images::ImagesGenerateParams params,
            ApiEventCallback callback = {}
        ) const -> drogon::Task<Result<ApiStreamResult>>;
    };
} // namespace drogon_openai::details
