/**
 * @file test_chat_features.cpp
 * @brief Verify incremental chat events, structured output, and stored completion pagination.
 */
#include <atomic>
#include <chrono>
#include <set>
#include <sstream>
#include <stdexcept>
#include <thread>

#include <drogon/drogon.h>
#include <drogon/drogon_test.h>

#include <drogon_openai/openai_client.hpp>

#include "details/chat_stream.hpp"
#include "details/transport.hpp"
#include "support.hpp"

using namespace drogon_openai;
namespace detail = drogon_openai::details;

namespace {
    std::atomic<bool> g_stream_finished{}; ///< Server's final chunk has been sent.
    std::atomic<int> g_stream_requests{};  ///< Streams requested, including any unwanted retries.

    /**
     * @brief Decode fixture JSON.
     * @param text JSON fixture.
     * @return Parsed value.
     */
    auto json(std::string const& text) -> Json::Value {
        return detail::decode_json(text).value();
    }

    /**
     * @brief Return a fixture completion with one structured reply and strict function call.
     * @param id Completion ID.
     * @return Response object.
     */
    auto completion(std::string const& id = "id-1") -> Json::Value {
        auto root{ json(R"JSON({"id":"id-1","object":"chat.completion","created":1,"model":"fake","choices":[{"index":0,"finish_reason":"stop","message":{"role":"assistant","content":"{\"answer\":42}","tool_calls":[{"id":"call-1","type":"function","function":{"name":"weather","arguments":"{\"city\":\"Paris\"}"}}]}}]})JSON") };
        root["id"] = id;
        return root;
    }

    /**
     * @brief Return the local fixture client.
     * @return Client with normal retry defaults.
     */
    auto client() -> OpenAI {
        return OpenAI{ { .base_url = std::string{ test_support::SERVER_URL } + "/features", .model = "fake" } };
    }

    /**
     * @brief Build one SSE chat chunk.
     * @param delta Message delta.
     * @param finish Whether this is the final candidate delta.
     * @return Encoded SSE event.
     */
    auto chunk(Json::Value delta, bool finish = false) -> std::string {
        Json::Value root;
        root["id"] = "stream-id";
        root["object"] = "chat.completion.chunk";
        root["model"] = "fake";
        root["choices"][0]["index"] = 0;
        root["choices"][0]["delta"] = std::move(delta);
        root["choices"][0]["finish_reason"] = finish ? Json::Value{ "stop" } : Json::Value{};
        Json::StreamWriterBuilder writer;
        writer["indentation"] = "";
        return "data: " + Json::writeString(writer, root) + "\n\n";
    }

    /**
     * @brief Register streamed and stored chat fixtures.
     */
    auto register_fixtures() -> void {
        drogon::app().registerHandler("/features/chat/completions", [](drogon::HttpRequestPtr const& req, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            if (req->method() == drogon::Get) {
                Json::Value root;
                root["object"] = "list";
                bool const first{ req->getParameter("after").empty() };
                root["data"].append(completion(first ? "id-1" : "id-2"));
                root["has_more"] = first;
                root["seen_query"] = req->getParameter("model");
                root["seen_header"] = req->getHeader("x-pages");
                auto response{ drogon::HttpResponse::newHttpJsonResponse(root) };
                response->addHeader("x-request-id", first ? "page-1" : "page-2");
                callback(response);
                return;
            }
            auto const body{ req->getJsonObject() };
            std::string const scenario{ body->get("scenario", "").asString() };
            if ((*body)["stream"] == true) {
                ++g_stream_requests;
                g_stream_finished = false;
                auto response{ drogon::HttpResponse::newAsyncStreamResponse([scenario](drogon::ResponseStreamPtr stream) {
                    auto owned{ std::shared_ptr<drogon::ResponseStream>{ std::move(stream) } };
                    owned->send(chunk(json(R"JSON({"role":"assistant","content":"Hello ","reasoning_content":"think"})JSON")));
                    drogon::app().getLoop()->runAfter(0.12, [owned, scenario] {
                        if (scenario == "truncated" || scenario == "timeout") {
                            owned->close();
                            return;
                        }
                        owned->send(chunk(json(R"JSON({"content":"世界","reasoning_content":"ing"})JSON")));
                        owned->send(chunk(Json::Value{ Json::objectValue }, true));
                        owned->send("data: {\"id\":\"stream-id\",\"choices\":[],\"usage\":{\"prompt_tokens\":2,\"completion_tokens\":3,\"total_tokens\":5}}\n\n");
                        g_stream_finished = true;
                        owned->send("data: [DONE]\n\n");
                        owned->close();
                    });
                }) };
                response->setContentTypeString("text/event-stream");
                response->addHeader("x-request-id", "req-stream");
                callback(response);
                return;
            }
            auto root{ completion() };
            if (scenario == "length" || scenario == "content_filter") {
                root["choices"][0]["finish_reason"] = scenario;
            }
            if (scenario == "bad") {
                root["choices"][0]["message"]["content"] = "{bad";
            }
            auto response{ drogon::HttpResponse::newHttpJsonResponse(root) };
            response->addHeader("x-request-id", "req-create");
            callback(response);
        },
                                      { drogon::Get, drogon::Post });
        drogon::app().registerHandler("/features/chat/completions/{id}", [](drogon::HttpRequestPtr const& req, std::function<void(drogon::HttpResponsePtr const&)>&& callback, std::string id) {
            auto root{ completion(id) };
            if (req->method() == drogon::Delete) {
                root = Json::objectValue;
                root["id"] = id;
                root["object"] = "chat.completion.deleted";
                root["deleted"] = true;
            } else if (req->method() == drogon::Post) {
                root["metadata"] = (*req->getJsonObject())["metadata"];
            }
            callback(drogon::HttpResponse::newHttpJsonResponse(root));
        },
                                      { drogon::Get, drogon::Post, drogon::Delete });
        drogon::app().registerHandler("/features/chat/completions/{id}/messages", [](drogon::HttpRequestPtr const&, std::function<void(drogon::HttpResponsePtr const&)>&& callback, std::string) {
            callback(drogon::HttpResponse::newHttpJsonResponse(json(R"JSON({"data":[{"id":"msg-1","role":"user","content":"hello","content_parts":[{"type":"text","text":"hello"}]}],"has_more":false})JSON")));
        });
    }
} // namespace

