/**
 * @file test_resource_helpers.cpp
 * @brief Verify structured Responses, incremental SSE, upload polling and webhook signatures.
 */
#include <atomic>
#include <chrono>
#include <climits>
#include <string>
#include <vector>

#include <drogon/drogon.h>
#include <drogon/drogon_test.h>

#include <drogon_openai/openai_client.hpp>

#include "details/transport.hpp"
#include "support.hpp"

using namespace drogon_openai;

namespace {
    std::atomic<int> g_uploads{};          ///< Uploaded file count.
    std::atomic<int> g_active{};           ///< Concurrent uploads.
    std::atomic<int> g_peak{};             ///< Maximum concurrent uploads.
    std::atomic<int> g_polls{};            ///< Processing polls.
    std::atomic<bool> g_stream_finished{}; ///< Terminal SSE event was sent.
    std::vector<std::string> g_parts;      ///< Ordered chunked upload bytes.
    Json::Value g_batch_ids;               ///< IDs passed when creating a vector store batch.
    std::string g_poll_beta;               ///< Effective polling protocol override.
    std::string g_poll_helper;             ///< Effective polling helper override.

    /**
     * @brief Decode fixture JSON.
     * @param text Wire fixture.
     * @return Parsed JSON.
     */
    auto json(std::string const& text) -> Json::Value {
        return details::decode_json(text).value();
    }

    /**
     * @brief Return a local client with an explicit signing secret.
     * @return Owned client.
     */
    auto client() -> OpenAI {
        return OpenAI{ { .base_url = std::string{ test_support::SERVER_URL } + "/helpers", .model = "fake", .max_retries = 0 } };
    }

    /**
     * @brief Return a structured final-answer response with a function call.
     * @return Resource fixture.
     */
    auto response() -> Json::Value {
        return json(R"JSON({"id":"resp","object":"response","status":"completed","output":[{"type":"message","phase":"final_answer","content":[{"type":"output_text","text":"{\"answer\":42}"}]},{"type":"function_call","name":"weather","arguments":"{\"city\":\"Paris\"}"}],"provider":"retained"})JSON");
    }

    /**
     * @brief Encode an event with CRLF boundaries and Unicode data.
     * @param event Event JSON.
     * @return Wire SSE event.
     */
    auto event(Json::Value const& event) -> std::string {
        Json::StreamWriterBuilder writer;
        writer["indentation"] = "";
        return "data: " + Json::writeString(writer, event) + "\r\n\r\n";
    }

    /**
     * @brief Register independent protocol and upload fixtures.
     */
    auto register_fixtures() -> void {
        drogon::app().registerHandler("/helpers/responses", [](drogon::HttpRequestPtr const& req, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            auto body{ req->getJsonObject() };
            auto root{ response() };
            if ((*body)["scenario"] == "commentary") {
                root["output"][0]["phase"] = "commentary";
            }
            if ((*body)["scenario"] == "malformed") {
                root["output"][0]["content"][0]["text"] = "{bad";
            }
            if ((*body)["stream"] == true) {
                g_stream_finished = false;
                bool const truncated{ (*body)["scenario"] == "truncated" };
                auto reply{ drogon::HttpResponse::newAsyncStreamResponse([root, truncated](drogon::ResponseStreamPtr stream) {
                    auto owned{ std::shared_ptr<drogon::ResponseStream>{ std::move(stream) } };
                    owned->send("\xEF\xBB\xBF: keep-alive\r\n\r\n");
                    Json::Value initial;
                    initial["type"] = "response.created";
                    initial["response"]["id"] = "resp";
                    initial["response"]["output"] = Json::arrayValue;
                    owned->send(event(initial));
                    owned->send("event: response.output_text.delta\r\ndata: {\"type\":\"response.output_text.delta\",\r\ndata: \"output_index\":0,\"content_index\":0,\"delta\":\"世界\"}\r\n\r\n");
                    drogon::app().getLoop()->runAfter(0.03, [owned, root, truncated] {
                        if (!truncated) {
                            Json::Value terminal;
                            terminal["type"] = "response.completed";
                            terminal["response"] = root;
                            g_stream_finished = true;
                            owned->send(event(terminal));
                        }
                        owned->close();
                    });
                }) };
                reply->setContentTypeString("text/event-stream");
                reply->addHeader("x-request-id", "req-stream");
                callback(reply);
            } else {
                auto reply{ drogon::HttpResponse::newHttpJsonResponse(root) };
                reply->addHeader("x-request-id", "req-response");
                callback(reply);
            }
        },
                                      { drogon::Post });
        drogon::app().registerHandler("/helpers/files", [](drogon::HttpRequestPtr const&, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            auto const count{ ++g_uploads };
            auto const active{ ++g_active };
            g_peak = std::max(g_peak.load(), active);
            drogon::app().getLoop()->runAfter(0.02, [callback = std::move(callback), count] {
                --g_active;
                Json::Value root;
                root["id"] = "file-" + std::to_string(count);
                root["status"] = "uploaded";
                callback(drogon::HttpResponse::newHttpJsonResponse(root));
            });
        },
                                      { drogon::Post });
        drogon::app().registerHandler("/helpers/files/{id}", [](drogon::HttpRequestPtr const&, std::function<void(drogon::HttpResponsePtr const&)>&& callback, std::string id) {
            Json::Value root;
            root["id"] = id;
            root["status"] = "processed";
            auto reply{ drogon::HttpResponse::newHttpJsonResponse(root) };
            reply->addHeader("x-request-id", "req-file");
            callback(reply);
        },
                                      { drogon::Get });
        drogon::app().registerHandlerViaRegex("/helpers/vector_stores/.*", [](drogon::HttpRequestPtr const& req, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            Json::Value root;
            if (req->method() == drogon::Post) {
                g_polls = 0;
                auto body{ req->getJsonObject() };
                if (req->path().ends_with("file_batches")) {
                    root["id"] = "batch";
                    g_batch_ids = (*body)["file_ids"];
                } else {
                    root["id"] = (*body)["file_id"];
                }
                root["status"] = "in_progress";
            } else {
                g_poll_beta = req->getHeader("openai-beta");
                g_poll_helper = req->getHeader("x-stainless-poll-helper");
                root["id"] = req->path().substr(req->path().rfind('/') + 1);
                root["status"] = ++g_polls >= 2 ? "completed" : "in_progress";
            }
            auto reply{ drogon::HttpResponse::newHttpJsonResponse(root) };
            reply->addHeader("openai-poll-after-ms", "1");
            reply->addHeader("x-request-id", "req-poll");
            callback(reply);
        });
        drogon::app().registerHandlerViaRegex("/helpers/uploads.*", [](drogon::HttpRequestPtr const& req, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            Json::Value root;
            root["id"] = "upload";
            root["status"] = "pending";
            if (req->path().ends_with("/parts")) {
                drogon::MultiPartParser parser;
                if (parser.parse(req) == 0 && !parser.getFiles().empty()) {
                    g_parts.emplace_back(parser.getFiles().front().fileContent());
                }
                root["id"] = "part-" + std::to_string(g_parts.size());
            } else if (req->path().ends_with("/complete")) {
                root["status"] = "completed";
                root["file"]["id"] = "file-upload";
                root["seen_parts"] = (*req->getJsonObject())["part_ids"];
            }
            callback(drogon::HttpResponse::newHttpJsonResponse(root));
        });
    }
} // namespace

