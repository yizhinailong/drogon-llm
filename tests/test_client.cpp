/**
 * @file test_client.cpp
 * @brief Verify client defaults, overrides, retries, metadata, uploads, and polling offline.
 */
#include <atomic>
#include <chrono>
#include <string>
#include <utility>

#include <drogon/MultiPart.h>
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>

#include <drogon_openai/openai_client.hpp>

#include "support.hpp"

namespace {

    std::atomic<int> g_slow_requests{}; ///< Slow requests received across timeout retries.
    std::atomic<int> g_poll_requests{}; ///< Job retrieval count.

    /**
     * @brief Create a client for this suite's local endpoints.
     * @return Client with SDK defaults and a fake credential.
     */
    auto client() -> drogon_openai::OpenAI {
        return drogon_openai::OpenAI{ {
            .base_url = std::string{ test_support::SERVER_URL } + "/client",
            .api_key = "client-key",
            .model = "fake-model",
        } };
    }

    /**
     * @brief Echo the request and optionally emulate transient API errors.
     * @param request Received request.
     * @param callback Response sender.
     */
    auto echo(
        drogon::HttpRequestPtr const& request,
        std::function<void(drogon::HttpResponsePtr const&)>&& callback
    ) -> void {
        Json::Value body;
        body["method"] = drogon::to_string_view(request->method()).data();
        body["body"] = std::string{ request->body() };
        body["headers"] = Json::objectValue;
        for (auto const& [name, value] : request->headers()) {
            body["headers"][name] = value;
        }
        body["query"] = Json::objectValue;
        for (auto const& [name, value] : request->parameters()) {
            body["query"][name] = value;
        }
        int const attempt{ std::stoi(request->getHeader("x-stainless-retry-count")) };
        body["attempt"] = attempt;
        int status{ 200 };
        auto const requested_status{ request->getHeader("x-test-status") };
        if (!requested_status.empty()) {
            status = std::stoi(requested_status);
            if (request->getHeader("x-test-eventual") == "true" && attempt >= 2) {
                status = 200;
            }
        }
        if (status != 200) {
            body["error"]["message"] = "test API error";
            body["error"]["type"] = "rate_limit_error";
            body["error"]["code"] = "quota";
            if (request->getHeader("x-test-numeric-code") == "true") {
                body["error"]["code"] = 123;
            }
            body["error"]["param"] = "model";
        }
        auto response{ drogon::HttpResponse::newHttpJsonResponse(body) };
        response->setStatusCode(static_cast<drogon::HttpStatusCode>(status));
        response->addHeader("x-request-id", "req-client");
        response->addHeader("x-ratelimit-remaining-requests", "42");
        auto const retry_after{ request->getHeader("x-test-retry-after") };
        auto const retry_after_ms{ request->getHeader("x-test-retry-after-ms") };
        response->addHeader("retry-after-ms", retry_after_ms.empty() ? "1" : retry_after_ms);
        if (!retry_after.empty()) {
            response->addHeader("retry-after", retry_after);
        }
        auto const should_retry{ request->getHeader("x-test-should-retry") };
        if (!should_retry.empty()) {
            response->addHeader("x-should-retry", should_retry);
        }
        callback(response);
    }

