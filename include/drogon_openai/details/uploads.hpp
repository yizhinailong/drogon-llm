/**
 * @file uploads.hpp
 * @brief uploads API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/uploads.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK uploads.parts resource.
     */
    class UploadsParts {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit UploadsParts(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /uploads/{upload_id}/parts.
         * @param upload_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            std::string upload_id,
            types::uploads::PartCreateParams params = {}
        ) const -> drogon::Task<Result<types::uploads::UploadPart>>;

        /**
         * @brief POST /uploads/{upload_id}/parts.
         * @param upload_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            std::string upload_id,
            types::uploads::PartCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::uploads::UploadPart>>>;
    };

    /**
     * @brief Value handle for the SDK uploads resource.
     */
    class Uploads {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Uploads(std::shared_ptr<Transport> transport);
        UploadsParts parts; ///< Nested SDK resource parts.

        /**
         * @brief POST /uploads.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::uploads::UploadCreateParams params = {}
        ) const -> drogon::Task<Result<types::uploads::Upload>>;

        /**
         * @brief POST /uploads.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::uploads::UploadCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::uploads::Upload>>>;

        /**
         * @brief POST /uploads/{upload_id}/cancel.
         * @param upload_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Cancel(
            std::string upload_id,
            types::uploads::UploadsCancelParams params = {}
        ) const -> drogon::Task<Result<types::uploads::Upload>>;

        /**
         * @brief POST /uploads/{upload_id}/cancel.
         * @param upload_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CancelRaw(
            std::string upload_id,
            types::uploads::UploadsCancelParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::uploads::Upload>>>;

        /**
         * @brief POST /uploads/{upload_id}/complete.
         * @param upload_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Complete(
            std::string upload_id,
            types::uploads::UploadCompleteParams params = {}
        ) const -> drogon::Task<Result<types::uploads::Upload>>;

        /**
         * @brief POST /uploads/{upload_id}/complete.
         * @param upload_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CompleteRaw(
            std::string upload_id,
            types::uploads::UploadCompleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::uploads::Upload>>>;

        /**
         * @brief Upload owned file bytes as ordered parts and complete the upload.
         * @param file Owned operation parameters.
         * @param purpose Owned operation parameters.
         * @param part_size Owned operation parameters.
         * @param options Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto UploadFileChunked(
            UploadFile file,
            std::string purpose,
            std::size_t part_size = 67108864,
            RequestOptions options = {}
        ) const -> drogon::Task<Result<types::uploads::Upload>>;
    };
} // namespace drogon_openai::details