DROGON_TEST(ResponsesParseKeepsSchemaAndStrictArguments) {
    types::responses::ResponsesCreateParams params;
    params.model = "fake";
    params.input = "question";
    params.text = json(R"JSON({"format":{"type":"json_schema","name":"answer","strict":true,"schema":{"type":"object","properties":{"answer":{"type":"integer"}}}}})JSON");
    params.tools = json(R"JSON([{"type":"function","name":"weather","strict":true,"parameters":{"type":"object"}}])JSON");
    auto result{ drogon::sync_wait(client().responses.Parse(params)) };
    REQUIRE(result);
    REQUIRE(result->output_parsed);
    CHECK((*result->output_parsed)["answer"] == 42);
    CHECK(result->output[1]["parsed_arguments"]["city"] == "Paris");
    CHECK(result->raw["output"][0]["content"][0]["parsed"].isNull());
    CHECK(result->_request_id == "req-response");
    CHECK(result->output_text == "{\"answer\":42}");
    params.extra_body["scenario"] = "commentary";
    auto commentary{ drogon::sync_wait(client().responses.Parse(params)) };
    REQUIRE(commentary);
    CHECK(!commentary->output_parsed);
    params.extra_body["scenario"] = "malformed";
    auto invalid{ drogon::sync_wait(client().responses.Parse(params)) };
    REQUIRE(!invalid);
    CHECK(invalid.error().kind == ErrorKind::ResponseValidation);
    CHECK(invalid.error().request_id == "req-response");
    params.tools = json(R"JSON([{"type":"function","name":"weather","strict":false}])JSON");
    auto nonstrict{ drogon::sync_wait(client().responses.Parse(params)) };
    REQUIRE(!nonstrict);
    CHECK(nonstrict.error().kind == ErrorKind::InvalidRequest);
}

