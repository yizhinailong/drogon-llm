/**
 * @file files.hpp
 * @brief files API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/files.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK files resource.
     */
    class Files {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Files(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /files.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::files::FileCreateParams params = {}
        ) const -> drogon::Task<Result<types::files::FileObject>>;

        /**
         * @brief POST /files.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::files::FileCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::files::FileObject>>>;

        /**
         * @brief GET /files/{file_id}.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string file_id,
            types::files::FilesRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::files::FileObject>>;

        /**
         * @brief GET /files/{file_id}.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string file_id,
            types::files::FilesRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::files::FileObject>>>;

        /**
         * @brief GET /files.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            types::files::FileListParams params = {}
        ) const -> drogon::Task<Result<Page<types::files::FileObject>>>;

        /**
         * @brief DELETE /files/{file_id}.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string file_id,
            types::files::FilesDeleteParams params = {}
        ) const -> drogon::Task<Result<types::files::FileDeleted>>;

        /**
         * @brief DELETE /files/{file_id}.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string file_id,
            types::files::FilesDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::files::FileDeleted>>>;

        /**
         * @brief GET /files/{file_id}/content.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Content(
            std::string file_id,
            types::files::FilesContentParams params = {}
        ) const -> drogon::Task<Result<RawResponse<std::string>>>;

        /**
         * @brief GET /files/{file_id}/content.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveContent(
            std::string file_id,
            types::files::FilesRetrieveContentParams params = {}
        ) const -> drogon::Task<Result<std::string>>;

        /**
         * @brief GET /files/{file_id}/content.
         * @param file_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveContentRaw(
            std::string file_id,
            types::files::FilesRetrieveContentParams params = {}
        ) const -> drogon::Task<Result<RawResponse<std::string>>>;

        /**
         * @brief Wait for a file to finish processing or enter an error state.
         * @param file_id Owned operation parameters.
         * @param poll Owned operation parameters.
         * @param options Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto WaitForProcessing(
            std::string file_id,
            PollOptions poll = {},
            RequestOptions options = {}
        ) const -> drogon::Task<Result<types::files::FileObject>>;
    };
} // namespace drogon_openai::details
