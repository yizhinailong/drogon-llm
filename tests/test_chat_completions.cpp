/**
 * @file test_chat_completions.cpp
 * @brief Call `chat.completions.Create` on a fake OpenAI-format API that the same app serves.
 *
 * Covers the request body, `Authorization`, the `base_url` path, response parsing, every
 * `ErrorKind`, concurrent requests, and the thread that the coroutine resumes on. Runs offline.
 */
#include <chrono>
#include <cstdint>
#include <format>
#include <functional>
#include <future>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include <drogon/utils/coroutine.h>

#include <drogon_openai/openai_client.hpp>

#include "support.hpp"

namespace {

    constexpr std::int64_t CREATED{ 1700000000 };              ///< `created` of the fake completions.
    constexpr char const* REPLY{ "Hello from the fake API." }; ///< `content` of the fake completions.
    constexpr char const* REASONING{ "The user greets me." };  ///< `reasoning_content` of the fake completions.
    constexpr double SLOW_DELAY{ 1.0 };                        ///< Seconds that the fake `/slow` endpoint takes.

    /**
     * @brief Request as the fake API received it.
     */
    struct ReceivedRequest {
        std::string path;          ///< Request path.
        std::string authorization; ///< `Authorization` header; empty when absent.
        Json::Value body;          ///< Parsed JSON body; null when it was not JSON.
    };

    /**
     * @brief Hold the last request that the fake API received.
     *
     * The fake endpoints record each request on the IO thread before answering it, so a
     * test that reads after `drogon::sync_wait()` returns sees its own request.
     */
    class LastRequest {
    private:
        std::mutex m_mutex;        ///< Guards `m_request`.
        ReceivedRequest m_request; ///< Latest request.

    public:
        /**
         * @brief Replace the stored request.
         * @param req Request that a fake endpoint received.
         */
        auto Record(drogon::HttpRequestPtr const& req) -> void {
            auto const& json{ req->getJsonObject() };
            std::lock_guard lock{ m_mutex };
            m_request = ReceivedRequest{
                .path = req->path(),
                .authorization = req->getHeader("authorization"),
                .body = json != nullptr ? *json : Json::Value{},
            };
        }

        /**
         * @brief Copy the stored request.
         * @return The latest request.
         */
        [[nodiscard]] auto Get() -> ReceivedRequest {
            std::lock_guard lock{ m_mutex };
            return m_request;
        }
    };

    LastRequest g_last_request{}; ///< Filled by every fake chat completions endpoint.

    /**
     * @brief Build an OpenAI-format chat completion shaped like DeepSeek's.
     * @param id Completion ID, which tells the tests which endpoint answered.
     * @param model Model to report; the fake endpoints echo the requested one.
     * @return Completion with `usage` and one choice that has `content` and `reasoning_content`.
     */
    auto completion_json(std::string const& id, Json::Value const& model) -> Json::Value {
        Json::Value message{ Json::objectValue };
        message["role"] = "assistant";
        message["content"] = REPLY;
        message["reasoning_content"] = REASONING;

        Json::Value choice{ Json::objectValue };
        choice["index"] = 0;
        choice["message"] = message;
        choice["finish_reason"] = "stop";

        Json::Value usage{ Json::objectValue };
        usage["prompt_tokens"] = 5;
        usage["completion_tokens"] = 7;
        usage["total_tokens"] = 12;

        Json::Value root{ Json::objectValue };
        root["id"] = id;
        root["object"] = "chat.completion";
        root["created"] = CREATED;
        root["model"] = model;
        root["choices"].append(choice);
        root["usage"] = usage;
        return root;
    }

    /**
     * @brief Read the `model` that a request asked for.
     * @param req Request to a fake endpoint.
     * @return The body's `model`, or null when the body is not JSON.
     */
    auto requested_model(drogon::HttpRequestPtr const& req) -> Json::Value {
        auto const& json{ req->getJsonObject() };
        return json != nullptr ? (*json)["model"] : Json::Value{};
    }

