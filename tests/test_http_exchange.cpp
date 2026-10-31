/**
 * @file test_http_exchange.cpp
 * @brief Exercise HTTP framing, incremental delivery, TLS rejection, and phase timeouts over raw TCP.
 */
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

#include <atomic>
#include <chrono>
#include <filesystem>
#include <functional>
#include <future>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <trantor/net/EventLoopThread.h>

#include "../src/details/http_exchange.hpp"
#include "support.hpp"

using namespace std::chrono_literals;
using namespace drogon_openai;
namespace wire = drogon_openai::details;

namespace {

#ifdef _WIN32
    using NativeSocket = SOCKET;                                   ///< Native socket handle.
    constexpr NativeSocket INVALID_SOCKET_VALUE{ INVALID_SOCKET }; ///< Invalid handle.
    using SocketLength = int;                                      ///< Socket address length.
#else
    using NativeSocket = int;                          ///< Native socket handle.
    constexpr NativeSocket INVALID_SOCKET_VALUE{ -1 }; ///< Invalid handle.
    using SocketLength = socklen_t;                    ///< Socket address length.
#endif

    /**
     * @brief Own a blocking raw socket with bounded reads and writes.
     */
    class Socket {
    private:
        NativeSocket m_socket; ///< Owned descriptor.

    public:
        /**
         * @brief Adopt a native descriptor.
         * @param socket Descriptor to close on destruction.
         */
        explicit Socket(NativeSocket socket) : m_socket{ socket } {
            if (socket == INVALID_SOCKET_VALUE) {
                throw std::runtime_error{ "fixture socket creation failed" };
            }
#ifdef _WIN32
            DWORD const timeout{ 3000 };
#else
            timeval const timeout{ 3, 0 };
#endif
            setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char const*>(&timeout), sizeof(timeout));
            setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char const*>(&timeout), sizeof(timeout));
        }

        /**
         * @brief Close the descriptor.
         */
        ~Socket() {
#ifdef _WIN32
            closesocket(m_socket);
#else
            close(m_socket);
#endif
        }

        Socket(Socket const&) = delete;
        auto operator=(Socket const&) -> Socket& = delete;

        /**
         * @brief Borrow the descriptor for platform and TLS operations.
         * @return Native descriptor.
         */
        [[nodiscard]] auto Get() const -> NativeSocket { return m_socket; }

        /**
         * @brief Send all bytes, allowing the tested client to close the connection early.
         * @param bytes Wire bytes.
         * @return Whether every byte was sent.
         */
        auto Send(std::string_view bytes) const -> bool {
            while (!bytes.empty()) {
                auto const count{ static_cast<int>(std::min<std::size_t>(bytes.size(), 1024 * 1024)) };
#ifdef MSG_NOSIGNAL
                auto const sent{ ::send(m_socket, bytes.data(), count, MSG_NOSIGNAL) };
#else
                auto const sent{ ::send(m_socket, bytes.data(), count, 0) };
#endif
                if (sent <= 0) {
                    return false;
                }
                bytes.remove_prefix(static_cast<std::size_t>(sent));
            }
            return true;
        }

        /**
         * @brief Read the next block, reporting EOF as an empty string.
         * @return Received bytes.
         * @throws std::runtime_error If no bytes arrive before the fixture deadline.
         */
        auto Read() const -> std::string {
            char buffer[65536];
            auto const received{ recv(m_socket, buffer, sizeof(buffer), 0) };
            if (received < 0) {
                throw std::runtime_error{ "fixture receive failed or timed out" };
            }
            return { buffer, static_cast<std::size_t>(received) };
        }

        /**
         * @brief Read a complete request using the client's Content-Length field.
         * @return Request line, fields, and body as received.
         */
        auto Request() const -> std::string {
            std::string bytes;
            std::size_t end{};
            while ((end = bytes.find("\r\n\r\n")) == std::string::npos) {
                auto block{ Read() };
                if (block.empty()) {
                    throw std::runtime_error{ "client closed before request headers" };
                }
                bytes += block;
            }
            auto const length_field{ bytes.find("\r\ncontent-length: ") };
            auto const length{ length_field == std::string::npos ? 0 : std::stoull(bytes.substr(length_field + 18)) };
            while (bytes.size() < end + 4 + length) {
                auto block{ Read() };
                if (block.empty()) {
                    throw std::runtime_error{ "client closed before request body" };
                }
                bytes += block;
            }
            return bytes;
        }
    };

    /**
     * @brief Listen on an ephemeral loopback port and run independent raw connection scripts.
     */
    class RawServer {
    public:
        using Script = std::function<void(Socket const&)>; ///< One accepted connection's behavior.

    private:
        Socket m_listener{ ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) }; ///< Ephemeral listener.
        unsigned short m_port{};                                          ///< Assigned port.
        std::future<void> m_worker;                                       ///< Acceptor and script completion.

    public:
        /**
         * @brief Bind and begin accepting exactly one connection per supplied script.
         * @param scripts Connection scripts, in acceptance order.
         */
        explicit RawServer(std::vector<Script> scripts) {
            sockaddr_in address{};
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            if (bind(m_listener.Get(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0 || listen(m_listener.Get(), 8) != 0) {
                throw std::runtime_error{ "fixture bind/listen failed" };
            }
            SocketLength length{ sizeof(address) };
            if (getsockname(m_listener.Get(), reinterpret_cast<sockaddr*>(&address), &length) != 0) {
                throw std::runtime_error{ "fixture getsockname failed" };
            }
            m_port = ntohs(address.sin_port);
            m_worker = std::async(std::launch::async, [this, scripts = std::move(scripts)] {
                std::vector<std::future<void>> workers;
                for (auto const& script : scripts) {
                    fd_set ready;
                    FD_ZERO(&ready);
                    FD_SET(m_listener.Get(), &ready);
                    timeval timeout{ 3, 0 };
                    if (select(static_cast<int>(m_listener.Get()) + 1, &ready, nullptr, nullptr, &timeout) != 1) {
                        throw std::runtime_error{ "fixture accept timed out" };
                    }
                    auto const accepted{ accept(m_listener.Get(), nullptr, nullptr) };
                    workers.push_back(std::async(std::launch::async, [accepted, script] {
                        Socket const socket{ accepted };
                        script(socket);
                    }));
                }
                for (auto& worker : workers) {
                    worker.get();
                }
            });
        }

        /**
         * @brief Join scripts before destroying the listener and captured test state.
         */
        ~RawServer() {
            if (m_worker.valid()) {
                m_worker.wait();
            }
        }

        /**
         * @brief Build a POST request for this fixture.
         * @return Request with short, bounded phase timeouts.
         */
        [[nodiscard]] auto Request() const -> wire::WireRequest {
            return wire::WireRequest{
                .endpoint = *wire::parse_endpoint("http://127.0.0.1:" + std::to_string(m_port)),
                .method = "POST",
                .target = "/v1/test?key=value",
                .headers = { { "content-type", "application/json" } },
                .body = "{}",
                .timeout = Timeout{ 2 },
            };
        }

        /**
         * @brief Join the server and propagate fixture failures into the test framework.
         */
        auto Check() -> void { m_worker.get(); }
    };

    /**
     * @brief Exchange one canned response and close the connection.
     * @param response Exact HTTP wire bytes.
     * @return Exchange result.
     */
    auto respond(std::string response) -> Result<RawResponse<std::string>> {
        RawServer server{ { [response = std::move(response)](Socket const& socket) {
            socket.Request();
            socket.Send(response);
        } } };
        auto result{ drogon::sync_wait(wire::exchange(server.Request())) };
        server.Check();
        return result;
    }

    /**
     * @brief Run an exchange on a chosen loop and report continuation affinity.
     * @param request Request to execute.
     * @param result Completion promise retained by the caller.
     * @return Coroutine task.
     */
    auto check_loop(
        wire::WireRequest request,
        std::shared_ptr<std::promise<bool>> result
    ) -> drogon::Task<void> {
        auto const thread{ std::this_thread::get_id() };
        auto const response{ co_await wire::exchange(std::move(request)) };
        result->set_value(response && thread == std::this_thread::get_id());
    }

    /**
     * @brief Create an ephemeral self-signed TLS server context without external files or commands.
     * @return Server context owning a generated private key and certificate.
     */
    auto self_signed_context() -> std::unique_ptr<SSL_CTX, decltype(&SSL_CTX_free)> {
        std::unique_ptr<EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free)> key_context{ EVP_PKEY_CTX_new_id(EVP_PKEY_EC, nullptr), EVP_PKEY_CTX_free };
        EVP_PKEY* generated{};
        if (!key_context || EVP_PKEY_keygen_init(key_context.get()) != 1 || EVP_PKEY_CTX_set_ec_paramgen_curve_nid(key_context.get(), NID_X9_62_prime256v1) != 1 || EVP_PKEY_keygen(key_context.get(), &generated) != 1) {
            throw std::runtime_error{ "fixture key generation failed" };
        }
        std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> key{ generated, EVP_PKEY_free };
        std::unique_ptr<X509, decltype(&X509_free)> certificate{ X509_new(), X509_free };
        if (!certificate) {
            throw std::runtime_error{ "fixture X509 allocation failed" };
        }
        X509_set_version(certificate.get(), 2);
        ASN1_INTEGER_set(X509_get_serialNumber(certificate.get()), 1);
        X509_gmtime_adj(X509_getm_notBefore(certificate.get()), -60);
        X509_gmtime_adj(X509_getm_notAfter(certificate.get()), 3600);
        X509_set_pubkey(certificate.get(), key.get());
        auto* name{ X509_get_subject_name(certificate.get()) };
        X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, reinterpret_cast<unsigned char const*>("localhost"), -1, -1, 0);
        X509_set_issuer_name(certificate.get(), name);
        std::unique_ptr<SSL_CTX, decltype(&SSL_CTX_free)> context{ SSL_CTX_new(TLS_server_method()), SSL_CTX_free };
        if (X509_sign(certificate.get(), key.get(), EVP_sha256()) == 0 || !context || SSL_CTX_use_certificate(context.get(), certificate.get()) != 1 || SSL_CTX_use_PrivateKey(context.get(), key.get()) != 1) {
            throw std::runtime_error{ "fixture TLS context setup failed" };
        }
        return context;
    }

} // namespace

