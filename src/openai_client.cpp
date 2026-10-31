/**
 * @file openai_client.cpp
 * @brief Implement the `OpenAI` client and the drogon plugin that holds its endpoint settings.
 */
#include "drogon_openai/openai_client.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <drogon/utils/Utilities.h>

#include "details/transport.hpp"

namespace {

    /**
     * @brief Read an environment variable of this process.
     *
     * Windows uses `_dupenv_s`, which returns a copy, because the UCRT deprecates `getenv`.
     *
     * @param name Variable name.
     * @return The variable's value, or an empty string when it is unset.
     */
    auto read_env(char const* name) -> std::string {
#ifdef _WIN32
        char* buffer{ nullptr };
        std::size_t size{ 0 };
        if (_dupenv_s(&buffer, &size, name) != 0 || buffer == nullptr) {
            return {};
        }
        std::unique_ptr<char, decltype(&std::free)> const owner{ buffer, &std::free };
        return std::string{ buffer };
#else
        char const* value{ std::getenv(name) };
        return value != nullptr ? value : "";
#endif
    }

    /**
     * @brief Read the API key from the named environment variable.
     * @param env_name Variable name from `api_key_env`; empty when the endpoint needs no key.
     * @return The variable's value, or an empty string when `env_name` is empty.
     * @throws std::invalid_argument If the variable is unset or empty.
     */
    auto read_api_key(std::string const& env_name) -> std::string {
        if (env_name.empty()) {
            return {};
        }
        std::string value{ read_env(env_name.c_str()) };
        if (value.empty()) {
            throw std::invalid_argument{ std::format("drogon_openai::OpenAIClient: environment variable {} is unset or empty", env_name) };
        }
        return value;
    }

} // namespace

namespace drogon_openai {

    /**
     * @brief Create a client for the endpoint in `config`.
     * @param config Endpoint settings; `api_key` may be empty for endpoints without auth.
     */
    OpenAI::OpenAI(OpenAIConfig config)
        : m_transport{ std::make_shared<details::Transport>(std::move(config)) }, chat{ m_transport }, models{ m_transport }, embeddings{ m_transport }, completions{ m_transport }, responses{ m_transport }, conversations{ m_transport }, moderations{ m_transport }, files{ m_transport }, uploads{ m_transport } {}

    /**
     * @brief Copy this client with selected settings overridden.
     * @param options Settings to override.
     * @return Independent value handle; the original settings remain valid.
     */
    auto OpenAI::WithOptions(OpenAIOptions options) const -> OpenAI {
        auto config{ Config() };
        if (options.base_url) {
            config.base_url = std::move(*options.base_url);
        }
        if (options.api_key) {
            config.api_key = std::move(*options.api_key);
        }
        if (options.model) {
            config.model = std::move(*options.model);
        }
        if (options.max_retries) {
            config.max_retries = *options.max_retries;
        }
        if (options.timeout) {
            config.timeout = *options.timeout;
        }
        if (options.organization) {
            config.organization = std::move(*options.organization);
        }
        if (options.project) {
            config.project = std::move(*options.project);
        }
        if (options.default_headers) {
            Headers merged;
            for (auto const* source : { &config.default_headers, &*options.default_headers }) {
                for (auto const& [name, value] : *source) {
                    std::string key{ name };
                    std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
                    merged[key] = value;
                }
            }
            config.default_headers = std::move(merged);
        }
        if (options.default_query) {
            if (!options.default_query->isObject() && !options.default_query->isNull()) {
                throw std::invalid_argument{ "default_query must be a JSON object" };
            }
            for (auto const& name : options.default_query->getMemberNames()) {
                config.default_query[name] = (*options.default_query)[name];
            }
        }
        return OpenAI{ std::move(config) };
    }

    /**
     * @brief Copy this client with selected settings overridden.
     * @param options Settings to override.
     * @return Independent value handle.
     */
    auto OpenAI::Copy(OpenAIOptions options) const -> OpenAI {
        return WithOptions(std::move(options));
    }

    /**
     * @brief Return the immutable client configuration.
     * @return Current endpoint and request defaults.
     */
    auto OpenAI::Config() const noexcept -> OpenAIConfig const& {
        return m_transport->Config();
    }

