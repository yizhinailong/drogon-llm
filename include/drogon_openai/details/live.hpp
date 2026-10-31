/**
 * @file live.hpp
 * @brief live API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/live.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK live.forks resource.
     */
    class LiveForks {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit LiveForks(std::shared_ptr<Transport> transport);

        /**
         * @brief Open an authenticated WebSocket session for this resource.
         * @param session_id Owned operation parameters.
         * @param options Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto Connect(
            std::string session_id,
            RequestOptions options = {}
        ) const -> drogon::Task<Result<WebSocketSession>>;
    };

    /**
     * @brief Value handle for the SDK live.sessions resource.
     */
    class LiveSessions {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit LiveSessions(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /live/sessions/{session_id}/accept.
         * @param session_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Accept(
            std::string session_id,
            types::live::SessionAcceptParams params = {}
        ) const -> drogon::Task<Result<void>>;

        /**
         * @brief POST /live/sessions/{session_id}/accept.
         * @param session_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto AcceptRaw(
            std::string session_id,
            types::live::SessionAcceptParams params = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;

        /**
         * @brief GET /live/sessions/{session_id}/content.
         * @param session_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DownloadRecording(
            std::string session_id,
            types::live::LiveSessionsDownloadRecordingParams params = {}
        ) const -> drogon::Task<Result<RawResponse<std::string>>>;

        /**
         * @brief POST /live/sessions/{session_id}/fork.
         * @param session_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Fork(
            std::string session_id,
            types::live::SessionForkParams params = {}
        ) const -> drogon::Task<Result<types::live::SessionForkResponse>>;

        /**
         * @brief POST /live/sessions/{session_id}/fork.
         * @param session_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto ForkRaw(
            std::string session_id,
            types::live::SessionForkParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::live::SessionForkResponse>>>;

        /**
         * @brief POST /live/sessions/{session_id}/hangup.
         * @param session_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Hangup(
            std::string session_id,
            types::live::LiveSessionsHangupParams params = {}
        ) const -> drogon::Task<Result<void>>;

        /**
         * @brief POST /live/sessions/{session_id}/hangup.
         * @param session_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto HangupRaw(
            std::string session_id,
            types::live::LiveSessionsHangupParams params = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;

        /**
         * @brief POST /live/sessions/{session_id}/refer.
         * @param session_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Refer(
            std::string session_id,
            types::live::SessionReferParams params = {}
        ) const -> drogon::Task<Result<void>>;

        /**
         * @brief POST /live/sessions/{session_id}/refer.
         * @param session_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto ReferRaw(
            std::string session_id,
            types::live::SessionReferParams params = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;

        /**
         * @brief POST /live/sessions/{session_id}/reject.
         * @param session_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Reject(
            std::string session_id,
            types::live::SessionRejectParams params = {}
        ) const -> drogon::Task<Result<void>>;

        /**
         * @brief POST /live/sessions/{session_id}/reject.
         * @param session_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RejectRaw(
            std::string session_id,
            types::live::SessionRejectParams params = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;
    };

    /**
     * @brief Value handle for the SDK live.sideband resource.
     */
    class LiveSideband {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit LiveSideband(std::shared_ptr<Transport> transport);

        /**
         * @brief Open an authenticated WebSocket session for this resource.
         * @param session_id Owned operation parameters.
         * @param options Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto Connect(
            std::string session_id,
            RequestOptions options = {}
        ) const -> drogon::Task<Result<WebSocketSession>>;
    };

    /**
     * @brief Value handle for the SDK live resource.
     */
    class Live {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Live(std::shared_ptr<Transport> transport);
        LiveSideband sideband; ///< Nested SDK resource sideband.
        LiveForks forks;       ///< Nested SDK resource forks.
        LiveSessions sessions; ///< Nested SDK resource sessions.

        /**
         * @brief POST /live/sessions.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::live::LiveCreateParams params = {}
        ) const -> drogon::Task<Result<types::live::LiveCreateResponse>>;

        /**
         * @brief POST /live/sessions.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::live::LiveCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::live::LiveCreateResponse>>>;

        /**
         * @brief Open an authenticated WebSocket session for this resource.
         * @param options Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto Connect(
            RequestOptions options = {}
        ) const -> drogon::Task<Result<WebSocketSession>>;
    };
} // namespace drogon_openai::details