DROGON_TEST(HttpExchangeContentLengthAndHeaders) {
    std::string received;
    RawServer server{ { [&received](Socket const& socket) {
        received = socket.Request();
        socket.Send("HTTP/1.1 200 OK\r\nCoNtEnT-LeNgTh: 5\r\nX-Request-Id:\t req-123 \t\r\nX-Test: first\r\nX-Test: second\r\n\r\nhelloEXTRA");
        std::this_thread::sleep_for(150ms);
    } } };
    auto request{ server.Request() };
    request.headers["Host"] = "wrong";
    request.headers["Content-Length"] = "999";
    request.headers["Transfer-Encoding"] = "chunked";
    request.headers["Authorization"] = "Bearer test";
    auto const result{ drogon::sync_wait(wire::exchange(std::move(request))) };
    server.Check();
    REQUIRE(result);
    CHECK(result->body == "hello");
    CHECK(result->parsed == "hello");
    CHECK(result->status_code == 200);
    CHECK(result->request_id == "req-123");
    CHECK(result->headers.at("x-test") == "first, second");
    CHECK(received.starts_with("POST /v1/test?key=value HTTP/1.1\r\n"));
    CHECK(received.ends_with("\r\n\r\n{}"));
    CHECK(received.find("\r\nhost: 127.0.0.1:") != std::string::npos);
    CHECK(received.find("\r\ncontent-length: 2\r\n") != std::string::npos);
    CHECK(received.find("\r\nconnection: close\r\n") != std::string::npos);
    CHECK(received.find("\r\naccept-encoding: identity\r\n") != std::string::npos);
    CHECK(received.find("\r\nauthorization: Bearer test\r\n") != std::string::npos);
    CHECK(received.find("transfer-encoding:") == std::string::npos);
}

