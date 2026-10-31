/**
 * @file vector_stores.hpp
 * @brief Request and response types for vector_stores; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::vector_stores {

    /**
     * @brief Response VectorStoreFileBatch, retaining SDK union fields as JSON.
     */
    struct VectorStoreFileBatch {
        std::string id{};              ///< Wire field id.
        std::int64_t created_at{};     ///< Wire field created_at.
        Json::Value file_counts{};     ///< Wire field file_counts.
        std::string object{};          ///< Wire field object.
        std::string status{};          ///< Wire field status.
        std::string vector_store_id{}; ///< Wire field vector_store_id.
        Json::Value raw{};             ///< Complete JSON including provider extensions.
        std::string _request_id;       ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<VectorStoreFileBatch>;
    };

    /**
     * @brief Response VectorStoreFile, retaining SDK union fields as JSON.
     */
    struct VectorStoreFile {
        std::string id{};                               ///< Wire field id.
        std::int64_t created_at{};                      ///< Wire field created_at.
        std::optional<Json::Value> last_error{};        ///< Wire field last_error.
        std::string object{};                           ///< Wire field object.
        std::string status{};                           ///< Wire field status.
        std::int64_t usage_bytes{};                     ///< Wire field usage_bytes.
        std::string vector_store_id{};                  ///< Wire field vector_store_id.
        std::optional<Json::Value> attributes{};        ///< Wire field attributes.
        std::optional<Json::Value> chunking_strategy{}; ///< Wire field chunking_strategy.
        Json::Value raw{};                              ///< Complete JSON including provider extensions.
        std::string _request_id;                        ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<VectorStoreFile>;
    };

    /**
     * @brief Response VectorStoreFileDeleted, retaining SDK union fields as JSON.
     */
    struct VectorStoreFileDeleted {
        std::string id{};        ///< Wire field id.
        bool deleted{};          ///< Wire field deleted.
        std::string object{};    ///< Wire field object.
        Json::Value raw{};       ///< Complete JSON including provider extensions.
        std::string _request_id; ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<VectorStoreFileDeleted>;
    };

    /**
     * @brief Response FileContentResponse, retaining SDK union fields as JSON.
     */
    struct FileContentResponse {
        std::optional<Json::Value> text{}; ///< Wire field text.
        std::optional<Json::Value> type{}; ///< Wire field type.
        Json::Value raw{};                 ///< Complete JSON including provider extensions.
        std::string _request_id;           ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<FileContentResponse>;
    };

    /**
     * @brief Response VectorStore, retaining SDK union fields as JSON.
     */
    struct VectorStore {
        std::string id{};                            ///< Wire field id.
        std::int64_t created_at{};                   ///< Wire field created_at.
        Json::Value file_counts{};                   ///< Wire field file_counts.
        std::optional<Json::Value> last_active_at{}; ///< Wire field last_active_at.
        std::optional<Json::Value> metadata{};       ///< Wire field metadata.
        std::string name{};                          ///< Wire field name.
        std::string object{};                        ///< Wire field object.
        std::string status{};                        ///< Wire field status.
        std::int64_t usage_bytes{};                  ///< Wire field usage_bytes.
        std::optional<Json::Value> expires_after{};  ///< Wire field expires_after.
        std::optional<Json::Value> expires_at{};     ///< Wire field expires_at.
        Json::Value raw{};                           ///< Complete JSON including provider extensions.
        std::string _request_id;                     ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<VectorStore>;
    };

    /**
     * @brief Response VectorStoreDeleted, retaining SDK union fields as JSON.
     */
    struct VectorStoreDeleted {
        std::string id{};        ///< Wire field id.
        bool deleted{};          ///< Wire field deleted.
        std::string object{};    ///< Wire field object.
        Json::Value raw{};       ///< Complete JSON including provider extensions.
        std::string _request_id; ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<VectorStoreDeleted>;
    };

    /**
     * @brief Response VectorStoreSearchResponse, retaining SDK union fields as JSON.
     */
    struct VectorStoreSearchResponse {
        std::optional<Json::Value> attributes{}; ///< Wire field attributes.
        Json::Value content{};                   ///< Wire field content.
        std::string file_id{};                   ///< Wire field file_id.
        std::string filename{};                  ///< Wire field filename.
        double score{};                          ///< Wire field score.
        Json::Value raw{};                       ///< Complete JSON including provider extensions.
        std::string _request_id;                 ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<VectorStoreSearchResponse>;
    };

    /**
     * @brief Parameters for FileBatchCreateParams; omitted optionals are absent on the wire.
     */
    struct FileBatchCreateParams {
        std::optional<Json::Value> attributes{};            ///< Optional SDK field attributes.
        std::optional<Json::Value> chunking_strategy{};     ///< Optional SDK field chunking_strategy.
        std::optional<std::vector<std::string>> file_ids{}; ///< Optional SDK field file_ids.
        std::optional<Json::Value> files{};                 ///< Optional SDK field files.
        Json::Value extra_body{};                           ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};                   ///< Override phase timeouts.
        std::optional<int> max_retries{};                   ///< Override retries.
        Headers extra_headers{};                            ///< Override request headers.
        Json::Value extra_query{};                          ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for VectorStoresFileBatchesRetrieveParams; omitted optionals are absent on the wire.
     */
    struct VectorStoresFileBatchesRetrieveParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for VectorStoresFileBatchesCancelParams; omitted optionals are absent on the wire.
     */
    struct VectorStoresFileBatchesCancelParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for FileBatchListFilesParams; omitted optionals are absent on the wire.
     */
    struct FileBatchListFilesParams {
        std::optional<std::string> after{};  ///< Optional SDK field after.
        std::optional<std::string> before{}; ///< Optional SDK field before.
        std::optional<std::string> filter{}; ///< Optional SDK field filter.
        std::optional<std::int64_t> limit{}; ///< Optional SDK field limit.
        std::optional<std::string> order{};  ///< Optional SDK field order.
        Json::Value extra_body{};            ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};    ///< Override phase timeouts.
        std::optional<int> max_retries{};    ///< Override retries.
        Headers extra_headers{};             ///< Override request headers.
        Json::Value extra_query{};           ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for FileCreateParams; omitted optionals are absent on the wire.
     */
    struct FileCreateParams {
        std::string file_id{};                          ///< Required SDK field file_id.
        std::optional<Json::Value> attributes{};        ///< Optional SDK field attributes.
        std::optional<Json::Value> chunking_strategy{}; ///< Optional SDK field chunking_strategy.
        Json::Value extra_body{};                       ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};               ///< Override phase timeouts.
        std::optional<int> max_retries{};               ///< Override retries.
        Headers extra_headers{};                        ///< Override request headers.
        Json::Value extra_query{};                      ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for VectorStoresFilesRetrieveParams; omitted optionals are absent on the wire.
     */
    struct VectorStoresFilesRetrieveParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for FileUpdateParams; omitted optionals are absent on the wire.
     */
    struct FileUpdateParams {
        Json::Value attributes{};         ///< Required SDK field attributes.
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for FileListParams; omitted optionals are absent on the wire.
     */
    struct FileListParams {
        std::optional<std::string> after{};  ///< Optional SDK field after.
        std::optional<std::string> before{}; ///< Optional SDK field before.
        std::optional<std::string> filter{}; ///< Optional SDK field filter.
        std::optional<std::int64_t> limit{}; ///< Optional SDK field limit.
        std::optional<std::string> order{};  ///< Optional SDK field order.
        Json::Value extra_body{};            ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};    ///< Override phase timeouts.
        std::optional<int> max_retries{};    ///< Override retries.
        Headers extra_headers{};             ///< Override request headers.
        Json::Value extra_query{};           ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for VectorStoresFilesDeleteParams; omitted optionals are absent on the wire.
     */
    struct VectorStoresFilesDeleteParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for VectorStoresFilesContentParams; omitted optionals are absent on the wire.
     */
    struct VectorStoresFilesContentParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for VectorStoreCreateParams; omitted optionals are absent on the wire.
     */
    struct VectorStoreCreateParams {
        std::optional<Json::Value> chunking_strategy{};     ///< Optional SDK field chunking_strategy.
        std::optional<std::string> description{};           ///< Optional SDK field description.
        std::optional<Json::Value> expires_after{};         ///< Optional SDK field expires_after.
        std::optional<std::vector<std::string>> file_ids{}; ///< Optional SDK field file_ids.
        std::optional<Json::Value> metadata{};              ///< Optional SDK field metadata.
        std::optional<std::string> name{};                  ///< Optional SDK field name.
        Json::Value extra_body{};                           ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};                   ///< Override phase timeouts.
        std::optional<int> max_retries{};                   ///< Override retries.
        Headers extra_headers{};                            ///< Override request headers.
        Json::Value extra_query{};                          ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for VectorStoresRetrieveParams; omitted optionals are absent on the wire.
     */
    struct VectorStoresRetrieveParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for VectorStoreUpdateParams; omitted optionals are absent on the wire.
     */
    struct VectorStoreUpdateParams {
        std::optional<Json::Value> expires_after{}; ///< Optional SDK field expires_after.
        std::optional<Json::Value> metadata{};      ///< Optional SDK field metadata.
        std::optional<Json::Value> name{};          ///< Optional SDK field name.
        Json::Value extra_body{};                   ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};           ///< Override phase timeouts.
        std::optional<int> max_retries{};           ///< Override retries.
        Headers extra_headers{};                    ///< Override request headers.
        Json::Value extra_query{};                  ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for VectorStoreListParams; omitted optionals are absent on the wire.
     */
    struct VectorStoreListParams {
        std::optional<std::string> after{};  ///< Optional SDK field after.
        std::optional<std::string> before{}; ///< Optional SDK field before.
        std::optional<std::int64_t> limit{}; ///< Optional SDK field limit.
        std::optional<std::string> order{};  ///< Optional SDK field order.
        Json::Value extra_body{};            ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};    ///< Override phase timeouts.
        std::optional<int> max_retries{};    ///< Override retries.
        Headers extra_headers{};             ///< Override request headers.
        Json::Value extra_query{};           ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for VectorStoresDeleteParams; omitted optionals are absent on the wire.
     */
    struct VectorStoresDeleteParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for VectorStoreSearchParams; omitted optionals are absent on the wire.
     */
    struct VectorStoreSearchParams {
        Json::Value query{};                           ///< Required SDK field query.
        std::optional<Json::Value> filters{};          ///< Optional SDK field filters.
        std::optional<std::int64_t> max_num_results{}; ///< Optional SDK field max_num_results.
        std::optional<Json::Value> ranking_options{};  ///< Optional SDK field ranking_options.
        std::optional<bool> rewrite_query{};           ///< Optional SDK field rewrite_query.
        Json::Value extra_body{};                      ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};              ///< Override phase timeouts.
        std::optional<int> max_retries{};              ///< Override retries.
        Headers extra_headers{};                       ///< Override request headers.
        Json::Value extra_query{};                     ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };
} // namespace drogon_openai::types::vector_stores