    /**
     * @brief Build a JSON response with a given status.
     * @param status HTTP status.
     * @param body Response body.
     * @return The response.
     */
    auto json_response(drogon::HttpStatusCode status, Json::Value const& body) -> drogon::HttpResponsePtr {
        auto response{ drogon::HttpResponse::newHttpJsonResponse(body) };
        response->setStatusCode(status);
        return response;
    }

    /**
     * @brief Build a plain-text response with a given status.
     * @param status HTTP status.
     * @param body Response body.
     * @return The response.
     */
    auto text_response(drogon::HttpStatusCode status, std::string body) -> drogon::HttpResponsePtr {
        auto response{ drogon::HttpResponse::newHttpResponse(status, drogon::CT_TEXT_PLAIN) };
        response->setBody(std::move(body));
        return response;
    }

    /**
     * @brief Build the one-message request that most tests send.
     * @return Params with the user message `Hello!` and every optional field unset.
     */
    auto hello() -> drogon_openai::ChatCompletionCreateParams {
        return { .messages = { { .role = "user", .content = "Hello!" } } };
    }

    /**
     * @brief Return the client that the plugin builds from config.json.
     * @return Client for `SERVER_URL` + `BASE_PATH` with the test key and `MODEL`.
     */
    auto plugin_client() -> drogon_openai::OpenAI {
        return drogon::app().getPlugin<drogon_openai::OpenAIClient>()->NewOpenAIClient();
    }

    /**
     * @brief Build a client for a path under the fake API, bypassing the plugin.
     * @param path_prefix Path appended to `SERVER_URL`, e.g. `/minimal`.
     * @param api_key Key to send; empty sends no `Authorization` header.
     * @return Client for `SERVER_URL + path_prefix` with `MODEL`.
     */
    auto client_at(
        std::string const& path_prefix,
        std::string api_key = {}
    ) -> drogon_openai::OpenAI {
        return drogon_openai::OpenAI{ drogon_openai::OpenAIConfig{
            .base_url = test_support::SERVER_URL + path_prefix,
            .api_key = std::move(api_key),
            .model = test_support::MODEL,
        } };
    }

    /**
     * @brief POST an empty request to the app's listener and wait for the answer.
     *
     * Blocks on `HttpClient::sendRequest`, which is safe on the test thread because the
     * client runs on the app's main loop.
     * @param path Request path.
     * @return The response.
     * @throws std::runtime_error If no response arrives.
     */
    auto post(std::string const& path) -> drogon::HttpResponsePtr {
        auto request{ drogon::HttpRequest::newHttpRequest() };
        request->setMethod(drogon::Post);
        request->setPath(path);
        auto const client{ drogon::HttpClient::newHttpClient(test_support::SERVER_URL) };
        auto const [result, response]{ client->sendRequest(request, 5.0) };
        if (result != drogon::ReqResult::Ok) {
            throw std::runtime_error{ std::format("POST {}: {}", path, drogon::to_string_view(result)) };
        }
        return response;
    }

    /**
     * @brief Serve `make(req)` at `POST path` on the app's listener, recording each request.
     * @param path Request path, e.g. `/v1/chat/completions`.
     * @param make Builds the response for each request.
     */
    auto serve(
        std::string const& path,
        std::function<drogon::HttpResponsePtr(drogon::HttpRequestPtr const&)> make
    ) -> void {
        drogon::app().registerHandler(
            path,
            [make = std::move(make)](
                drogon::HttpRequestPtr const& req,
                std::function<void(drogon::HttpResponsePtr const&)>&& callback
            ) {
                g_last_request.Record(req);
                callback(make(req));
            },
            { drogon::Post }
        );
    }

