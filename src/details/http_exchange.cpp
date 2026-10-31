/**
 * @file http_exchange.cpp
 * @brief Implement asynchronous HTTP framing, TLS, and connect/read/write deadlines.
 */
#include "details/http_exchange.hpp"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <chrono>
#include <cmath>
#include <cstring>
#include <limits>
#include <memory>
#include <utility>

#include <drogon/HttpAppFramework.h>
#include <trantor/net/Resolver.h>
#include <trantor/net/TcpClient.h>

namespace drogon_openai::details {

    namespace {

        /**
         * @brief Normalize an HTTP token to lowercase.
         * @param value Input token.
         * @return Lowercase token.
         */
        auto lowercase(std::string value) -> std::string {
            std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            return value;
        }

        /**
         * @brief Trim HTTP optional whitespace.
         * @param value Input field.
         * @return Trimmed view.
         */
        auto trim(std::string_view value) -> std::string_view {
            while (!value.empty() && (value.front() == ' ' || value.front() == '\t')) {
                value.remove_prefix(1);
            }
            while (!value.empty() && (value.back() == ' ' || value.back() == '\t')) {
                value.remove_suffix(1);
            }
            return value;
        }

        /**
         * @brief Check the ASCII token grammar used by methods and field names.
         * @param value HTTP token.
         * @return Whether every byte is a token character.
         */
        auto is_token(std::string_view value) -> bool {
            return !value.empty() && std::ranges::all_of(value, [](unsigned char c) {
                return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || std::string_view{ "!#$%&'*+-.^_`|~" }.find(c) != std::string_view::npos;
            });
        }

        /**
         * @brief Reject control characters in a response or request field value.
         * @param value Field value.
         * @return Whether the value contains only permitted field bytes.
         */
        auto is_field_value(std::string_view value) -> bool {
            return std::ranges::all_of(value, [](unsigned char c) { return c == '\t' || (c >= 32 && c != 127); });
        }

        /**
         * @brief Validate chunk extensions, including quoted strings and escaped bytes.
         * @param value Text after the hexadecimal chunk size.
         * @return Whether the extension grammar is valid.
         */
        auto valid_extensions(std::string_view value) -> bool {
            while (!value.empty()) {
                value = trim(value);
                if (value.empty() || value.front() != ';') {
                    return false;
                }
                value.remove_prefix(1);
                value = trim(value);
                auto end{ value.find_first_of("=; \t") };
                end = (std::min)(end, value.size());
                if (!is_token(value.substr(0, end))) {
                    return false;
                }
                value.remove_prefix(end);
                value = trim(value);
                if (value.empty() || value.front() == ';') {
                    continue;
                }
                if (value.front() != '=') {
                    return false;
                }
                value.remove_prefix(1);
                value = trim(value);
                if (value.empty()) {
                    return false;
                }
                if (value.front() == '"') {
                    value.remove_prefix(1);
                    bool closed{};
                    while (!value.empty()) {
                        auto const c{ value.front() };
                        value.remove_prefix(1);
                        if (c == '"') {
                            closed = true;
                            break;
                        }
                        if (c == '\\') {
                            if (value.empty()) {
                                return false;
                            }
                            if (!is_field_value(value.substr(0, 1))) {
                                return false;
                            }
                            value.remove_prefix(1);
                        } else if (!is_field_value(std::string_view{ &c, 1 })) {
                            return false;
                        }
                    }
                    if (!closed) {
                        return false;
                    }
                } else {
                    end = (std::min)(value.find_first_of("; \t"), value.size());
                    if (!is_token(value.substr(0, end))) {
                        return false;
                    }
                    value.remove_prefix(end);
                }
            }
            return true;
        }

