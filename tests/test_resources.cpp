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

DROGON_TEST(AudioSpeechCreateContract) {
    // SDK contract: audio.speech.create.
    types::audio::SpeechCreateParams params;
    params.input = "value";
    params.model = "value";
    params.voice = json(R"JSON({"vendor":"value"})JSON");
    params.instructions = "value";
    params.response_format = "value";
    params.speed = 0.5;
    params.stream_format = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = true;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().audio.speech.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/audio/speech");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("accept") == "application/octet-stream");
    CHECK(result->parsed == std::string("byte\0data", 9));
}

DROGON_TEST(AudioTranscriptionsCreateContract) {
    // SDK contract: audio.transcriptions.create.
    types::audio::AudioTranscriptionsCreateParams params;
    params.file = UploadFile{ .content = std::string{ "file\0data", 9 }, .filename = "input.bin" };
    params.model = "value";
    params.chunking_strategy = json(R"JSON({"vendor":"value"})JSON");
    params.include = json(R"JSON({"vendor":"value"})JSON");
    params.keywords = std::vector<std::string>{};
    params.known_speaker_names = std::vector<std::string>{};
    params.known_speaker_references = std::vector<std::string>{};
    params.language = "value";
    params.languages = std::vector<std::string>{};
    params.prompt = "value";
    params.response_format = json(R"JSON({"vendor":"value"})JSON");
    params.stream = false;
    params.temperature = 0.5;
    params.timestamp_granularities = std::vector<std::string>{};
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().audio.transcriptions.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/audio/transcriptions");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("content-type").starts_with("multipart/form-data; boundary="));
    CHECK(g_request->body().find(std::string_view("file\0data", 9)) != std::string_view::npos);
}

DROGON_TEST(AudioTranslationsCreateContract) {
    // SDK contract: audio.translations.create.
    types::audio::TranslationCreateParams params;
    params.file = UploadFile{ .content = std::string{ "file\0data", 9 }, .filename = "input.bin" };
    params.model = "value";
    params.prompt = "value";
    params.response_format = "value";
    params.temperature = 0.5;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().audio.translations.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/audio/translations");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("content-type").starts_with("multipart/form-data; boundary="));
    CHECK(g_request->body().find(std::string_view("file\0data", 9)) != std::string_view::npos);
}

DROGON_TEST(AudioVoiceConsentsCreateContract) {
    // SDK contract: audio.voice_consents.create.
    types::audio::VoiceConsentCreateParams params;
    params.language = "value";
    params.name = "value";
    params.recording = UploadFile{ .content = std::string{ "file\0data", 9 }, .filename = "input.bin" };
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"language":"resource","name":"resource","object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().audio.voice_consents.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/audio/voice_consents");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
    CHECK(g_request->getHeader("content-type").starts_with("multipart/form-data; boundary="));
    CHECK(g_request->body().find(std::string_view("file\0data", 9)) != std::string_view::npos);
}

