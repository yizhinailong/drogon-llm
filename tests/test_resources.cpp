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

DROGON_TEST(ModerationsCreateContract) {
    // SDK contract: moderations.create.
    types::moderations::ModerationCreateParams params;
    params.input = json(R"JSON({"vendor":"value"})JSON");
    params.model = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","model":"resource","results":[],"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().moderations.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/moderations");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FilesCreateContract) {
    // SDK contract: files.create.
    types::files::FileCreateParams params;
    params.file = UploadFile{ .content = std::string{ "file\0data", 9 }, .filename = "input.bin" };
    params.purpose = "value";
    params.expires_after = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","bytes":7,"created_at":7,"filename":"resource","object":"resource","purpose":"resource","status":"resource","expires_at":{},"status_details":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().files.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/files");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
    CHECK(g_request->getHeader("content-type").starts_with("multipart/form-data; boundary="));
    CHECK(g_request->body().find(std::string_view("file\0data", 9)) != std::string_view::npos);
}

DROGON_TEST(FilesRetrieveContract) {
    // SDK contract: files.retrieve.
    types::files::FilesRetrieveParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","bytes":7,"created_at":7,"filename":"resource","object":"resource","purpose":"resource","status":"resource","expires_at":{},"status_details":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().files.Retrieve("file_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/files/file_id-value");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FilesListContract) {
    // SDK contract: files.list.
    types::files::FileListParams params;
    params.after = "value";
    params.limit = 7;
    params.order = "value";
    params.purpose = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","bytes":7,"created_at":7,"filename":"resource","object":"resource","purpose":"resource","status":"resource","expires_at":{},"status_details":{},"provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().files.List(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/files");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FilesDeleteContract) {
    // SDK contract: files.delete.
    types::files::FilesDeleteParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","deleted":true,"object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().files.Delete("file_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/files/file_id-value");
    CHECK(std::string_view(g_request->methodString()) == "DELETE");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FilesContentContract) {
    // SDK contract: files.content.
    types::files::FilesContentParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = true;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().files.Content("file_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/files/file_id-value/content");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("accept") == "application/binary");
    CHECK(result->parsed == std::string("byte\0data", 9));
}