        /**
         * @brief Own one connection, its timers, and HTTP response framing state.
         */
        class Exchange : public std::enable_shared_from_this<Exchange> {
        private:
            WireRequest m_request;                                            ///< Request retained until completion.
            trantor::EventLoop* m_loop;                                       ///< Caller loop, or main loop for non-loop threads.
            std::shared_ptr<trantor::Resolver> m_resolver;                    ///< Resolver retained while DNS is pending.
            std::shared_ptr<trantor::TcpClient> m_client;                     ///< One client per attempt.
            trantor::TimerId m_deadline_timer{};                              ///< Optional absolute request deadline.
            trantor::TimerId m_timer{};                                       ///< Active phase timer.
            std::function<void(Result<RawResponse<std::string>>)> m_callback; ///< Coroutine continuation.
            RawResponse<std::string> m_response{};                            ///< Collected response.
            std::string m_buffer;                                             ///< Unconsumed HTTP framing bytes.
            bool m_finished{};                                                ///< Completion callback already scheduled.
            bool m_headers_read{};                                            ///< Final response headers parsed.
            bool m_chunked{};                                                 ///< Chunked transfer framing.
            bool m_trailers{};                                                ///< Zero chunk reached; reading trailers.
            bool m_chunk_crlf{};                                              ///< Chunk payload ended; CRLF pending.
            bool m_writing{};                                                 ///< Request is draining to the socket.
            std::size_t m_bytes_sent{};                                       ///< Last observed socket write progress.
            std::chrono::steady_clock::time_point m_write_progress{};         ///< Last write progress time.
            std::size_t m_trailer_bytes{};                                    ///< Trailer section size, including consumed lines.
            std::optional<std::size_t> m_remaining{};                         ///< Content-Length bytes remaining.
            std::optional<std::size_t> m_chunk_remaining{};                   ///< Current chunk bytes remaining.

        public:
            /**
             * @brief Bind a request to an event loop.
             * @param request Request to send.
             * @param loop Loop owning every callback.
             */
            Exchange(WireRequest request, trantor::EventLoop* loop)
                : m_request{ std::move(request) }, m_loop{ loop } {}

            /**
             * @brief Begin DNS resolution and connect timeout.
             * @param callback Completion callback, invoked once on the owning loop.
             */
            auto Start(
                std::function<void(Result<RawResponse<std::string>>)> callback
            ) -> void {
                m_callback = std::move(callback);
                if (m_request.endpoint.host.empty() || m_request.endpoint.authority.empty() || !is_field_value(m_request.endpoint.authority) || m_request.endpoint.authority.find_first_of(" \t") != std::string::npos || m_request.endpoint.port == 0) {
                    fail(ErrorKind::InvalidRequest, "invalid HTTP endpoint");
                    return;
                }
                if (!is_token(m_request.method) || m_request.target.empty() || m_request.target.front() != '/' || std::ranges::any_of(m_request.target, [](unsigned char c) { return c <= 32 || c == 127; })) {
                    fail(ErrorKind::InvalidRequest, "invalid HTTP method or target");
                    return;
                }
                Headers normalized;
                for (auto const& [name, value] : m_request.headers) {
                    if (!is_token(name) || !is_field_value(value)) {
                        fail(ErrorKind::InvalidRequest, "invalid HTTP request header");
                        return;
                    }
                    normalized[lowercase(name)] = value;
                }
                m_request.headers = std::move(normalized);
                for (auto const seconds : { m_request.timeout.connect, m_request.timeout.read, m_request.timeout.write }) {
                    if (!std::isfinite(seconds) || seconds < 0) {
                        fail(ErrorKind::InvalidRequest, "timeouts must be finite and nonnegative");
                        return;
                    }
                }
                if (m_request.deadline) {
                    auto const remaining{ std::chrono::duration<double>(*m_request.deadline - std::chrono::steady_clock::now()).count() };
                    if (remaining <= 0) {
                        fail(ErrorKind::Timeout, "polling deadline exceeded");
                        return;
                    }
                    auto const weak{ weak_from_this() };
                    m_deadline_timer = m_loop->runAfter(remaining, [weak] {
                        if (auto self{ weak.lock() }; self && !self->m_finished) {
                            self->fail(ErrorKind::Timeout, "polling deadline exceeded");
                        }
                    });
                }
                arm(m_request.timeout.connect, "connect");
                auto const weak{ weak_from_this() };
                m_resolver = trantor::Resolver::newResolver(m_loop);
                m_resolver->resolve(m_request.endpoint.host, [weak, loop = m_loop](trantor::InetAddress const& address) {
                    // NormalResolver may run this callback on its worker thread.
                    loop->queueInLoop([weak, address] {
                        auto self{ weak.lock() };
                        if (!self || self->m_finished) {
                            return;
                        }
                        if (address.isUnspecified()) {
                            self->fail(ErrorKind::Connection, "DNS resolution failed");
                            return;
                        }
                        auto remote{ address };
                        // InetAddress exposes a network-order port setter.
                        auto const port{ self->m_request.endpoint.port };
                        remote.setPortNetEndian(htons(port));
                        self->connect(remote);
                    });
                });
            }