DROGON_TEST(HttpExchangeInterimAndFragmentedChunked) {
    RawServer server{ { [](Socket const& socket) {
        socket.Request();
        std::string const response{ "HTTP/1.1 100 Continue\r\n\r\nHTTP/1.1 103 Early Hints\r\nLink: </x>\r\n\r\nHTTP/1.1 200 OK\r\nTransfer-Encoding: Chunked\r\nX-Request-ID: final\r\n\r\n3; ext=\"a\\\"b\"\r\na" };
        for (auto const c : response) {
            socket.Send(std::string_view{ &c, 1 });
        }
        std::this_thread::sleep_for(20ms);
        socket.Send("bc\r");
        std::this_thread::sleep_for(20ms);
        socket.Send("\n2\r\nde\r\n0\r\nX-Trailer: accepted\r\n\r\n");
    } } };
    auto const result{ drogon::sync_wait(wire::exchange(server.Request())) };
    server.Check();
    REQUIRE(result);
    CHECK(result->body == "abcde");
    CHECK(result->status_code == 200);
    CHECK(result->request_id == "final");
    CHECK(!result->headers.contains("link"));
}

DROGON_TEST(HttpExchangeEofFraming) {
    auto const result{ respond("HTTP/1.0 200 OK\r\nX-Test: eof\r\n\r\nbody until EOF") };
    REQUIRE(result);
    CHECK(result->body == "body until EOF");
    auto const empty{ respond("HTTP/1.1 200 OK\r\n\r\n") };
    REQUIRE(empty);
    CHECK(empty->body.empty());
}