DROGON_TEST(ChatRawStructuredAndStoredMethods) {
    auto api{ client() };
    auto raw{ drogon::sync_wait(api.chat.completions.CreateRaw({ .messages = { { .role = "user", .content = "hi" } } })) };
    REQUIRE(raw);
    CHECK(raw->headers.at("x-request-id") == "req-create");
    CHECK(raw->parsed._request_id == "req-create");
    ChatCompletionCreateParams params;
    params.response_format = json(R"JSON({"type":"json_schema","json_schema":{"name":"answer","schema":{"type":"object"}}})JSON");
    params.tools = std::vector<ChatCompletionToolParam>{ { .function = json(R"JSON({"name":"weather","strict":true})JSON") } };
    auto parsed{ drogon::sync_wait(api.chat.completions.Parse(params)) };
    REQUIRE(parsed);
    CHECK((*parsed->choices[0].message.parsed)["answer"] == 42);
    CHECK((*parsed->choices[0].message.tool_calls->at(0).function.parsed_arguments)["city"] == "Paris");
    for (auto const* scenario : { "length", "content_filter", "bad" }) {
        params.extra_body["scenario"] = scenario;
        auto failure{ drogon::sync_wait(api.chat.completions.Parse(params)) };
        REQUIRE(!failure);
        CHECK(failure.error().request_id == "req-create");
        CHECK(failure.error().kind == (std::string{ scenario } == "length" ? ErrorKind::LengthFinishReason : std::string{ scenario } == "content_filter" ? ErrorKind::ContentFilterFinishReason :
                                                                                                                                                           ErrorKind::ResponseValidation));
    }
    params.tools->at(0).function["strict"] = false;
    auto rejected{ drogon::sync_wait(api.chat.completions.Parse(params)) };
    REQUIRE(!rejected);
    CHECK(rejected.error().kind == ErrorKind::InvalidRequest);
    auto retrieved{ drogon::sync_wait(api.chat.completions.Retrieve("id-1")) };
    REQUIRE(retrieved);
    CHECK(retrieved->id == "id-1");
    auto updated{ drogon::sync_wait(api.chat.completions.Update("id-1", { .metadata = json(R"JSON({"tag":"updated"})JSON") })) };
    REQUIRE(updated);
    CHECK((*updated->metadata)["tag"] == "updated");
    auto deleted{ drogon::sync_wait(api.chat.completions.Delete("id-1")) };
    REQUIRE(deleted);
    CHECK(deleted->deleted);
    auto messages{ drogon::sync_wait(api.chat.completions.messages.List("id-1")) };
    REQUIRE(messages);
    CHECK(messages->data[0].role == "user");
    CHECK(messages->data[0].content_parts->size() == 1);
    CHECK(!messages->HasNextPage());
}