DROGON_TEST(AudioVoiceConsentsRetrieveContract) {
    // SDK contract: audio.voice_consents.retrieve.
    types::audio::AudioVoiceConsentsRetrieveParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"language":"resource","name":"resource","object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().audio.voice_consents.Retrieve("consent_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/audio/voice_consents/consent_id-value");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(AudioVoiceConsentsUpdateContract) {
    // SDK contract: audio.voice_consents.update.
    types::audio::VoiceConsentUpdateParams params;
    params.name = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"language":"resource","name":"resource","object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().audio.voice_consents.Update("consent_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/audio/voice_consents/consent_id-value");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(AudioVoiceConsentsListContract) {
    // SDK contract: audio.voice_consents.list.
    types::audio::VoiceConsentListParams params;
    params.after = "value";
    params.limit = 7;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","created_at":7,"language":"resource","name":"resource","object":"resource","provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().audio.voice_consents.List(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/audio/voice_consents");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(AudioVoiceConsentsDeleteContract) {
    // SDK contract: audio.voice_consents.delete.
    types::audio::AudioVoiceConsentsDeleteParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","deleted":true,"object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().audio.voice_consents.Delete("consent_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/audio/voice_consents/consent_id-value");
    CHECK(std::string_view(g_request->methodString()) == "DELETE");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(AudioVoicesCreateContract) {
    // SDK contract: audio.voices.create.
    types::audio::VoiceCreateParams params;
    params.audio_sample = UploadFile{ .content = std::string{ "file\0data", 9 }, .filename = "input.bin" };
    params.consent = "value";
    params.name = "value";
    params.type = "value";
    params.prompt = "value";
    params.model = "value";
    params.script_hint = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"name":"resource","object":"resource","type":"resource","prompt":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().audio.voices.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/audio/voices");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
    CHECK(g_request->getHeader("content-type").starts_with("multipart/form-data; boundary="));
    CHECK(g_request->body().find(std::string_view("file\0data", 9)) != std::string_view::npos);
}

DROGON_TEST(AudioVoicesDeleteContract) {
    // SDK contract: audio.voices.delete.
    types::audio::AudioVoicesDeleteParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","deleted":true,"object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().audio.voices.Delete("voice_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/audio/voices/voice_id-value");
    CHECK(std::string_view(g_request->methodString()) == "DELETE");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(BatchesCreateContract) {
    // SDK contract: batches.create.
    types::batches::BatchCreateParams params;
    params.completion_window = "value";
    params.endpoint = "value";
    params.input_file_id = "value";
    params.metadata = json(R"JSON({"vendor":"value"})JSON");
    params.output_expires_after = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","completion_window":"resource","created_at":7,"endpoint":"resource","input_file_id":"resource","object":"resource","status":"resource","cancelled_at":{},"cancelling_at":{},"completed_at":{},"error_file_id":{},"errors":{},"expired_at":{},"expires_at":{},"failed_at":{},"finalizing_at":{},"in_progress_at":{},"metadata":{},"model":{},"output_file_id":{},"request_counts":{},"usage":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().batches.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/batches");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(BatchesRetrieveContract) {
    // SDK contract: batches.retrieve.
    types::batches::BatchesRetrieveParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","completion_window":"resource","created_at":7,"endpoint":"resource","input_file_id":"resource","object":"resource","status":"resource","cancelled_at":{},"cancelling_at":{},"completed_at":{},"error_file_id":{},"errors":{},"expired_at":{},"expires_at":{},"failed_at":{},"finalizing_at":{},"in_progress_at":{},"metadata":{},"model":{},"output_file_id":{},"request_counts":{},"usage":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().batches.Retrieve("batch_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/batches/batch_id-value");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(BatchesListContract) {
    // SDK contract: batches.list.
    types::batches::BatchListParams params;
    params.after = "value";
    params.limit = 7;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","completion_window":"resource","created_at":7,"endpoint":"resource","input_file_id":"resource","object":"resource","status":"resource","cancelled_at":{},"cancelling_at":{},"completed_at":{},"error_file_id":{},"errors":{},"expired_at":{},"expires_at":{},"failed_at":{},"finalizing_at":{},"in_progress_at":{},"metadata":{},"model":{},"output_file_id":{},"request_counts":{},"usage":{},"provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().batches.List(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/batches");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(BatchesCancelContract) {
    // SDK contract: batches.cancel.
    types::batches::BatchesCancelParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","completion_window":"resource","created_at":7,"endpoint":"resource","input_file_id":"resource","object":"resource","status":"resource","cancelled_at":{},"cancelling_at":{},"completed_at":{},"error_file_id":{},"errors":{},"expired_at":{},"expires_at":{},"failed_at":{},"finalizing_at":{},"in_progress_at":{},"metadata":{},"model":{},"output_file_id":{},"request_counts":{},"usage":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().batches.Cancel("batch_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/batches/batch_id-value/cancel");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresFileBatchesCreateContract) {
    // SDK contract: vector_stores.file_batches.create.
    types::vector_stores::FileBatchCreateParams params;
    params.attributes = json(R"JSON({"vendor":"value"})JSON");
    params.chunking_strategy = json(R"JSON({"vendor":"value"})JSON");
    params.file_ids = std::vector<std::string>{};
    params.files = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"file_counts":{},"object":"resource","status":"resource","vector_store_id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.file_batches.Create("vector_store_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores/vector_store_id-value/file_batches");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresFileBatchesRetrieveContract) {
    // SDK contract: vector_stores.file_batches.retrieve.
    types::vector_stores::VectorStoresFileBatchesRetrieveParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"file_counts":{},"object":"resource","status":"resource","vector_store_id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.file_batches.Retrieve("batch_id-value", "vector_store_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores/vector_store_id-value/file_batches/batch_id-value");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresFileBatchesCancelContract) {
    // SDK contract: vector_stores.file_batches.cancel.
    types::vector_stores::VectorStoresFileBatchesCancelParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"file_counts":{},"object":"resource","status":"resource","vector_store_id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.file_batches.Cancel("batch_id-value", "vector_store_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores/vector_store_id-value/file_batches/batch_id-value/cancel");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresFileBatchesListFilesContract) {
    // SDK contract: vector_stores.file_batches.list_files.
    types::vector_stores::FileBatchListFilesParams params;
    params.after = "value";
    params.before = "value";
    params.filter = "value";
    params.limit = 7;
    params.order = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","created_at":7,"last_error":{},"object":"resource","status":"resource","usage_bytes":7,"vector_store_id":"resource","attributes":{},"chunking_strategy":{},"provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.file_batches.ListFiles("batch_id-value", "vector_store_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores/vector_store_id-value/file_batches/batch_id-value/files");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresFilesCreateContract) {
    // SDK contract: vector_stores.files.create.
    types::vector_stores::FileCreateParams params;
    params.file_id = "value";
    params.attributes = json(R"JSON({"vendor":"value"})JSON");
    params.chunking_strategy = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"last_error":{},"object":"resource","status":"resource","usage_bytes":7,"vector_store_id":"resource","attributes":{},"chunking_strategy":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.files.Create("vector_store_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores/vector_store_id-value/files");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresFilesRetrieveContract) {
    // SDK contract: vector_stores.files.retrieve.
    types::vector_stores::VectorStoresFilesRetrieveParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"last_error":{},"object":"resource","status":"resource","usage_bytes":7,"vector_store_id":"resource","attributes":{},"chunking_strategy":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.files.Retrieve("file_id-value", "vector_store_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores/vector_store_id-value/files/file_id-value");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresFilesUpdateContract) {
    // SDK contract: vector_stores.files.update.
    types::vector_stores::FileUpdateParams params;
    params.attributes = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"last_error":{},"object":"resource","status":"resource","usage_bytes":7,"vector_store_id":"resource","attributes":{},"chunking_strategy":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.files.Update("file_id-value", "vector_store_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores/vector_store_id-value/files/file_id-value");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresFilesListContract) {
    // SDK contract: vector_stores.files.list.
    types::vector_stores::FileListParams params;
    params.after = "value";
    params.before = "value";
    params.filter = "value";
    params.limit = 7;
    params.order = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","created_at":7,"last_error":{},"object":"resource","status":"resource","usage_bytes":7,"vector_store_id":"resource","attributes":{},"chunking_strategy":{},"provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.files.List("vector_store_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores/vector_store_id-value/files");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresFilesDeleteContract) {
    // SDK contract: vector_stores.files.delete.
    types::vector_stores::VectorStoresFilesDeleteParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","deleted":true,"object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.files.Delete("file_id-value", "vector_store_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores/vector_store_id-value/files/file_id-value");
    CHECK(std::string_view(g_request->methodString()) == "DELETE");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresFilesContentContract) {
    // SDK contract: vector_stores.files.content.
    types::vector_stores::VectorStoresFilesContentParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"text":{},"type":{},"id":"resource","provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.files.Content("file_id-value", "vector_store_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores/vector_store_id-value/files/file_id-value/content");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresCreateContract) {
    // SDK contract: vector_stores.create.
    types::vector_stores::VectorStoreCreateParams params;
    params.chunking_strategy = json(R"JSON({"vendor":"value"})JSON");
    params.description = "value";
    params.expires_after = json(R"JSON({"vendor":"value"})JSON");
    params.file_ids = std::vector<std::string>{};
    params.metadata = json(R"JSON({"vendor":"value"})JSON");
    params.name = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"file_counts":{},"last_active_at":{},"metadata":{},"name":"resource","object":"resource","status":"resource","usage_bytes":7,"expires_after":{},"expires_at":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresRetrieveContract) {
    // SDK contract: vector_stores.retrieve.
    types::vector_stores::VectorStoresRetrieveParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"file_counts":{},"last_active_at":{},"metadata":{},"name":"resource","object":"resource","status":"resource","usage_bytes":7,"expires_after":{},"expires_at":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.Retrieve("vector_store_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores/vector_store_id-value");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresUpdateContract) {
    // SDK contract: vector_stores.update.
    types::vector_stores::VectorStoreUpdateParams params;
    params.expires_after = json(R"JSON({"vendor":"value"})JSON");
    params.metadata = json(R"JSON({"vendor":"value"})JSON");
    params.name = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"file_counts":{},"last_active_at":{},"metadata":{},"name":"resource","object":"resource","status":"resource","usage_bytes":7,"expires_after":{},"expires_at":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.Update("vector_store_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores/vector_store_id-value");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresListContract) {
    // SDK contract: vector_stores.list.
    types::vector_stores::VectorStoreListParams params;
    params.after = "value";
    params.before = "value";
    params.limit = 7;
    params.order = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","created_at":7,"file_counts":{},"last_active_at":{},"metadata":{},"name":"resource","object":"resource","status":"resource","usage_bytes":7,"expires_after":{},"expires_at":{},"provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.List(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresDeleteContract) {
    // SDK contract: vector_stores.delete.
    types::vector_stores::VectorStoresDeleteParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","deleted":true,"object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.Delete("vector_store_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores/vector_store_id-value");
    CHECK(std::string_view(g_request->methodString()) == "DELETE");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(VectorStoresSearchContract) {
    // SDK contract: vector_stores.search.
    types::vector_stores::VectorStoreSearchParams params;
    params.query = json(R"JSON({"vendor":"value"})JSON");
    params.filters = json(R"JSON({"vendor":"value"})JSON");
    params.max_num_results = 7;
    params.ranking_options = json(R"JSON({"vendor":"value"})JSON");
    params.rewrite_query = true;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"attributes":{},"content":[],"file_id":"resource","filename":"resource","score":0.5,"id":"resource","provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().vector_stores.Search("vector_store_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/vector_stores/vector_store_id-value/search");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("openai-beta") == "assistants=v2");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(WebhooksEventTypesListContract) {
    // SDK contract: webhooks.event_types.list.
    types::webhooks::WebhooksEventTypesListParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[],"object":"resource","id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().webhooks.event_types.List(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/webhook_event_types");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(WebhooksCreateContract) {
    // SDK contract: webhooks.create.
    types::webhooks::WebhookCreateParams params;
    params.event_types = std::vector<std::string>{};
    params.name = "value";
    params.url = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"event_types":[],"name":"resource","object":"resource","signing_secret":"resource","signing_secret_hint":{},"url":"resource","updated_at":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().webhooks.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/webhook_endpoints");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(WebhooksRetrieveContract) {
    // SDK contract: webhooks.retrieve.
    types::webhooks::WebhooksRetrieveParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"event_types":[],"name":"resource","object":"resource","signing_secret_hint":{},"url":"resource","updated_at":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().webhooks.Retrieve("webhook_endpoint_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/webhook_endpoints/webhook_endpoint_id-value");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(WebhooksUpdateContract) {
    // SDK contract: webhooks.update.
    types::webhooks::WebhookUpdateParams params;
    params.event_types = std::vector<std::string>{};
    params.name = "value";
    params.url = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"event_types":[],"name":"resource","object":"resource","signing_secret_hint":{},"url":"resource","updated_at":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().webhooks.Update("webhook_endpoint_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/webhook_endpoints/webhook_endpoint_id-value");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(WebhooksListContract) {
    // SDK contract: webhooks.list.
    types::webhooks::WebhookListParams params;
    params.after = json(R"JSON({"vendor":"value"})JSON");
    params.limit = 7;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","created_at":7,"event_types":[],"name":"resource","object":"resource","signing_secret_hint":{},"url":"resource","updated_at":{},"provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().webhooks.List(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/webhook_endpoints");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(WebhooksDeleteContract) {
    // SDK contract: webhooks.delete.
    types::webhooks::WebhooksDeleteParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","deleted":true,"object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().webhooks.Delete("webhook_endpoint_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/webhook_endpoints/webhook_endpoint_id-value");
    CHECK(std::string_view(g_request->methodString()) == "DELETE");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(WebhooksRotateSecretContract) {
    // SDK contract: webhooks.rotate_secret.
    types::webhooks::WebhookRotateSecretParams params;
    params.keep_old_secret_active_for_24_hours = true;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"event_types":[],"name":"resource","object":"resource","signing_secret":"resource","signing_secret_hint":{},"url":"resource","updated_at":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().webhooks.RotateSecret("webhook_endpoint_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/webhook_endpoints/webhook_endpoint_id-value/rotate_secret");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(WebhooksTestContract) {
    // SDK contract: webhooks.test.
    types::webhooks::WebhookTestParams params;
    params.event_type = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"event_type":"resource","object":"resource","status_code":7,"success":{},"webhook_endpoint_id":"resource","id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().webhooks.Test("webhook_endpoint_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/webhook_endpoints/webhook_endpoint_id-value/test");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(RealtimeCallsCreateContract) {
    // SDK contract: realtime.calls.create.
    types::realtime::CallCreateParams params;
    params.sdp = "value";
    params.session = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = true;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().realtime.calls.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/realtime/calls");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("accept") == "application/sdp");
    CHECK(result->parsed == std::string("byte\0data", 9));
    CHECK(g_request->getHeader("content-type").starts_with("multipart/form-data; boundary="));
}

DROGON_TEST(RealtimeCallsAcceptContract) {
    // SDK contract: realtime.calls.accept.
    types::realtime::CallAcceptParams params;
    params.type = "value";
    params.audio = json(R"JSON({"vendor":"value"})JSON");
    params.include = std::vector<std::string>{};
    params.instructions = "value";
    params.max_output_tokens = json(R"JSON({"vendor":"value"})JSON");
    params.model = "value";
    params.output_modalities = std::vector<std::string>{};
    params.parallel_tool_calls = true;
    params.prompt = json(R"JSON({"vendor":"value"})JSON");
    params.reasoning = json(R"JSON({"vendor":"value"})JSON");
    params.tool_choice = json(R"JSON({"vendor":"value"})JSON");
    params.tools = json(R"JSON({"vendor":"value"})JSON");
    params.tracing = json(R"JSON({"vendor":"value"})JSON");
    params.truncation = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = true;
    auto result{ drogon::sync_wait(client().realtime.calls.Accept("call_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/realtime/calls/call_id-value/accept");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("accept") == "*/*");
}

DROGON_TEST(RealtimeCallsHangupContract) {
    // SDK contract: realtime.calls.hangup.
    types::realtime::RealtimeCallsHangupParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = true;
    auto result{ drogon::sync_wait(client().realtime.calls.Hangup("call_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/realtime/calls/call_id-value/hangup");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("accept") == "*/*");
}

DROGON_TEST(RealtimeCallsReferContract) {
    // SDK contract: realtime.calls.refer.
    types::realtime::CallReferParams params;
    params.target_uri = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = true;
    auto result{ drogon::sync_wait(client().realtime.calls.Refer("call_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/realtime/calls/call_id-value/refer");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("accept") == "*/*");
}

DROGON_TEST(RealtimeCallsRejectContract) {
    // SDK contract: realtime.calls.reject.
    types::realtime::CallRejectParams params;
    params.status_code = 7;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = true;
    auto result{ drogon::sync_wait(client().realtime.calls.Reject("call_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/realtime/calls/call_id-value/reject");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("accept") == "*/*");
}

DROGON_TEST(RealtimeClientSecretsCreateContract) {
    // SDK contract: realtime.client_secrets.create.
    types::realtime::ClientSecretCreateParams params;
    params.expires_after = json(R"JSON({"vendor":"value"})JSON");
    params.session = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"expires_at":7,"session":{},"value":"resource","id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().realtime.client_secrets.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/realtime/client_secrets");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(RealtimeTranslationsClientSecretsCreateContract) {
    // SDK contract: realtime.translations.client_secrets.create.
    types::realtime::RealtimeTranslationsClientSecretsCreateParams params;
    params.session = json(R"JSON({"vendor":"value"})JSON");
    params.expires_after = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"expires_at":7,"session":{},"value":"resource","id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().realtime.translations.client_secrets.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/realtime/translations/client_secrets");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(LiveCreateContract) {
    // SDK contract: live.create.
    types::live::LiveCreateParams params;
    params.session = json(R"JSON({"vendor":"value"})JSON");
    params.transport = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"session":{},"transport":{},"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().live.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/live/sessions");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(LiveSessionsAcceptContract) {
    // SDK contract: live.sessions.accept.
    types::live::SessionAcceptParams params;
    params.session = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = true;
    auto result{ drogon::sync_wait(client().live.sessions.Accept("session_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/live/sessions/session_id-value/accept");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("accept") == "*/*");
}

DROGON_TEST(LiveSessionsDownloadRecordingContract) {
    // SDK contract: live.sessions.download_recording.
    types::live::LiveSessionsDownloadRecordingParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = true;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().live.sessions.DownloadRecording("session_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/live/sessions/session_id-value/content");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("accept") == "application/binary");
    CHECK(result->parsed == std::string("byte\0data", 9));
}

DROGON_TEST(LiveSessionsForkContract) {
    // SDK contract: live.sessions.fork.
    types::live::SessionForkParams params;
    params.transport = json(R"JSON({"vendor":"value"})JSON");
    params.session = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"session":{},"transport":{},"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().live.sessions.Fork("session_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/live/sessions/session_id-value/fork");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(LiveSessionsHangupContract) {
    // SDK contract: live.sessions.hangup.
    types::live::LiveSessionsHangupParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = true;
    auto result{ drogon::sync_wait(client().live.sessions.Hangup("session_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/live/sessions/session_id-value/hangup");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("accept") == "*/*");
}

DROGON_TEST(LiveSessionsReferContract) {
    // SDK contract: live.sessions.refer.
    types::live::SessionReferParams params;
    params.target_uri = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = true;
    auto result{ drogon::sync_wait(client().live.sessions.Refer("session_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/live/sessions/session_id-value/refer");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("accept") == "*/*");
}

DROGON_TEST(LiveSessionsRejectContract) {
    // SDK contract: live.sessions.reject.
    types::live::SessionRejectParams params;
    params.status_code = 7;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = true;
    auto result{ drogon::sync_wait(client().live.sessions.Reject("session_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/live/sessions/session_id-value/reject");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(g_request->getHeader("accept") == "*/*");
}

DROGON_TEST(FineTuningAlphaGradersRunContract) {
    // SDK contract: fine_tuning.alpha.graders.run.
    types::fine_tuning::GraderRunParams params;
    params.grader = json(R"JSON({"vendor":"value"})JSON");
    params.model_sample = "value";
    params.item = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"metadata":{},"api_model_grader_token_usage_per_model":{},"reward":0.5,"sub_rewards":{},"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().fine_tuning.alpha.graders.Run(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/fine_tuning/alpha/graders/run");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FineTuningAlphaGradersValidateContract) {
    // SDK contract: fine_tuning.alpha.graders.validate.
    types::fine_tuning::GraderValidateParams params;
    params.grader = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"grader":{},"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().fine_tuning.alpha.graders.Validate(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/fine_tuning/alpha/graders/validate");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FineTuningCheckpointsPermissionsCreateContract) {
    // SDK contract: fine_tuning.checkpoints.permissions.create.
    types::fine_tuning::PermissionCreateParams params;
    params.project_ids = std::vector<std::string>{};
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","created_at":7,"object":"resource","project_id":"resource","provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().fine_tuning.checkpoints.permissions.Create("fine_tuned_model_checkpoint-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/fine_tuning/checkpoints/fine_tuned_model_checkpoint-value/permissions");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FineTuningCheckpointsPermissionsRetrieveContract) {
    // SDK contract: fine_tuning.checkpoints.permissions.retrieve.
    types::fine_tuning::PermissionRetrieveParams params;
    params.after = "value";
    params.limit = 7;
    params.order = "value";
    params.project_id = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[],"has_more":true,"object":"resource","first_id":{},"last_id":{},"id":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().fine_tuning.checkpoints.permissions.Retrieve("fine_tuned_model_checkpoint-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/fine_tuning/checkpoints/fine_tuned_model_checkpoint-value/permissions");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FineTuningCheckpointsPermissionsListContract) {
    // SDK contract: fine_tuning.checkpoints.permissions.list.
    types::fine_tuning::PermissionListParams params;
    params.after = "value";
    params.limit = 7;
    params.order = "value";
    params.project_id = "value";
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","created_at":7,"object":"resource","project_id":"resource","provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().fine_tuning.checkpoints.permissions.List("fine_tuned_model_checkpoint-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/fine_tuning/checkpoints/fine_tuned_model_checkpoint-value/permissions");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FineTuningCheckpointsPermissionsDeleteContract) {
    // SDK contract: fine_tuning.checkpoints.permissions.delete.
    types::fine_tuning::FineTuningCheckpointsPermissionsDeleteParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","deleted":true,"object":"resource","provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().fine_tuning.checkpoints.permissions.Delete("permission_id-value", "fine_tuned_model_checkpoint-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/fine_tuning/checkpoints/fine_tuned_model_checkpoint-value/permissions/permission_id-value");
    CHECK(std::string_view(g_request->methodString()) == "DELETE");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FineTuningJobsCheckpointsListContract) {
    // SDK contract: fine_tuning.jobs.checkpoints.list.
    types::fine_tuning::CheckpointListParams params;
    params.after = "value";
    params.limit = 7;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","created_at":7,"fine_tuned_model_checkpoint":"resource","fine_tuning_job_id":"resource","metrics":{},"object":"resource","step_number":7,"provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().fine_tuning.jobs.checkpoints.List("fine_tuning_job_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/fine_tuning/jobs/fine_tuning_job_id-value/checkpoints");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FineTuningJobsCreateContract) {
    // SDK contract: fine_tuning.jobs.create.
    types::fine_tuning::JobCreateParams params;
    params.model = "value";
    params.training_file = "value";
    params.hyperparameters = json(R"JSON({"vendor":"value"})JSON");
    params.integrations = json(R"JSON({"vendor":"value"})JSON");
    params.metadata = json(R"JSON({"vendor":"value"})JSON");
    params.method = json(R"JSON({"vendor":"value"})JSON");
    params.seed = json(R"JSON({"vendor":"value"})JSON");
    params.suffix = json(R"JSON({"vendor":"value"})JSON");
    params.validation_file = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"error":{},"fine_tuned_model":{},"finished_at":{},"hyperparameters":{},"model":"resource","object":"resource","organization_id":"resource","result_files":[],"seed":7,"status":"resource","trained_tokens":{},"training_file":"resource","validation_file":{},"estimated_finish":{},"integrations":[],"metadata":{},"method":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().fine_tuning.jobs.Create(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/fine_tuning/jobs");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FineTuningJobsRetrieveContract) {
    // SDK contract: fine_tuning.jobs.retrieve.
    types::fine_tuning::FineTuningJobsRetrieveParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"error":{},"fine_tuned_model":{},"finished_at":{},"hyperparameters":{},"model":"resource","object":"resource","organization_id":"resource","result_files":[],"seed":7,"status":"resource","trained_tokens":{},"training_file":"resource","validation_file":{},"estimated_finish":{},"integrations":[],"metadata":{},"method":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().fine_tuning.jobs.Retrieve("fine_tuning_job_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/fine_tuning/jobs/fine_tuning_job_id-value");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FineTuningJobsListContract) {
    // SDK contract: fine_tuning.jobs.list.
    types::fine_tuning::JobListParams params;
    params.after = "value";
    params.limit = 7;
    params.metadata = json(R"JSON({"vendor":"value"})JSON");
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","created_at":7,"error":{},"fine_tuned_model":{},"finished_at":{},"hyperparameters":{},"model":"resource","object":"resource","organization_id":"resource","result_files":[],"seed":7,"status":"resource","trained_tokens":{},"training_file":"resource","validation_file":{},"estimated_finish":{},"integrations":[],"metadata":{},"method":{},"provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().fine_tuning.jobs.List(params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/fine_tuning/jobs");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FineTuningJobsCancelContract) {
    // SDK contract: fine_tuning.jobs.cancel.
    types::fine_tuning::FineTuningJobsCancelParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"error":{},"fine_tuned_model":{},"finished_at":{},"hyperparameters":{},"model":"resource","object":"resource","organization_id":"resource","result_files":[],"seed":7,"status":"resource","trained_tokens":{},"training_file":"resource","validation_file":{},"estimated_finish":{},"integrations":[],"metadata":{},"method":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().fine_tuning.jobs.Cancel("fine_tuning_job_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/fine_tuning/jobs/fine_tuning_job_id-value/cancel");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FineTuningJobsListEventsContract) {
    // SDK contract: fine_tuning.jobs.list_events.
    types::fine_tuning::JobListEventsParams params;
    params.after = "value";
    params.limit = 7;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"data":[{"id":"resource","created_at":7,"level":"resource","message":"resource","object":"resource","data":{},"type":{},"provider_extension":{"preserved":true}}],"has_more":false,"object":"list"})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().fine_tuning.jobs.ListEvents("fine_tuning_job_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/fine_tuning/jobs/fine_tuning_job_id-value/events");
    CHECK(std::string_view(g_request->methodString()) == "GET");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->data.size() == 1);
    CHECK(!result->HasNextPage());
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FineTuningJobsPauseContract) {
    // SDK contract: fine_tuning.jobs.pause.
    types::fine_tuning::FineTuningJobsPauseParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"error":{},"fine_tuned_model":{},"finished_at":{},"hyperparameters":{},"model":"resource","object":"resource","organization_id":"resource","result_files":[],"seed":7,"status":"resource","trained_tokens":{},"training_file":"resource","validation_file":{},"estimated_finish":{},"integrations":[],"metadata":{},"method":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().fine_tuning.jobs.Pause("fine_tuning_job_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/fine_tuning/jobs/fine_tuning_job_id-value/pause");
    CHECK(std::string_view(g_request->methodString()) == "POST");
    CHECK(g_request->getHeader("authorization") == "Bearer resource-key");
    CHECK(g_request->getHeader("x-resource-contract") == "yes");
    CHECK(result->raw["provider_extension"]["preserved"] == true);
    CHECK(result->_request_id == "req-resource");
}

DROGON_TEST(FineTuningJobsResumeContract) {
    // SDK contract: fine_tuning.jobs.resume.
    types::fine_tuning::FineTuningJobsResumeParams params;
    params.extra_headers["x-resource-contract"] = "yes";
    g_reply = json(R"JSON({"id":"resource","created_at":7,"error":{},"fine_tuned_model":{},"finished_at":{},"hyperparameters":{},"model":"resource","object":"resource","organization_id":"resource","result_files":[],"seed":7,"status":"resource","trained_tokens":{},"training_file":"resource","validation_file":{},"estimated_finish":{},"integrations":[],"metadata":{},"method":{},"provider_extension":{"preserved":true}})JSON");
    g_binary = false;
    g_text = false;
    g_void = false;
    auto result{ drogon::sync_wait(client().fine_tuning.jobs.Resume("fine_tuning_job_id-value", params)) };
    REQUIRE(result);
    REQUIRE(g_request);
    CHECK(g_request->getOriginalPath() == "/resources/fine_tuning/jobs/fine_tuning_job_id-value/resume");
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
