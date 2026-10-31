/**
 * @file containers.hpp
 * @brief containers API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/containers.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK containers.files.content resource.
     */
    class ContainersFilesContent {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit ContainersFilesContent(std::shared_ptr<Transport> transport);

        /**
         * @brief GET /containers/{container_id}/files/{file_id}/content.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param container_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string file_id,
            std::string container_id,
            types::containers::ContainersFilesContentRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<std::string>>>;
    };

    /**
     * @brief Value handle for the SDK containers.files resource.
     */
    class ContainersFiles {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit ContainersFiles(std::shared_ptr<Transport> transport);
        ContainersFilesContent content; ///< Nested SDK resource content.

        /**
         * @brief POST /containers/{container_id}/files.
         * @param container_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            std::string container_id,
            types::containers::FileCreateParams params = {}
        ) const -> drogon::Task<Result<types::containers::FileCreateResponse>>;

        /**
         * @brief POST /containers/{container_id}/files.
         * @param container_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            std::string container_id,
            types::containers::FileCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::containers::FileCreateResponse>>>;

        /**
         * @brief GET /containers/{container_id}/files/{file_id}.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param container_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string file_id,
            std::string container_id,
            types::containers::ContainersFilesRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::containers::FileRetrieveResponse>>;

        /**
         * @brief GET /containers/{container_id}/files/{file_id}.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param container_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string file_id,
            std::string container_id,
            types::containers::ContainersFilesRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::containers::FileRetrieveResponse>>>;

        /**
         * @brief GET /containers/{container_id}/files.
         * @param container_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            std::string container_id,
            types::containers::FileListParams params = {}
        ) const -> drogon::Task<Result<Page<types::containers::FileListResponse>>>;

        /**
         * @brief DELETE /containers/{container_id}/files/{file_id}.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param container_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string file_id,
            std::string container_id,
            types::containers::ContainersFilesDeleteParams params = {}
        ) const -> drogon::Task<Result<void>>;

        /**
         * @brief DELETE /containers/{container_id}/files/{file_id}.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param container_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string file_id,
            std::string container_id,
            types::containers::ContainersFilesDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;
    };

    /**
     * @brief Value handle for the SDK containers resource.
     */
    class Containers {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Containers(std::shared_ptr<Transport> transport);
        ContainersFiles files; ///< Nested SDK resource files.

        /**
         * @brief POST /containers.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::containers::ContainerCreateParams params = {}
        ) const -> drogon::Task<Result<types::containers::ContainerCreateResponse>>;

        /**
         * @brief POST /containers.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::containers::ContainerCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::containers::ContainerCreateResponse>>>;

        /**
         * @brief GET /containers/{container_id}.
         * @param container_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string container_id,
            types::containers::ContainersRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::containers::ContainerRetrieveResponse>>;

        /**
         * @brief GET /containers/{container_id}.
         * @param container_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string container_id,
            types::containers::ContainersRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::containers::ContainerRetrieveResponse>>>;

        /**
         * @brief GET /containers.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            types::containers::ContainerListParams params = {}
        ) const -> drogon::Task<Result<Page<types::containers::ContainerListResponse>>>;

        /**
         * @brief DELETE /containers/{container_id}.
         * @param container_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string container_id,
            types::containers::ContainersDeleteParams params = {}
        ) const -> drogon::Task<Result<void>>;

        /**
         * @brief DELETE /containers/{container_id}.
         * @param container_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string container_id,
            types::containers::ContainersDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;
    };
} // namespace drogon_openai::details
