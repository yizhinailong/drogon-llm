/**
 * @file uploads.hpp
 * @brief Request and response types for uploads; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::uploads {

    /**
     * @brief Response UploadPart, retaining SDK union fields as JSON.
     */
    struct UploadPart {
        std::string id{};          ///< Wire field id.
        std::int64_t created_at{}; ///< Wire field created_at.
        std::string object{};      ///< Wire field object.
        std::string upload_id{};   ///< Wire field upload_id.
        Json::Value raw{};         ///< Complete JSON including provider extensions.
        std::string _request_id;   ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<UploadPart>;
    };

    /**
     * @brief Response Upload, retaining SDK union fields as JSON.
     */
    struct Upload {
        std::string id{};                  ///< Wire field id.
        std::int64_t bytes{};              ///< Wire field bytes.
        std::int64_t created_at{};         ///< Wire field created_at.
        std::int64_t expires_at{};         ///< Wire field expires_at.
        std::string filename{};            ///< Wire field filename.
        std::string object{};              ///< Wire field object.
        std::string purpose{};             ///< Wire field purpose.
        std::string status{};              ///< Wire field status.
        std::optional<Json::Value> file{}; ///< Wire field file.
        Json::Value raw{};                 ///< Complete JSON including provider extensions.
        std::string _request_id;           ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<Upload>;
    };

    /**
     * @brief Parameters for PartCreateParams; omitted optionals are absent on the wire.
     */
    struct PartCreateParams {
        FileInput data{};                 ///< Required SDK field data.
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
     * @brief Parameters for UploadCreateParams; omitted optionals are absent on the wire.
     */
    struct UploadCreateParams {
        std::int64_t bytes{};                       ///< Required SDK field bytes.
        std::string filename{};                     ///< Required SDK field filename.
        std::string mime_type{};                    ///< Required SDK field mime_type.
        Json::Value purpose{};                      ///< Required SDK field purpose.
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
     * @brief Parameters for UploadsCancelParams; omitted optionals are absent on the wire.
     */
    struct UploadsCancelParams {
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
     * @brief Parameters for UploadCompleteParams; omitted optionals are absent on the wire.
     */
    struct UploadCompleteParams {
        std::vector<std::string> part_ids{}; ///< Required SDK field part_ids.
        std::optional<std::string> md5{};    ///< Optional SDK field md5.
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
} // namespace drogon_openai::types::uploads