DROGON_TEST(HttpExchangeNoBodyResponsesCompleteWithoutEof) {
    for (auto const status : { 200, 204, 304 }) {
        std::promise<void> completed;
        auto future{ completed.get_future() };
        RawServer server{ { [status, &future](Socket const& socket) {
            socket.Request();
            socket.Send("HTTP/1.1 " + std::to_string(status) + " OK\r\nContent-Length: " + (status == 200 ? "0" : "999") + "\r\n\r\n");
            if (future.wait_for(1s) != std::future_status::ready) {
                throw std::runtime_error{ "bodyless response waited for EOF" };
            }
        } } };
        auto const result{ drogon::sync_wait(wire::exchange(server.Request())) };
        completed.set_value();
        server.Check();
        REQUIRE(result);
        CHECK(result->body.empty());
    }
    RawServer server{ { [](Socket const& socket) {
        socket.Request();
        socket.Send("HTTP/1.1 200 OK\r\nContent-Length: 999\r\n\r\n");
    } } };
    auto request{ server.Request() };
    request.method = "HEAD";
    auto const result{ drogon::sync_wait(wire::exchange(std::move(request))) };
    server.Check();
    REQUIRE(result);
    CHECK(result->body.empty());
}

DROGON_TEST(HttpExchangeSseArrivesBeforeHttpChunkCompletes) {
    std::promise<void> first_received;
    auto first_future{ first_received.get_future() };
    std::atomic<bool> acknowledged{};
    RawServer server{ { [&first_future, &acknowledged](Socket const& socket) {
        socket.Request();
        socket.Send("HTTP/1.1 200 OK\r\nContent-Type: text/event-stream\r\nTransfer-Encoding: chunked\r\n\r\n1b\r\ndata: first\n\n");
        acknowledged = first_future.wait_for(1s) == std::future_status::ready;
        socket.Send("data: second\n\n\r\n0\r\n\r\n");
    } } };
    auto request{ server.Request() };
    std::string streamed;
    bool signaled{};
    request.on_data = [&](std::string_view bytes, Headers const& headers) -> Result<bool> {
        CHECK(headers.at("content-type") == "text/event-stream");
        streamed += bytes;
        if (!signaled && streamed.find("data: first\n\n") != std::string::npos) {
            signaled = true;
            first_received.set_value();
        }
        return true;
    };
    auto const result{ drogon::sync_wait(wire::exchange(std::move(request))) };
    server.Check();
    REQUIRE(result);
    CHECK(acknowledged.load());
    CHECK(streamed == "data: first\n\ndata: second\n\n");
    CHECK(result->body.empty());
    CHECK(result->parsed.empty());
}

