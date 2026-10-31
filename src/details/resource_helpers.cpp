/**
 * @file resource_helpers.cpp
 * @brief Implement structured Responses parsing, file processing and upload polling helpers.
 */
#include <algorithm>
#include <cctype>
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/openai_client.hpp"

namespace drogon_openai::details {
    namespace {
        /**
         * @brief Apply a helper header only when no casing variant was supplied.
         * @param headers Explicit request overrides.
         * @param name Default header name.
         * @param value Default header value.
         */
        auto default_header(Headers& headers, std::string name, std::string value) -> void {
            auto lower{ [](std::string text) {
                for (auto& c : text) {
                    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                }
                return text;
            } };
            auto const normalized{ lower(name) };
            if (std::none_of(headers.begin(), headers.end(), [&](auto const& header) { return lower(header.first) == normalized; })) {
                headers.emplace(std::move(name), std::move(value));
            }
        }

        /**
         * @brief Validate response parsing options before making a request.
         * @param params Request fields and JSON schema.
         * @param strict Whether non-strict function tools must be rejected.
         * @return Valid configuration or invalid-request error.
         */
        auto validate_response_parse(types::responses::ResponsesCreateParams const& params, bool strict = true) -> Result<void> {
            if (params.text && !params.text->isNull() && !params.text->isObject()) {
                return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "Responses text must be an object" } };
            }
            if (params.tools && !params.tools->isArray()) {
                return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "Responses tools must be an array" } };
            }
            if (strict && params.tools) {
                for (auto const& tool : *params.tools) {
                    if (!tool.isObject() || (tool["type"] == "function" && tool["strict"] != true)) {
                        return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "automatic function parsing requires strict: true" } };
                    }
                }
            }
            return {};
        }

        /**
         * @brief Decode structured final-answer text and strict function calls.
         * @param response Resource to enrich; raw remains the original JSON.
         * @param params Request schema and tool definitions.
         * @return Success or JSON validation error.
         */
        auto parse_response(types::responses::Response& response, types::responses::ResponsesCreateParams const& params) -> Result<void> {
            if (!response.output.isArray()) {
                return {};
            }
            bool const structured{ params.text && params.text->isObject() && (*params.text)["format"].isObject() && (*params.text)["format"]["type"] == "json_schema" };
            for (auto& output : response.output) {
                if (!output.isObject()) {
                    continue;
                }
                if (structured && output["type"] == "message" && output["content"].isArray() && (output["phase"].isNull() || output["phase"] == "final_answer")) {
                    for (auto& part : output["content"]) {
                        if (!part.isObject() || part["type"] != "output_text" || !part["text"].isString()) {
                            continue;
                        }
                        auto parsed{ decode_json(part["text"].asString()) };
                        if (!parsed) {
                            auto error{ parsed.error() };
                            error.request_id = response._request_id;
                            return std::unexpected{ std::move(error) };
                        }
                        part["parsed"] = *parsed;
                        if (!response.output_parsed) {
                            response.output_parsed = std::move(*parsed);
                        }
                    }
                } else if (output["type"] == "function_call" && params.tools && params.tools->isArray()) {
                    for (auto const& tool : *params.tools) {
                        if (!tool.isObject() || tool["type"] != "function" || tool["strict"] != true || tool["name"] != output["name"]) {
                            continue;
                        }
                        if (!output["arguments"].isString() || output["arguments"].asString().empty()) {
                            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "function arguments are missing JSON", .request_id = response._request_id } };
                        }
                        auto parsed{ decode_json(output["arguments"].asString()) };
                        if (!parsed) {
                            return std::unexpected{ std::move(parsed.error()) };
                        }
                        output["parsed_arguments"] = std::move(*parsed);
                    }
                }
            }
            return {};
        }

        /**
         * @brief Preserve metadata while polling a typed resource.
         * @tparam T Resource type.
         * @param transport Shared settings.
         * @param path Resource path.
         * @param poll Polling conditions and absolute deadline.
         * @param options Request overrides.
         * @return Terminal typed resource or an error.
         */
        template <typename T>
        auto poll_typed(std::shared_ptr<Transport> transport, std::string path, PollOptions poll, RequestOptions options) -> drogon::Task<Result<T>> {
            OpenAI client{ transport->Config() };
            default_header(options.extra_headers, "X-Stainless-Poll-Helper", "true");
            auto response{ co_await client.PollRaw(std::move(path), std::move(poll), std::move(options)) };
            if (!response) {
                co_return std::unexpected{ std::move(response.error()) };
            }
            auto decoded{ resource_decode_response<T>(std::move(*response)) };
            if (!decoded) {
                co_return std::unexpected{ std::move(decoded.error()) };
            }
            co_return std::move(decoded->parsed);
        }
    } // namespace

    /**
     * @brief Create and parse JSON Schema output and strict function arguments.
     * @param params Owned operation parameters.
     * @return Operation result or an error.
     */
    auto Responses::Parse(
        types::responses::ResponsesCreateParams params
    ) const -> drogon::Task<Result<types::responses::Response>> {

        auto group{ *this };
        auto valid{ validate_response_parse(params) };
        if (!valid) {
            co_return std::unexpected{ std::move(valid.error()) };
        }
        auto response{ co_await group.CreateRaw(params) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ parse_response(response->parsed, params) };
        if (!parsed) {
            auto error{ std::move(parsed.error()) };
            error.status_code = response->status_code;
            error.headers = response->headers;
            error.body = response->body;
            error.request_id = response->request_id;
            co_return std::unexpected{ std::move(error) };
        }
        co_return std::move(response->parsed);
    }

} // namespace drogon_openai::details