        private:
            /**
             * @brief Replace the active inactivity deadline.
             * @param seconds Timeout; zero disables it.
             * @param phase Phase included in timeout errors.
             */
            auto arm(double seconds, std::string phase) -> void {
                if (m_timer != 0) {
                    m_loop->invalidateTimer(m_timer);
                    m_timer = 0;
                }
                if (seconds > 0) {
                    auto const weak{ weak_from_this() };
                    m_timer = m_loop->runAfter(seconds, [weak, phase = std::move(phase)] {
                        if (auto self{ weak.lock() }; self && !self->m_finished) {
                            self->fail(ErrorKind::Timeout, phase + " timeout");
                        }
                    });
                }
            }

            /**
             * @brief Bound write inactivity while allowing a progressing upload to continue.
             */
            auto watchWrite() -> void {
                auto const seconds{ m_request.timeout.write };
                if (seconds == 0) {
                    return;
                }
                auto const weak{ weak_from_this() };
                m_timer = m_loop->runAfter((std::min)(seconds / 4, 0.05), [weak] {
                    if (auto self{ weak.lock() }; self && !self->m_finished && self->m_writing) {
                        self->m_timer = 0;
                        auto const now{ std::chrono::steady_clock::now() };
                        auto const sent{ self->m_client->connection()->bytesSent() };
                        if (sent != self->m_bytes_sent) {
                            self->m_bytes_sent = sent;
                            self->m_write_progress = now;
                        } else if (std::chrono::duration<double>{ now - self->m_write_progress }.count() >= self->m_request.timeout.write) {
                            self->fail(ErrorKind::Timeout, "write timeout");
                            return;
                        }
                        self->watchWrite();
                    }
                });
            }

            /**
             * @brief Establish TCP/TLS and install weakly owned connection callbacks.
             * @param address Resolved server address.
             */
            auto connect(trantor::InetAddress const& address) -> void {
                m_client = std::make_shared<trantor::TcpClient>(m_loop, address, "drogon-openai");
                if (m_request.endpoint.tls) {
                    auto policy{ trantor::TLSPolicy::defaultClientPolicy() };
                    policy->setHostname(m_request.endpoint.host).setValidate(true).setAllowBrokenChain(false).setUseSystemCertStore(true);
                    m_client->enableSSL(std::move(policy));
                }
                auto const weak{ weak_from_this() };
                m_client->setConnectionErrorCallback([weak] {
                    if (auto self{ weak.lock() }; self && !self->m_finished) {
                        self->fail(ErrorKind::Connection, "connection failed");
                    }
                });
                m_client->setSSLErrorCallback([weak](trantor::SSLError) {
                    if (auto self{ weak.lock() }; self && !self->m_finished) {
                        self->fail(ErrorKind::Connection, "TLS handshake or certificate validation failed");
                    }
                });
                m_client->setConnectionCallback([weak](trantor::TcpConnectionPtr const& connection) {
                    if (auto self{ weak.lock() }; self && !self->m_finished) {
                        if (connection->connected()) {
                            self->send(connection);
                        } else if (self->m_headers_read && !self->m_chunked && !self->m_remaining) {
                            self->finish(std::move(self->m_response));
                        } else {
                            self->fail(ErrorKind::Connection, "connection closed before the response completed");
                        }
                    }
                });
                m_client->setMessageCallback([weak](trantor::TcpConnectionPtr const&, trantor::MsgBuffer* buffer) {
                    if (auto self{ weak.lock() }; self && !self->m_finished) {
                        if (!self->m_writing) {
                            self->arm(self->m_request.timeout.read, "read");
                        }
                        self->m_buffer.append(buffer->peek(), buffer->readableBytes());
                        buffer->retrieveAll();
                        self->consume();
                    }
                });
                m_client->connect();
            }