    /**
     * @brief Register the fake OpenAI-format API on the app's listener.
     */
    auto serve_fake_openai() -> void {
        serve("/v1/chat/completions", [](drogon::HttpRequestPtr const& req) {
            return drogon::HttpResponse::newHttpJsonResponse(completion_json("chatcmpl-v1", requested_model(req)));
        });
        serve("/chat/completions", [](drogon::HttpRequestPtr const& req) {
            return drogon::HttpResponse::newHttpJsonResponse(completion_json("chatcmpl-root", requested_model(req)));
        });
        serve("/minimal/chat/completions", [](auto const&) {
            Json::Value message{ Json::objectValue };
            message["role"] = "assistant";
            message["content"] = Json::Value{};
            Json::Value choice{ Json::objectValue };
            choice["message"] = message;
            Json::Value root{ Json::objectValue };
            root["choices"].append(choice);
            return drogon::HttpResponse::newHttpJsonResponse(root);
        });
        serve("/unauthorized/chat/completions", [](auto const&) {
            Json::Value root{ Json::objectValue };
            root["error"]["message"] = "Authentication Fails, Your api key: ****-key is invalid";
            root["error"]["type"] = "authentication_error";
            root["error"]["code"] = "invalid_request_error";
            return json_response(drogon::k401Unauthorized, root);
        });
        serve("/overloaded/chat/completions", [](auto const&) {
            return text_response(drogon::k503ServiceUnavailable, "upstream overloaded");
        });
        serve("/not_json/chat/completions", [](auto const&) {
            return text_response(drogon::k200OK, "<html>gateway</html>");
        });
        serve("/no_choices/chat/completions", [](auto const&) {
            Json::Value root{ Json::objectValue };
            root["id"] = "chatcmpl-empty";
            root["object"] = "chat.completion";
            return drogon::HttpResponse::newHttpJsonResponse(root);
        });
        serve("/bad_choice/chat/completions", [](auto const&) {
            Json::Value choice{ Json::objectValue };
            choice["index"] = 0;
            choice["finish_reason"] = "stop";
            Json::Value root{ Json::objectValue };
            root["choices"].append(choice);
            return drogon::HttpResponse::newHttpJsonResponse(root);
        });
        drogon::app().registerHandler(
            "/slow/chat/completions",
            [](drogon::HttpRequestPtr req) -> drogon::Task<drogon::HttpResponsePtr> {
                g_last_request.Record(req);
                co_await drogon::sleepCoro(trantor::EventLoop::getEventLoopOfCurrentThread(), SLOW_DELAY);
                co_return drogon::HttpResponse::newHttpJsonResponse(completion_json("chatcmpl-slow", requested_model(req)));
            },
            { drogon::Post }
        );
        // Awaits Create from an IO thread, as an app's handler would, and reports whether
        // the coroutine came back on that thread.
        drogon::app().registerHandler(
            "/resume_thread",
            [](drogon::HttpRequestPtr /*req*/) -> drogon::Task<drogon::HttpResponsePtr> {
                auto const client{ client_at(test_support::BASE_PATH) };
                auto const before{ std::this_thread::get_id() };
                auto const completion{ co_await client.chat.completions.Create(hello()) };
                Json::Value root{ Json::objectValue };
                root["ok"] = completion.has_value();
                root["same_thread"] = std::this_thread::get_id() == before;
                co_return drogon::HttpResponse::newHttpJsonResponse(root);
            },
            { drogon::Post }
        );
    }

} // namespace

DROGON_TEST(CreateParsesCompletion) {
    auto const completion{ drogon::sync_wait(plugin_client().chat.completions.Create(hello())) };
    REQUIRE(completion.has_value());
    CHECK(completion->id == "chatcmpl-v1");
    CHECK(completion->object == "chat.completion");
    CHECK(completion->created == CREATED);
    CHECK(completion->model == test_support::MODEL);

    REQUIRE(completion->choices.size() == 1);
    auto const& choice{ completion->choices.front() };
    CHECK(choice.index == 0);
    CHECK(choice.finish_reason == "stop");
    CHECK(choice.message.role == "assistant");
    CHECK(choice.message.content == REPLY);
    CHECK(choice.message.reasoning_content == REASONING);

    REQUIRE(completion->usage.has_value());
    CHECK(completion->usage->prompt_tokens == 5);
    CHECK(completion->usage->completion_tokens == 7);
    CHECK(completion->usage->total_tokens == 12);
}

