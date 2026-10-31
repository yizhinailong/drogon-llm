/**
 * @file websocket_transport.cpp
 * @brief Implement authenticated WebSocket sessions, owned frames and asynchronous deadlines.
 */
#include "details/websocket_transport.hpp"

#include <cmath>
#include <coroutine>
#include <deque>
#include <functional>
#include <optional>
#include <utility>

#include <drogon/WebSocketClient.h>
#include <drogon/WebSocketConnection.h>
#include <json/writer.h>

namespace drogon_openai::details {

    namespace {
        /**
         * @brief Adapt one completion callback and resume on the awaiting loop.
         * @tparam T Completion result type.
         */
        template <typename T>
        class SocketAwaiter : public drogon::CallbackAwaiter<T> {
        private:
            std::function<void(std::function<void(T)>)> m_start; ///< Owned callback operation.
            trantor::EventLoop* m_resume;                        ///< Awaiting loop.

        public:
            /**
             * @brief Bind an operation that completes exactly once.
             * @param start Operation initializer.
             * @param resume Awaiting loop.
             */
            SocketAwaiter(std::function<void(std::function<void(T)>)> start, trantor::EventLoop* resume)
                : m_start{ std::move(start) }, m_resume{ resume } {}

            /**
             * @brief Start work and marshal its continuation onto the awaiting loop.
             * @param handle Suspended coroutine.
             */
            auto await_suspend(std::coroutine_handle<> handle) -> void {
                m_start([this, handle](T result) {
                    this->setValue(std::move(result));
                    m_resume->queueInLoop([handle] { handle.resume(); });
                });
            }
        };

        /**
         * @brief Append nested query parameters using SDK bracket notation.
         * @param target Encoded request target.
         * @param key Query field path.
         * @param value Field value.
         */
        auto socket_query(std::string& target, std::string const& key, Json::Value const& value) -> void {
            if (value.isObject()) {
                for (auto const& name : value.getMemberNames()) {
                    socket_query(target, key.empty() ? name : key + "[" + name + "]", value[name]);
                }
            } else if (value.isArray()) {
                for (auto const& item : value) {
                    socket_query(target, key + "[]", item);
                }
            } else if (!value.isNull()) {
                target += target.find('?') == std::string::npos ? '?' : '&';
                target += encode_path_segment(key) + "=" + encode_path_segment(value.asString());
            }
        }

        /**
         * @brief Normalize and validate a handshake header.
         * @param headers Destination headers.
         * @param name Header name.
         * @param value Header value.
         * @return Whether the header is valid.
         */
        auto socket_header(Headers& headers, std::string name, std::string const& value) -> bool {
            if (name.empty() || name.find_first_of(":\r\n \t") != std::string::npos || name.find('\0') != std::string::npos || value.find_first_of("\r\n") != std::string::npos || value.find('\0') != std::string::npos) {
                return false;
            }
            for (auto& c : name) {
                c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            }
            headers[std::move(name)] = value;
            return true;
        }
    } // namespace

    /**
     * @brief Keep connection callbacks, frames and pending receivers on one owner loop.
     */
    class WebSocketState : public std::enable_shared_from_this<WebSocketState> {
    private:
        drogon::WebSocketClientPtr m_client;                ///< Dedicated persistent session connection.
        trantor::EventLoop* m_loop;                         ///< Socket owner loop.
        bool m_open{};                                      ///< Handshake succeeded and socket remains open.
        std::deque<std::string> m_frames;                   ///< Data frames not yet consumed.
        std::size_t m_queued_bytes{};                       ///< Bound unconsumed memory.
        std::optional<Error> m_failure{};                   ///< Terminal connection error.
        std::function<void(Result<void>)> m_connect;        ///< Pending handshake completion.
        std::function<void(Result<std::string>)> m_receive; ///< Pending data receiver.
        trantor::TimerId m_connect_timer{};                 ///< Handshake deadline.
        trantor::TimerId m_read_timer{};                    ///< Pending receive deadline.

    public:
        /**
         * @brief Bind one socket owner loop.
         * @param loop Loop used by this connection.
         */
        explicit WebSocketState(trantor::EventLoop* loop) : m_loop{ loop } {}

        /**
         * @brief Stop the underlying client when the last shared handle disappears.
         */
        ~WebSocketState() {
            if (m_client) {
                auto client{ std::move(m_client) };
                m_loop->queueInLoop([client] { client->stop(); });
            }
        }

