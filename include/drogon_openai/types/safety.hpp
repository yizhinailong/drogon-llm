/**
 * @file safety.hpp
 * @brief Request and response types for safety; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::safety {

    /**
     * @brief Response SafetyAlert, retaining SDK union fields as JSON.
     */
    struct SafetyAlert {
        std::string id{};                                  ///< Wire field id.
        std::int64_t created_at{};                         ///< Wire field created_at.
        std::string error_type{};                          ///< Wire field error_type.
        std::string model{};                               ///< Wire field model.
        std::string object{};                              ///< Wire field object.
        std::optional<Json::Value> reason{};               ///< Wire field reason.
        std::string request_id{};                          ///< Wire field request_id.
        bool request_paused{};                             ///< Wire field request_paused.
        std::string response_id{};                         ///< Wire field response_id.
        std::optional<Json::Value> detailed_explanation{}; ///< Wire field detailed_explanation.
        Json::Value raw{};                                 ///< Complete JSON including provider extensions.
        std::string _request_id;                           ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<SafetyAlert>;
    };

    /**
     * @brief Response SafetyCase, retaining SDK union fields as JSON.
     */
    struct SafetyCase {
        std::string id{};                    ///< Wire field id.
        std::int64_t created_at{};           ///< Wire field created_at.
        std::string entity_identifier{};     ///< Wire field entity_identifier.
        Json::Value notice{};                ///< Wire field notice.
        std::string object{};                ///< Wire field object.
        std::optional<Json::Value> reason{}; ///< Wire field reason.
        Json::Value raw{};                   ///< Complete JSON including provider extensions.
        std::string _request_id;             ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<SafetyCase>;
    };

    /**
     * @brief Parameters for SafetyAlertsRetrieveParams; omitted optionals are absent on the wire.
     */
    struct SafetyAlertsRetrieveParams {
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
     * @brief Parameters for SafetyCasesRetrieveParams; omitted optionals are absent on the wire.
     */
    struct SafetyCasesRetrieveParams {
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
} // namespace drogon_openai::types::safety
