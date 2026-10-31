/**
 * @file test_websocket.cpp
 * @brief Verify WebSocket API routes, frame ownership, deadlines and loop affinity.
 */
#include <future>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <drogon/HttpFilter.h>
#include <drogon/WebSocketConnection.h>
#include <drogon/WebSocketController.h>
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>

#include <drogon_openai/openai_client.hpp>

#include "details/transport.hpp"
#include "support.hpp"

using namespace drogon_openai;

/**
 * @brief Keep the WebSocket handshake pending to test the connect deadline.
 */
class StalledHandshake : public drogon::HttpFilter<StalledHandshake, false> {
public:
    /**
     * @brief Delay this upgrade without blocking an event loop.
     * @param request Original upgrade request.
     * @param reject Unused rejection callback.
     * @param next Continue the handshake after the client deadline.
     */
    auto doFilter(drogon::HttpRequestPtr const& request, drogon::FilterCallback&& reject, drogon::FilterChainCallback&& next) -> void override {
        static_cast<void>(request);
        static_cast<void>(reject);
        drogon::app().getLoop()->runAfter(0.2, [next = std::move(next)] { next(); });
    }
};

/**
 * @brief Echo authenticated JSON and binary frames through the API WebSocket routes.
 */
class ResourceSocket : public drogon::WebSocketController<ResourceSocket> {
public:
    WS_PATH_LIST_BEGIN
    WS_ADD_PATH_VIA_REGEX("/socket/.*");
    WS_PATH_ADD("/stalled/realtime", "StalledHandshake");
    WS_PATH_LIST_END

    /**
     * @brief Echo frames or deliberately keep a receive pending.
     * @param connection Established socket.
     * @param bytes Client frame bytes.
     * @param type Frame type.
     */
    auto handleNewMessage(
        drogon::WebSocketConnectionPtr const& connection,
        std::string&& bytes,
        drogon::WebSocketMessageType const& type
    ) -> void override {
        if (type != drogon::WebSocketMessageType::Text && type != drogon::WebSocketMessageType::Binary) {
            return;
        }
        if (bytes == "quiet") {
            return;
        }
        if (bytes == "close") {
            connection->shutdown();
            return;
        }
        connection->send(bytes, type);
    }

    /**
     * @brief Report the handshake route and headers before accepting events.
     * @param request Original handshake request.
     * @param connection Established socket.
     */
    auto handleNewConnection(
        drogon::HttpRequestPtr const& request,
        drogon::WebSocketConnectionPtr const& connection
    ) -> void override {
        Json::Value event;
        event["type"] = "session.created";
        event["path"] = request->path();
        event["model"] = request->getParameter("model");
        event["call_id"] = request->getParameter("call_id");
        event["authorization"] = request->getHeader("authorization");
        event["organization"] = request->getHeader("openai-organization");
        event["project"] = request->getHeader("openai-project");
        event["extra"] = request->getHeader("x-extra");
        Json::StreamWriterBuilder writer;
        writer["indentation"] = "";
        connection->send(Json::writeString(writer, event));
    }

    /**
     * @brief Release a closed fixture connection.
     * @param connection Closed connection.
     */
    auto handleConnectionClosed(drogon::WebSocketConnectionPtr const& connection) -> void override { static_cast<void>(connection); }
};

namespace {
    /**
     * @brief Build a client whose socket routes share the fixture listener.
     * @return Owned client.
     */
    auto client() -> OpenAI {
        return OpenAI{ { .base_url = std::string{ test_support::SERVER_URL } + "/socket", .api_key = "socket-key", .model = "configured", .max_retries = 0, .default_headers = { { "X-Extra", "default" } }, .organization = "org", .project = "project" } };
    }