            /**
             * @brief Serialize and send the request once TLS is established.
             * @param connection Ready connection.
             */
            auto send(trantor::TcpConnectionPtr const& connection) -> void {
                std::string bytes{ m_request.method + " " + m_request.target + " HTTP/1.1\r\n" };
                m_request.headers["host"] = m_request.endpoint.authority;
                m_request.headers["connection"] = "close";
                m_request.headers["accept-encoding"] = "identity";
                m_request.headers["content-length"] = std::to_string(m_request.body.size());
                m_request.headers.erase("transfer-encoding");
                for (auto const& [name, value] : m_request.headers) {
                    bytes += name + ": " + value + "\r\n";
                }
                bytes += "\r\n";
                bytes += m_request.body;
                m_writing = true;
                arm(0, "");
                m_bytes_sent = connection->bytesSent();
                m_write_progress = std::chrono::steady_clock::now();
                watchWrite();
                // Trantor does not consistently call write-complete for immediate sends.
                // Stream EOF is reached after all request bytes have been accepted by the socket/TLS layer.
                auto const weak{ weak_from_this() };
                connection->sendStream([weak, bytes = std::move(bytes), position = std::size_t{}](char* buffer, std::size_t capacity) mutable -> std::size_t {
                    auto self{ weak.lock() };
                    if (buffer == nullptr || !self || self->m_finished) {
                        return 0;
                    }
                    if (position == bytes.size()) {
                        self->m_writing = false;
                        self->arm(self->m_request.timeout.read, "read");
                        return 0;
                    }
                    auto const count{ (std::min)(capacity, bytes.size() - position) };
                    std::memcpy(buffer, bytes.data() + position, count);
                    position += count;
                    return count;
                });
            }

            /**
             * @brief Parse final response headers, ignoring informational responses.
             * @return Whether final headers are available.
             */
            auto headers() -> bool {
                // Consume informational responses iteratively, even when several share a packet.
                while (true) {
                    auto const end{ m_buffer.find("\r\n\r\n") };
                    if ((end == std::string::npos && m_buffer.size() > 65536) || (end != std::string::npos && end + 4 > 65536)) {
                        fail(ErrorKind::ResponseValidation, "HTTP headers exceed 64 KiB");
                        return false;
                    }
                    if (end == std::string::npos) {
                        return false;
                    }
                    auto const first_end{ m_buffer.find("\r\n") };
                    auto const line{ std::string_view{ m_buffer }.substr(0, first_end) };
                    int status{};
                    if (line.size() < 13 || (!line.starts_with("HTTP/1.0 ") && !line.starts_with("HTTP/1.1 ")) || line[12] != ' ' || !is_field_value(line.substr(13))) {
                        fail(ErrorKind::ResponseValidation, "invalid HTTP status line");
                        return false;
                    }
                    auto const parsed_status{ std::from_chars(line.data() + 9, line.data() + 12, status) };
                    if (parsed_status.ec != std::errc{} || parsed_status.ptr != line.data() + 12 || status < 100 || status > 599) {
                        fail(ErrorKind::ResponseValidation, "invalid HTTP status code");
                        return false;
                    }
                    Headers fields;
                    std::size_t position{ first_end + 2 };
                    while (position < end) {
                        auto const next{ m_buffer.find("\r\n", position) };
                        auto const colon{ m_buffer.find(':', position) };
                        if (colon == std::string::npos || colon >= next) {
                            fail(ErrorKind::ResponseValidation, "invalid HTTP header");
                            return false;
                        }
                        auto name{ lowercase(m_buffer.substr(position, colon - position)) };
                        auto value{ std::string{ trim(std::string_view{ m_buffer }.substr(colon + 1, next - colon - 1)) } };
                        if (!is_token(name) || !is_field_value(value)) {
                            fail(ErrorKind::ResponseValidation, "invalid HTTP header name or value");
                            return false;
                        }
                        if (fields.contains(name)) {
                            if (name == "content-length" && fields[name] != value) {
                                fail(ErrorKind::ResponseValidation, "conflicting Content-Length headers");
                                return false;
                            }
                            if (name != "content-length") {
                                fields[name] += ", " + value;
                            }
                        } else {
                            fields.emplace(std::move(name), std::move(value));
                        }
                        position = next + 2;
                    }
                    m_buffer.erase(0, end + 4);
                    if (status == 101) {
                        fail(ErrorKind::ResponseValidation, "HTTP protocol upgrades are unsupported");
                        return false;
                    }
                    if (status < 200) {
                        continue;
                    }
                    m_response.status_code = status;
                    m_response.headers = std::move(fields);
                    if (auto it{ m_response.headers.find("x-request-id") }; it != m_response.headers.end()) {
                        m_response.request_id = it->second;
                    }
                    m_headers_read = true;
                    if (m_request.method == "HEAD" || status == 204 || status == 304) {
                        finish(std::move(m_response));
                        return false;
                    }
                    if (auto it{ m_response.headers.find("transfer-encoding") }; it != m_response.headers.end()) {
                        if (m_response.headers.contains("content-length")) {
                            fail(ErrorKind::ResponseValidation, "conflicting Transfer-Encoding and Content-Length");
                            return false;
                        }
                        if (lowercase(it->second) != "chunked") {
                            fail(ErrorKind::ResponseValidation, "unsupported HTTP transfer encoding");
                            return false;
                        }
                        m_chunked = true;
                    } else if (auto length{ m_response.headers.find("content-length") }; length != m_response.headers.end()) {
                        std::size_t size{};
                        auto const& value{ length->second };
                        auto const parsed{ std::from_chars(value.data(), value.data() + value.size(), size) };
                        if (parsed.ec != std::errc{} || parsed.ptr != value.data() + value.size()) {
                            fail(ErrorKind::ResponseValidation, "invalid Content-Length");
                            return false;
                        }
                        m_remaining = size;
                    }
                    return true;
                }
            }

