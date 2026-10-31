/**
 * @file test_resource_wire.cpp
 * @brief Check independent resource wire fixtures, cursor ownership and multipart encodings.
 */
#include <string>
#include <vector>

#include <drogon/drogon.h>
#include <drogon/drogon_test.h>

#include <drogon_openai/openai_client.hpp>

#include "details/transport.hpp"
#include "support.hpp"

using namespace drogon_openai;

namespace {
    std::vector<drogon::HttpRequestPtr> g_requests; ///< Requests retained after each completed call.

    /**
     * @brief Decode independent wire fixtures.
     * @param text Fixture bytes.
     * @return Parsed JSON.
     */
    auto json(std::string const& text) -> Json::Value {
        return details::decode_json(text).value();
    }

    /**
     * @brief Isolate this suite under one path prefix.
     * @param scenario Server behavior.
     * @return Owned client with defaults for precedence checks.
     */
    auto client(std::string scenario = "normal") -> OpenAI {
        return OpenAI{ { .base_url = std::string{ test_support::SERVER_URL } + "/wire/" + scenario, .model = "fake", .max_retries = 0, .default_query = json(R"({"limit":1,"default":"yes"})") } };
    }

    /**
     * @brief Serve independent JSON, text, binary and SSE protocol fixtures.
     */
    auto register_fixture() -> void {
        drogon::app().registerHandlerViaRegex("/wire/.*", [](drogon::HttpRequestPtr const& req, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            g_requests.push_back(req);
            auto reply{ drogon::HttpResponse::newHttpResponse() };
            auto const& path{ req->path() };
            if (path.starts_with("/wire/status/")) {
                reply->setStatusCode(drogon::k429TooManyRequests);
                reply->setBody(R"({"error":{"message":"slow down","type":"quota","code":"rate_limit","param":"input"}})");
            } else if (path.starts_with("/wire/invalid/")) {
                reply->setContentTypeString("application/json");
                reply->setBody(R"({"id":99})");
            } else if (path.starts_with("/wire/stream/")) {
                reply->setContentTypeString("text/event-stream");
                reply->setBody("data: {\"choices\":[{\"text\":\"hello\"}]}\n\ndata: [DONE]\n\n");
            } else if (path.ends_with("/audio/transcriptions") || path.ends_with("/audio/translations")) {
                reply->setContentTypeString("text/plain");
                reply->setBody("1\n00:00:00,000 --> 00:00:01,000\nhello\n");
            } else if (path.ends_with("/realtime/calls")) {
                reply->setContentTypeString("application/sdp");
                reply->setBody("v=0\r\ns=answer\r\n");
            } else if (path.ends_with("/search")) {
                reply = drogon::HttpResponse::newHttpJsonResponse(json(R"({"data":[{"file_id":"file-1","score":0.9,"content":[]}],"object":"vector_store.search_results.page"})"));
            } else if (path.ends_with("/items") || (path.ends_with("/files") && req->method() == drogon::Get)) {
                bool const second{ req->getParameter("after") == "opaque-cursor" };
                auto root{ json(R"({"data":[{"id":"item-1","object":"file"}],"last_id":"opaque-cursor","has_more":true})") };
                if (second) {
                    root["has_more"] = false;
                    root["data"][0]["id"] = "item-2";
                }
                reply = drogon::HttpResponse::newHttpJsonResponse(root);
            } else {
                reply = drogon::HttpResponse::newHttpJsonResponse(json(R"({"id":"resource","data":[],"model":"test","output":[],"object":"test","usage":{}})"));
            }
            reply->addHeader("x-request-id", "req-wire");
            reply->addHeader("retry-after", "0");
            callback(reply);
        });
    }
} // namespace