DROGON_TEST(CreateSendsConfiguredModelAndMessages) {
    auto const completion{ drogon::sync_wait(plugin_client().chat.completions.Create({
        .messages = {
            { .role = "system", .content = "Be brief." },
            { .role = "user", .content = "Hello!" },
        },
    })) };
    REQUIRE(completion.has_value());

    auto const request{ g_last_request.Get() };
    CHECK(request.path == "/v1/chat/completions");
    CHECK(request.authorization == std::string{ "Bearer " } + test_support::API_KEY);
    CHECK(request.body["model"].asString() == test_support::MODEL);
    REQUIRE(request.body["messages"].size() == 2);
    CHECK(request.body["messages"][0]["role"].asString() == "system");
    CHECK(request.body["messages"][0]["content"].asString() == "Be brief.");
    CHECK(request.body["messages"][1]["role"].asString() == "user");
    CHECK(request.body["messages"][1]["content"].asString() == "Hello!");

    // Unset optional fields stay out of the body, so the endpoint's defaults apply.
    std::vector<std::string> const keys{ "messages", "model" };
    CHECK(request.body.getMemberNames() == keys);
}

DROGON_TEST(CreateSendsSetOptionalFields) {
    drogon_openai::ChatCompletionCreateParams params{
        .messages = { { .role = "user", .content = "Hello!" } },
        .model = "other-model",
        .frequency_penalty = 0.5,
        .max_tokens = 64,
        .presence_penalty = -0.5,
        .temperature = 0.25,
        .top_p = 0.75,
    };
    Json::Value object{ Json::objectValue };
    object["name"] = "test";
    params.tools = std::vector<drogon_openai::ChatCompletionToolParam>{ { .function = object }, { .type = "custom", .custom = object } };
    params.tool_choice = "auto";
    params.parallel_tool_calls = false;
    params.response_format = object;
    params.stop = "STOP";
    params.max_completion_tokens = 100;
    params.n = 2;
    params.seed = 1;
    params.logprobs = true;
    params.top_logprobs = 2;
    params.logit_bias = object;
    params.reasoning_effort = "high";
    params.verbosity = "low";
    params.safety_identifier = "test-user";
    params.prompt_cache_key = "test-cache";
    params.user = "test-user";
    params.metadata = object;
    params.store = true;
    params.service_tier = "priority";
    params.modalities = std::vector<std::string>{ "text", "audio" };
    params.audio = object;
    params.prediction = object;
    params.web_search_options = object;
    params.prompt_cache_retention = "24h";
    params.prompt_cache_options = object;
    params.moderation = object;
    params.timeout = drogon_openai::Timeout{ 10.0 };
    params.max_retries = 0;
    auto const completion{ drogon::sync_wait(plugin_client().chat.completions.Create(params)) };
    REQUIRE(completion.has_value());
    CHECK(completion->model == "other-model");

    auto const request{ g_last_request.Get() };
    CHECK(request.body["model"].asString() == "other-model");
    CHECK(request.body["frequency_penalty"].asDouble() == 0.5);
    CHECK(request.body["max_tokens"].asInt64() == 64);
    CHECK(request.body["presence_penalty"].asDouble() == -0.5);
    CHECK(request.body["temperature"].asDouble() == 0.25);
    CHECK(request.body["top_p"].asDouble() == 0.75);
    for (auto const* field : { "tools", "tool_choice", "parallel_tool_calls", "response_format", "stop", "max_completion_tokens", "n", "seed", "logprobs", "top_logprobs", "logit_bias", "reasoning_effort", "verbosity", "safety_identifier", "prompt_cache_key", "user", "metadata", "store", "service_tier", "modalities", "audio", "prediction", "web_search_options", "prompt_cache_retention", "prompt_cache_options", "moderation" }) {
        CHECK(request.body.isMember(field));
    }
    CHECK(request.body["parallel_tool_calls"] == false);
    CHECK(!request.body.isMember("timeout"));
    CHECK(!request.body.isMember("max_retries"));
}

