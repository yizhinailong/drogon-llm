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