        /**
         * @brief Return the owner loop used to serialize state operations.
         * @return Socket owner loop.
         */
        auto Loop() const noexcept -> trantor::EventLoop* { return m_loop; }

        /**
         * @brief Establish the socket and install weakly owned callbacks.
         * @param host WebSocket scheme and authority.
         * @param request Handshake request with its path and headers.
         * @param timeout Handshake deadline in seconds.
         * @param done Completion callback.
         */
        auto Connect(std::string host, drogon::HttpRequestPtr request, double timeout, std::function<void(Result<void>)> done) -> void {
            m_connect = std::move(done);
            auto weak{ weak_from_this() };
            m_client = drogon::WebSocketClient::newWebSocketClient(host, m_loop, false, true);
            m_client->setMessageHandler([weak](std::string&& bytes, drogon::WebSocketClientPtr const&, drogon::WebSocketMessageType const& type) {
                auto self{ weak.lock() };
                if (!self || (type != drogon::WebSocketMessageType::Text && type != drogon::WebSocketMessageType::Binary)) {
                    return;
                }
                if (self->m_receive) {
                    if (self->m_read_timer) {
                        self->m_loop->invalidateTimer(std::exchange(self->m_read_timer, 0));
                    }
                    auto receiver{ std::exchange(self->m_receive, {}) };
                    receiver(std::move(bytes));
                } else if (bytes.size() > 16777216 || self->m_queued_bytes > 16777216 - bytes.size()) {
                    self->fail({ .kind = ErrorKind::ResponseValidation, .message = "unconsumed WebSocket frames exceed 16 MiB" });
                } else {
                    self->m_queued_bytes += bytes.size();
                    self->m_frames.push_back(std::move(bytes));
                }
            });
            m_client->setConnectionClosedHandler([weak](drogon::WebSocketClientPtr const&) {
                if (auto self{ weak.lock() }) {
                    self->fail({ .kind = ErrorKind::Connection, .message = "WebSocket peer closed the connection" });
                }
            });
            if (timeout > 0) {
                m_connect_timer = m_loop->runAfter(timeout, [weak] {
                    if (auto self{ weak.lock() }) {
                        self->fail({ .kind = ErrorKind::Timeout, .message = "WebSocket handshake timeout" });
                    }
                });
            }
            m_client->connectToServer(request, [weak](drogon::ReqResult result, drogon::HttpResponsePtr const& response, drogon::WebSocketClientPtr const&) {
                auto self{ weak.lock() };
                if (!self || !self->m_connect) {
                    return;
                }
                if (result != drogon::ReqResult::Ok) {
                    Error error{ .kind = result == drogon::ReqResult::Timeout ? ErrorKind::Timeout : ErrorKind::Connection, .message = "WebSocket handshake failed" };
                    if (response) {
                        error.kind = ErrorKind::Status;
                        error.status_code = static_cast<int>(response->statusCode());
                        error.body = std::string{ response->body() };
                        for (auto const& [key, value] : response->headers()) {
                            error.headers[key] = value;
                        }
                        error.request_id = response->getHeader("x-request-id");
                    }
                    self->fail(std::move(error));
                    return;
                }
                self->m_open = true;
                if (self->m_connect_timer) {
                    self->m_loop->invalidateTimer(std::exchange(self->m_connect_timer, 0));
                }
                auto callback{ std::exchange(self->m_connect, {}) };
                callback({});
            });
        }

        /**
         * @brief Send a frame while running on the owner loop.
         * @param bytes Frame bytes.
         * @param binary Whether this is a binary frame.
         * @return Success after the frame is queued, or a closed-session error.
         */
        auto Send(std::string bytes, bool binary) -> Result<void> {
            if (!m_open || !m_client->getConnection()) {
                return std::unexpected{ m_failure.value_or(Error{ .kind = ErrorKind::Connection, .message = "WebSocket is closed" }) };
            }
            m_client->getConnection()->send(bytes, binary ? drogon::WebSocketMessageType::Binary : drogon::WebSocketMessageType::Text);
            return {};
        }