            /**
             * @brief Deliver decoded body bytes without retaining successful stream bodies.
             * @param size Bytes at the beginning of the input buffer.
             */
            auto deliver(std::size_t size) -> void {
                if (size == 0 || m_finished) {
                    return;
                }
                if (m_request.on_data && m_response.status_code >= 200 && m_response.status_code < 300) {
                    auto result{ m_request.on_data(std::string_view{ m_buffer }.substr(0, size), m_response.headers) };
                    if (!result) {
                        auto error{ result.error() };
                        error.status_code = m_response.status_code;
                        error.headers = m_response.headers;
                        error.request_id = m_response.request_id;
                        finish(std::unexpected{ std::move(error) });
                        return;
                    }
                    if (!*result) {
                        finish(std::move(m_response));
                        return;
                    }
                } else {
                    m_response.body.append(m_buffer.data(), size);
                }
                m_buffer.erase(0, size);
            }

            /**
             * @brief Consume Content-Length, chunked, or connection-delimited response data.
             */
            auto consume() -> void {
                if (!m_headers_read && !headers()) {
                    return;
                }
                if (m_finished) {
                    return;
                }
                if (!m_chunked) {
                    auto const size{ m_remaining ? (std::min)(*m_remaining, m_buffer.size()) : m_buffer.size() };
                    deliver(size);
                    if (!m_finished && m_remaining) {
                        *m_remaining -= size;
                        if (*m_remaining == 0) {
                            finish(std::move(m_response));
                        }
                    }
                    return;
                }
                while (!m_finished) {
                    if (m_trailers) {
                        auto const end{ m_buffer.find("\r\n") };
                        auto const size{ end == std::string::npos ? m_buffer.size() : end + 2 };
                        if (m_trailer_bytes + size > 65536) {
                            fail(ErrorKind::ResponseValidation, "HTTP trailers exceed 64 KiB");
                            return;
                        }
                        if (end == std::string::npos) {
                            return;
                        }
                        if (end == 0) {
                            finish(std::move(m_response));
                            return;
                        }
                        auto const line{ std::string_view{ m_buffer }.substr(0, end) };
                        auto const colon{ line.find(':') };
                        if (colon == std::string_view::npos || !is_token(line.substr(0, colon)) || !is_field_value(line.substr(colon + 1))) {
                            fail(ErrorKind::ResponseValidation, "invalid HTTP trailer");
                            return;
                        }
                        auto const name{ lowercase(std::string{ line.substr(0, colon) }) };
                        if (name == "content-length" || name == "transfer-encoding" || name == "host") {
                            fail(ErrorKind::ResponseValidation, "forbidden HTTP framing trailer");
                            return;
                        }
                        // Trailers do not replace metadata already supplied to body callbacks.
                        m_trailer_bytes += end + 2;
                        m_buffer.erase(0, end + 2);
                        continue;
                    }
                    if (m_chunk_crlf) {
                        if (m_buffer.size() < 2) {
                            return;
                        }
                        if (!m_buffer.starts_with("\r\n")) {
                            fail(ErrorKind::ResponseValidation, "invalid HTTP chunk terminator");
                            return;
                        }
                        m_buffer.erase(0, 2);
                        m_chunk_crlf = false;
                    }
                    if (!m_chunk_remaining) {
                        auto const end{ m_buffer.find("\r\n") };
                        if ((end == std::string::npos && m_buffer.size() > 8192) || (end != std::string::npos && end > 8192)) {
                            fail(ErrorKind::ResponseValidation, "HTTP chunk size exceeds 8 KiB");
                            return;
                        }
                        if (end == std::string::npos) {
                            return;
                        }
                        auto token{ std::string_view{ m_buffer }.substr(0, end) };
                        auto const extension{ token.find_first_of("; \t") };
                        if (extension != std::string_view::npos && !valid_extensions(token.substr(extension))) {
                            fail(ErrorKind::ResponseValidation, "invalid HTTP chunk extension");
                            return;
                        }
                        token = token.substr(0, extension);
                        std::size_t size{};
                        auto const parsed{ std::from_chars(token.data(), token.data() + token.size(), size, 16) };
                        if (token.empty() || parsed.ec != std::errc{} || parsed.ptr != token.data() + token.size()) {
                            fail(ErrorKind::ResponseValidation, "invalid HTTP chunk size");
                            return;
                        }
                        m_buffer.erase(0, end + 2);
                        if (size == 0) {
                            m_trailers = true;
                            continue;
                        }
                        m_chunk_remaining = size;
                    }
                    auto const size{ (std::min)(*m_chunk_remaining, m_buffer.size()) };
                    deliver(size);
                    if (m_finished) {
                        return;
                    }
                    *m_chunk_remaining -= size;
                    if (*m_chunk_remaining != 0) {
                        return;
                    }
                    m_chunk_remaining.reset();
                    m_chunk_crlf = true;
                }
            }