DROGON_TEST(CreateMergesExtraBody) {
    Json::Value extra_body{ Json::objectValue };
    extra_body["temperature"] = 0.75;
    extra_body["thinking"]["type"] = "disabled";
    auto const completion{ drogon::sync_wait(plugin_client().chat.completions.Create({
        .messages = { { .role = "user", .content = "Hello!" } },
        .temperature = 0.25,
        .extra_body = extra_body,
    })) };
    REQUIRE(completion.has_value());

    auto const request{ g_last_request.Get() };
    CHECK(request.body["temperature"].asDouble() == 0.75);
    CHECK(request.body["thinking"]["type"].asString() == "disabled");
}

DROGON_TEST(CreateSendsBearerKeyOnlyWhenSet) {
    auto const with_key{ drogon::sync_wait(client_at(test_support::BASE_PATH, "sk-other").chat.completions.Create(hello())) };
    REQUIRE(with_key.has_value());
    CHECK(g_last_request.Get().authorization == "Bearer sk-other");

    auto const without_key{ drogon::sync_wait(client_at(test_support::BASE_PATH).chat.completions.Create(hello())) };
    REQUIRE(without_key.has_value());
    CHECK(g_last_request.Get().authorization.empty());
}

DROGON_TEST(CreateUsesBaseUrlPath) {
    auto const without_path{ drogon::sync_wait(client_at("").chat.completions.Create(hello())) };
    REQUIRE(without_path.has_value());
    CHECK(without_path->id == "chatcmpl-root");
    CHECK(g_last_request.Get().path == "/chat/completions");

    auto const trailing_slash{ drogon::sync_wait(client_at("/v1/").chat.completions.Create(hello())) };
    REQUIRE(trailing_slash.has_value());
    CHECK(trailing_slash->id == "chatcmpl-v1");
    CHECK(g_last_request.Get().path == "/v1/chat/completions");
}

DROGON_TEST(CreateLeavesMissingFieldsUnset) {
    auto const completion{ drogon::sync_wait(client_at("/minimal").chat.completions.Create(hello())) };
    REQUIRE(completion.has_value());
    CHECK(completion->id.empty());
    CHECK(completion->created == 0);
    CHECK(completion->model.empty());
    CHECK(completion->object.empty());
    CHECK(!completion->usage.has_value());

    REQUIRE(completion->choices.size() == 1);
    auto const& choice{ completion->choices.front() };
    CHECK(choice.finish_reason.empty());
    CHECK(choice.message.role == "assistant");
    CHECK(!choice.message.content.has_value());
    CHECK(!choice.message.reasoning_content.has_value());
}

DROGON_TEST(CreateNon2xxIsStatusError) {
    auto const unauthorized{ drogon::sync_wait(client_at("/unauthorized").chat.completions.Create(hello())) };
    REQUIRE(!unauthorized.has_value());
    CHECK(unauthorized.error().kind == drogon_openai::ErrorKind::Status);
    CHECK(unauthorized.error().status_code == 401);
    CHECK(unauthorized.error().message.starts_with("HTTP 401: Authentication Fails"));
    CHECK(unauthorized.error().body.contains("authentication_error"));

    auto const overloaded{ drogon::sync_wait(client_at("/overloaded").chat.completions.Create(hello())) };
    REQUIRE(!overloaded.has_value());
    CHECK(overloaded.error().kind == drogon_openai::ErrorKind::Status);
    CHECK(overloaded.error().status_code == 503);
    CHECK(overloaded.error().message == "HTTP 503: upstream overloaded");
    CHECK(overloaded.error().body == "upstream overloaded");

    auto const missing{ drogon::sync_wait(client_at("/missing").chat.completions.Create(hello())) };
    REQUIRE(!missing.has_value());
    CHECK(missing.error().kind == drogon_openai::ErrorKind::Status);
    CHECK(missing.error().status_code == 404);
}

