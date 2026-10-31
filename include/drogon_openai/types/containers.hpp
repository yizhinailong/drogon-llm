/**
 * @file containers.hpp
 * @brief Request and response types for containers; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::containers {

    /**
     * @brief Response ContainerCreateResponse, retaining SDK union fields as JSON.
     */
    struct ContainerCreateResponse {
        std::string id{};                            ///< Wire field id.
        std::int64_t created_at{};                   ///< Wire field created_at.
        std::string name{};                          ///< Wire field name.
        std::string object{};                        ///< Wire field object.
        std::string status{};                        ///< Wire field status.
        std::optional<Json::Value> expires_after{};  ///< Wire field expires_after.
        std::optional<Json::Value> last_active_at{}; ///< Wire field last_active_at.
        std::optional<Json::Value> memory_limit{};   ///< Wire field memory_limit.
        std::optional<Json::Value> network_policy{}; ///< Wire field network_policy.
        Json::Value raw{};                           ///< Complete JSON including provider extensions.
        std::string _request_id;                     ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<ContainerCreateResponse>;
    };

    /**
     * @brief Response ContainerRetrieveResponse, retaining SDK union fields as JSON.
     */
    struct ContainerRetrieveResponse {
        std::string id{};                            ///< Wire field id.
        std::int64_t created_at{};                   ///< Wire field created_at.
        std::string name{};                          ///< Wire field name.
        std::string object{};                        ///< Wire field object.
        std::string status{};                        ///< Wire field status.
        std::optional<Json::Value> expires_after{};  ///< Wire field expires_after.
        std::optional<Json::Value> last_active_at{}; ///< Wire field last_active_at.
        std::optional<Json::Value> memory_limit{};   ///< Wire field memory_limit.
        std::optional<Json::Value> network_policy{}; ///< Wire field network_policy.
        Json::Value raw{};                           ///< Complete JSON including provider extensions.
        std::string _request_id;                     ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<ContainerRetrieveResponse>;
    };

    /**
     * @brief Response ContainerListResponse, retaining SDK union fields as JSON.
     */
    struct ContainerListResponse {
        std::string id{};                            ///< Wire field id.
        std::int64_t created_at{};                   ///< Wire field created_at.
        std::string name{};                          ///< Wire field name.
        std::string object{};                        ///< Wire field object.
        std::string status{};                        ///< Wire field status.
        std::optional<Json::Value> expires_after{};  ///< Wire field expires_after.
        std::optional<Json::Value> last_active_at{}; ///< Wire field last_active_at.
        std::optional<Json::Value> memory_limit{};   ///< Wire field memory_limit.
        std::optional<Json::Value> network_policy{}; ///< Wire field network_policy.
        Json::Value raw{};                           ///< Complete JSON including provider extensions.
        std::string _request_id;                     ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<ContainerListResponse>;
    };

    /**
     * @brief Response FileCreateResponse, retaining SDK union fields as JSON.
     */
    struct FileCreateResponse {
        std::string id{};           ///< Wire field id.
        std::int64_t bytes{};       ///< Wire field bytes.
        std::string container_id{}; ///< Wire field container_id.
        std::int64_t created_at{};  ///< Wire field created_at.
        std::string object{};       ///< Wire field object.
        std::string path{};         ///< Wire field path.
        std::string source{};       ///< Wire field source.
        Json::Value raw{};          ///< Complete JSON including provider extensions.
        std::string _request_id;    ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<FileCreateResponse>;
    };

    /**
     * @brief Response FileRetrieveResponse, retaining SDK union fields as JSON.
     */
    struct FileRetrieveResponse {
        std::string id{};           ///< Wire field id.
        std::int64_t bytes{};       ///< Wire field bytes.
        std::string container_id{}; ///< Wire field container_id.
        std::int64_t created_at{};  ///< Wire field created_at.
        std::string object{};       ///< Wire field object.
        std::string path{};         ///< Wire field path.
        std::string source{};       ///< Wire field source.
        Json::Value raw{};          ///< Complete JSON including provider extensions.
        std::string _request_id;    ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<FileRetrieveResponse>;
    };

    /**
     * @brief Response FileListResponse, retaining SDK union fields as JSON.
     */
    struct FileListResponse {
        std::string id{};           ///< Wire field id.
        std::int64_t bytes{};       ///< Wire field bytes.
        std::string container_id{}; ///< Wire field container_id.
        std::int64_t created_at{};  ///< Wire field created_at.
        std::string object{};       ///< Wire field object.
        std::string path{};         ///< Wire field path.
        std::string source{};       ///< Wire field source.
        Json::Value raw{};          ///< Complete JSON including provider extensions.
        std::string _request_id;    ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<FileListResponse>;
    };

    /**
     * @brief Parameters for ContainerCreateParams; omitted optionals are absent on the wire.
     */
    struct ContainerCreateParams {
        std::string name{};                                 ///< Required SDK field name.
        std::optional<Json::Value> expires_after{};         ///< Optional SDK field expires_after.
        std::optional<std::vector<std::string>> file_ids{}; ///< Optional SDK field file_ids.
        std::optional<std::string> memory_limit{};          ///< Optional SDK field memory_limit.
        std::optional<Json::Value> network_policy{};        ///< Optional SDK field network_policy.
        std::optional<Json::Value> skills{};                ///< Optional SDK field skills.
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
     * @brief Parameters for ContainersRetrieveParams; omitted optionals are absent on the wire.
     */
    struct ContainersRetrieveParams {
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
     * @brief Parameters for ContainerListParams; omitted optionals are absent on the wire.
     */
    struct ContainerListParams {
        std::optional<std::string> after{};  ///< Optional SDK field after.
        std::optional<std::int64_t> limit{}; ///< Optional SDK field limit.
        std::optional<std::string> name{};   ///< Optional SDK field name.
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
     * @brief Parameters for ContainersDeleteParams; omitted optionals are absent on the wire.
     */
    struct ContainersDeleteParams {
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
     * @brief Parameters for ContainersFilesContentRetrieveParams; omitted optionals are absent on the wire.
     */
    struct ContainersFilesContentRetrieveParams {
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
     * @brief Parameters for FileCreateParams; omitted optionals are absent on the wire.
     */
    struct FileCreateParams {
        std::optional<FileInput> file{};      ///< Optional SDK field file.
        std::optional<std::string> file_id{}; ///< Optional SDK field file_id.
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
     * @brief Parameters for ContainersFilesRetrieveParams; omitted optionals are absent on the wire.
     */
    struct ContainersFilesRetrieveParams {
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
     * @brief Parameters for ContainersFilesDeleteParams; omitted optionals are absent on the wire.
     */
    struct ContainersFilesDeleteParams {
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
} // namespace drogon_openai::types::containers
