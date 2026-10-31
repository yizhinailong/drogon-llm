/**
 * @file test_resources.cpp
 * @brief Exercise every HTTP resource binding against the pinned SDK wire contract.
 */
#include <string>

#include <drogon/drogon.h>
#include <drogon/drogon_test.h>

#include <drogon_openai/openai_client.hpp>

#include "details/transport.hpp"
#include "support.hpp"

using namespace drogon_openai;

namespace {
    drogon::HttpRequestPtr g_request; ///< Last request, retained through its assertion.
    Json::Value g_reply;              ///< Current resource fixture.
    bool g_binary{};                  ///< Send binary content for this contract.
    bool g_text{};                    ///< Send legacy text content for this contract.
    bool g_void{};                    ///< Send a no-content response for this contract.

    /**
     * @brief Create the isolated contract client with a base path.
     * @return Owned client.
     */
    auto client() -> OpenAI {
        return OpenAI{ { .base_url = std::string{ test_support::SERVER_URL } + "/resources", .api_key = "resource-key", .model = "fake", .max_retries = 0 } };
    }

    /**
     * @brief Decode fixture JSON without exposing request data in logs.
     * @param text Fixture bytes.
     * @return JSON fixture.
     */
    auto json(std::string const& text) -> Json::Value {
        return details::decode_json(text).value();
    }

    /**
     * @brief Serve arbitrary SDK routes while retaining method, body and headers.
     */
    auto register_fixture() -> void {
        drogon::app().registerHandlerViaRegex("/resources/.*", [](drogon::HttpRequestPtr const& req, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            g_request = req;
            auto response{ drogon::HttpResponse::newHttpResponse() };
            if (g_void) {
                response->setStatusCode(drogon::k204NoContent);
            } else if (g_binary || g_text) {
                response->setContentTypeString(g_text ? "text/plain" : "application/octet-stream");
                response->setBody(g_text ? "legacy text" : std::string{ "byte\0data", 9 });
            } else {
                response = drogon::HttpResponse::newHttpJsonResponse(g_reply);
            }
            response->addHeader("x-request-id", "req-resource");
            callback(response);
        });
    }
} // namespace