DROGON_TEST(ResourceQueryBodyAndOverridesStayIndependent) {
    types::files::FileListParams list;
    list.limit = 2;
    list.purpose = "assistants";
    list.extra_query = json(R"({"limit":3,"vendor":"query"})");
    list.extra_body = json(R"({"vendor":"body"})");
    REQUIRE(drogon::sync_wait(client().files.List(list)));
    auto request{ g_requests.back() };
    CHECK(request->getParameter("limit") == "3");
    CHECK(request->getParameter("purpose") == "assistants");
    CHECK(request->getParameter("default") == "yes");
    CHECK(request->getParameter("vendor") == "query");
    REQUIRE(request->getJsonObject());
    CHECK((*request->getJsonObject())["vendor"] == "body");
    CHECK(!request->getJsonObject()->isMember("limit"));

    types::conversations::ItemCreateParams create;
    create.items = json(R"([{"type":"message","role":"user","content":"hello"}])");
    create.include = json(R"(["item.input_audio_transcription.logprobs"])");
    REQUIRE(drogon::sync_wait(client().conversations.items.Create("conversation", create)));
    request = g_requests.back();
    REQUIRE(request->getJsonObject());
    CHECK((*request->getJsonObject())["items"][0]["role"] == "user");
    CHECK(!request->getJsonObject()->isMember("include"));
    CHECK(request->getParameter("include[]") == "item.input_audio_transcription.logprobs");

    REQUIRE(drogon::sync_wait(client().models.Retrieve("name /?#%世界")));
    CHECK(g_requests.back()->getOriginalPath() == "/wire/normal/models/name%20%2F%3F%23%25%E4%B8%96%E7%95%8C");
    CHECK(g_requests.back()->body().empty());
    auto const sent{ g_requests.size() };
    list.extra_body = "invalid";
    auto invalid_body{ drogon::sync_wait(client().files.List(list)) };
    REQUIRE(!invalid_body);
    CHECK(invalid_body.error().kind == ErrorKind::InvalidRequest);
    list.extra_body = Json::Value{};
    list.extra_query = "invalid";
    auto invalid_query{ drogon::sync_wait(client().files.List(list)) };
    REQUIRE(!invalid_query);
    CHECK(invalid_query.error().kind == ErrorKind::InvalidRequest);
    CHECK(g_requests.size() == sent);
}

DROGON_TEST(ResourceJsonOmissionNullAndExtensionsSurvive) {
    types::responses::ResponsesCreateParams params;
    params.model = "original";
    params.input = "hello";
    params.temperature = Json::Value{};
    params.store = false;
    params.extra_body = json(R"({"model":"override","vendor":{"retained":true}})");
    REQUIRE(drogon::sync_wait(client().responses.Create(params)));
    auto body{ g_requests.back()->getJsonObject() };
    REQUIRE(body);
    CHECK((*body)["model"] == "override");
    CHECK(body->isMember("temperature"));
    CHECK((*body)["temperature"].isNull());
    CHECK((*body)["store"] == false);
    CHECK((*body)["vendor"]["retained"] == true);
    CHECK(!body->isMember("max_output_tokens"));
    CHECK(!body->isMember("stream"));
}

DROGON_TEST(ResourceMultipartKeepsBinaryArraysAndExplicitJsonParts) {
    types::images::ImagesEditParams edit;
    edit.image = std::vector<UploadFile>{ { .content = std::string{ "first\0image", 11 }, .filename = "a.png", .content_type = "image/png" }, { .content = "second", .filename = "b.png" } };
    edit.prompt = "change";
    edit.extra_body = json(R"({"vendor":{"nested":true}})");
    REQUIRE(drogon::sync_wait(client().images.Edit(edit)));
    auto request{ g_requests.back() };
    drogon::MultiPartParser parser;
    REQUIRE(parser.parse(request) == 0);
    auto const files{ parser.getFiles() };
    REQUIRE(files.size() == 2);
    CHECK(files[0].getItemName() == "image[]");
    CHECK(files[0].getFileName() == "a.png");
    CHECK(files[0].fileContent() == std::string_view("first\0image", 11));
    CHECK(files[1].getFileName() == "b.png");
    CHECK(parser.getParameter<std::string>("prompt") == "change");
    CHECK(parser.getParameter<std::string>("vendor[nested]") == "true");

    types::realtime::CallCreateParams call;
    call.sdp = "v=0\r\ns=test\r\n";
    auto raw{ drogon::sync_wait(client().realtime.calls.Create(call)) };
    REQUIRE(raw);
    CHECK(raw->parsed == "v=0\r\ns=answer\r\n");
    CHECK(g_requests.back()->getHeader("content-type") == "application/sdp");
    CHECK(g_requests.back()->body() == call.sdp);
    call.session = json(R"({"type":"realtime","audio":{"output":{"voice":"marin"}}})");
    REQUIRE(drogon::sync_wait(client().realtime.calls.Create(call)));
    request = g_requests.back();
    CHECK(request->body().find("name=\"session\"\r\nContent-Type: application/json\r\n\r\n{\"audio\":") != std::string_view::npos);
    CHECK(request->body().find("session[audio]") == std::string_view::npos);
    CHECK(request->body().find("Content-Type: application/sdp\r\n\r\nv=0") != std::string_view::npos);
    call.session = Json::Value{};
    REQUIRE(drogon::sync_wait(client().realtime.calls.Create(call)));
    CHECK(g_requests.back()->body().find("Content-Type: application/json\r\n\r\nnull\r\n") != std::string_view::npos);
    call.extra_body = json(R"({"sdp":23})");
    auto invalid{ drogon::sync_wait(client().realtime.calls.Create(call)) };
    REQUIRE(!invalid);
    CHECK(invalid.error().kind == ErrorKind::InvalidRequest);
}

