/**
 * @file vector_stores.hpp
 * @brief vector_stores API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/vector_stores.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK vector_stores.file_batches resource.
     */
    class VectorStoresFileBatches {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit VectorStoresFileBatches(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /vector_stores/{vector_store_id}/file_batches.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            std::string vector_store_id,
            types::vector_stores::FileBatchCreateParams params = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFileBatch>>;

        /**
         * @brief POST /vector_stores/{vector_store_id}/file_batches.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            std::string vector_store_id,
            types::vector_stores::FileBatchCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreFileBatch>>>;

        /**
         * @brief GET /vector_stores/{vector_store_id}/file_batches/{batch_id}.
         * @param batch_id Owned path identifier, encoded as one segment.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string batch_id,
            std::string vector_store_id,
            types::vector_stores::VectorStoresFileBatchesRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFileBatch>>;

        /**
         * @brief GET /vector_stores/{vector_store_id}/file_batches/{batch_id}.
         * @param batch_id Owned path identifier, encoded as one segment.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string batch_id,
            std::string vector_store_id,
            types::vector_stores::VectorStoresFileBatchesRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreFileBatch>>>;

        /**
         * @brief POST /vector_stores/{vector_store_id}/file_batches/{batch_id}/cancel.
         * @param batch_id Owned path identifier, encoded as one segment.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Cancel(
            std::string batch_id,
            std::string vector_store_id,
            types::vector_stores::VectorStoresFileBatchesCancelParams params = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFileBatch>>;

        /**
         * @brief POST /vector_stores/{vector_store_id}/file_batches/{batch_id}/cancel.
         * @param batch_id Owned path identifier, encoded as one segment.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CancelRaw(
            std::string batch_id,
            std::string vector_store_id,
            types::vector_stores::VectorStoresFileBatchesCancelParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreFileBatch>>>;

        /**
         * @brief GET /vector_stores/{vector_store_id}/file_batches/{batch_id}/files.
         * @param batch_id Owned path identifier, encoded as one segment.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto ListFiles(
            std::string batch_id,
            std::string vector_store_id,
            types::vector_stores::FileBatchListFilesParams params = {}
        ) const -> drogon::Task<Result<Page<types::vector_stores::VectorStoreFile>>>;
    };

    /**
     * @brief Value handle for the SDK vector_stores.files resource.
     */
    class VectorStoresFiles {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit VectorStoresFiles(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /vector_stores/{vector_store_id}/files.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            std::string vector_store_id,
            types::vector_stores::FileCreateParams params = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFile>>;

        /**
         * @brief POST /vector_stores/{vector_store_id}/files.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            std::string vector_store_id,
            types::vector_stores::FileCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreFile>>>;

        /**
         * @brief GET /vector_stores/{vector_store_id}/files/{file_id}.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string file_id,
            std::string vector_store_id,
            types::vector_stores::VectorStoresFilesRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFile>>;

        /**
         * @brief GET /vector_stores/{vector_store_id}/files/{file_id}.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string file_id,
            std::string vector_store_id,
            types::vector_stores::VectorStoresFilesRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreFile>>>;

        /**
         * @brief POST /vector_stores/{vector_store_id}/files/{file_id}.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Update(
            std::string file_id,
            std::string vector_store_id,
            types::vector_stores::FileUpdateParams params = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFile>>;

        /**
         * @brief POST /vector_stores/{vector_store_id}/files/{file_id}.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto UpdateRaw(
            std::string file_id,
            std::string vector_store_id,
            types::vector_stores::FileUpdateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreFile>>>;

        /**
         * @brief GET /vector_stores/{vector_store_id}/files.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            std::string vector_store_id,
            types::vector_stores::FileListParams params = {}
        ) const -> drogon::Task<Result<Page<types::vector_stores::VectorStoreFile>>>;

        /**
         * @brief DELETE /vector_stores/{vector_store_id}/files/{file_id}.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string file_id,
            std::string vector_store_id,
            types::vector_stores::VectorStoresFilesDeleteParams params = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFileDeleted>>;

        /**
         * @brief DELETE /vector_stores/{vector_store_id}/files/{file_id}.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string file_id,
            std::string vector_store_id,
            types::vector_stores::VectorStoresFilesDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreFileDeleted>>>;

        /**
         * @brief GET /vector_stores/{vector_store_id}/files/{file_id}/content.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Content(
            std::string file_id,
            std::string vector_store_id,
            types::vector_stores::VectorStoresFilesContentParams params = {}
        ) const -> drogon::Task<Result<Page<types::vector_stores::FileContentResponse>>>;

        /**
         * @brief Poll an attached vector store file until processing stops.
         * @param file_id Owned operation parameters.
         * @param vector_store_id Owned operation parameters.
         * @param poll Owned operation parameters.
         * @param options Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto Poll(
            std::string file_id,
            std::string vector_store_id,
            PollOptions poll = {},
            RequestOptions options = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFile>>;

        /**
         * @brief Attach an uploaded file and wait for processing.
         * @param vector_store_id Owned operation parameters.
         * @param params Owned operation parameters.
         * @param poll Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto CreateAndPoll(
            std::string vector_store_id,
            types::vector_stores::FileCreateParams params,
            PollOptions poll = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFile>>;

        /**
         * @brief Upload a file and attach its ID to a vector store.
         * @param vector_store_id Owned operation parameters.
         * @param file Owned operation parameters.
         * @param params Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto Upload(
            std::string vector_store_id,
            UploadFile file,
            types::vector_stores::FileCreateParams params = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFile>>;

        /**
         * @brief Upload and attach a file, then wait for processing.
         * @param vector_store_id Owned operation parameters.
         * @param file Owned operation parameters.
         * @param params Owned operation parameters.
         * @param poll Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto UploadAndPoll(
            std::string vector_store_id,
            UploadFile file,
            types::vector_stores::FileCreateParams params = {},
            PollOptions poll = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStoreFile>>;
    };

    /**
     * @brief Value handle for the SDK vector_stores resource.
     */
    class VectorStores {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit VectorStores(std::shared_ptr<Transport> transport);
        VectorStoresFiles files;              ///< Nested SDK resource files.
        VectorStoresFileBatches file_batches; ///< Nested SDK resource file_batches.

        /**
         * @brief POST /vector_stores.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::vector_stores::VectorStoreCreateParams params = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStore>>;

        /**
         * @brief POST /vector_stores.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::vector_stores::VectorStoreCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStore>>>;

        /**
         * @brief GET /vector_stores/{vector_store_id}.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string vector_store_id,
            types::vector_stores::VectorStoresRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStore>>;

        /**
         * @brief GET /vector_stores/{vector_store_id}.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string vector_store_id,
            types::vector_stores::VectorStoresRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStore>>>;

        /**
         * @brief POST /vector_stores/{vector_store_id}.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Update(
            std::string vector_store_id,
            types::vector_stores::VectorStoreUpdateParams params = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStore>>;

        /**
         * @brief POST /vector_stores/{vector_store_id}.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto UpdateRaw(
            std::string vector_store_id,
            types::vector_stores::VectorStoreUpdateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStore>>>;

        /**
         * @brief GET /vector_stores.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            types::vector_stores::VectorStoreListParams params = {}
        ) const -> drogon::Task<Result<Page<types::vector_stores::VectorStore>>>;

        /**
         * @brief DELETE /vector_stores/{vector_store_id}.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string vector_store_id,
            types::vector_stores::VectorStoresDeleteParams params = {}
        ) const -> drogon::Task<Result<types::vector_stores::VectorStoreDeleted>>;

        /**
         * @brief DELETE /vector_stores/{vector_store_id}.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string vector_store_id,
            types::vector_stores::VectorStoresDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::vector_stores::VectorStoreDeleted>>>;

        /**
         * @brief POST /vector_stores/{vector_store_id}/search.
         * @param vector_store_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Search(
            std::string vector_store_id,
            types::vector_stores::VectorStoreSearchParams params = {}
        ) const -> drogon::Task<Result<Page<types::vector_stores::VectorStoreSearchResponse>>>;
    };
} // namespace drogon_openai::details