    /**
     * @brief Register deterministic fixtures for client-level operations.
     */
    auto register_fixtures() -> void {
        drogon::app().registerHandler("/client/echo", &echo, { drogon::Get, drogon::Post, drogon::Put, drogon::Patch, drogon::Delete });
        drogon::app().registerHandler("/client/binary", [](drogon::HttpRequestPtr const&, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            auto response{ drogon::HttpResponse::newHttpResponse() };
            response->setContentTypeCode(drogon::CT_APPLICATION_OCTET_STREAM);
            response->setBody(std::string{ "\0\xff\x01hello", 8 });
            response->addHeader("x-request-id", "req-binary");
            callback(response);
        });
        drogon::app().registerHandler("/client/empty", [](drogon::HttpRequestPtr const&, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            callback(drogon::HttpResponse::newHttpResponse(drogon::k204NoContent, drogon::CT_NONE));
        });
        drogon::app().registerHandler("/client/bad-json", [](drogon::HttpRequestPtr const&, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            auto response{ drogon::HttpResponse::newHttpResponse() };
            response->setBody("not JSON");
            response->addHeader("x-request-id", "req-invalid");
            callback(response);
        });
        drogon::app().registerHandler("/client/upload", [](drogon::HttpRequestPtr const& request, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            drogon::MultiPartParser parser;
            Json::Value body;
            body["valid"] = parser.parse(request) == 0;
            body["purpose"] = parser.getParameter<std::string>("purpose");
            body["files"] = Json::arrayValue;
            for (auto const& file : parser.getFiles()) {
                Json::Value value;
                value["name"] = file.getFileName();
                value["field"] = file.getItemName();
                value["bytes"] = drogon::utils::base64Encode(file.fileContent());
                body["files"].append(value);
            }
            callback(drogon::HttpResponse::newHttpJsonResponse(body));
        },
                                      { drogon::Post });
        drogon::app().registerHandler("/client/slow", [](drogon::HttpRequestPtr const&, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            ++g_slow_requests;
            drogon::app().getLoop()->runAfter(0.15, [callback = std::move(callback)] { callback(drogon::HttpResponse::newHttpJsonResponse(Json::Value{})); });
        });
        drogon::app().registerHandler("/client/jobs", [](drogon::HttpRequestPtr const&, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            Json::Value body;
            body["id"] = "job";
            body["status"] = "in_progress";
            callback(drogon::HttpResponse::newHttpJsonResponse(body));
        },
                                      { drogon::Post });
        drogon::app().registerHandler("/client/jobs/job", [](drogon::HttpRequestPtr const&, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            Json::Value body;
            body["id"] = "job";
            body["status"] = ++g_poll_requests < 2 ? "in_progress" : "completed";
            callback(drogon::HttpResponse::newHttpJsonResponse(body));
        });
        drogon::app().registerHandler("/client/job", [](drogon::HttpRequestPtr const& request, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            Json::Value body;
            body["status"] = request->getParameter("forever") == "true" || ++g_poll_requests < 3 ? "in_progress" : "completed";
            auto response{ drogon::HttpResponse::newHttpJsonResponse(body) };
            response->addHeader("openai-poll-after-ms", "1");
            callback(response);
        });
    }

} // namespace

DROGON_TEST(DefaultsOverridesAndClientCopies) {
    auto base{ client().WithOptions({
        .default_headers = drogon_openai::Headers{ { "X-Mode", "default" }, { "X-Old", "keep" } },
        .default_query = Json::Value{ Json::objectValue },
        .organization = "org-test",
        .project = "proj-test",
    }) };
    auto options{ base.Config() };
    options.default_query["mode"] = "default";
    options.default_query["old"] = "keep";
    base = drogon_openai::OpenAI{ options };
    Json::Value query;
    query["mode"] = "request /?&中文";
    query["filter"]["name"] = "nested";
    query["tags"].append("x");
    auto result{ drogon::sync_wait(base.Get("echo", { .extra_headers = { { "x-MODE", "request" } }, .extra_query = query })) };
    REQUIRE(result.has_value());
    CHECK((*result)["headers"]["x-mode"] == "request");
    CHECK((*result)["headers"]["x-old"] == "keep");
    CHECK((*result)["headers"]["authorization"] == "Bearer client-key");
    CHECK((*result)["headers"]["openai-organization"] == "org-test");
    CHECK((*result)["headers"]["openai-project"] == "proj-test");
    CHECK((*result)["query"]["mode"] == "request /?&中文");
    CHECK((*result)["query"]["old"] == "keep");
    CHECK((*result)["query"]["filter[name]"] == "nested");
    CHECK((*result)["query"]["tags[]"] == "x");
    auto copy{ base.Copy({ .model = "copy-model", .default_headers = drogon_openai::Headers{ { "x-mode", "copy" } }, .organization = "", .project = "" }) };
    auto copied{ drogon::sync_wait(copy.Get("echo")) };
    REQUIRE(copied.has_value());
    CHECK((*copied)["headers"]["x-mode"] == "copy");
    CHECK(!(*copied)["headers"].isMember("openai-organization"));
    CHECK(!(*copied)["headers"].isMember("openai-project"));
    CHECK(copy.Config().model == "copy-model");
    CHECK(base.Config().model == "fake-model");
    CHECK(base.Config().default_headers.at("x-mode") == "default");
    CHECK(base.Config().max_retries == 2);
    CHECK(base.Config().timeout.connect == 5.0);
    CHECK(base.Config().timeout.read == 600.0);
    CHECK(base.Config().timeout.write == 600.0);
}