            /**
             * @brief Construct a transport error with request context.
             * @param kind Error category.
             * @param reason Failure description.
             */
            auto fail(ErrorKind kind, std::string reason) -> void {
                finish(std::unexpected{ Error{
                    .kind = kind,
                    .message = m_request.method + " " + (m_request.endpoint.tls ? "https://" : "http://") + m_request.endpoint.authority + m_request.target + ": " + reason,
                    .status_code = m_response.status_code,
                    .body = m_response.body,
                    .request_id = m_response.request_id,
                    .headers = m_response.headers,
                } });
            }

            /**
             * @brief Stop the socket and schedule exactly one coroutine continuation.
             * @param result Attempt result.
             */
            auto finish(Result<RawResponse<std::string>> result) -> void {
                if (m_finished) {
                    return;
                }
                m_finished = true;
                if (m_deadline_timer != 0) {
                    m_loop->invalidateTimer(m_deadline_timer);
                    m_deadline_timer = 0;
                }
                arm(0, "");
                if (m_client) {
                    m_client->stop();
                    if (auto connection{ m_client->connection() }) {
                        connection->forceClose();
                    }
                }
                auto self{ shared_from_this() };
                m_loop->queueInLoop([self, result = std::move(result)]() mutable {
                    auto callback{ std::move(self->m_callback) };
                    if (result) {
                        result->parsed = result->body;
                    }
                    callback(std::move(result));
                });
            }
        };

        /**
         * @brief Adapt a callback-driven connection to a coroutine awaiter.
         */
        struct ExchangeAwaiter : drogon::CallbackAwaiter<Result<RawResponse<std::string>>> {
            std::shared_ptr<Exchange> state; ///< Retains the request while suspended.
            trantor::EventLoop* loop;        ///< Owning event loop.

