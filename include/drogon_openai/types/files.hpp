/**
 * @file files.hpp
 * @brief Request and response types for files; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::files {

    /**
     * @brief Response FileObject, retaining SDK union fields as JSON.
     */
    struct FileObject {
        std::string id{};                            ///< Wire field id.
        std::int64_t bytes{};                        ///< Wire field bytes.
        std::int64_t created_at{};                   ///< Wire field created_at.
        std::string filename{};                      ///< Wire field filename.
        std::string object{};                        ///< Wire field object.
        std::string purpose{};                       ///< Wire field purpose.
        std::string status{};                        ///< Wire field status.
        std::optional<Json::Value> expires_at{};     ///< Wire field expires_at.
        std::optional<Json::Value> status_details{}; ///< Wire field status_details.
        Json::Value raw{};                           ///< Complete JSON including provider extensions.
        std::string _request_id;                     ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<FileObject>;
    };

    /**
     * @brief Response FileDeleted, retaining SDK union fields as JSON.
     */
    struct FileDeleted {
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
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<FileDeleted>;
    };

    /**
     * @brief Parameters for FileCreateParams; omitted optionals are absent on the wire.
     */
    struct FileCreateParams {
        FileInput file{};                           ///< Required SDK field file.
        std::string purpose{};                      ///< Required SDK field purpose.
        std::optional<Json::Value> expires_after{}; ///< Optional SDK field expires_after.
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
     * @brief Parameters for FilesRetrieveParams; omitted optionals are absent on the wire.
     */
    struct FilesRetrieveParams {
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
        std::optional<std::string> after{};   ///< Optional SDK field after.
        std::optional<std::int64_t> limit{};  ///< Optional SDK field limit.
        std::optional<std::string> order{};   ///< Optional SDK field order.
        std::optional<std::string> purpose{}; ///< Optional SDK field purpose.
        Json::Value extra_body{};             ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};     ///< Override phase timeouts.
        std::optional<int> max_retries{};     ///< Override retries.
        Headers extra_headers{};              ///< Override request headers.
        Json::Value extra_query{};            ///< Override query parameters.

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
     * @brief Parameters for FilesDeleteParams; omitted optionals are absent on the wire.
     */
    struct FilesDeleteParams {
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
     * @brief Parameters for FilesContentParams; omitted optionals are absent on the wire.
     */
    struct FilesContentParams {
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
     * @brief Parameters for FilesRetrieveContentParams; omitted optionals are absent on the wire.
     */
    struct FilesRetrieveContentParams {
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
} // namespace drogon_openai::types::files