        /**
         * @brief Consume a queued frame or wait for the next one.
         * @param timeout Pending read deadline.
         * @param done Exactly-once completion callback.
         */
        auto Receive(double timeout, std::function<void(Result<std::string>)> done) -> void {
            if (m_receive) {
                done(std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "one WebSocket receive may be pending" } });
                return;
            }
            if (!m_frames.empty()) {
                auto frame{ std::move(m_frames.front()) };
                m_frames.pop_front();
                m_queued_bytes -= frame.size();
                done(std::move(frame));
                return;
            }
            if (!m_open) {
                done(std::unexpected{ m_failure.value_or(Error{ .kind = ErrorKind::Connection, .message = "WebSocket is closed" }) });
                return;
            }
            m_receive = std::move(done);
            if (timeout > 0) {
                auto weak{ weak_from_this() };
                m_read_timer = m_loop->runAfter(timeout, [weak] {
                    if (auto self{ weak.lock() }; self && self->m_receive) {
                        self->m_read_timer = 0;
                        auto receiver{ std::exchange(self->m_receive, {}) };
                        receiver(std::unexpected{ Error{ .kind = ErrorKind::Timeout, .message = "WebSocket receive timeout" } });
                    }
                });
            }
        }

        /**
         * @brief Close the shared socket and wake pending operations.
         */
        auto Close() -> void {
            m_frames.clear();
            m_queued_bytes = 0;
            fail({ .kind = ErrorKind::Connection, .message = "WebSocket was closed locally" });
        }

    private:
        /**
         * @brief Store a terminal failure and complete all pending operations once.
         * @param error Connection failure.
         */
        auto fail(Error error) -> void {
            m_open = false;
            if (!m_failure) {
                m_failure = std::move(error);
            }
            if (m_connect_timer) {
                m_loop->invalidateTimer(std::exchange(m_connect_timer, 0));
            }
            if (m_read_timer) {
                m_loop->invalidateTimer(std::exchange(m_read_timer, 0));
            }
            if (m_client) {
                m_client->stop();
            }
            if (auto callback{ std::exchange(m_connect, {}) }) {
                callback(std::unexpected{ *m_failure });
            }
            if (auto callback{ std::exchange(m_receive, {}) }) {
                callback(std::unexpected{ *m_failure });
            }
        }
    };

    /**
     * @brief Open a verified TLS or local plain WebSocket connection.
     * @param transport Shared endpoint settings.
     * @param path Relative WebSocket API path.
     * @param options Handshake headers, query and connect timeout.
     * @return Owned session or connection/handshake error.
     */
    auto websocket_connect(
        std::shared_ptr<Transport> transport,
        std::string path,
        RequestOptions options
    ) -> drogon::Task<Result<WebSocketSession>> {
        auto const config{ transport->Config() };
        auto endpoint{ parse_endpoint(config.base_url) };
        if (!endpoint) {
            co_return std::unexpected{ std::move(endpoint.error()) };
        }
        auto const timeout{ options.timeout.value_or(config.timeout).connect };
        if (!std::isfinite(timeout) || timeout < 0 || !path.starts_with('/') || path.find_first_of("\r\n #?") != std::string::npos) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "invalid WebSocket path or handshake timeout" } };
        }
        Headers headers;
        if (!config.api_key.empty()) {
            headers["authorization"] = "Bearer " + config.api_key;
        }
        if (!config.organization.empty()) {
            headers["openai-organization"] = config.organization;
        }
        if (!config.project.empty()) {
            headers["openai-project"] = config.project;
        }
        for (auto const* source : std::initializer_list<Headers const*>{ &config.default_headers, &options.extra_headers }) {
            for (auto const& [key, value] : *source) {
                if (!socket_header(headers, key, value)) {
                    co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "invalid WebSocket header" } };
                }
            }
        }
        auto query{ config.default_query };
        if ((!query.isNull() && !query.isObject()) || (!options.extra_query.isNull() && !options.extra_query.isObject())) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "WebSocket query must be an object" } };
        }
        if (options.extra_query.isObject()) {
            for (auto const& key : options.extra_query.getMemberNames()) {
                query[key] = options.extra_query[key];
            }
        }
        auto target{ endpoint->prefix + path };
        socket_query(target, "", query);
        auto request{ drogon::HttpRequest::newHttpRequest() };
        request->setPath(std::move(target));
        request->setPathEncode(false);
        for (auto const& [key, value] : headers) {
            if (value.find_first_of("\r\n") != std::string::npos || value.find('\0') != std::string::npos) {
                co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "invalid WebSocket authorization header" } };
            }
            request->addHeader(key, value);
        }
        auto* loop{ current_loop() };
        auto state{ std::make_shared<WebSocketState>(loop) };
        auto host{ std::string{ endpoint->tls ? "wss://" : "ws://" } + endpoint->authority };
        auto connected{ co_await SocketAwaiter<Result<void>>{ [state, host, request, timeout](auto done) {
                                                                 state->Loop()->queueInLoop([state, host, request, timeout, done = std::move(done)]() mutable { state->Connect(host, request, timeout, std::move(done)); });
                                                             },
                                                              loop } };
        if (!connected) {
            co_return std::unexpected{ std::move(connected.error()) };
        }
        co_return WebSocketSession{ std::move(state) };
    }

} // namespace drogon_openai::details