DROGON_TEST(ModelsRetrieveContract) {
    // SDK contract: models.retrieve.
    types::models::ModelsRetrieveParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created":7,"object":"resource","owned_by":"resource","shutdown_date":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().models.Retrieve("model-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/models/model-value");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ModelsListContract) {
    // SDK contract: models.list.
    types::models::ModelsListParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","created":7,"object":"resource","owned_by":"resource","shutdown_date":{},"provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().models.List(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/models");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ModelsDeleteContract) {
    // SDK contract: models.delete.
    types::models::ModelsDeleteParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","deleted":true,"object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().models.Delete("model-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/models/model-value");
    CHECK(std::string_view(g_request->methodString()) == "DELETE");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(EmbeddingsCreateContract) {
    // SDK contract: embeddings.create.
    types::embeddings::EmbeddingCreateParams params;
    params.input = json(R"JSON({"vendor":"value"})JSON");
    params.model = "value";
    params.dimensions = 7;
    params.encoding_format = "value";
    params.user = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[],"model":"resource","object":"resource","usage":{},"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().embeddings.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/embeddings");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(TextCompletionsCreateContract) {
    // SDK contract: completions.create.
    types::completions::CompletionsCreateParams params;
    params.model = "value";
    params.prompt = json(R"JSON({"vendor":"value"})JSON");
    params.best_of = json(R"JSON({"vendor":"value"})JSON");
    params.echo = json(R"JSON({"vendor":"value"})JSON");
    params.frequency_penalty = json(R"JSON({"vendor":"value"})JSON");
    params.logit_bias = json(R"JSON({"vendor":"value"})JSON");
    params.logprobs = json(R"JSON({"vendor":"value"})JSON");
    params.max_tokens = json(R"JSON({"vendor":"value"})JSON");
    params.n = json(R"JSON({"vendor":"value"})JSON");
    params.presence_penalty = json(R"JSON({"vendor":"value"})JSON");
    params.seed = json(R"JSON({"vendor":"value"})JSON");
    params.stop = json(R"JSON({"vendor":"value"})JSON");
    params.stream = false;
    params.stream_options = json(R"JSON({"vendor":"value"})JSON");
    params.suffix = json(R"JSON({"vendor":"value"})JSON");
    params.temperature = json(R"JSON({"vendor":"value"})JSON");
    params.top_p = json(R"JSON({"vendor":"value"})JSON");
    params.user = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","choices":[],"created":7,"model":"resource","object":"resource","system_fingerprint":{},"usage":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().completions.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/completions");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ResponsesInputItemsListContract) {
    // SDK contract: responses.input_items.list.
    types::responses::InputItemListParams params;
    params.after = "value";
    params.include = json(R"JSON({"vendor":"value"})JSON");
    params.limit = 7;
    params.order = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().responses.input_items.List("response_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/responses/response_id-value/input_items");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ResponsesInputTokensCountContract) {
    // SDK contract: responses.input_tokens.count.
    types::responses::InputTokenCountParams params;
    params.conversation = json(R"JSON({"vendor":"value"})JSON");
    params.input = json(R"JSON({"vendor":"value"})JSON");
    params.instructions = json(R"JSON({"vendor":"value"})JSON");
    params.model = json(R"JSON({"vendor":"value"})JSON");
    params.parallel_tool_calls = json(R"JSON({"vendor":"value"})JSON");
    params.personality = "value";
    params.previous_response_id = json(R"JSON({"vendor":"value"})JSON");
    params.reasoning = json(R"JSON({"vendor":"value"})JSON");
    params.text = json(R"JSON({"vendor":"value"})JSON");
    params.tool_choice = json(R"JSON({"vendor":"value"})JSON");
    params.tools = json(R"JSON({"vendor":"value"})JSON");
    params.truncation = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"input_tokens":7,"object":"resource","id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().responses.input_tokens.Count(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/responses/input_tokens");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ResponsesCreateContract) {
    // SDK contract: responses.create.
    types::responses::ResponsesCreateParams params;
    params.access_programs = json(R"JSON({"vendor":"value"})JSON");
    params.background = json(R"JSON({"vendor":"value"})JSON");
    params.context_management = json(R"JSON({"vendor":"value"})JSON");
    params.conversation = json(R"JSON({"vendor":"value"})JSON");
    params.include = json(R"JSON({"vendor":"value"})JSON");
    params.input = json(R"JSON({"vendor":"value"})JSON");
    params.instructions = json(R"JSON({"vendor":"value"})JSON");
    params.max_output_tokens = json(R"JSON({"vendor":"value"})JSON");
    params.max_tool_calls = json(R"JSON({"vendor":"value"})JSON");
    params.metadata = json(R"JSON({"vendor":"value"})JSON");
    params.model = "value";
    params.moderation = json(R"JSON({"vendor":"value"})JSON");
    params.parallel_tool_calls = json(R"JSON({"vendor":"value"})JSON");
    params.previous_response_id = json(R"JSON({"vendor":"value"})JSON");
    params.prompt = json(R"JSON({"vendor":"value"})JSON");
    params.prompt_cache_key = json(R"JSON({"vendor":"value"})JSON");
    params.prompt_cache_options = json(R"JSON({"vendor":"value"})JSON");
    params.prompt_cache_retention = json(R"JSON({"vendor":"value"})JSON");
    params.reasoning = json(R"JSON({"vendor":"value"})JSON");
    params.safety_identifier = json(R"JSON({"vendor":"value"})JSON");
    params.service_tier = json(R"JSON({"vendor":"value"})JSON");
    params.store = json(R"JSON({"vendor":"value"})JSON");
    params.stream = false;
    params.stream_options = json(R"JSON({"vendor":"value"})JSON");
    params.temperature = json(R"JSON({"vendor":"value"})JSON");
    params.text = json(R"JSON({"vendor":"value"})JSON");
    params.tool_choice = json(R"JSON({"vendor":"value"})JSON");
    params.tools = json(R"JSON({"vendor":"value"})JSON");
    params.top_logprobs = json(R"JSON({"vendor":"value"})JSON");
    params.top_p = json(R"JSON({"vendor":"value"})JSON");
    params.truncation = json(R"JSON({"vendor":"value"})JSON");
    params.user = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","access_programs":{},"created_at":0.5,"error":{},"incomplete_details":{},"instructions":[],"metadata":{},"model":"resource","object":"resource","output":[],"parallel_tool_calls":true,"temperature":{},"tool_choice":{},"tools":[],"top_p":{},"background":{},"completed_at":{},"conversation":{},"max_output_tokens":{},"max_tool_calls":{},"moderation":{},"previous_response_id":{},"prompt":{},"prompt_cache_diagnostics":{},"prompt_cache_key":{},"prompt_cache_options":{},"prompt_cache_retention":{},"reasoning":{},"safety_identifier":{},"service_tier":{},"status":{},"text":{},"tool_usage":{},"top_logprobs":{},"truncation":{},"usage":{},"user":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().responses.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/responses");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ResponsesRetrieveContract) {
    // SDK contract: responses.retrieve.
    types::responses::ResponseRetrieveParams params;
    params.include = json(R"JSON({"vendor":"value"})JSON");
    params.include_obfuscation = true;
    params.starting_after = 7;
    params.stream = false;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","access_programs":{},"created_at":0.5,"error":{},"incomplete_details":{},"instructions":[],"metadata":{},"model":"resource","object":"resource","output":[],"parallel_tool_calls":true,"temperature":{},"tool_choice":{},"tools":[],"top_p":{},"background":{},"completed_at":{},"conversation":{},"max_output_tokens":{},"max_tool_calls":{},"moderation":{},"previous_response_id":{},"prompt":{},"prompt_cache_diagnostics":{},"prompt_cache_key":{},"prompt_cache_options":{},"prompt_cache_retention":{},"reasoning":{},"safety_identifier":{},"service_tier":{},"status":{},"text":{},"tool_usage":{},"top_logprobs":{},"truncation":{},"usage":{},"user":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().responses.Retrieve("response_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/responses/response_id-value");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ResponsesDeleteContract) {
    // SDK contract: responses.delete.
    types::responses::ResponsesDeleteParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = true;
    auto result{ drogon::sync_wait(client().responses.Delete("response_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/responses/response_id-value");
    CHECK(std::string_view(g_request->methodString()) == "DELETE");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("accept") == "*/*");
}

DROGON_TEST(ResponsesCancelContract) {
    // SDK contract: responses.cancel.
    types::responses::ResponsesCancelParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","access_programs":{},"created_at":0.5,"error":{},"incomplete_details":{},"instructions":[],"metadata":{},"model":"resource","object":"resource","output":[],"parallel_tool_calls":true,"temperature":{},"tool_choice":{},"tools":[],"top_p":{},"background":{},"completed_at":{},"conversation":{},"max_output_tokens":{},"max_tool_calls":{},"moderation":{},"previous_response_id":{},"prompt":{},"prompt_cache_diagnostics":{},"prompt_cache_key":{},"prompt_cache_options":{},"prompt_cache_retention":{},"reasoning":{},"safety_identifier":{},"service_tier":{},"status":{},"text":{},"tool_usage":{},"top_logprobs":{},"truncation":{},"usage":{},"user":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().responses.Cancel("response_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/responses/response_id-value/cancel");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ResponsesCompactContract) {
    // SDK contract: responses.compact.
    types::responses::ResponseCompactParams params;
    params.model = json(R"JSON({"vendor":"value"})JSON");
    params.input = json(R"JSON({"vendor":"value"})JSON");
    params.instructions = json(R"JSON({"vendor":"value"})JSON");
    params.previous_response_id = json(R"JSON({"vendor":"value"})JSON");
    params.prompt_cache_key = json(R"JSON({"vendor":"value"})JSON");
    params.prompt_cache_options = json(R"JSON({"vendor":"value"})JSON");
    params.prompt_cache_retention = json(R"JSON({"vendor":"value"})JSON");
    params.service_tier = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"object":"resource","output":[],"usage":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().responses.Compact(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/responses/compact");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ConversationsCreateContract) {
    // SDK contract: conversations.create.
    types::conversations::ConversationCreateParams params;
    params.items = json(R"JSON({"vendor":"value"})JSON");
    params.metadata = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"metadata":{},"object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().conversations.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/conversations");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ConversationsRetrieveContract) {
    // SDK contract: conversations.retrieve.
    types::conversations::ConversationsRetrieveParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"metadata":{},"object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().conversations.Retrieve("conversation_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/conversations/conversation_id-value");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ConversationsUpdateContract) {
    // SDK contract: conversations.update.
    types::conversations::ConversationUpdateParams params;
    params.metadata = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"metadata":{},"object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().conversations.Update("conversation_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/conversations/conversation_id-value");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ConversationsDeleteContract) {
    // SDK contract: conversations.delete.
    types::conversations::ConversationsDeleteParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","deleted":true,"object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().conversations.Delete("conversation_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/conversations/conversation_id-value");
    CHECK(std::string_view(g_request->methodString()) == "DELETE");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ConversationsItemsCreateContract) {
    // SDK contract: conversations.items.create.
    types::conversations::ItemCreateParams params;
    params.items = json(R"JSON({"vendor":"value"})JSON");
    params.include = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[],"first_id":"resource","has_more":true,"last_id":"resource","object":"resource","id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().conversations.items.Create("conversation_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/conversations/conversation_id-value/items");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ConversationsItemsRetrieveContract) {
    // SDK contract: conversations.items.retrieve.
    types::conversations::ItemRetrieveParams params;
    params.include = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().conversations.items.Retrieve("item_id-value", "conversation_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/conversations/conversation_id-value/items/item_id-value");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
}

DROGON_TEST(ConversationsItemsListContract) {
    // SDK contract: conversations.items.list.
    types::conversations::ItemListParams params;
    params.after = "value";
    params.include = json(R"JSON({"vendor":"value"})JSON");
    params.limit = 7;
    params.order = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().conversations.items.List("conversation_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/conversations/conversation_id-value/items");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ConversationsItemsDeleteContract) {
    // SDK contract: conversations.items.delete.
    types::conversations::ConversationsItemsDeleteParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"metadata":{},"object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().conversations.items.Delete("item_id-value", "conversation_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/conversations/conversation_id-value/items/item_id-value");
    CHECK(std::string_view(g_request->methodString()) == "DELETE");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

/**
 * @brief Run HTTP contract checks on the local fixture app.
 * @param argc Framework argument count.
 * @param argv Framework options.
 * @return Test status.
 */
auto main(int argc, char** argv) -> int {
    test_support::set_env(test_support::API_KEY_ENV, test_support::API_KEY);
    drogon::app().loadConfigFile("config.json");
    register_fixture();
    return test_support::run_tests_with_app(argc, argv);
}