    /**
     * @brief Receive from a different event loop and preserve its continuation thread.
     * @param session Shared socket owned by the main loop.
     * @param result Result promise consumed by the non-loop test thread.
     * @return Coroutine completion.
     */
    auto receive_on_io(
        WebSocketSession session,
        std::shared_ptr<std::promise<Result<bool>>> result
    ) -> drogon::Task<> {
        auto const before{ std::this_thread::get_id() };
        auto event{ co_await session.Receive(1) };
        if (!event) {
            result->set_value(std::unexpected{ std::move(event.error()) });
        } else {
            result->set_value(before == std::this_thread::get_id() && (*event)["type"] == "response.create");
        }
        co_return;
    }

    /**
     * @brief Start two receivers together to verify exclusive receive ownership.
     * @param session Shared established socket.
     * @return First timeout and second invalid-request result.
     */
    auto receive_pair(WebSocketSession session) -> drogon::Task<std::tuple<Result<std::string>, Result<std::string>>> {
        co_return co_await drogon::when_all(session.ReceiveBytes(0.02), session.ReceiveBytes(0.02));
    }
} // namespace

DROGON_TEST(WebSocketRoutesAndHeadersMatchResources) {
    auto c{ client() };
    auto check{ [&](Result<WebSocketSession> result, std::string const& path, std::string const& model, std::string const& extra = "default") {
        REQUIRE(result);
        auto hello{ drogon::sync_wait(result->Receive(1)) };
        REQUIRE(hello);
        CHECK((*hello)["path"] == path);
        CHECK((*hello)["model"] == model);
        CHECK((*hello)["authorization"] == "Bearer socket-key");
        CHECK((*hello)["organization"] == "org");
        CHECK((*hello)["project"] == "project");
        CHECK((*hello)["extra"] == extra);
        CHECK(drogon::sync_wait(result->Close()));
    } };
    check(drogon::sync_wait(c.realtime.Connect("model /name", { .extra_headers = { { "x-extra", "request" } } })), "/socket/realtime", "model /name", "request");
    check(drogon::sync_wait(c.realtime.translations.Connect()), "/socket/realtime/translations", "configured");
    check(drogon::sync_wait(c.responses.Connect()), "/socket/responses", "");
    check(drogon::sync_wait(c.live.Connect()), "/socket/live/sessions", "");
    check(drogon::sync_wait(c.live.sideband.Connect("session-1")), "/socket/live/sessions/session-1/attach", "");
    check(drogon::sync_wait(c.live.forks.Connect("session-1")), "/socket/live/sessions/session-1/fork", "");
    auto empty{ drogon::sync_wait(c.live.sideband.Connect("")) };
    REQUIRE(!empty);
    CHECK(empty.error().kind == ErrorKind::InvalidRequest);
    Json::Value query;
    query["call_id"] = "call /id";
    auto attached{ drogon::sync_wait(c.realtime.Connect("", { .extra_query = query })) };
    REQUIRE(attached);
    auto hello{ drogon::sync_wait(attached->Receive(1)) };
    REQUIRE(hello);
    CHECK((*hello)["call_id"] == "call /id");
    CHECK((*hello)["model"] == "");
    REQUIRE(drogon::sync_wait(attached->Close()));
    query["model"] = "override";
    check(drogon::sync_wait(c.realtime.Connect("named", { .extra_query = query })), "/socket/realtime", "override");
    auto invalid{ drogon::sync_wait(c.realtime.Connect("named", { .extra_query = "invalid" })) };
    REQUIRE(!invalid);
    CHECK(invalid.error().kind == ErrorKind::InvalidRequest);
}