DROGON_TEST(FilesRetrieveContentContract) {
    // SDK contract: files.retrieve_content.
    types::files::FilesRetrieveContentParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = true;
    g_void = false;
    auto result{ drogon::sync_wait(client().files.RetrieveContent("file_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/files/file_id-value/content");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(*result == "legacy text");
}

DROGON_TEST(UploadsPartsCreateContract) {
    // SDK contract: uploads.parts.create.
    types::uploads::PartCreateParams params;
    params.data = UploadFile{ .content = std::string{ "file\0data", 9 }, .filename = "input.bin" };
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"object":"resource","upload_id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().uploads.parts.Create("upload_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/uploads/upload_id-value/parts");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
    CHECK(g_request->getHeader("content-type").starts_with("multipart/form-data; boundary="));
    CHECK(g_request->body().find(std::string_view("file\0data", 9)) != std::string_view::npos);
}

DROGON_TEST(UploadsCreateContract) {
    // SDK contract: uploads.create.
    types::uploads::UploadCreateParams params;
    params.bytes = 7;
    params.filename = "value";
    params.mime_type = "value";
    params.purpose = json(R"JSON({"vendor":"value"})JSON");
    params.expires_after = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","bytes":7,"created_at":7,"expires_at":7,"filename":"resource","object":"resource","purpose":"resource","status":"resource","file":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().uploads.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/uploads");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(UploadsCancelContract) {
    // SDK contract: uploads.cancel.
    types::uploads::UploadsCancelParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","bytes":7,"created_at":7,"expires_at":7,"filename":"resource","object":"resource","purpose":"resource","status":"resource","file":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().uploads.Cancel("upload_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/uploads/upload_id-value/cancel");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(UploadsCompleteContract) {
    // SDK contract: uploads.complete.
    types::uploads::UploadCompleteParams params;
    params.part_ids = std::vector<std::string>{};
    params.md5 = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","bytes":7,"created_at":7,"expires_at":7,"filename":"resource","object":"resource","purpose":"resource","status":"resource","file":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().uploads.Complete("upload_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/uploads/upload_id-value/complete");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(ImagesCreateVariationContract) {
    // SDK contract: images.create_variation.
    types::images::ImageCreateVariationParams params;
    params.image = UploadFile{ .content = std::string{ "file\0data", 9 }, .filename = "input.bin" };
    params.model = json(R"JSON({"vendor":"value"})JSON");
    params.n = json(R"JSON({"vendor":"value"})JSON");
    params.response_format = json(R"JSON({"vendor":"value"})JSON");
    params.size = json(R"JSON({"vendor":"value"})JSON");
    params.user = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"created":7,"background":{},"data":[],"output_format":{},"quality":{},"safety_identifier":{},"size":{},"usage":{},"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().images.CreateVariation(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/images/variations");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
    CHECK(g_request->getHeader("content-type").starts_with("multipart/form-data; boundary="));
    CHECK(g_request->body().find(std::string_view("file\0data", 9)) != std::string_view::npos);
}

DROGON_TEST(ImagesEditContract) {
    // SDK contract: images.edit.
    types::images::ImagesEditParams params;
    params.image = UploadFile{ .content = std::string{ "file\0data", 9 }, .filename = "input.bin" };
    params.prompt = "value";
    params.background = json(R"JSON({"vendor":"value"})JSON");
    params.input_fidelity = json(R"JSON({"vendor":"value"})JSON");
    params.markup = UploadFile{ .content = std::string{ "file\0data", 9 }, .filename = "input.bin" };
    params.mask = UploadFile{ .content = std::string{ "file\0data", 9 }, .filename = "input.bin" };
    params.model = json(R"JSON({"vendor":"value"})JSON");
    params.n = json(R"JSON({"vendor":"value"})JSON");
    params.output_compression = json(R"JSON({"vendor":"value"})JSON");
    params.output_format = json(R"JSON({"vendor":"value"})JSON");
    params.partial_images = json(R"JSON({"vendor":"value"})JSON");
    params.quality = json(R"JSON({"vendor":"value"})JSON");
    params.response_format = json(R"JSON({"vendor":"value"})JSON");
    params.safety_identifier = json(R"JSON({"vendor":"value"})JSON");
    params.size = json(R"JSON({"vendor":"value"})JSON");
    params.stream = false;
    params.user = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"created":7,"background":{},"data":[],"output_format":{},"quality":{},"safety_identifier":{},"size":{},"usage":{},"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().images.Edit(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/images/edits");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
    CHECK(g_request->getHeader("content-type").starts_with("multipart/form-data; boundary="));
    CHECK(g_request->body().find(std::string_view("file\0data", 9)) != std::string_view::npos);
}

DROGON_TEST(ImagesGenerateContract) {
    // SDK contract: images.generate.
    types::images::ImagesGenerateParams params;
    params.prompt = "value";
    params.background = json(R"JSON({"vendor":"value"})JSON");
    params.model = json(R"JSON({"vendor":"value"})JSON");
    params.moderation = json(R"JSON({"vendor":"value"})JSON");
    params.n = json(R"JSON({"vendor":"value"})JSON");
    params.output_compression = json(R"JSON({"vendor":"value"})JSON");
    params.output_format = json(R"JSON({"vendor":"value"})JSON");
    params.partial_images = json(R"JSON({"vendor":"value"})JSON");
    params.quality = json(R"JSON({"vendor":"value"})JSON");
    params.response_format = json(R"JSON({"vendor":"value"})JSON");
    params.safety_identifier = json(R"JSON({"vendor":"value"})JSON");
    params.size = json(R"JSON({"vendor":"value"})JSON");
    params.stream = false;
    params.style = json(R"JSON({"vendor":"value"})JSON");
    params.user = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"created":7,"background":{},"data":[],"output_format":{},"quality":{},"safety_identifier":{},"size":{},"usage":{},"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().images.Generate(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/images/generations");
    CHECK(std::string_view(g_request->methodString()) == "POST");
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
