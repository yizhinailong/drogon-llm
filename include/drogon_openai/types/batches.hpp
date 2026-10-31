/**
 * @file batches.hpp
 * @brief Request and response types for batches; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::batches {

    /**
     * @brief Response Batch, retaining SDK union fields as JSON.
     */
    struct Batch {
        std::string id{};                            ///< Wire field id.
        std::string completion_window{};             ///< Wire field completion_window.
        std::int64_t created_at{};                   ///< Wire field created_at.
        std::string endpoint{};                      ///< Wire field endpoint.
        std::string input_file_id{};                 ///< Wire field input_file_id.
        std::string object{};                        ///< Wire field object.
        std::string status{};                        ///< Wire field status.
        std::optional<Json::Value> cancelled_at{};   ///< Wire field cancelled_at.
        std::optional<Json::Value> cancelling_at{};  ///< Wire field cancelling_at.
        std::optional<Json::Value> completed_at{};   ///< Wire field completed_at.
        std::optional<Json::Value> error_file_id{};  ///< Wire field error_file_id.
        std::optional<Json::Value> errors{};         ///< Wire field errors.
        std::optional<Json::Value> expired_at{};     ///< Wire field expired_at.
        std::optional<Json::Value> expires_at{};     ///< Wire field expires_at.
        std::optional<Json::Value> failed_at{};      ///< Wire field failed_at.
        std::optional<Json::Value> finalizing_at{};  ///< Wire field finalizing_at.
        std::optional<Json::Value> in_progress_at{}; ///< Wire field in_progress_at.
        std::optional<Json::Value> metadata{};       ///< Wire field metadata.
        std::optional<Json::Value> model{};          ///< Wire field model.
        std::optional<Json::Value> output_file_id{}; ///< Wire field output_file_id.
        std::optional<Json::Value> request_counts{}; ///< Wire field request_counts.
        std::optional<Json::Value> usage{};          ///< Wire field usage.
        Json::Value raw{};                           ///< Complete JSON including provider extensions.
        std::string _request_id;                     ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<Batch>;
    };

    /**
     * @brief Parameters for BatchCreateParams; omitted optionals are absent on the wire.
     */
    struct BatchCreateParams {
        std::string completion_window{};                   ///< Required SDK field completion_window.
        std::string endpoint{};                            ///< Required SDK field endpoint.
        std::string input_file_id{};                       ///< Required SDK field input_file_id.
        std::optional<Json::Value> metadata{};             ///< Optional SDK field metadata.
        std::optional<Json::Value> output_expires_after{}; ///< Optional SDK field output_expires_after.
        Json::Value extra_body{};                          ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};                  ///< Override phase timeouts.
        std::optional<int> max_retries{};                  ///< Override retries.
        Headers extra_headers{};                           ///< Override request headers.
        Json::Value extra_query{};                         ///< Override query parameters.

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
     * @brief Parameters for BatchesRetrieveParams; omitted optionals are absent on the wire.
     */
    struct BatchesRetrieveParams {
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
     * @brief Parameters for BatchListParams; omitted optionals are absent on the wire.
     */
    struct BatchListParams {
        std::optional<std::string> after{};  ///< Optional SDK field after.
        std::optional<std::int64_t> limit{}; ///< Optional SDK field limit.
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
     * @brief Parameters for BatchesCancelParams; omitted optionals are absent on the wire.
     */
    struct BatchesCancelParams {
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
} // namespace drogon_openai::types::batches
