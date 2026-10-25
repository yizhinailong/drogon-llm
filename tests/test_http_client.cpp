/**
 * @file test_http_client.cpp
 * @brief Send one chat completion request to the configured LLM endpoint with drogon::HttpClient.
 */
#include <print>
#include <string>
#include <string_view>
#include <utility>

#include <drogon/drogon.h>
#include <drogon/utils/coroutine.h>

#include <drogon_llm/llm_client.hpp>

namespace {

    constexpr std::string_view PROMPT{ "Introduce yourself." }; ///< User message to send.
    constexpr double REQUEST_TIMEOUT{ 120.0 };                  ///< Seconds; thinking mode can be slow.

    /**
     * @brief Split a base URL into the host part for HttpClient and the path prefix.
     * @param base_url Base URL such as `https://api.openai.com/v1`; a trailing `/` is ignored.
     * @return Host and path prefix, e.g. `https://api.openai.com` and `/v1`; the prefix is empty without a path.
     */
    auto split_base_url(std::string_view base_url) -> std::pair<std::string, std::string> {
        if (base_url.ends_with('/')) {
            base_url.remove_suffix(1);
        }
        auto const scheme_end{ base_url.find("://") };
        auto const path_begin{ base_url.find('/', scheme_end == std::string_view::npos ? 0 : scheme_end + 3) };
        if (path_begin == std::string_view::npos) {
            return { std::string{ base_url }, std::string{} };
        }
        return {
            std::string{ base_url.substr(0, path_begin) },
            std::string{ base_url.substr(path_begin) }
        };
    }

    /**
     * @brief POST `PROMPT` to `<base_url>/chat/completions` and print the reply.
     * @param config Endpoint settings from the LlmClient plugin.
     * @return True if the endpoint answered with HTTP 200 and a JSON body.
     */
    auto send_chat(drogon_llm::LlmConfig config) -> drogon::Task<bool> {
        auto const [host, path_prefix]{ split_base_url(config.base_url) };
        auto client{ drogon::HttpClient::newHttpClient(host) };

        Json::Value message{};
        message["role"] = "user";
        message["content"] = std::string{ PROMPT };
        Json::Value body{};
        body["model"] = config.model;
        body["messages"].append(message);

        auto request{ drogon::HttpRequest::newHttpJsonRequest(body) };
        request->setMethod(drogon::Post);
        request->setPath(path_prefix + "/chat/completions");
        if (!config.api_key.empty()) {
            request->addHeader("Authorization", "Bearer " + config.api_key);
        }

        std::println("POST {}{}/chat/completions (model: {})", host, path_prefix, config.model);
        try {
            auto const response{ co_await client->sendRequestCoro(request, REQUEST_TIMEOUT) };
            auto const& json{ response->getJsonObject() };
            if (response->getStatusCode() != drogon::k200OK || json == nullptr) {
                std::println(
                    stderr,
                    "HTTP {}: {}",
                    static_cast<int>(response->getStatusCode()),
                    response->body()
                );
                co_return false;
            }

            Json::Value const& root{ *json };
            Json::Value const& reply{ root["choices"][0]["message"] };
            if (reply.isMember("reasoning_content")) {
                std::println("reasoning: {}", reply["reasoning_content"].asString());
            }
            std::println("content: {}", reply["content"].asString());

            Json::Value const& usage{ root["usage"] };
            std::println(
                "tokens: prompt {} + completion {} = {}",
                usage["prompt_tokens"].asInt(),
                usage["completion_tokens"].asInt(),
                usage["total_tokens"].asInt()
            );
            co_return true;
        } catch (drogon::HttpException const& e) {
            std::println(stderr, "request failed: {}", e.what());
            co_return false;
        }
    }

} // namespace

auto main() -> int {
    int exit_code{ 1 };
    drogon::app()
        .loadConfigFile("config.json")
        .registerBeginningAdvice([&exit_code] {
            drogon::async_run([&exit_code]() -> drogon::Task<> {
                bool const ok{
                    co_await send_chat(drogon::app().getPlugin<drogon_llm::LlmClient>()->Config())
                };
                exit_code = ok ? 0 : 1;
                drogon::app().quit();
            });
        })
        .run();
    return exit_code;
}