DROGON_TEST(HttpExchangeIncrementalLengthAndEofBodies) {
    for (auto const chunked : { false, true }) {
        std::promise<void> received;
        auto future{ received.get_future() };
        std::atomic<bool> early{};
        RawServer server{ { [chunked, &future, &early](Socket const& socket) {
            socket.Request();
            socket.Send(std::string{ "HTTP/1.1 200 OK\r\n" } + (chunked ? "Content-Length: 6\r\n" : "") + "\r\none");
            early = future.wait_for(1s) == std::future_status::ready;
            socket.Send("two");
        } } };
        auto request{ server.Request() };
        std::string body;
        bool signaled{};
        request.on_data = [&](std::string_view bytes, Headers const&) -> Result<bool> {
            body += bytes;
            if (!signaled && body == "one") {
                signaled = true;
                received.set_value();
            }
            return true;
        };
        auto const result{ drogon::sync_wait(wire::exchange(std::move(request))) };
        server.Check();
        REQUIRE(result);
        CHECK(early.load());
        CHECK(body == "onetwo");
    }
}

DROGON_TEST(HttpExchangeCallbackStopClosesSocket) {
    std::atomic<bool> closed{};
    RawServer server{ { [&closed](Socket const& socket) {
        socket.Request();
        socket.Send("HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n100\r\ndata: [DONE]\n\n");
        closed = socket.Read().empty();
    } } };
    auto request{ server.Request() };
    request.on_data = [](std::string_view, Headers const&) -> Result<bool> {
        return false;
    };
    auto const result{ drogon::sync_wait(wire::exchange(std::move(request))) };
    server.Check();
    REQUIRE(result);
    CHECK(closed.load());
}

DROGON_TEST(HttpExchangeCallbackFailureAndErrorBody) {
    RawServer server{ { [](Socket const& socket) {
        socket.Request();
        socket.Send("HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nbad");
    } } };
    auto request{ server.Request() };
    request.on_data = [](std::string_view, Headers const&) -> Result<bool> {
        return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "consumer rejected data" } };
    };
    auto const result{ drogon::sync_wait(wire::exchange(std::move(request))) };
    server.Check();
    REQUIRE(!result);
    CHECK(result.error().message == "consumer rejected data");

    RawServer error_server{ { [](Socket const& socket) {
        socket.Request();
        socket.Send("HTTP/1.1 429 Too Many Requests\r\nContent-Length: 4\r\nX-Request-ID: error\r\n\r\noops");
    } } };
    auto error_request{ error_server.Request() };
    bool called{};
    error_request.on_data = [&called](std::string_view, Headers const&) -> Result<bool> {
        called = true;
        return true;
    };
    auto const error_result{ drogon::sync_wait(wire::exchange(std::move(error_request))) };
    error_server.Check();
    REQUIRE(error_result);
    CHECK(error_result->status_code == 429);
    CHECK(error_result->body == "oops");
    CHECK(error_result->request_id == "error");
    CHECK(!called);
}