    /**
     * @brief Send JSON and retain response headers and raw bytes.
     * @param method HTTP method.
     * @param path API path.
     * @param body JSON request body.
     * @param options Request overrides.
     * @return Raw response with parsed JSON or an error.
     */
    auto OpenAI::RequestRaw(
        std::string method,
        std::string path,
        Json::Value body,
        RequestOptions options
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto const transport{ m_transport };
        co_return co_await transport->Request(std::move(method), std::move(path), std::move(body), std::move(options));
    }

    /**
     * @brief Request an unwrapped API path and parse its JSON response.
     * @param method HTTP method, e.g. `GET` or `POST`.
     * @param path Path relative to base_url.
     * @param body JSON body; null omits the body.
     * @param options Request overrides.
     * @return JSON value or a request error; an empty successful body yields null.
     */
    auto OpenAI::Request(
        std::string method,
        std::string path,
        Json::Value body,
        RequestOptions options
    ) const -> drogon::Task<Result<Json::Value>> {
        auto const transport{ m_transport };
        auto response{ co_await transport->Request(std::move(method), std::move(path), std::move(body), std::move(options)) };
        if (!response) {
            co_return std::unexpected{ response.error() };
        }
        co_return std::move(response->parsed);
    }

    /**
     * @brief GET an unwrapped JSON path.
     * @param path Path below base_url.
     * @param options Request overrides.
     * @return Parsed JSON or an error.
     */
    auto OpenAI::Get(std::string path, RequestOptions options) const -> drogon::Task<Result<Json::Value>> {
        auto const transport{ m_transport };
        auto response{ co_await transport->Request("GET", std::move(path), {}, std::move(options)) };
        if (!response) {
            co_return std::unexpected{ response.error() };
        }
        co_return std::move(response->parsed);
    }

    /**
     * @brief DELETE an unwrapped JSON path.
     * @param path Path below base_url.
     * @param options Request overrides.
     * @return Parsed JSON or an error.
     */
    auto OpenAI::Delete(std::string path, RequestOptions options) const -> drogon::Task<Result<Json::Value>> {
        auto const transport{ m_transport };
        auto response{ co_await transport->Request("DELETE", std::move(path), {}, std::move(options)) };
        if (!response) {
            co_return std::unexpected{ response.error() };
        }
        co_return std::move(response->parsed);
    }

    /**
     * @brief POST an unwrapped JSON path.
     * @param path Path below base_url.
     * @param body JSON request body.
     * @param options Request overrides.
     * @return Parsed JSON or an error.
     */
    auto OpenAI::Post(
        std::string path,
        Json::Value body,
        RequestOptions options
    ) const -> drogon::Task<Result<Json::Value>> {
        auto const transport{ m_transport };
        auto response{ co_await transport->Request("POST", std::move(path), std::move(body), std::move(options)) };
        if (!response) {
            co_return std::unexpected{ response.error() };
        }
        co_return std::move(response->parsed);
    }

    /**
     * @brief PUT an unwrapped JSON path.
     * @param path Path below base_url.
     * @param body JSON request body.
     * @param options Request overrides.
     * @return Parsed JSON or an error.
     */
    auto OpenAI::Put(
        std::string path,
        Json::Value body,
        RequestOptions options
    ) const -> drogon::Task<Result<Json::Value>> {
        auto const transport{ m_transport };
        auto response{ co_await transport->Request("PUT", std::move(path), std::move(body), std::move(options)) };
        if (!response) {
            co_return std::unexpected{ response.error() };
        }
        co_return std::move(response->parsed);
    }

    /**
     * @brief PATCH an unwrapped JSON path.
     * @param path Path below base_url.
     * @param body JSON request body.
     * @param options Request overrides.
     * @return Parsed JSON or an error.
     */
    auto OpenAI::Patch(
        std::string path,
        Json::Value body,
        RequestOptions options
    ) const -> drogon::Task<Result<Json::Value>> {
        auto const transport{ m_transport };
        auto response{ co_await transport->Request("PATCH", std::move(path), std::move(body), std::move(options)) };
        if (!response) {
            co_return std::unexpected{ response.error() };
        }
        co_return std::move(response->parsed);
    }