DROGON_TEST(PaginationOwnsContinuationAndRetainsOverrides) {
    auto page{ drogon::sync_wait(client().chat.completions.List({ .model = "fake" }, { .extra_headers = { { "x-pages", "retained" } } })) };
    REQUIRE(page);
    CHECK(page->HasNextPage());
    CHECK(page->data[0]._request_id == "page-1");
    auto next{ drogon::sync_wait(page->GetNextPage()) };
    REQUIRE(next);
    CHECK(next->data[0].id == "id-2");
    CHECK(next->raw["seen_query"] == "fake");
    CHECK(next->raw["seen_header"] == "retained");
    CHECK(!next->HasNextPage());
    auto all{ drogon::sync_wait(page->All()) };
    REQUIRE(all);
    CHECK(all->size() == 2);
    auto callback_failure{ drogon::sync_wait(page->ForEach([](auto const&) -> Result<bool> { throw std::runtime_error{ "consumer failed" }; })) };
    REQUIRE(!callback_failure);
    CHECK(callback_failure.error().kind == ErrorKind::InvalidRequest);
    auto generic{ drogon::sync_wait(client().List("chat/completions")) };
    REQUIRE(generic);
    auto generic_all{ drogon::sync_wait(generic->All()) };
    REQUIRE(generic_all);
    CHECK(generic_all->size() == 2);
    auto single{ drogon::sync_wait(client().List("chat/completions", {}, false)) };
    REQUIRE(single);
    CHECK(!single->HasNextPage());
}

DROGON_TEST(StreamDeliversIncrementallyAndReturnsFinalCompletion) {
    std::set<std::string> events;
    bool first_before_end{};
    bool callback_on_loop{};
    auto result{ drogon::sync_wait(client().chat.completions.Stream({}, [&](auto const& value) -> Result<bool> {
        if (!first_before_end){ first_before_end = !g_stream_finished.load();
}
        callback_on_loop = trantor::EventLoop::getEventLoopOfCurrentThread() == drogon::app().getLoop();
        return true; }, [&](auto const& event) -> Result<bool> { events.insert(event.type); return true; })) };
    REQUIRE(result);
    CHECK(first_before_end);
    CHECK(callback_on_loop);
    CHECK(result->choices[0].message.content == "Hello 世界");
    CHECK(result->choices[0].message.reasoning_content == "thinking");
    CHECK(result->usage->total_tokens == 5);
    CHECK(result->_request_id == "req-stream");
    CHECK(events.contains("content.delta"));
    CHECK(events.contains("content.done"));
    CHECK(events.contains("reasoning_content.done"));
    int const requests{ g_stream_requests.load() };
    ChatCompletionCreateParams params;
    params.extra_body["scenario"] = "timeout";
    params.timeout = Timeout{ 0.03 };
    auto failed{ drogon::sync_wait(client().chat.completions.Stream(params)) };
    REQUIRE(!failed);
    CHECK(failed.error().kind == ErrorKind::Timeout);
    CHECK(g_stream_requests == requests + 1);
    params.timeout = Timeout{ 1.0 };
    params.extra_body["scenario"] = "truncated";
    failed = drogon::sync_wait(client().chat.completions.Stream(params));
    REQUIRE(!failed);
    CHECK(failed.error().kind == ErrorKind::ResponseValidation);
    auto cancelled{ drogon::sync_wait(client().chat.completions.Stream({}, [](auto const&) -> Result<bool> { return false; })) };
    REQUIRE(cancelled);
    CHECK(cancelled->choices[0].message.content == "Hello ");
    failed = drogon::sync_wait(client().chat.completions.Stream({}, [](auto const&) -> Result<bool> { throw std::runtime_error{ "callback" }; }));
    REQUIRE(!failed);
    CHECK(failed.error().kind == ErrorKind::InvalidRequest);
}