DROGON_TEST(HttpExchangeRejectsMalformedFraming) {
    std::vector<std::string> const responses{
        "HTTP/1.1 20x OK\r\n\r\n",
        "HTTP/1.1 200x OK\r\n\r\n",
        "HTTP/1.9 200 OK\r\n\r\n",
        "HTTP/1.1 101 Switching Protocols\r\n\r\n",
        "HTTP/1.1 200 OK\r\nBad Header: x\r\n\r\n",
        "HTTP/1.1 200 OK\r\n: value\r\n\r\n",
        "HTTP/1.1 200 OK\r\nHeader: bad\x01value\r\n\r\n",
        "HTTP/1.1 200 OK\r\nContent-Length: 2\r\nContent-Length: 3\r\n\r\nabc",
        "HTTP/1.1 200 OK\r\nContent-Length: -1\r\n\r\n",
        "HTTP/1.1 200 OK\r\nContent-Length: 1x\r\n\r\n",
        "HTTP/1.1 200 OK\r\nContent-Length: 99999999999999999999999999\r\n\r\n",
        "HTTP/1.1 200 OK\r\nTransfer-Encoding: gzip, chunked\r\n\r\n",
        "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\nContent-Length: 4\r\n\r\n0\r\n\r\n",
        "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\nz\r\n",
        "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n+1\r\na\r\n0\r\n\r\n",
        "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n1\r\naXX0\r\n\r\n",
        "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n1;bad=\r\na\r\n0\r\n\r\n",
        "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n0\r\nbroken trailer\r\n\r\n",
        "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n0\r\nContent-Length: 0\r\n\r\n",
    };
    for (auto const& response : responses) {
        auto const result{ respond(response) };
        REQUIRE(!result);
        CHECK(result.error().kind == ErrorKind::ResponseValidation);
    }
}

DROGON_TEST(HttpExchangeBoundsFramingMetadata) {
    for (auto const& response : std::vector<std::string>{
             "HTTP/1.1 200 OK\r\nX-Huge: " + std::string(65536, 'a') + "\r\n\r\n",
             "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n1;ext=" + std::string(8192, 'a') + "\r\na\r\n0\r\n\r\n",
             "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n0\r\nX-Huge: " + std::string(65536, 'a') + "\r\n\r\n",
         }) {
        auto const result{ respond(response) };
        REQUIRE(!result);
        CHECK(result.error().kind == ErrorKind::ResponseValidation);
    }
}

DROGON_TEST(HttpExchangeTruncationIsFailure) {
    for (auto const& response : std::vector<std::string>{
             "",
             "HTTP/1.1 200 OK\r\nContent-Len",
             "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nabc",
             "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nabc",
             "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n1\r\na",
             "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n1\r\na\r\n",
             "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n0\r\nX-Trailer: value\r\n",
         }) {
        auto const result{ respond(response) };
        REQUIRE(!result);
        CHECK(result.error().kind == ErrorKind::Connection);
    }
}

DROGON_TEST(HttpExchangeReadTimeoutCoversHeadersBodyAndTrailers) {
    for (auto const& response : std::vector<std::string>{
             "",
             "HTTP/1.1 200 OK\r\nContent-Length: 5\r\nX-Request-ID: timeout\r\n\r\na",
             "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n0\r\nX-Trailer: partial",
         }) {
        std::atomic<bool> closed{};
        RawServer server{ { [&response, &closed](Socket const& socket) {
            socket.Request();
            socket.Send(response);
            closed = socket.Read().empty();
        } } };
        auto request{ server.Request() };
        request.timeout.read = 0.08;
        auto const result{ drogon::sync_wait(wire::exchange(std::move(request))) };
        server.Check();
        REQUIRE(!result);
        CHECK(result.error().kind == ErrorKind::Timeout);
        CHECK(result.error().message.substr(result.error().message.rfind(": ") + 2) == "read timeout");
        CHECK(closed.load());
        if (response.find("X-Request-ID") != std::string::npos) {
            CHECK(result.error().request_id == "timeout");
            CHECK(result.error().status_code == 200);
            CHECK(result.error().body == "a");
        }
    }
}

