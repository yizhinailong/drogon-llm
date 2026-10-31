/**
 * @file completions.cpp
 * @brief Implement creation, parsing, streaming, and management of chat completions.
 */
#include "drogon_openai/details/completions.hpp"

#include <utility>

#include <json/writer.h>

#include "details/chat_json.hpp"
#include "details/chat_stream.hpp"
#include "details/transport.hpp"

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

        /**
         * @brief Retrieve or update a typed completion using an owned transport.
         * @param transport Shared endpoint and request defaults.
         * @param method HTTP method.
         * @param completion_id Stored completion ID.
         * @param body Owned JSON request body.
         * @param options Owned request overrides.
         * @return Stored completion or enriched request/response error.
         */
        auto stored_completion(
            std::shared_ptr<Transport> transport,
            std::string method,
            std::string completion_id,
            Json::Value body,
            RequestOptions options
        ) -> drogon::Task<Result<ChatCompletion>> {
            if (completion_id.empty()) {
                co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "completion ID must not be empty" } };
            }
            auto response{ co_await transport->Request(std::move(method), "/chat/completions/" + encode_path_segment(completion_id), std::move(body), std::move(options)) };
            if (!response) {
                co_return std::unexpected{ std::move(response.error()) };
            }
            auto result{ completion_response(std::move(*response)) };
            if (!result) {
                co_return std::unexpected{ std::move(result.error()) };
            }
            co_return std::move(result->parsed);
        }

        /**
         * @brief Fetch a cursor page and bind its next request without borrowing an API handle.
         * @tparam T Typed page item.
         * @param transport Owned transport handle.
         * @param path Collection path.
         * @param options Query and HTTP overrides, copied for each successor.
         * @param decoder Decode one item.
         * @return Parsed page or enriched request/response error.
         */
        template <typename T>
        auto fetch_page(
            std::shared_ptr<Transport> transport,
            std::string path,
            RequestOptions options,
            typename Page<T>::Decoder decoder
        ) -> drogon::Task<Result<Page<T>>> {
            auto response{ co_await transport->Request("GET", path, {}, options) };
            if (!response) {
                co_return std::unexpected{ std::move(response.error()) };
            }
            auto item_decoder = [decoder, request_id = response->request_id](Json::Value const& item) -> Result<T> {
                auto parsed{ decoder(item) };
                if (parsed) {
                    parsed->_request_id = request_id;
                }
                return parsed;
            };
            auto next = [transport, path, options, decoder](std::string after) mutable -> drogon::Task<Result<Page<T>>> {
                auto next_options{ options };
                next_options.extra_query["after"] = std::move(after);
                return fetch_page<T>(transport, path, std::move(next_options), decoder);
            };
            co_return Page<T>::FromResponse(std::move(*response), std::move(item_decoder), std::move(next));
        }

        /**
         * @brief Decode an original conversation message from the stored messages endpoint.
         * @param value Message JSON.
         * @return Typed message retaining unknown fields or a validation error.
         */
        auto store_message(Json::Value const& value) -> Result<ChatCompletionStoreMessage> {
            if (!value.isObject() || !value["id"].isString() || !value["role"].isString() || (!value["content"].isNull() && !value["content"].isString()) || (!value["content_parts"].isNull() && !value["content_parts"].isArray())) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid stored chat message" } };
            }
            ChatCompletionStoreMessage result{
                .id = value["id"].asString(),
                .role = value["role"].asString(),
                .raw = value,
            };
            if (value["content"].isString()) {
                result.content = value["content"].asString();
            }
            if (value["content_parts"].isArray()) {
                result.content_parts = value["content_parts"];
            }
            if (value["name"].isString()) {
                result.name = value["name"].asString();
            }
            return result;
        }

        /**
         * @brief Merge list parameters and extra query fields, with extra fields taking precedence.
         * @tparam Params Completion or message list parameters.
         * @param params Pagination parameters.
         * @param options Request options to enrich.
         * @return Request options retaining explicit extra query overrides.
         */
        template <typename Params>
        auto list_options(Params const& params, RequestOptions options) -> RequestOptions {
            Json::Value query{ Json::objectValue };
            if (params.after) {
                query["after"] = *params.after;
            }
            if (params.limit) {
                query["limit"] = Json::Int64{ *params.limit };
            }
            if (params.order) {
                query["order"] = *params.order;
            }
            if constexpr (requires { params.model; params.metadata; }) {
                if (params.model) {
                    query["model"] = *params.model;
                }
                if (params.metadata) {
                    query["metadata"] = *params.metadata;
                }
            }
            merge_fields(query, options.extra_query);
            options.extra_query = std::move(query);
            return options;
        }

    } // namespace

    /**
     * @brief Bind the messages group to an endpoint.
     * @param transport Shared HTTP transport.
     */
    CompletionMessages::CompletionMessages(std::shared_ptr<Transport> transport)
        : m_transport{ std::move(transport) } {}

    /**
     * @brief List the original messages of a stored completion.
     * @param completion_id Stored completion ID, encoded as one path segment.
     * @param params Cursor, page size, and order.
     * @param options Per-request HTTP overrides, retained across pages.
     * @return Typed page supporting manual and automatic traversal.
     */
    auto CompletionMessages::List(
        std::string completion_id,
        ChatCompletionMessageListParams params,
        RequestOptions options
    ) const -> drogon::Task<Result<Page<ChatCompletionStoreMessage>>> {
        auto transport{ m_transport };
        if (completion_id.empty()) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "completion ID must not be empty" } };
        }
        co_return co_await fetch_page<ChatCompletionStoreMessage>(std::move(transport), "/chat/completions/" + encode_path_segment(completion_id) + "/messages", list_options(params, std::move(options)), store_message);
    }

    /**
     * @brief Bind the group to an endpoint.
     * @param transport Connection shared by the API groups of one `OpenAI` client.
     */
    Completions::Completions(std::shared_ptr<Transport> transport)
        : m_transport{ transport }, messages{ std::move(transport) } {}

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

    /**
     * @brief Stream SSE chunks and high-level events, returning the final completion.
     * @param params Owned request fields; this method sets `stream` to true.
     * @param on_chunk Optional per-chunk consumer.
     * @param on_event Optional `chunk`, text, refusal, tool, reasoning, and logprobs events.
     * @return Final completion, current snapshot after callback cancellation, or an error.
     * @note Callbacks run on the calling loop and must not block. A started stream is
     * never retried. Callback false stops immediately without synthesizing done events.
     */
    auto Completions::Stream(
        ChatCompletionCreateParams params,
        ChatChunkCallback on_chunk,
        ChatEventCallback on_event
    ) const -> drogon::Task<Result<ChatCompletion>> {
        auto transport{ m_transport };
        auto body{ to_request_body(params, transport->Config().model) };
        body["stream"] = true;
        Json::StreamWriterBuilder writer;
        writer["indentation"] = "";
        ChatStream stream{ params, std::move(on_chunk), std::move(on_event) };
        auto response{ co_await transport->Send("POST", "/chat/completions", Json::writeString(writer, body), "application/json", request_options(params), [&stream](std::string_view bytes, Headers const& headers) { return stream.Feed(bytes, headers); }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto completion{ stream.Finish() };
        if (!completion) {
            co_return std::unexpected{ response_error(std::move(completion.error()), *response) };
        }
        completion->_request_id = response->request_id;
        co_return std::move(*completion);
    }

    /**
     * @brief Retrieve a completion created with `store: true`.
     * @param completion_id Stored completion ID, encoded as one path segment.
     * @param options Per-request HTTP overrides.
     * @return Stored completion or a request/response error.
     */
    auto Completions::Retrieve(
        std::string completion_id,
        RequestOptions options
    ) const -> drogon::Task<Result<ChatCompletion>> {
        auto transport{ m_transport };
        co_return co_await stored_completion(std::move(transport), "GET", std::move(completion_id), {}, std::move(options));
    }

    /**
     * @brief Update the metadata of a stored completion.
     * @param completion_id Stored completion ID, encoded as one path segment.
     * @param params Metadata and additional body fields.
     * @param options Per-request HTTP overrides.
     * @return Updated completion or a request/response error.
     */
    auto Completions::Update(
        std::string completion_id,
        ChatCompletionUpdateParams params,
        RequestOptions options
    ) const -> drogon::Task<Result<ChatCompletion>> {
        auto transport{ m_transport };
        Json::Value body{ Json::objectValue };
        body["metadata"] = std::move(params.metadata);
        merge_fields(body, params.extra_body);
        co_return co_await stored_completion(std::move(transport), "POST", std::move(completion_id), std::move(body), std::move(options));
    }

    /**
     * @brief Delete a stored completion and its stored messages.
     * @param completion_id Stored completion ID, encoded as one path segment.
     * @param options Per-request HTTP overrides.
     * @return Deletion confirmation or a request/response error.
     */
    auto Completions::Delete(
        std::string completion_id,
        RequestOptions options
    ) const -> drogon::Task<Result<ChatCompletionDeleted>> {
        auto transport{ m_transport };
        if (completion_id.empty()) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "completion ID must not be empty" } };
        }
        auto response{ co_await transport->Request("DELETE", "/chat/completions/" + encode_path_segment(completion_id), {}, std::move(options)) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto const& root{ response->parsed };
        if (!root.isObject() || !root["id"].isString() || !root["deleted"].isBool() || !root["object"].isString()) {
            co_return std::unexpected{ response_error(Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid chat completion deletion response" }, *response) };
        }
        co_return ChatCompletionDeleted{
            .id = root["id"].asString(),
            .deleted = root["deleted"].asBool(),
            .object = root["object"].asString(),
            .raw = root,
            ._request_id = response->request_id,
        };
    }

    /**
     * @brief List stored completions with optional filters.
     * @param params Cursor, page size, metadata, model, and sort order.
     * @param options Per-request HTTP overrides, retained across pages.
     * @return Typed page supporting manual and automatic traversal.
     */
    auto Completions::List(
        ChatCompletionListParams params,
        RequestOptions options
    ) const -> drogon::Task<Result<Page<ChatCompletion>>> {
        auto transport{ m_transport };
        co_return co_await fetch_page<ChatCompletion>(std::move(transport), "/chat/completions", list_options(params, std::move(options)), parse_chat_completion);
    }

} // namespace drogon_openai::details
