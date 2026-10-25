/**
 * @file transport.cpp
 * @brief Implement the HTTP connection that the API groups of one `Llm` share.
 */
#include "details/transport.hpp"

#include <format>
#include <utility>

namespace {

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
     * @brief Describe a non-2xx response.
     * @param response Response from the endpoint.
     * @return The OpenAI-format `error.message` when the body has one, otherwise the raw body.
     */
    auto status_error_message(drogon::HttpResponse const& response) -> std::string {
        auto const& json{ response.getJsonObject() };
        if (json != nullptr && json->isObject()) {
            Json::Value const& error{ (*json)["error"] };
            if (error.isObject() && error["message"].isString()) {
                return error["message"].asString();
            }
        }
        return std::string{ response.body() };
    }

} // namespace

namespace drogon_llm::details {

    /**
     * @brief Create an HTTP client for the scheme, host, and port of `config.base_url`.
     * @param config Endpoint settings; `api_key` may be empty for endpoints without auth.
     */
    Transport::Transport(LlmConfig config)
        : m_config{ std::move(config) } {
        auto [host, path_prefix]{ split_base_url(m_config.base_url) };
        m_path_prefix = std::move(path_prefix);
        m_http_client = drogon::HttpClient::newHttpClient(host);
    }

    /**
     * @brief Return the endpoint settings.
     * @return Settings passed to the constructor.
     */
    auto Transport::Config() const noexcept -> LlmConfig const& {
        return m_config;
    }

    /**
     * @brief POST a JSON body and wait for the JSON response.
     * @param path Path below `base_url`, e.g. `/chat/completions`.
     * @param body Request body.
     * @param timeout Seconds to wait for the response; 0 waits forever.
     * @return The parsed body of a 2xx response, or why the request failed.
     * @note Blocks the calling thread; never call it on the main event loop's thread.
     */
    auto Transport::Post(
        std::string_view path,
        Json::Value const& body,
        double timeout
    ) -> Result<Json::Value> {
        auto request{ drogon::HttpRequest::newHttpJsonRequest(body) };
        request->setMethod(drogon::Post);
        request->setPath(m_path_prefix + std::string{ path });
        if (!m_config.api_key.empty()) {
            request->addHeader("Authorization", "Bearer " + m_config.api_key);
        }

        auto const [result, response]{ m_http_client->sendRequest(request, timeout) };
        if (result != drogon::ReqResult::Ok) {
            return std::unexpected{ Error{
                .kind = result == drogon::ReqResult::Timeout ? ErrorKind::Timeout : ErrorKind::Connection,
                .message = std::format("POST {}{}: {}", m_config.base_url, path, drogon::to_string_view(result)),
            } };
        }

        auto const status{ static_cast<int>(response->getStatusCode()) };
        if (status < 200 || status >= 300) {
            return std::unexpected{ Error{
                .kind = ErrorKind::Status,
                .message = std::format("HTTP {}: {}", status, status_error_message(*response)),
                .status_code = status,
                .body = std::string{ response->body() },
            } };
        }

        auto const& json{ response->getJsonObject() };
        if (json == nullptr) {
            return std::unexpected{ Error{
                .kind = ErrorKind::ResponseValidation,
                .message = std::format("response body is not JSON: {}", response->getJsonError()),
                .body = std::string{ response->body() },
            } };
        }
        return *json;
    }

} // namespace drogon_llm::details