DROGON_TEST(HttpExchangeReadTimeoutResetsOnProgress) {
    RawServer server{ { [](Socket const& socket) {
        socket.Request();
        socket.Send("HTTP/1.1 200 OK\r\nContent-Length: 8\r\n\r\n");
        for (int i{}; i < 8; ++i) {
            std::this_thread::sleep_for(40ms);
            socket.Send("x");
        }
    } } };
    auto request{ server.Request() };
    request.timeout.read = 0.2;
    auto const result{ drogon::sync_wait(wire::exchange(std::move(request))) };
    server.Check();
    REQUIRE(result);
    CHECK(result->body == "xxxxxxxx");
}

DROGON_TEST(HttpExchangeZeroReadTimeoutIsDisabled) {
    RawServer server{ { [](Socket const& socket) {
        socket.Request();
        std::this_thread::sleep_for(120ms);
        socket.Send("HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nok");
    } } };
    auto request{ server.Request() };
    request.timeout.read = 0;
    auto const result{ drogon::sync_wait(wire::exchange(std::move(request))) };
    server.Check();
    REQUIRE(result);
    CHECK(result->body == "ok");
}

DROGON_TEST(HttpExchangeTlsHandshakeUsesConnectTimeout) {
    RawServer server{ { [](Socket const&) { std::this_thread::sleep_for(300ms); } } };
    auto request{ server.Request() };
    request.endpoint.tls = true;
    request.timeout.connect = 0.08;
    auto const result{ drogon::sync_wait(wire::exchange(std::move(request))) };
    server.Check();
    REQUIRE(!result);
    CHECK(result.error().kind == ErrorKind::Timeout);
    CHECK(result.error().message.find("connect timeout") != std::string::npos);
}

DROGON_TEST(HttpExchangeWriteTimeoutWhenPeerDoesNotRead) {
    RawServer server{ { [](Socket const&) { std::this_thread::sleep_for(500ms); } } };
    auto request{ server.Request() };
    request.body = std::string(32 * 1024 * 1024, 'x');
    request.timeout.write = 0.08;
    auto const result{ drogon::sync_wait(wire::exchange(std::move(request))) };
    server.Check();
    REQUIRE(!result);
    CHECK(result.error().kind == ErrorKind::Timeout);
    CHECK(result.error().message.find("write timeout") != std::string::npos);
}

DROGON_TEST(HttpExchangeNewConnectionsAreConcurrent) {
    std::promise<void> both;
    auto ready{ both.get_future().share() };
    std::atomic<int> accepted{};
    auto script{ [&](Socket const& socket) {
        socket.Request();
        if (++accepted == 2) {
            both.set_value();
        }
        if (ready.wait_for(1s) != std::future_status::ready) {
            throw std::runtime_error{ "requests queued behind a shared connection" };
        }
        socket.Send("HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nok");
    } };
    RawServer server{ { script, script } };
    auto first{ std::async(std::launch::async, [&] { return drogon::sync_wait(wire::exchange(server.Request())); }) };
    auto second{ std::async(std::launch::async, [&] { return drogon::sync_wait(wire::exchange(server.Request())); }) };
    auto const result_one{ first.get() };
    auto const result_two{ second.get() };
    server.Check();
    REQUIRE(result_one);
    REQUIRE(result_two);
    CHECK(accepted.load() == 2);
}

DROGON_TEST(HttpExchangeReturnsToCallingLoop) {
    RawServer server{ { [](Socket const& socket) {
        socket.Request();
        socket.Send("HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nok");
    } } };
    trantor::EventLoopThread thread;
    thread.run();
    auto completion{ std::make_shared<std::promise<bool>>() };
    auto future{ completion->get_future() };
    auto request{ server.Request() };
    auto* const loop{ thread.getLoop() };
    request.on_data = [loop](std::string_view, Headers const&) -> Result<bool> {
        return loop == trantor::EventLoop::getEventLoopOfCurrentThread();
    };
    loop->queueInLoop([request = std::move(request), completion]() mutable {
        drogon::async_run([request = std::move(request), completion]() mutable {
            return check_loop(std::move(request), completion);
        });
    });
    REQUIRE(future.wait_for(3s) == std::future_status::ready);
    CHECK(future.get());
    server.Check();
}