DROGON_TEST(RetriesHonorStatusAndOverrides) {
    for (auto const& code : { "408", "409", "429", "500", "503" }) {
        auto result{ drogon::sync_wait(client().Get("echo", { .extra_headers = { { "x-test-status", code }, { "x-test-eventual", "true" } } })) };
        REQUIRE(result.has_value());
        CHECK((*result)["attempt"] == 2);
    }
    auto forced{ drogon::sync_wait(client().Get("echo", { .extra_headers = { { "x-test-status", "400" }, { "x-test-eventual", "true" }, { "x-test-should-retry", "true" } } })) };
    REQUIRE(forced.has_value());
    CHECK((*forced)["attempt"] == 2);
    for (auto const& overrides : { drogon_openai::RequestOptions{ .max_retries = 0 }, drogon_openai::RequestOptions{ .extra_headers = { { "x-test-should-retry", "false" } } } }) {
        auto options{ overrides };
        options.extra_headers["x-test-status"] = "429";
        auto result{ drogon::sync_wait(client().Get("echo", options)) };
        REQUIRE(!result.has_value());
        CHECK((result.error().body.find("\"attempt\":0") != std::string::npos || result.error().body.find("\"attempt\": 0") != std::string::npos));
        CHECK(result.error().IsRateLimit());
        CHECK(result.error().type == "rate_limit_error");
        CHECK(result.error().code == "quota");
        CHECK(result.error().param == "model");
        CHECK(result.error().request_id == "req-client");
        CHECK(result.error().headers.at("retry-after-ms") == "1");
    }
    auto numeric{ drogon::sync_wait(client().Get("echo", { .max_retries = 0, .extra_headers = { { "x-test-status", "400" }, { "x-test-numeric-code", "true" } } })) };
    REQUIRE(!numeric);
    CHECK(numeric.error().code == "123");
    auto exhausted{ drogon::sync_wait(client().WithOptions({ .max_retries = 1 }).Get("echo", { .extra_headers = { { "x-test-status", "503" } } })) };
    REQUIRE(!exhausted.has_value());
    CHECK(exhausted.error().IsInternalServer());
    auto overridden{ drogon::sync_wait(client().WithOptions({ .max_retries = 0 }).Get("echo", { .max_retries = 2, .extra_headers = { { "x-test-status", "503" }, { "x-test-eventual", "true" } } })) };
    REQUIRE(overridden.has_value());
}

DROGON_TEST(RetryAfterDelaysAndCaps) {
    auto start{ std::chrono::steady_clock::now() };
    auto result{ drogon::sync_wait(client().Get("echo", { .extra_headers = { { "x-test-status", "429" }, { "x-test-eventual", "true" }, { "x-test-retry-after-ms", "30" }, { "x-test-retry-after", "1" } } })) };
    REQUIRE(result.has_value());
    auto elapsed{ std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count() };
    CHECK(elapsed >= 0.05);
    CHECK(elapsed < 1.0);
    start = std::chrono::steady_clock::now();
    auto seconds{ drogon::sync_wait(client().Get("echo", { .extra_headers = { { "x-test-status", "429" }, { "x-test-eventual", "true" }, { "x-test-retry-after-ms", "invalid" }, { "x-test-retry-after", "0.03" } } })) };
    REQUIRE(seconds.has_value());
    CHECK(std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count() >= 0.05);
    auto capped{ drogon::sync_wait(client().Get("echo", { .extra_headers = { { "x-test-status", "429" }, { "x-test-retry-after-ms", "invalid" }, { "x-test-retry-after", "121" }, { "x-test-should-retry", "true" } } })) };
    REQUIRE(!capped.has_value());
    CHECK((capped.error().body.find("\"attempt\":0") != std::string::npos || capped.error().body.find("\"attempt\": 0") != std::string::npos));
}