    /**
     * @brief List an unwrapped collection with automatic or manual pagination.
     * @param path API collection path.
     * @param options Query and request overrides retained across pages.
     * @param cursor False for a single SDK-style page without cursor traversal.
     * @return Owned JSON page or an error.
     */
    auto OpenAI::List(
        std::string path,
        RequestOptions options,
        bool cursor
    ) const -> drogon::Task<Result<JsonPage>> {
        auto const transport{ m_transport };
        auto owned_client{ *this };
        auto response{ co_await transport->Request("GET", path, {}, options) };
        if (!response) {
            co_return std::unexpected{ response.error() };
        }
        auto next{ [client = std::move(owned_client), path, options](std::string after) mutable {
            auto query{ options };
            query.extra_query["after"] = std::move(after);
            return client.List(path, std::move(query));
        } };
        co_return JsonPage::FromResponse(std::move(*response), {}, std::move(next), cursor);
    }

    /**
     * @brief GET binary content without interpreting it as JSON.
     * @param path API path.
     * @param options Request overrides.
     * @return Response bytes and metadata or an error.
     */
    auto OpenAI::GetBinary(
        std::string path,
        RequestOptions options
    ) const -> drogon::Task<Result<RawResponse<std::string>>> {
        auto const transport{ m_transport };
        co_return co_await transport->Send("GET", std::move(path), "", "", std::move(options));
    }