DROGON_TEST(ResourcePagesOwnCursorsAndSearchIsSinglePage) {
    types::conversations::ItemListParams params;
    params.limit = 2;
    params.extra_query = json(R"({"after":"initial","vendor":"query"})");
    auto first{ drogon::sync_wait(client().conversations.items.List("conversation", params)) };
    REQUIRE(first);
    CHECK(first->HasNextPage());
    auto second{ drogon::sync_wait(first->GetNextPage()) };
    REQUIRE(second);
    CHECK(g_requests.back()->getParameter("after") == "opaque-cursor");
    CHECK(g_requests.back()->getParameter("limit") == "2");
    CHECK(g_requests.back()->getParameter("vendor") == "query");
    CHECK(!second->HasNextPage());
    CHECK(second->_request_id == "req-wire");

    types::vector_stores::VectorStoreSearchParams search;
    search.query = "needle";
    search.max_num_results = 3;
    auto results{ drogon::sync_wait(client().vector_stores.Search("store", search)) };
    REQUIRE(results);
    CHECK(!results->HasNextPage());
    CHECK(results->data.size() == 1);
    REQUIRE(g_requests.back()->getJsonObject());
    CHECK((*g_requests.back()->getJsonObject())["query"] == "needle");
    CHECK((*g_requests.back()->getJsonObject())["max_num_results"] == 3);
}

DROGON_TEST(ResourceErrorsTextAndLegacyStreamsKeepTheirFormats) {
    auto invalid{ drogon::sync_wait(client("invalid").models.Retrieve("x")) };
    REQUIRE(!invalid);
    CHECK(invalid.error().kind == ErrorKind::ResponseValidation);
    CHECK(invalid.error().status_code == 200);
    CHECK(invalid.error().request_id == "req-wire");
    CHECK(invalid.error().body == R"({"id":99})");
    auto limited{ drogon::sync_wait(client("status").embeddings.Create({ .input = "x", .model = "test" })) };
    REQUIRE(!limited);
    CHECK(limited.error().IsRateLimit());
    CHECK(limited.error().code == "rate_limit");
    CHECK(limited.error().param == "input");
    CHECK(limited.error().request_id == "req-wire");

    types::audio::AudioTranscriptionsCreateParams transcription;
    transcription.file = UploadFile{ .content = "audio", .filename = "audio.wav" };
    transcription.model = "whisper-1";
    transcription.response_format = "srt";
    auto text{ drogon::sync_wait(client().audio.transcriptions.Create(transcription)) };
    REQUIRE(text);
    CHECK(text->isString());
    CHECK(text->asString().starts_with("1\n00:00"));
    types::completions::CompletionsCreateParams completion;
    completion.model = "text";
    completion.prompt = "hello";
    int count{};
    auto stream{ drogon::sync_wait(client("stream").completions.CreateStream(completion, [&count, drogon_test_ctx_](ApiStreamEvent const& event) -> Result<bool> {
        ++count;
        CHECK(event.data["choices"][0]["text"] == "hello");
        return true;
    })) };
    REQUIRE(stream);
    CHECK(count == 1);
    CHECK(stream->event_count == 1);
    CHECK(stream->_request_id == "req-wire");
    REQUIRE(g_requests.back()->getJsonObject());
    CHECK((*g_requests.back()->getJsonObject())["stream"] == true);
}

/**
 * @brief Run wire fixtures on the shared local app.
 * @param argc Test argument count.
 * @param argv Test arguments.
 * @return Test result.
 */
auto main(int argc, char** argv) -> int {
    test_support::set_env(test_support::API_KEY_ENV, test_support::API_KEY);
    drogon::app().loadConfigFile("config.json");
    register_fixture();
    return test_support::run_tests_with_app(argc, argv);
}