DROGON_TEST(RetryAfterHttpDateIsRespected) {
    auto const date{ std::string{ drogon::utils::getHttpFullDate(trantor::Date::now().after(2.0)) } };
    auto const start{ std::chrono::steady_clock::now() };
    auto result{ drogon::sync_wait(client().Get("echo", { .max_retries = 1, .extra_headers = { { "x-test-status", "429" }, { "x-test-retry-after-ms", "invalid" }, { "x-test-retry-after", date } } })) };
    REQUIRE(!result);
    CHECK(result.error().IsRateLimit());
    CHECK(std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count() >= 0.9);
}

DROGON_TEST(TimeoutDefaultsCanBeOverriddenAndRetried) {
    auto slow{ client().WithOptions({ .max_retries = 0, .timeout = drogon_openai::Timeout{ 0.02 } }) };
    g_slow_requests = 0;
    auto timeout{ drogon::sync_wait(slow.Get("slow")) };
    REQUIRE(!timeout.has_value());
    CHECK(timeout.error().kind == drogon_openai::ErrorKind::Timeout);
    CHECK(g_slow_requests == 1);
    auto success{ drogon::sync_wait(slow.Get("slow", { .timeout = drogon_openai::Timeout{ 1.0 } })) };
    REQUIRE(success.has_value());
    g_slow_requests = 0;
    auto retried{ drogon::sync_wait(slow.Get("slow", { .max_retries = 1 })) };
    REQUIRE(!retried.has_value());
    CHECK(retried.error().kind == drogon_openai::ErrorKind::Timeout);
    CHECK(g_slow_requests == 2);
}

DROGON_TEST(StatusPredicatesAndInvalidOptions) {
    using drogon_openai::Error;
    using drogon_openai::ErrorKind;
    CHECK((Error{ ErrorKind::Status, "", 400 }).IsBadRequest());
    CHECK((Error{ ErrorKind::Status, "", 401 }).IsAuthentication());
    CHECK((Error{ ErrorKind::Status, "", 403 }).IsPermissionDenied());
    CHECK((Error{ ErrorKind::Status, "", 404 }).IsNotFound());
    CHECK((Error{ ErrorKind::Status, "", 409 }).IsConflict());
    CHECK((Error{ ErrorKind::Status, "", 422 }).IsUnprocessableEntity());
    CHECK((Error{ ErrorKind::Status, "", 429 }).IsRateLimit());
    CHECK((Error{ ErrorKind::Status, "", 503 }).IsInternalServer());
    CHECK(!(Error{ ErrorKind::Connection, "", 429 }).IsRateLimit());
    auto invalid{ drogon::sync_wait(client().Get("echo", { .max_retries = -1 })) };
    REQUIRE(!invalid.has_value());
    CHECK(invalid.error().kind == ErrorKind::InvalidRequest);
    invalid = drogon::sync_wait(client().Get("echo", { .extra_headers = { { "x-inject", "a\r\nb" } } }));
    REQUIRE(!invalid.has_value());
    CHECK(invalid.error().kind == ErrorKind::InvalidRequest);
}

/**
 * @brief Start the shared local listener and execute this test suite.
 * @param argc Argument count.
 * @param argv Test framework arguments.
 * @return Test status.
 */
auto main(int argc, char** argv) -> int {
    test_support::set_env(test_support::API_KEY_ENV, test_support::API_KEY);
    drogon::app().loadConfigFile("config.json");
    register_fixtures();
    return test_support::run_tests_with_app(argc, argv);
}