    /**
     * @brief POST multipart form fields and files.
     * @param path API path.
     * @param parts Form fields and files, in order.
     * @param options Request overrides.
     * @return Parsed JSON response or an error.
     */
    auto OpenAI::PostMultipart(
        std::string path,
        std::vector<MultipartPart> parts,
        RequestOptions options
    ) const -> drogon::Task<Result<Json::Value>> {
        auto const transport{ m_transport };
        auto const boundary{ "drogon-openai-" + drogon::utils::genRandomString(32) };
        std::string body;
        for (auto const& part : parts) {
            auto quote{ [](std::string const& value) {
                std::string escaped;
                for (char c : value) {
                    if (c == '"') {
                        escaped += "%22";
                    } else if (c == '\r') {
                        escaped += "%0D";
                    } else if (c == '\n') {
                        escaped += "%0A";
                    } else if (c == '\\') {
                        escaped += "\\\\";
                    } else {
                        escaped += c;
                    }
                }
                return escaped;
            } };
            if (part.name.find('\0') != std::string::npos || (part.filename && part.filename->find('\0') != std::string::npos) || (part.content_type && (part.content_type->find_first_of("\r\n") != std::string::npos || part.content_type->find('\0') != std::string::npos))) {
                co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "invalid multipart disposition or media type" } };
            }
            body += "--" + boundary + "\r\nContent-Disposition: form-data; name=\"" + quote(part.name) + "\"";
            if (part.filename) {
                body += "; filename=\"" + quote(*part.filename) + "\"";
            }
            body += "\r\n";
            if (part.content_type || part.filename) {
                body += "Content-Type: " + part.content_type.value_or("application/octet-stream") + "\r\n";
            }
            body += "\r\n";
            body += part.content;
            body += "\r\n";
        }
        body += "--" + boundary + "--\r\n";
        auto response{ co_await transport->Send("POST", std::move(path), std::move(body), "multipart/form-data; boundary=" + boundary, std::move(options)) };
        if (!response) {
            co_return std::unexpected{ response.error() };
        }
        auto parsed{ details::decode_json(response->body) };
        if (!parsed) {
            auto error{ parsed.error() };
            error.headers = response->headers;
            error.request_id = response->request_id;
            error.status_code = response->status_code;
            co_return std::unexpected{ std::move(error) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Create an unwrapped resource and poll its collection/id path to a terminal state.
     * @param path Collection POST path.
     * @param body Creation JSON body.
     * @param poll Polling options; the total deadline starts after creation.
     * @param options Request overrides for creation and polling.
     * @return Terminal resource or an error.
     */
    auto OpenAI::PostAndPoll(
        std::string path,
        Json::Value body,
        PollOptions poll,
        RequestOptions options
    ) const -> drogon::Task<Result<Json::Value>> {
        auto client{ *this };
        auto resource{ co_await client.Post(path, std::move(body), options) };
        if (!resource) {
            co_return std::unexpected{ resource.error() };
        }
        if (!resource->isObject() || !(*resource)["id"].isString() || (*resource)["id"].asString().empty()) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "created resource has no string id", .body = resource->toStyledString() } };
        }
        auto const query{ path.find('?') };
        auto collection{ path.substr(0, query) };
        while (collection.ends_with('/')) {
            collection.pop_back();
        }
        auto target{ collection + "/" + details::encode_path_segment((*resource)["id"].asString()) };
        if (query != std::string::npos) {
            target += path.substr(query);
        }
        co_return co_await client.Poll(std::move(target), std::move(poll), std::move(options));
    }

    /**
     * @brief Upload an unwrapped resource and poll its collection/id path to a terminal state.
     * @param path Collection upload path.
     * @param parts Multipart fields and files.
     * @param poll Polling options; the total deadline starts after creation.
     * @param options Request overrides for upload and polling.
     * @return Terminal resource or an error.
     */
    auto OpenAI::UploadAndPoll(
        std::string path,
        std::vector<MultipartPart> parts,
        PollOptions poll,
        RequestOptions options
    ) const -> drogon::Task<Result<Json::Value>> {
        auto client{ *this };
        auto resource{ co_await client.PostMultipart(path, std::move(parts), options) };
        if (!resource) {
            co_return std::unexpected{ resource.error() };
        }
        if (!resource->isObject() || !(*resource)["id"].isString() || (*resource)["id"].asString().empty()) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "created resource has no string id", .body = resource->toStyledString() } };
        }
        auto const query{ path.find('?') };
        auto collection{ path.substr(0, query) };
        while (collection.ends_with('/')) {
            collection.pop_back();
        }
        auto target{ collection + "/" + details::encode_path_segment((*resource)["id"].asString()) };
        if (query != std::string::npos) {
            target += path.substr(query);
        }
        co_return co_await client.Poll(std::move(target), std::move(poll), std::move(options));
    }

    /**
     * @brief Poll an unwrapped resource until it reaches a terminal state.
     * @param path Resource GET path.
     * @param poll Polling interval, deadline, and terminal states.
     * @param options Request overrides, applied to each poll.
     * @return Terminal resource or an error.
     */
    auto OpenAI::Poll(
        std::string path,
        PollOptions poll,
        RequestOptions options
    ) const -> drogon::Task<Result<Json::Value>> {

        auto client{ *this };
        auto response{ co_await client.PollRaw(std::move(path), std::move(poll), std::move(options)) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        co_return std::move(response->parsed);
    }

    /**
     * @brief Poll a resource and preserve the final HTTP response metadata.
     * @param path Resource GET path.
     * @param poll Polling interval, deadline, and terminal states.
     * @param options Request overrides, applied to each poll.
     * @return Terminal raw resource or an error.
     */
    auto OpenAI::PollRaw(
        std::string path,
        PollOptions poll,
        RequestOptions options
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto const transport{ m_transport };
        if (!std::isfinite(poll.interval) || poll.interval < 0 || !std::isfinite(poll.timeout) || poll.timeout < 0 || poll.terminal_states.empty()) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "invalid polling interval, deadline, or terminal states" } };
        }
        auto const start{ std::chrono::steady_clock::now() };
        std::optional<std::chrono::steady_clock::time_point> deadline;
        if (poll.timeout > 0) {
            deadline = start + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>{ poll.timeout });
        }
        while (true) {
            double const elapsed{ std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count() };
            double const remaining{ poll.timeout - elapsed };
            if (poll.timeout > 0 && remaining <= 0) {
                co_return std::unexpected{ Error{ .kind = ErrorKind::Timeout, .message = "polling deadline exceeded" } };
            }
            auto request_options{ options };
            if (poll.timeout > 0) {
                auto phase{ options.timeout.value_or(transport->Config().timeout) };
                auto cap{ [remaining](double value) { return value == 0 ? remaining : std::min(value, remaining); } };
                phase.connect = cap(phase.connect);
                phase.read = cap(phase.read);
                phase.write = cap(phase.write);
                request_options.timeout = phase;
            }
            auto response{ co_await transport->Request("GET", path, {}, std::move(request_options), deadline) };
            if (!response) {
                co_return std::unexpected{ response.error() };
            }
            if (!response->parsed.isObject() || !response->parsed[poll.status_field].isString()) {
                co_return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "poll response has no string status", .status_code = response->status_code, .body = response->body, .request_id = response->request_id, .headers = response->headers } };
            }
            auto const status{ response->parsed[poll.status_field].asString() };
            if (std::find(poll.terminal_states.begin(), poll.terminal_states.end(), status) != poll.terminal_states.end()) {
                co_return std::move(*response);
            }
            double delay{ poll.interval };
            if (auto const it{ response->headers.find("openai-poll-after-ms") }; it != response->headers.end()) {
                char* end{};
                double const value{ std::strtod(it->second.c_str(), &end) };
                if (end != it->second.c_str() && *end == '\0' && std::isfinite(value) && value >= 0) {
                    delay = value / 1000;
                }
            }
            if (poll.timeout > 0) {
                delay = std::min(delay, std::max(0.0, poll.timeout - std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count()));
            }
            co_await drogon::sleepCoro(details::current_loop(), delay);
        }
    }

    /**
     * @brief Read the endpoint settings from the plugin's `config` object.
     * @param config The plugin's `config` object from config.json.
     * @throws std::invalid_argument If `base_url` or `model` is missing or empty,
     * or if `api_key_env` names an unset or empty environment variable, or request defaults are invalid.
     */
    auto OpenAIClient::initAndStart(Json::Value const& config) -> void {
        m_config = OpenAIConfig{
            .base_url = config.get("base_url", "").asString(),
            .api_key = read_api_key(config.get("api_key_env", "").asString()),
            .model = config.get("model", "").asString(),
        };
        if (m_config.base_url.empty() || m_config.model.empty()) {
            throw std::invalid_argument{ "drogon_openai::OpenAIClient: config needs non-empty base_url and model" };
        }
        auto invalid{ [] { throw std::invalid_argument{ "drogon_openai::OpenAIClient: invalid request defaults" }; } };
        if (config.isMember("max_retries")) {
            if (!config["max_retries"].isInt() || config["max_retries"].asInt() < 0) {
                invalid();
            }
            m_config.max_retries = config["max_retries"].asInt();
        }
        if (config.isMember("timeout")) {
            auto const& value{ config["timeout"] };
            if (value.isNumeric()) {
                m_config.timeout = Timeout{ value.asDouble() };
            } else if (value.isObject()) {
                for (auto const& [name, field] : { std::pair{ "connect", &m_config.timeout.connect }, { "read", &m_config.timeout.read }, { "write", &m_config.timeout.write } }) {
                    if (value.isMember(name)) {
                        if (!value[name].isNumeric()) {
                            invalid();
                        }
                        *field = value[name].asDouble();
                    }
                }
            } else {
                invalid();
            }
            for (double duration : { m_config.timeout.connect, m_config.timeout.read, m_config.timeout.write }) {
                if (!std::isfinite(duration) || duration < 0) {
                    invalid();
                }
            }
        }
        if (config.isMember("default_headers")) {
            if (!config["default_headers"].isObject()) {
                invalid();
            }
            for (auto const& name : config["default_headers"].getMemberNames()) {
                if (!config["default_headers"][name].isString()) {
                    invalid();
                }
                m_config.default_headers[name] = config["default_headers"][name].asString();
            }
        }
        if (config.isMember("default_query")) {
            if (!config["default_query"].isObject()) {
                invalid();
            }
            m_config.default_query = config["default_query"];
        }
        for (auto const& [name, field] : { std::pair{ "organization", &m_config.organization }, { "project", &m_config.project } }) {
            if (config.isMember(name)) {
                if (!config[name].isString()) {
                    invalid();
                }
                *field = config[name].asString();
            }
        }
    }

    /**
     * @brief Release resources on application shutdown.
     */
    auto OpenAIClient::shutdown() -> void {}

    /**
     * @brief Return the settings parsed from config.json.
     * @return Settings valid after drogon has initialized the plugin.
     */
    auto OpenAIClient::Config() const noexcept -> OpenAIConfig const& {
        return m_config;
    }

    /**
     * @brief Create an `OpenAI` client for the configured endpoint.
     * @return A new client; call only after drogon has initialized the plugin.
     */
    auto OpenAIClient::NewOpenAIClient() const -> OpenAI {
        return OpenAI{ m_config };
    }

} // namespace drogon_openai
