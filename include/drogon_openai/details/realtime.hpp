/**
 * @file realtime.hpp
 * @brief realtime API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/realtime.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK realtime.translations.client_secrets resource.
     */
    class RealtimeTranslationsClientSecrets {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit RealtimeTranslationsClientSecrets(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /realtime/translations/client_secrets.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::realtime::RealtimeTranslationsClientSecretsCreateParams params = {}
        ) const -> drogon::Task<Result<types::realtime::RealtimeTranslationClientSecretCreateResponse>>;

        /**
         * @brief POST /realtime/translations/client_secrets.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::realtime::RealtimeTranslationsClientSecretsCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::realtime::RealtimeTranslationClientSecretCreateResponse>>>;
    };

    /**
     * @brief Value handle for the SDK realtime.calls resource.
     */
    class RealtimeCalls {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit RealtimeCalls(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /realtime/calls.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::realtime::CallCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<std::string>>>;

        /**
         * @brief POST /realtime/calls/{call_id}/accept.
         * @param call_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Accept(
            std::string call_id,
            types::realtime::CallAcceptParams params = {}
        ) const -> drogon::Task<Result<void>>;

        /**
         * @brief POST /realtime/calls/{call_id}/accept.
         * @param call_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto AcceptRaw(
            std::string call_id,
            types::realtime::CallAcceptParams params = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;

        /**
         * @brief POST /realtime/calls/{call_id}/hangup.
         * @param call_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Hangup(
            std::string call_id,
            types::realtime::RealtimeCallsHangupParams params = {}
        ) const -> drogon::Task<Result<void>>;

        /**
         * @brief POST /realtime/calls/{call_id}/hangup.
         * @param call_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto HangupRaw(
            std::string call_id,
            types::realtime::RealtimeCallsHangupParams params = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;

        /**
         * @brief POST /realtime/calls/{call_id}/refer.
         * @param call_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Refer(
            std::string call_id,
            types::realtime::CallReferParams params = {}
        ) const -> drogon::Task<Result<void>>;

        /**
         * @brief POST /realtime/calls/{call_id}/refer.
         * @param call_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto ReferRaw(
            std::string call_id,
            types::realtime::CallReferParams params = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;

        /**
         * @brief POST /realtime/calls/{call_id}/reject.
         * @param call_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Reject(
            std::string call_id,
            types::realtime::CallRejectParams params = {}
        ) const -> drogon::Task<Result<void>>;

        /**
         * @brief POST /realtime/calls/{call_id}/reject.
         * @param call_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RejectRaw(
            std::string call_id,
            types::realtime::CallRejectParams params = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;
    };

    /**
     * @brief Value handle for the SDK realtime.client_secrets resource.
     */
    class RealtimeClientSecrets {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit RealtimeClientSecrets(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /realtime/client_secrets.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::realtime::ClientSecretCreateParams params = {}
        ) const -> drogon::Task<Result<types::realtime::ClientSecretCreateResponse>>;

        /**
         * @brief POST /realtime/client_secrets.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::realtime::ClientSecretCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::realtime::ClientSecretCreateResponse>>>;
    };

    /**
     * @brief Value handle for the SDK realtime.translations resource.
     */
    class RealtimeTranslations {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit RealtimeTranslations(std::shared_ptr<Transport> transport);
        RealtimeTranslationsClientSecrets client_secrets; ///< Nested SDK resource client_secrets.

        /**
         * @brief Open an authenticated WebSocket session for this resource.
         * @param model Owned operation parameters.
         * @param options Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto Connect(
            std::string model = "",
            RequestOptions options = {}
        ) const -> drogon::Task<Result<WebSocketSession>>;
    };

    /**
     * @brief Value handle for the SDK realtime resource.
     */
    class Realtime {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Realtime(std::shared_ptr<Transport> transport);
        RealtimeClientSecrets client_secrets; ///< Nested SDK resource client_secrets.
        RealtimeCalls calls;                  ///< Nested SDK resource calls.
        RealtimeTranslations translations;    ///< Nested SDK resource translations.

        /**
         * @brief Open an authenticated WebSocket session for this resource.
         * @param model Owned operation parameters.
         * @param options Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto Connect(
            std::string model = "",
            RequestOptions options = {}
        ) const -> drogon::Task<Result<WebSocketSession>>;
    };
} // namespace drogon_openai::details