DROGON_TEST(ResponsesStreamIsIncrementalAndDetectsTruncation) {
    types::responses::ResponsesCreateParams params;
    params.model = "fake";
    bool early{};
    std::string text;
    auto result{ drogon::sync_wait(client().responses.Stream(params, [&](ApiStreamEvent const& event) -> Result<bool> {
        if (event.type == "response.output_text.delta") {
            early = !g_stream_finished;
            text += event.data["delta"].asString();
        }
        return true;
    })) };
    REQUIRE(result);
    CHECK(early);
    CHECK(text == "世界");
    CHECK(result->id == "resp");
    CHECK(result->raw["provider"] == "retained");
    CHECK(result->_request_id == "req-stream");
    params.extra_body["scenario"] = "truncated";
    auto truncated{ drogon::sync_wait(client().responses.Stream(params)) };
    REQUIRE(!truncated);
    CHECK(truncated.error().kind == ErrorKind::ResponseValidation);
    CHECK(truncated.error().request_id == "req-stream");
    auto cancelled{ drogon::sync_wait(client().responses.Stream({}, [](ApiStreamEvent const&) -> Result<bool> { return false; })) };
    REQUIRE(cancelled);
    CHECK(cancelled->id == "resp");
    auto thrown{ drogon::sync_wait(client().responses.CreateStream({}, [](ApiStreamEvent const&) -> Result<bool> { throw std::runtime_error{ "consumer failed" }; })) };
    REQUIRE(!thrown);
    CHECK(thrown.error().kind == ErrorKind::InvalidRequest);
}

DROGON_TEST(VectorStoreUploadsPreserveIdsAndBoundConcurrency) {
    g_uploads = 0;
    auto attached{ drogon::sync_wait(client().vector_stores.files.UploadAndPoll("store", { .content = "data", .filename = "input.txt" }, {}, { .interval = 0.001 })) };
    REQUIRE(attached);
    CHECK(attached->status == "completed");
    CHECK(attached->id == "file-1");
    CHECK(attached->_request_id == "req-poll");
    g_uploads = 0;
    g_peak = 0;
    std::vector<UploadFile> files(4, { .content = "data", .filename = "file.txt" });
    auto batch{ drogon::sync_wait(client().vector_stores.file_batches.UploadAndPoll("store", files, { .file_ids = std::vector<std::string>{ "already-uploaded" } }, { .interval = 0.001 }, 2)) };
    REQUIRE(batch);
    CHECK(batch->status == "completed");
    CHECK(batch->_request_id == "req-poll");
    CHECK(g_uploads == 4);
    CHECK(g_peak == 2);
    CHECK(g_batch_ids.size() == 5);
    CHECK(g_batch_ids[0] == "already-uploaded");
    auto invalid{ drogon::sync_wait(client().vector_stores.file_batches.UploadAndPoll("store", {}, {}, {}, 0)) };
    REQUIRE(!invalid);
    CHECK(invalid.error().kind == ErrorKind::InvalidRequest);
}

DROGON_TEST(PollingHeadersRespectCaseInsensitiveOverrides) {
    auto result{ drogon::sync_wait(client().vector_stores.files.Poll("file", "store", { .interval = 0.001 }, { .extra_headers = { { "OPENAI-BETA", "custom" }, { "X-STAINLESS-POLL-HELPER", "custom" } } })) };
    REQUIRE(result);
    CHECK(result->status == "completed");
    CHECK(g_poll_beta == "custom");
    CHECK(g_poll_helper == "custom");
}

DROGON_TEST(ChunkedUploadKeepsBinaryAndPartOrder) {
    g_parts.clear();
    auto result{ drogon::sync_wait(client().uploads.UploadFileChunked({ .content = std::string{ "ab\0cdefgh", 9 }, .filename = "input.bin" }, "user_data", 3)) };
    REQUIRE(result);
    CHECK(result->status == "completed");
    REQUIRE(g_parts.size() == 3);
    CHECK(g_parts[0] + g_parts[1] + g_parts[2] == std::string("ab\0cdefgh", 9));
    CHECK(result->raw["seen_parts"][0] == "part-1");
    CHECK(result->raw["seen_parts"][2] == "part-3");
    auto invalid{ drogon::sync_wait(client().uploads.UploadFileChunked({}, "user_data", 0)) };
    REQUIRE(!invalid);
    CHECK(invalid.error().kind == ErrorKind::InvalidRequest);
    auto processed{ drogon::sync_wait(client().files.WaitForProcessing("file-1")) };
    REQUIRE(processed);
    CHECK(processed->status == "processed");
    CHECK(processed->_request_id == "req-file");
}

/**
 * @brief Run resource helpers against the local app.
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
