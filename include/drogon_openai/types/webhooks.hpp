/**
 * @file webhooks.hpp
 * @brief Request and response types for webhooks; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::webhooks {

    /**
     * @brief Response WebhookEventTypeList, retaining SDK union fields as JSON.
     */
    struct WebhookEventTypeList {
        std::vector<std::string> data{}; ///< Wire field data.
        std::string object{};            ///< Wire field object.
        Json::Value raw{};               ///< Complete JSON including provider extensions.
        std::string _request_id;         ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<WebhookEventTypeList>;
    };

    /**
     * @brief Response WebhookEndpointWithSecret, retaining SDK union fields as JSON.
     */
    struct WebhookEndpointWithSecret {
        std::string id{};                                 ///< Wire field id.
        std::int64_t created_at{};                        ///< Wire field created_at.
        std::vector<std::string> event_types{};           ///< Wire field event_types.
        std::string name{};                               ///< Wire field name.
        std::string object{};                             ///< Wire field object.
        std::string signing_secret{};                     ///< Wire field signing_secret.
        std::optional<Json::Value> signing_secret_hint{}; ///< Wire field signing_secret_hint.
        std::string url{};                                ///< Wire field url.
        std::optional<Json::Value> updated_at{};          ///< Wire field updated_at.
        Json::Value raw{};                                ///< Complete JSON including provider extensions.
        std::string _request_id;                          ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<WebhookEndpointWithSecret>;
    };

    /**
     * @brief Response WebhookEndpoint, retaining SDK union fields as JSON.
     */
    struct WebhookEndpoint {
        std::string id{};                                 ///< Wire field id.
        std::int64_t created_at{};                        ///< Wire field created_at.
        std::vector<std::string> event_types{};           ///< Wire field event_types.
        std::string name{};                               ///< Wire field name.
        std::string object{};                             ///< Wire field object.
        std::optional<Json::Value> signing_secret_hint{}; ///< Wire field signing_secret_hint.
        std::string url{};                                ///< Wire field url.
        std::optional<Json::Value> updated_at{};          ///< Wire field updated_at.
        Json::Value raw{};                                ///< Complete JSON including provider extensions.
        std::string _request_id;                          ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<WebhookEndpoint>;
    };

    /**
     * @brief Response DeletedWebhookEndpoint, retaining SDK union fields as JSON.
     */
    struct DeletedWebhookEndpoint {
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
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<DeletedWebhookEndpoint>;
    };

    /**
     * @brief Response WebhookEndpointTestResult, retaining SDK union fields as JSON.
     */
    struct WebhookEndpointTestResult {
        std::string event_type{};          ///< Wire field event_type.
        std::string object{};              ///< Wire field object.
        std::int64_t status_code{};        ///< Wire field status_code.
        Json::Value success{};             ///< Wire field success.
        std::string webhook_endpoint_id{}; ///< Wire field webhook_endpoint_id.
        Json::Value raw{};                 ///< Complete JSON including provider extensions.
        std::string _request_id;           ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<WebhookEndpointTestResult>;
    };

    /**
     * @brief Parameters for WebhooksEventTypesListParams; omitted optionals are absent on the wire.
     */
    struct WebhooksEventTypesListParams {
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
     * @brief Parameters for WebhookCreateParams; omitted optionals are absent on the wire.
     */
    struct WebhookCreateParams {
        std::vector<std::string> event_types{}; ///< Required SDK field event_types.
        std::string name{};                     ///< Required SDK field name.
        std::string url{};                      ///< Required SDK field url.
        Json::Value extra_body{};               ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};       ///< Override phase timeouts.
        std::optional<int> max_retries{};       ///< Override retries.
        Headers extra_headers{};                ///< Override request headers.
        Json::Value extra_query{};              ///< Override query parameters.

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
     * @brief Parameters for WebhooksRetrieveParams; omitted optionals are absent on the wire.
     */
    struct WebhooksRetrieveParams {
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
     * @brief Parameters for WebhookUpdateParams; omitted optionals are absent on the wire.
     */
    struct WebhookUpdateParams {
        std::optional<std::vector<std::string>> event_types{}; ///< Optional SDK field event_types.
        std::optional<std::string> name{};                     ///< Optional SDK field name.
        std::optional<std::string> url{};                      ///< Optional SDK field url.
        Json::Value extra_body{};                              ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};                      ///< Override phase timeouts.
        std::optional<int> max_retries{};                      ///< Override retries.
        Headers extra_headers{};                               ///< Override request headers.
        Json::Value extra_query{};                             ///< Override query parameters.

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
     * @brief Parameters for WebhookListParams; omitted optionals are absent on the wire.
     */
    struct WebhookListParams {
        std::optional<Json::Value> after{};  ///< Optional SDK field after.
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
     * @brief Parameters for WebhooksDeleteParams; omitted optionals are absent on the wire.
     */
    struct WebhooksDeleteParams {
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
     * @brief Parameters for WebhookRotateSecretParams; omitted optionals are absent on the wire.
     */
    struct WebhookRotateSecretParams {
        std::optional<bool> keep_old_secret_active_for_24_hours{}; ///< Optional SDK field keep_old_secret_active_for_24_hours.
        Json::Value extra_body{};                                  ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};                          ///< Override phase timeouts.
        std::optional<int> max_retries{};                          ///< Override retries.
        Headers extra_headers{};                                   ///< Override request headers.
        Json::Value extra_query{};                                 ///< Override query parameters.

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
     * @brief Parameters for WebhookTestParams; omitted optionals are absent on the wire.
     */
    struct WebhookTestParams {
        std::string event_type{};         ///< Required SDK field event_type.
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
} // namespace drogon_openai::types::webhooks