namespace drogon_openai {

    /**
     * @brief Bind an established socket state.
     * @param state Owned session state.
     */
    WebSocketSession::WebSocketSession(std::shared_ptr<details::WebSocketState> state) : m_state{ std::move(state) } {}

    /**
     * @brief Send one JSON client event.
     * @param event Complete event object, including its type.
     * @return Success or a local/connection error.
     */
    auto WebSocketSession::Send(Json::Value event) const -> drogon::Task<Result<void>> {
        auto state{ m_state };
        if (!state || !event.isObject()) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "WebSocket event must be an object" } };
        }
        Json::StreamWriterBuilder writer;
        writer["indentation"] = "";
        auto bytes{ Json::writeString(writer, event) };
        co_return co_await details::SocketAwaiter<Result<void>>{ [state, bytes = std::move(bytes)](auto done) mutable {
                                                                    state->Loop()->queueInLoop([state, bytes = std::move(bytes), done = std::move(done)]() mutable { done(state->Send(std::move(bytes), false)); });
                                                                },
                                                                 details::current_loop() };
    }

    /**
     * @brief Send one binary frame.
     * @param bytes Frame bytes, including NULs.
     * @return Success or a connection error.
     */
    auto WebSocketSession::SendBytes(std::string bytes) const -> drogon::Task<Result<void>> {
        auto state{ m_state };
        if (!state) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "WebSocket has no session" } };
        }
        co_return co_await details::SocketAwaiter<Result<void>>{ [state, bytes = std::move(bytes)](auto done) mutable {
                                                                    state->Loop()->queueInLoop([state, bytes = std::move(bytes), done = std::move(done)]() mutable { done(state->Send(std::move(bytes), true)); });
                                                                },
                                                                 details::current_loop() };
    }

    /**
     * @brief Receive and decode the next JSON server event.
     * @param timeout Seconds to wait; zero disables the receive deadline.
     * @return Event, timeout, protocol or connection error.
     */
    auto WebSocketSession::Receive(double timeout) const -> drogon::Task<Result<Json::Value>> {
        auto session{ *this };
        auto bytes{ co_await session.ReceiveBytes(timeout) };
        if (!bytes) {
            co_return std::unexpected{ std::move(bytes.error()) };
        }
        co_return details::decode_json(*bytes);
    }

    /**
     * @brief Receive the next data frame without interpreting its content.
     * @param timeout Seconds to wait; zero disables the receive deadline.
     * @return Bytes, timeout or connection error.
     */
    auto WebSocketSession::ReceiveBytes(double timeout) const -> drogon::Task<Result<std::string>> {
        auto state{ m_state };
        if (!state || !std::isfinite(timeout) || timeout < 0) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "invalid WebSocket session or receive timeout" } };
        }
        co_return co_await details::SocketAwaiter<Result<std::string>>{ [state, timeout](auto done) {
                                                                           state->Loop()->queueInLoop([state, timeout, done = std::move(done)]() mutable { state->Receive(timeout, std::move(done)); });
                                                                       },
                                                                        details::current_loop() };
    }

    /**
     * @brief Close this shared session and wake a pending receiver.
     * @return Success after the connection has been stopped on its owner loop.
     */
    auto WebSocketSession::Close() const -> drogon::Task<Result<void>> {
        auto state{ m_state };
        if (!state) {
            co_return Result<void>{};
        }
        co_return co_await details::SocketAwaiter<Result<void>>{ [state](auto done) {
                                                                    state->Loop()->queueInLoop([state, done = std::move(done)]() mutable { state->Close(); done({}); });
                                                                },
                                                                 details::current_loop() };
    }

} // namespace drogon_openai
