/**
 * @file websocket.hpp
 * @brief Owned asynchronous JSON and binary WebSocket sessions.
 */
#pragma once

#include <memory>
#include <string>

#include <drogon/utils/coroutine.h>

#include "drogon_openai/error.hpp"

namespace drogon_openai {

    namespace details {
        class WebSocketState;
    }

    /**
     * @brief Shared session whose operations resume on the awaiting event loop.
     * @note Send and receive are asynchronous. Only one receive may be pending per session.
     */
    class WebSocketSession {
    private:
        std::shared_ptr<details::WebSocketState> m_state; ///< Owned connection and queued frames.

    public:
        /**
         * @brief Bind an established socket state.
         * @param state Owned session state.
         */
        explicit WebSocketSession(std::shared_ptr<details::WebSocketState> state);

        /**
         * @brief Send one JSON client event.
         * @param event Complete event object, including its type.
         * @return Success or a local/connection error.
         */
        [[nodiscard]] auto Send(Json::Value event) const -> drogon::Task<Result<void>>;

        /**
         * @brief Send one binary frame.
         * @param bytes Frame bytes, including NULs.
         * @return Success or a connection error.
         */
        [[nodiscard]] auto SendBytes(std::string bytes) const -> drogon::Task<Result<void>>;

        /**
         * @brief Receive and decode the next JSON server event.
         * @param timeout Seconds to wait; zero disables the receive deadline.
         * @return Event, timeout, protocol or connection error.
         */
        [[nodiscard]] auto Receive(double timeout = 0) const -> drogon::Task<Result<Json::Value>>;

        /**
         * @brief Receive the next data frame without interpreting its content.
         * @param timeout Seconds to wait; zero disables the receive deadline.
         * @return Bytes, timeout or connection error.
         */
        [[nodiscard]] auto ReceiveBytes(double timeout = 0) const -> drogon::Task<Result<std::string>>;

        /**
         * @brief Close this shared session and wake a pending receiver.
         * @return Success after the connection has been stopped on its owner loop.
         */
        auto Close() const -> drogon::Task<Result<void>>;
    };

} // namespace drogon_openai
