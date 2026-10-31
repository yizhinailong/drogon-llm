/**
 * @file completions.cpp
 * @brief Implement creation, parsing, streaming, and management of chat completions.
 */
#include <utility>

#include <json/writer.h>

#include "details/chat_json.hpp"
#include "details/transport.hpp"
#include "drogon_openai/details/completions.hpp"

namespace drogon_openai::details {

    namespace {

        /**
         * @brief Carry response metadata into a decoding error.
         * @tparam T HTTP response payload type.
         * @param error Decoding error to enrich.
         * @param response Original HTTP response.
         * @return Error with original status, bytes, headers, and request ID.
         */
        template <typename T>
        auto response_error(Error error, RawResponse<T> const& response) -> Error {
            error.status_code = response.status_code;
            error.body = response.body;
            error.request_id = response.request_id;
            error.headers = response.headers;
            return error;
        }

        /**
         * @brief Convert chat request overrides into common transport options.
         * @param params Chat parameters.
         * @return HTTP request options.
         */
        auto request_options(ChatCompletionCreateParams const& params) -> RequestOptions {
            return {
                .timeout = params.timeout,
                .max_retries = params.max_retries,
                .extra_headers = params.extra_headers,
                .extra_query = params.extra_query,
            };
        }

        /**
         * @brief Merge object members with the second object taking precedence.
         * @param target Object receiving fields.
         * @param extra Additional fields; null means no overrides.
         */
        auto merge_fields(Json::Value& target, Json::Value const& extra) -> void {
            if (extra.isObject()) {
                for (auto const& key : extra.getMemberNames()) {
                    target[key] = extra[key];
                }
            }
        }

        /**
         * @brief Decode an HTTP response as a typed completion with metadata.
         * @param response Parsed HTTP response.
         * @return Typed raw response or an enriched decoding error.
         */
        auto completion_response(
            RawResponse<Json::Value> response
        ) -> Result<RawResponse<ChatCompletion>> {
            auto parsed{ parse_chat_completion(response.parsed) };
            if (!parsed) {
                return std::unexpected{ response_error(std::move(parsed.error()), response) };
            }
            parsed->_request_id = response.request_id;
            return RawResponse<ChatCompletion>{
                .status_code = response.status_code,
                .headers = std::move(response.headers),
                .body = std::move(response.body),
                .parsed = std::move(*parsed),
                .request_id = std::move(response.request_id),
            };
        }

        /**
         * @brief Create a completion using a transport owned by the coroutine frame.
         * @param transport Shared endpoint and request defaults.
         * @param params Owned request fields.
         * @return Typed HTTP response or request/response error.
         */
        auto create_raw(
            std::shared_ptr<Transport> transport,
            ChatCompletionCreateParams params
        ) -> drogon::Task<Result<RawResponse<ChatCompletion>>> {
            auto body{ to_request_body(params, transport->Config().model) };
            if (body.isMember("stream") && body["stream"].isBool() && body["stream"].asBool()) {
                co_return std::unexpected{ Error{
                    .kind = ErrorKind::InvalidRequest,
                    .message = "use Stream for streaming chat completions",
                } };
            }
            auto response{ co_await transport->Request("POST", "/chat/completions", std::move(body), request_options(params)) };
            if (!response) {
                co_return std::unexpected{ std::move(response.error()) };
            }
            co_return completion_response(std::move(*response));
        }

    } // namespace

    /**
     * @brief Bind the group to an endpoint.
     * @param transport Connection shared by the API groups of one `OpenAI` client.
     */
    Completions::Completions(std::shared_ptr<Transport> transport)
        : m_transport{ std::move(transport) } {}

    /**
     * @brief Generate a reply to a conversation.
     * @param params Owned request fields and HTTP overrides.
     * @return Completion or a request/response error.
     * @note Requests and continuations use the awaiting thread's loop. Use `sync_wait`
     * only on a thread without an event loop. Pass client handles to coroutines by value.
     */
    auto Completions::Create(
        ChatCompletionCreateParams params
    ) const -> drogon::Task<Result<ChatCompletion>> {
        auto transport{ m_transport };
        auto response{ co_await create_raw(std::move(transport), std::move(params)) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        co_return std::move(response->parsed);
    }

    /**
     * @brief Generate a reply and retain HTTP metadata and original response bytes.
     * @param params Owned request fields and HTTP overrides.
     * @return Raw response with a typed completion or an error with response metadata.
     */
    auto Completions::CreateRaw(
        ChatCompletionCreateParams params
    ) const -> drogon::Task<Result<RawResponse<ChatCompletion>>> {
        auto transport{ m_transport };
        co_return co_await create_raw(std::move(transport), std::move(params));
    }

    /**
     * @brief Generate structured JSON output and decode strict function tool arguments.
     * @param params Owned request fields, schema, strict tools, and HTTP overrides.
     * @return Parsed completion, or validation, finish-reason, or request error.
     */
    auto Completions::Parse(
        ChatCompletionCreateParams params
    ) const -> drogon::Task<Result<ChatCompletion>> {
        auto transport{ m_transport };
        auto valid{ validate_parse_tools(params) };
        if (!valid) {
            co_return std::unexpected{ std::move(valid.error()) };
        }
        auto response{ co_await create_raw(std::move(transport), params) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ parse_structured(response->parsed, params) };
        if (!parsed) {
            co_return std::unexpected{ response_error(std::move(parsed.error()), *response) };
        }
        co_return std::move(response->parsed);
    }

} // namespace drogon_openai::details