DROGON_TEST(HttpExchangeRejectsUntrustedTlsCertificate) {
    auto context{ self_signed_context() };
    std::atomic<bool> http_received{};
    RawServer server{ { [&](Socket const& socket) {
        std::unique_ptr<SSL, decltype(&SSL_free)> tls{ SSL_new(context.get()), SSL_free };
        SSL_set_fd(tls.get(), static_cast<int>(socket.Get()));
        if (SSL_accept(tls.get()) == 1) {
            char bytes[1024];
            http_received = SSL_read(tls.get(), bytes, sizeof(bytes)) > 0;
        }
    } } };
    auto request{ server.Request() };
    request.endpoint.tls = true;
    auto const result{ drogon::sync_wait(wire::exchange(std::move(request))) };
    server.Check();
    REQUIRE(!result);
    CHECK(result.error().kind == ErrorKind::Connection);
    CHECK(result.error().message.find("TLS") != std::string::npos);
    CHECK(!http_received.load());
}

DROGON_TEST(HttpExchangeRejectsUnsafeLocalRequests) {
    for (auto const kind : { 0, 1, 2, 3, 4, 5 }) {
        wire::WireRequest request{
            .endpoint = *wire::parse_endpoint("http://127.0.0.1:1"),
            .method = "POST",
            .target = "/",
        };
        switch (kind) {
            case 0: request.method = "POST\r\nInjected: yes"; break;
            case 1: request.target = "/ HTTP/1.1\r\nInjected: yes"; break;
            case 2: request.headers["x-test"] = "a\r\nInjected: yes"; break;
            case 3: request.headers["bad name"] = "x"; break;
            case 4: request.timeout.read = std::numeric_limits<double>::infinity(); break;
            case 5: request.timeout.write = -1; break;
        }
        auto const result{ drogon::sync_wait(wire::exchange(std::move(request))) };
        REQUIRE(!result);
        CHECK(result.error().kind == ErrorKind::InvalidRequest);
    }
}

DROGON_TEST(HttpExchangeParsesEndpoints) {
    auto const plain{ wire::parse_endpoint("http://localhost:8080/v1///") };
    REQUIRE(plain);
    CHECK(plain->host == "localhost");
    CHECK(plain->authority == "localhost:8080");
    CHECK(plain->prefix == "/v1");
    CHECK(plain->port == 8080);
    CHECK(!plain->tls);
    auto const secure{ wire::parse_endpoint("https://[::1]/v1") };
    REQUIRE(secure);
    CHECK(secure->host == "::1");
    CHECK(secure->authority == "[::1]");
    CHECK(secure->port == 443);
    CHECK(secure->tls);
    for (auto const* invalid : { "ftp://localhost", "http://", "http://user@host", "http://a:0", "http://a:65536", "http://a:", "http://a?x=y", "http://a/#fragment", "http://a b/", "http://a/\t", "http://[invalid]/", "http://[::1]junk/", "http://a\\b/" }) {
        CHECK(!wire::parse_endpoint(invalid));
    }
}

/**
 * @brief Run the HTTP fixture tests with the main app loop available to non-loop callers.
 * @param argc Framework argument count.
 * @param argv Framework arguments.
 * @return Framework status.
 */
auto main(int argc, char** argv) -> int {
#ifdef _WIN32
    WSADATA data{};
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        return 1;
    }
#endif
    drogon::app().setDocumentRoot((std::filesystem::temp_directory_path() / "drogon-openai-http-tests").string());
    return test_support::run_tests_with_app(argc, argv);
}