DROGON_TEST(WebSocketSessionOwnsFramesAndPreservesAwaitingLoop) {
    auto connection{ drogon::sync_wait(client().realtime.Connect()) };
    REQUIRE(connection);
    REQUIRE(drogon::sync_wait(connection->Receive(1)));
    Json::Value event;
    event["type"] = "response.create";
    event["response"]["input"] = "世界";
    REQUIRE(drogon::sync_wait(connection->Send(event)));
    auto promise{ std::make_shared<std::promise<Result<bool>>>() };
    auto future{ promise->get_future() };
    drogon::app().getIOLoop(0)->queueInLoop([session = *connection, promise] {
        drogon::async_run([session, promise]() -> drogon::Task<> { co_await receive_on_io(session, promise); });
    });
    auto affinity{ future.get() };
    REQUIRE(affinity);
    CHECK(*affinity);
    std::string const bytes{ "binary\0data", 11 };
    REQUIRE(drogon::sync_wait(connection->SendBytes(bytes)));
    auto echoed{ drogon::sync_wait(connection->ReceiveBytes(1)) };
    REQUIRE(echoed);
    CHECK(*echoed == bytes);
    REQUIRE(drogon::sync_wait(connection->SendBytes("not-json")));
    auto invalid{ drogon::sync_wait(connection->Receive(1)) };
    REQUIRE(!invalid);
    CHECK(invalid.error().kind == ErrorKind::ResponseValidation);
    REQUIRE(drogon::sync_wait(connection->Close()));
    auto closed{ drogon::sync_wait(connection->Receive(1)) };
    REQUIRE(!closed);
    CHECK(closed.error().kind == ErrorKind::Connection);
}

DROGON_TEST(WebSocketReceiveTimeoutAndConcurrentReceiverAreExplicit) {
    auto connection{ drogon::sync_wait(client().responses.Connect()) };
    REQUIRE(connection);
    REQUIRE(drogon::sync_wait(connection->Receive(1)));
    auto timeout{ drogon::sync_wait(connection->Receive(0.02)) };
    REQUIRE(!timeout);
    CHECK(timeout.error().kind == ErrorKind::Timeout);
    auto pair{ drogon::sync_wait(receive_pair(*connection)) };
    REQUIRE(!std::get<0>(pair));
    REQUIRE(!std::get<1>(pair));
    CHECK(std::get<0>(pair).error().kind == ErrorKind::Timeout);
    CHECK(std::get<1>(pair).error().kind == ErrorKind::InvalidRequest);
    REQUIRE(drogon::sync_wait(connection->SendBytes("close")));
    auto closed{ drogon::sync_wait(connection->Receive(1)) };
    REQUIRE(!closed);
    CHECK(closed.error().kind == ErrorKind::Connection);
    CHECK(drogon::sync_wait(connection->Close()));
}

DROGON_TEST(WebSocketHandshakeFailuresRespectDeadline) {
    auto c{ client().WithOptions({ .base_url = std::string{ test_support::SERVER_URL } + "/missing" }) };
    auto missing{ drogon::sync_wait(c.responses.Connect()) };
    REQUIRE(!missing);
    // Drogon 1.9.13 discards non-101 handshake responses in WebSocketClientImpl.cc.
    CHECK(missing.error().kind == ErrorKind::Connection);
    c = client().WithOptions({ .base_url = std::string{ test_support::SERVER_URL } + "/stalled" });
    auto stalled{ drogon::sync_wait(c.realtime.Connect("", { .timeout = Timeout{ 0.02 } })) };
    REQUIRE(!stalled);
    CHECK(stalled.error().kind == ErrorKind::Timeout);
    auto invalid{ drogon::sync_wait(client().realtime.Connect("", { .extra_headers = { { "x-extra", "bad\r\nvalue" } } })) };
    REQUIRE(!invalid);
    CHECK(invalid.error().kind == ErrorKind::InvalidRequest);
}

/**
 * @brief Run WebSocket tests on the shared fixture listener.
 * @param argc Framework argument count.
 * @param argv Framework options.
 * @return Test status.
 */
auto main(int argc, char** argv) -> int {
    test_support::set_env(test_support::API_KEY_ENV, test_support::API_KEY);
    drogon::app().loadConfigFile("config.json");
    drogon::app().registerFilter(std::make_shared<StalledHandshake>());
    return test_support::run_tests_with_app(argc, argv);
}