DROGON_TEST(CreateMalformedResponseIsResponseValidation) {
    auto const not_json{ drogon::sync_wait(client_at("/not_json").chat.completions.Create(hello())) };
    REQUIRE(!not_json.has_value());
    CHECK(not_json.error().kind == drogon_openai::ErrorKind::ResponseValidation);
    CHECK(not_json.error().message.starts_with("response body is not JSON"));
    CHECK(not_json.error().body == "<html>gateway</html>");

    auto const no_choices{ drogon::sync_wait(client_at("/no_choices").chat.completions.Create(hello())) };
    REQUIRE(!no_choices.has_value());
    CHECK(no_choices.error().kind == drogon_openai::ErrorKind::ResponseValidation);
    CHECK(no_choices.error().message == "chat completion has no choices array");
    CHECK(no_choices.error().body.contains("chatcmpl-empty"));

    auto const bad_choice{ drogon::sync_wait(client_at("/bad_choice").chat.completions.Create(hello())) };
    REQUIRE(!bad_choice.has_value());
    CHECK(bad_choice.error().kind == drogon_openai::ErrorKind::ResponseValidation);
    CHECK(bad_choice.error().message == "chat completion has a choice without a message object");
    CHECK(!bad_choice.error().body.empty());
}

DROGON_TEST(CreateTimeout) {
    auto const result{ drogon::sync_wait(client_at("/slow").chat.completions.Create({
        .messages = { { .role = "user", .content = "Hello!" } },
        .timeout = 0.2,
    })) };
    REQUIRE(!result.has_value());
    CHECK(result.error().kind == drogon_openai::ErrorKind::Timeout);
}

DROGON_TEST(CreateUnreachableIsConnectionError) {
    drogon_openai::OpenAI const client{ drogon_openai::OpenAIConfig{
        .base_url = "http://127.0.0.1:1",
        .model = test_support::MODEL,
    } };
    // Windows retries a refused connect for about 2 s before reporting it, so a
    // shorter timeout would make this a Timeout instead of a Connection error.
    auto const result{ drogon::sync_wait(client.chat.completions.Create({
        .messages = { { .role = "user", .content = "Hello!" } },
        .timeout = 10.0,
    })) };
    REQUIRE(!result.has_value());
    CHECK(result.error().kind == drogon_openai::ErrorKind::Connection);
    CHECK(result.error().message.starts_with("POST http://127.0.0.1:1/chat/completions"));
}

DROGON_TEST(ConcurrentRequestsDoNotQueue) {
    auto const client{ client_at("/slow") };
    auto const create{ [client] { return drogon::sync_wait(client.chat.completions.Create(hello())); } };
    auto const started{ std::chrono::steady_clock::now() };
    auto first{ std::async(std::launch::async, create) };
    auto second{ std::async(std::launch::async, create) };
    CHECK(first.get().has_value());
    CHECK(second.get().has_value());

    // On one shared HttpClient, the second request would wait for the first response.
    std::chrono::duration<double> const elapsed{ std::chrono::steady_clock::now() - started };
    CHECK(elapsed.count() < SLOW_DELAY * 1.8);
}

DROGON_TEST(CreateResumesOnCallingThread) {
    auto const response{ post("/resume_thread") };
    auto const& json{ response->getJsonObject() };
    REQUIRE(json != nullptr);
    CHECK((*json)["ok"].asBool());
    CHECK((*json)["same_thread"].asBool());
}

auto main(int argc, char** argv) -> int {
    test_support::set_env(test_support::API_KEY_ENV, test_support::API_KEY);
    drogon::app().loadConfigFile("config.json");
    serve_fake_openai();
    return test_support::run_tests_with_app(argc, argv);
}