DROGON_TEST(SseFramingToolsRefusalLogprobsAndErrors) {
    std::set<std::string> types;
    detail::ChatStream stream{ {}, {}, [&](auto const& event) -> Result<bool> { types.insert(event.type); return true; } };
    auto first{ json(R"JSON({"role":"assistant","refusal":"No ","tool_calls":[{"index":2,"id":"f","type":"function","function":{"name":"wea","arguments":"{\"x\":"}},{"index":0,"id":"c","type":"custom","custom":{"name":"code","input":"print("}}]})JSON") };
    std::string bytes{ "\xef\xbb\xbf: keepalive\r\n\r\n" + chunk(first) };
    for (char byte : bytes) {
        REQUIRE(stream.Feed(std::string_view{ &byte, 1 }, {}));
    }
    auto second{ json(R"JSON({"refusal":"thanks","tool_calls":[{"index":0,"custom":{"input":"1)"}},{"index":2,"function":{"name":"ther","arguments":"1}"}}]})JSON") };
    REQUIRE(stream.Feed(chunk(second), {}));
    auto final{ json(R"JSON({"choices":[{"index":0,"delta":{},"finish_reason":"tool_calls","logprobs":{"content":[{"token":"x","logprob":-1,"bytes":[120],"top_logprobs":[]}],"refusal":[{"token":"no","logprob":-2,"bytes":null,"top_logprobs":[]}]}}]})JSON") };
    std::istringstream lines{ final.toStyledString() };
    std::string event;
    for (std::string line; std::getline(lines, line);) {
        event += "data: " + line + "\r";
    }
    REQUIRE(stream.Feed(event + "\r", {}));
    auto done{ stream.Feed("data: [DONE]\n\n", {}) };
    REQUIRE(done.has_value());
    CHECK(!*done);
    auto result{ stream.Finish() };
    REQUIRE(result);
    CHECK(result->choices[0].message.refusal == "No thanks");
    REQUIRE(result->choices[0].message.tool_calls);
    CHECK(result->choices[0].message.tool_calls->at(0).custom.input == "print(1)");
    CHECK(result->choices[0].message.tool_calls->at(1).function.name == "weather");
    CHECK(result->choices[0].message.tool_calls->at(1).function.arguments == "{\"x\":1}");
    CHECK(types.contains("tool_calls.function.arguments.delta"));
    CHECK(types.contains("tool_calls.function.arguments.done"));
    CHECK(types.contains("tool_calls.custom.input.done"));
    CHECK(types.contains("refusal.done"));
    CHECK(types.contains("logprobs.content.done"));
    CHECK(types.contains("logprobs.refusal.done"));
    detail::ChatStream error_stream{ {}, {}, {} };
    auto error{ error_stream.Feed("event: error\ndata: {\"error\":{\"message\":\"quota\",\"code\":\"limit\"}}\n\n", {}) };
    REQUIRE(!error);
    CHECK(error.error().message == "quota");
    CHECK(error.error().code == "limit");
    detail::ChatStream invalid{ {}, {}, {} };
    CHECK(!invalid.Feed("data: {invalid}\n\n", {}));
    detail::ChatStream truncated{ {}, {}, {} };
    REQUIRE(truncated.Feed(chunk(Json::Value{ Json::objectValue }), {}));
    CHECK(!truncated.Finish());
    detail::ChatStream invalid_format{ { .response_format = "invalid" }, {}, {} };
    REQUIRE(invalid_format.Feed(chunk(Json::Value{ Json::objectValue }, true) + "data: [DONE]\n\n", {}));
    auto invalid_result{ invalid_format.Finish() };
    REQUIRE(!invalid_result);
    CHECK(invalid_result.error().kind == ErrorKind::InvalidRequest);
    detail::ChatStream malformed_tool{ { .tools = std::vector<ChatCompletionToolParam>{ { .function = "invalid" } } }, {}, {} };
    REQUIRE(malformed_tool.Feed(chunk(Json::Value{ Json::objectValue }, true) + "data: [DONE]\n\n", {}));
    CHECK(malformed_tool.Finish());
}

/**
 * @brief Run feature tests with the fixture app's event loops.
 * @param argc Framework argument count.
 * @param argv Framework options.
 * @return Test status.
 */
auto main(int argc, char** argv) -> int {
    test_support::set_env(test_support::API_KEY_ENV, test_support::API_KEY);
    drogon::app().loadConfigFile("config.json");
    register_fixtures();
    return test_support::run_tests_with_app(argc, argv);
}