            /**
             * @brief Create an awaiter with owned request state.
             * @param request Request to send.
             * @param event_loop Owning event loop.
             */
            ExchangeAwaiter(WireRequest request, trantor::EventLoop* event_loop)
                : state{ std::make_shared<Exchange>(std::move(request), event_loop) }, loop{ event_loop } {}

            /**
             * @brief Start the connection on its event loop and resume after completion.
             * @param handle Suspended coroutine.
             */
            auto await_suspend(std::coroutine_handle<> handle) -> void {
                loop->queueInLoop([this, handle] {
                    state->Start([this, handle](Result<RawResponse<std::string>> result) {
                        setValue(std::move(result));
                        handle.resume();
                    });
                });
            }
        };

    } // namespace

    /**
     * @brief Parse an HTTP or HTTPS base URL.
     * @param url Base URL without a query or fragment.
     * @return Endpoint or a local configuration error.
     */
    auto parse_endpoint(std::string const& url) -> Result<Endpoint> {
        auto invalid{ [] {
            return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "base_url must be an HTTP(S) URL with a valid host and port and no query/fragment" } };
        } };
        Endpoint endpoint;
        std::size_t begin{};
        if (url.starts_with("https://")) {
            endpoint.tls = true;
            endpoint.port = 443;
            begin = 8;
        } else if (url.starts_with("http://")) {
            endpoint.port = 80;
            begin = 7;
        } else {
            return invalid();
        }
        if (url.find_first_of("?#\\", begin) != std::string::npos || std::ranges::any_of(url, [](unsigned char c) { return c <= 32 || c == 127; })) {
            return invalid();
        }
        auto const slash{ url.find('/', begin) };
        endpoint.authority = url.substr(begin, slash == std::string::npos ? slash : slash - begin);
        if (endpoint.authority.empty() || endpoint.authority.find_first_of("@ \t") != std::string::npos) {
            return invalid();
        }
        std::string port;
        if (endpoint.authority.front() == '[') {
            auto const end{ endpoint.authority.find(']') };
            if (end == std::string::npos) {
                return invalid();
            }
            endpoint.host = endpoint.authority.substr(1, end - 1);
            in6_addr address{};
            if (inet_pton(AF_INET6, endpoint.host.c_str(), &address) != 1) {
                return invalid();
            }
            if (end + 1 < endpoint.authority.size()) {
                if (endpoint.authority[end + 1] != ':') {
                    return invalid();
                }
                port = endpoint.authority.substr(end + 2);
            }
        } else {
            auto const colon{ endpoint.authority.find(':') };
            endpoint.host = endpoint.authority.substr(0, colon);
            if (endpoint.host.find_first_of("[]") != std::string::npos) {
                return invalid();
            }
            if (colon != std::string::npos) {
                port = endpoint.authority.substr(colon + 1);
            }
        }
        if (endpoint.host.empty()) {
            return invalid();
        }
        if (!port.empty()) {
            unsigned int number{};
            auto const parsed{ std::from_chars(port.data(), port.data() + port.size(), number) };
            if (parsed.ec != std::errc{} || parsed.ptr != port.data() + port.size() || number == 0 || number > 65535) {
                return invalid();
            }
            endpoint.port = static_cast<unsigned short>(number);
        } else if (endpoint.authority.back() == ':') {
            return invalid();
        }
        endpoint.prefix = slash == std::string::npos ? "" : url.substr(slash);
        while (endpoint.prefix.ends_with('/')) {
            endpoint.prefix.pop_back();
        }
        return endpoint;
    }

    /**
     * @brief Send a single request using a new connection on the calling event loop.
     * @param request Owned request data.
     * @return Response metadata and bytes or a connection/protocol error.
     * @note TLS verifies the hostname and certificate chain against the system trust store.
     * A callback consumes successful bodies without retaining them. Other bodies are buffered.
     * Non-loop callers use the main app loop; that loop must already be running.
     */
    auto exchange(WireRequest request) -> drogon::Task<Result<RawResponse<std::string>>> {
        auto* loop{ trantor::EventLoop::getEventLoopOfCurrentThread() };
        if (loop == nullptr) {
            loop = drogon::app().getLoop();
        }
        co_return co_await ExchangeAwaiter{ std::move(request), loop };
    }

} // namespace drogon_openai::details
