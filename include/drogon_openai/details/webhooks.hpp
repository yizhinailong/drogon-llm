/**
 * @file webhooks.hpp
 * @brief webhooks API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/webhooks.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK webhooks.event_types resource.
     */
    class WebhooksEventTypes {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit WebhooksEventTypes(std::shared_ptr<Transport> transport);

        /**
         * @brief GET /webhook_event_types.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            types::webhooks::WebhooksEventTypesListParams params = {}
        ) const -> drogon::Task<Result<types::webhooks::WebhookEventTypeList>>;

        /**
         * @brief GET /webhook_event_types.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto ListRaw(
            types::webhooks::WebhooksEventTypesListParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::webhooks::WebhookEventTypeList>>>;
    };

    /**
     * @brief Value handle for the SDK webhooks resource.
     */
    class Webhooks {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Webhooks(std::shared_ptr<Transport> transport);
        WebhooksEventTypes event_types; ///< Nested SDK resource event_types.

        /**
         * @brief POST /webhook_endpoints.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::webhooks::WebhookCreateParams params = {}
        ) const -> drogon::Task<Result<types::webhooks::WebhookEndpointWithSecret>>;

        /**
         * @brief POST /webhook_endpoints.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::webhooks::WebhookCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::webhooks::WebhookEndpointWithSecret>>>;

        /**
         * @brief GET /webhook_endpoints/{webhook_endpoint_id}.
         * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string webhook_endpoint_id,
            types::webhooks::WebhooksRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::webhooks::WebhookEndpoint>>;

        /**
         * @brief GET /webhook_endpoints/{webhook_endpoint_id}.
         * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string webhook_endpoint_id,
            types::webhooks::WebhooksRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::webhooks::WebhookEndpoint>>>;

        /**
         * @brief POST /webhook_endpoints/{webhook_endpoint_id}.
         * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Update(
            std::string webhook_endpoint_id,
            types::webhooks::WebhookUpdateParams params = {}
        ) const -> drogon::Task<Result<types::webhooks::WebhookEndpoint>>;

        /**
         * @brief POST /webhook_endpoints/{webhook_endpoint_id}.
         * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto UpdateRaw(
            std::string webhook_endpoint_id,
            types::webhooks::WebhookUpdateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::webhooks::WebhookEndpoint>>>;

        /**
         * @brief GET /webhook_endpoints.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            types::webhooks::WebhookListParams params = {}
        ) const -> drogon::Task<Result<Page<types::webhooks::WebhookEndpoint>>>;

        /**
         * @brief DELETE /webhook_endpoints/{webhook_endpoint_id}.
         * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string webhook_endpoint_id,
            types::webhooks::WebhooksDeleteParams params = {}
        ) const -> drogon::Task<Result<types::webhooks::DeletedWebhookEndpoint>>;

        /**
         * @brief DELETE /webhook_endpoints/{webhook_endpoint_id}.
         * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string webhook_endpoint_id,
            types::webhooks::WebhooksDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::webhooks::DeletedWebhookEndpoint>>>;

        /**
         * @brief POST /webhook_endpoints/{webhook_endpoint_id}/rotate_secret.
         * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RotateSecret(
            std::string webhook_endpoint_id,
            types::webhooks::WebhookRotateSecretParams params = {}
        ) const -> drogon::Task<Result<types::webhooks::WebhookEndpointWithSecret>>;

        /**
         * @brief POST /webhook_endpoints/{webhook_endpoint_id}/rotate_secret.
         * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RotateSecretRaw(
            std::string webhook_endpoint_id,
            types::webhooks::WebhookRotateSecretParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::webhooks::WebhookEndpointWithSecret>>>;

        /**
         * @brief POST /webhook_endpoints/{webhook_endpoint_id}/test.
         * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Test(
            std::string webhook_endpoint_id,
            types::webhooks::WebhookTestParams params = {}
        ) const -> drogon::Task<Result<types::webhooks::WebhookEndpointTestResult>>;

        /**
         * @brief POST /webhook_endpoints/{webhook_endpoint_id}/test.
         * @param webhook_endpoint_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto TestRaw(
            std::string webhook_endpoint_id,
            types::webhooks::WebhookTestParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::webhooks::WebhookEndpointTestResult>>>;

        /**
         * @brief Verify the webhook HMAC and replay window before consuming its body.
         * @param payload Owned operation parameters.
         * @param headers Owned operation parameters.
         * @param secret Owned operation parameters.
         * @param tolerance Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto VerifySignature(
            std::string payload,
            Headers headers,
            std::optional<std::string> secret = {},
            int tolerance = 300
        ) const -> Result<void>;

        /**
         * @brief Verify a webhook signature and decode the original JSON event.
         * @param payload Owned operation parameters.
         * @param headers Owned operation parameters.
         * @param secret Owned operation parameters.
         * @param tolerance Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto Unwrap(
            std::string payload,
            Headers headers,
            std::optional<std::string> secret = {},
            int tolerance = 300
        ) const -> Result<Json::Value>;
    };
} // namespace drogon_openai::details
