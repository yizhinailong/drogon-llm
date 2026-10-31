# drogon plugin: `drogon_openai::OpenAIClient`

English · [简体中文](../zh-CN/drogon-plugin.md)

This page describes how drogon-openai works as a drogon plugin: how to add it to an app, its configuration, its lifecycle, how to use it, and the design constraints that come from drogon's threading model. It talks to OpenAI-format HTTP APIs: OpenAI itself, or any endpoint that uses the same request format, such as DeepSeek.

## Overview

`drogon_openai::OpenAIClient` derives from `drogon::Plugin<OpenAIClient>` and does two things:

1. When drogon starts, it reads the configuration (`OpenAIConfig`) from config.json and validates it.
2. `NewOpenAIClient()` creates `OpenAI` clients bound to that configuration.

The client has the same layout as the `AsyncOpenAI` client of the OpenAI Python SDK:

| OpenAI Python SDK | drogon-openai |
|---|---|
| `client = AsyncOpenAI(base_url=..., api_key=...)` | `auto const client{ plugin->NewOpenAIClient() };` |
| `await client.chat.completions.create(messages=[...])` | `co_await client.chat.completions.Create({ .messages = { ... } })` |
| The types in `openai.types.chat` | The structs of the same names in `types/chat.hpp`, such as `ChatCompletion` |
| `extra_body={...}` | `.extra_body = Json::Value` |
| Exceptions such as `APIConnectionError` | An `ErrorKind` in the return value; nothing is thrown |

```cpp
auto const client{ drogon::app().getPlugin<drogon_openai::OpenAIClient>()->NewOpenAIClient() };
auto const completion{ co_await client.chat.completions.Create({
    .messages = { { .role = "user", .content = "Hello!" } },
}) };
if (completion && !completion->choices.empty()) {
    LOG_INFO << completion->choices.front().message.content.value_or("");
}
```

## Integration

The library needs C++23 (the public headers use `std::expected`). `drogon-openai::drogon-openai` passes on the `Drogon::Drogon` dependency, so consumers don't need their own `find_package(Drogon)`. Use either of two ways:

```cmake
# From source
add_subdirectory(path/to/drogon-openai)

# Installed: run just publish in drogon-openai first, then add publish/ to CMAKE_PREFIX_PATH
find_package(drogon-openai CONFIG REQUIRED)

target_link_libraries(app PRIVATE drogon-openai::drogon-openai)
```

## Registration and configuration

```json
"plugins": [
    {
        "name": "drogon_openai::OpenAIClient",
        "config": {
            "base_url": "https://api.deepseek.com",
            "api_key_env": "DEEPSEEK_API_KEY",
            "model": "deepseek-flash"
        }
    }
]
```

| Key | Required | Description |
|---|---|---|
| `base_url` | Yes | The API address. It may include a path prefix, such as `https://api.openai.com/v1`. Requests append the resource path, such as `/chat/completions`; a trailing `/` is ignored. |
| `model` | Yes | Default for Chat Completions and Realtime; set model explicitly for other resources that require it. |
| `api_key_env` | No | The **name of the environment variable** that holds the API key. Without it, no `Authorization` header is sent, which suits endpoints without auth, such as a locally deployed model. |
| `max_retries` | No | Retries after the initial attempt; default 2, 0 disables retries. |
| `timeout` | No | Seconds for all phases, or `{ "connect": 5, "read": 600, "write": 600 }`; omitted phases use these defaults. Zero disables a phase timeout. |
| `default_headers`, `default_query` | No | Objects merged into every request. Header values must be strings. |
| `organization`, `project` | No | `OpenAI-Organization` and `OpenAI-Project`; empty omits the header. |
| `webhook_secret_env` | No | Environment variable holding the webhook signing secret; omitted or an empty name leaves it unconfigured. A named variable must be set and non-empty. |

- The API key is never written in config.json. It is read only from the environment variable and sent as `Authorization: Bearer <key>`.
- `initAndStart` throws `std::invalid_argument` in any of these cases:
  - `base_url` or `model` is missing or empty;
  - `api_key_env` or `webhook_secret_env` is set, but the variable it names is unset or empty;
  - request defaults have invalid types, retries are negative, or timeouts are negative or non-finite.

  The exception propagates out of `app().run()`; if nothing catches it, the program terminates.
- A client can also be constructed directly, without the plugin: `drogon_openai::OpenAI{ drogon_openai::OpenAIConfig{ ... } }`. In that case `base_url` defaults to OpenAI's address.

## Lifecycle

```
app().loadConfigFile("config.json")       reads the plugins section
app().run()
 ├─ OpenAIClient::initAndStart(config)    parses and validates the configuration; throws on failure
 ├─ beginning advices                     getPlugin<OpenAIClient>() works from here on
 ├─ startListening()                      each IO loop starts listening asynchronously
 └─ the event loops run and handle requests
app().quit()                              drogon calls OpenAIClient::shutdown()
```

- `getPlugin` returns an `OpenAIClient*` that drogon owns. Don't delete it.
- `NewOpenAIClient()` returns a new client on every call. You can call it in every request, or keep one copy and reuse it.
- With static linking, the app's code must reference `OpenAIClient` (one call to `getPlugin<drogon_openai::OpenAIClient>()` is enough). Otherwise the linker drops `openai_client.o`, and drogon prints `Plugin drogon_openai::OpenAIClient undefined!` and keeps running.

## Client settings and HTTP operations

`OpenAI` is a cheap value handle. Copies share immutable transport settings; pass handles and coroutine parameters by value. `WithOptions(OpenAIOptions)` and `Copy(...)` create a client with selected overrides. Headers merge without regard to case, query objects merge by key, and request overrides win over client defaults. `Config()` exposes the resulting settings.

HTTP and WebSocket methods return `drogon::Task<Result<T>>`; local webhook verification returns `Result<T>`. Use `Get`, `Post`, `Put`, `Patch`, `Delete`, or `Request(method, path, body, options)` for unwrapped relative paths; JSON results include provider-specific fields. A successful empty body yields JSON null. `RequestRaw` returns `RawResponse<Json::Value>` with `status_code`, lowercase `headers`, `body`, `parsed`, and `request_id`. `GetBinary` returns these metadata with unchanged bytes in `parsed` and `body`.

`PostMultipart(path, vector<MultipartPart>, options)` uploads in-memory fields and files. Each part has `name`, `content`, optional `filename`, and optional `content_type`. File bytes, including NULs, are preserved. Parts remain in memory so retries can replay the request.

`RequestOptions` has optional `timeout` and `max_retries`, plus `extra_headers` and `extra_query`. `Timeout{seconds}` sets all phases; default construction uses connect 5/read 600/write 600 seconds, and individual members can be changed. These are inactivity timeouts, not a total request duration. Connect covers DNS/TCP/TLS; read and write timers reset on progress. The default two retries apply to connection errors, timeouts, HTTP 408/409/429 and >=500. Exponential delays start at 0.5 seconds, cap at 8 seconds, and include jitter. `retry-after-ms`, seconds or HTTP-date `retry-after`, and `x-should-retry` control retries; finite server delays above 120 seconds suppress retries. Set `OPENAI_LOG=info` or `debug` to log methods, hosts, attempts, and retry delays without credentials or bodies.

`List(path, options, cursor=true)` returns a `JsonPage`; pass false for a single page. `Page<T>`/`CursorPage<T>` offer `HasNextPage()`, `GetNextPage()`, `ForEach(callback)` and `All()`. Continuations own the transport and preserve options. An explicit `has_more=false` or empty data stops traversal; repeated cursors fail validation. Callback false stops successfully, and errors abort traversal.

`Poll(path, PollOptions, options)` GETs a resource until its status is completed, failed, cancelled, expired, or incomplete. Customize the status field, terminal states, interval and total timeout (default 600 seconds; zero disables it). `openai-poll-after-ms` overrides the interval. The deadline covers requests, retry delays, and polling delays. `PostAndPoll(path, body, poll, options)` and `UploadAndPoll(path, parts, poll, options)` create a resource and poll `<path>/<id>` using its returned string ID. They preserve the collection query and encode the ID as one path segment; their polling deadline starts after creation. Resources with different retrieval paths can compose creation with `Poll`. `PollRaw` also retains terminal HTTP metadata. Typed Files, Videos and Vector Stores helpers are documented in [API resources](api-resources.md).

## Other API resources

The client also exposes models, embeddings, completions, responses, conversations, moderations, files, uploads, images, audio, batches, vector_stores, webhooks, realtime, live, fine_tuning, evals, containers, videos, skills, decisions, content_provenance_checks and safety. [API resources](api-resources.md) lists all 146 HTTP methods and their parameter types; Doxygen documents the response fields.

## Calling chat completions

`Create(ChatCompletionCreateParams)` returns `Result<ChatCompletion>`. `CreateRaw` retains HTTP metadata, and the completion's `_request_id` comes from `x-request-id`. Unset optional request fields are omitted; JSON-valued union fields use the SDK's documented wire shape and allow provider extensions. Designated initializers follow declaration order in `types/chat.hpp`.

| Field | C++ type (std:: prefixes omitted) | Meaning |
|---|---|---|
| `messages` | `vector<ChatCompletionMessageParam>` | Conversation in chronological order. |
| `model` | `optional<string>` | Defaults to the configured model. |
| `frequency_penalty, presence_penalty, temperature, top_p` | `optional<double>` | Sampling settings. |
| `max_tokens, max_completion_tokens` | `optional<int64_t>` | Token limits; max_tokens is deprecated. |
| `tools` | `optional<vector<ChatCompletionToolParam>>` | Function or custom definitions; function accepts strict and parameters JSON Schema. |
| `tool_choice` | `optional<Json::Value>` | none, auto, required, a named function/custom tool, or allowed_tools. |
| `parallel_tool_calls` | `optional<bool>` | Allow parallel invocations. |
| `response_format` | `optional<Json::Value>` | text, json_object, or json_schema object. |
| `stop` | `optional<Json::Value>` | A string or array of strings. |
| `n, seed, top_logprobs` | `optional<int64_t>` | Candidate count, seed, and alternative token count. |
| `logprobs` | `optional<bool>` | Return token probabilities. |
| `logit_bias` | `optional<Json::Value>` | Token IDs mapped to probability biases. |
| `reasoning_effort, verbosity` | `optional<string>` | Reasoning effort and response detail. |
| `safety_identifier, prompt_cache_key, user` | `optional<string>` | End-user/cache identifiers; user is deprecated. |
| `metadata, moderation` | `optional<Json::Value>` | Metadata and input/output moderation settings. |
| `store` | `optional<bool>` | Store this completion for management endpoints. |
| `service_tier` | `optional<string>` | auto, default, flex, scale, priority, fast; provider dependent. |
| `modalities` | `optional<vector<string>>` | Requested text/audio modalities. |
| `audio, prediction, web_search_options` | `optional<Json::Value>` | Audio output, predicted output, and web search configuration. |
| `prompt_cache_retention` | `optional<string>` | Cache retention policy. |
| `prompt_cache_options` | `optional<Json::Value>` | Cache breakpoints and vendor-compatible options. |
| `stream_options` | `optional<Json::Value>` | include_usage and other streaming settings; use with Stream. |
| `extra_body` | `Json::Value` | Object merged last, overriding named fields. |
| `timeout, max_retries, extra_headers, extra_query` | `request overrides` | Same fields as RequestOptions; never serialized into the JSON body. |
| `schema_validator` | `function<Result<void>(Json const&, Json const&)>` | Optional local validator for decoded structured content and its supplied schema. |

`ChatCompletionMessageParam` accepts system/developer/user/assistant/tool roles. `content` is `Json::Value`: a string, null, or an array of text, image_url (including detail), input_audio, file, or refusal parts. Messages also expose `name`, `tool_call_id`, `tool_calls`, `refusal`, `audio` (previous reply ID), `reasoning_content`, and an `extra_body` merged last. `completion.choices[0].message.ToParam()` preserves tool calls and unknown provider fields for the next turn; changes to typed response fields are reflected in the resulting request.

Responses expose the original `raw` JSON at the completion and nested response objects. In addition to ID/model/object/created, choices contain finish_reason, index, message and logprobs. Messages include content, reasoning_content, refusal, function/custom tool_calls, URL annotations, and audio (id, expires_at, data, transcript). Logprobs expose content/refusal arrays of token, logprob, bytes and top_logprobs. Usage exposes the three totals, completion_tokens_details (reasoning_tokens, accepted_prediction_tokens, rejected_prediction_tokens, audio_tokens, text_tokens), and prompt_tokens_details (cached_tokens, cache_write_tokens, audio_tokens, image_tokens, text_tokens). system_fingerprint, service_tier, metadata and moderation are retained. Unknown counters such as DeepSeek's prompt_cache_hit_tokens remain accessible through usage.raw. Nullable fields use optional; absent scalar fields keep empty/zero defaults.

## Structured parsing and streaming

`Parse(params)` sends a supplied `response_format` JSON Schema and decodes the response into `message.parsed`. It also decodes strict function arguments into `function.parsed_arguments`; only function tools with strict=true are accepted, checked before sending. A length or content_filter finish reason returns `LengthFinishReason` or `ContentFilterFinishReason`. Refusals have no parsed content. Schema enforcement is performed by the provider; a caller can supply `schema_validator(decoded, schema)` to integrate a complete local JSON Schema validator. The library does not generate schemas from C++ types or implement a partial schema validator.

```cpp
Json::Value format;
format["type"] = "json_schema";
format["json_schema"]["name"] = "answer";
format["json_schema"]["strict"] = true;
format["json_schema"]["schema"]["type"] = "object";
format["json_schema"]["schema"]["properties"]["answer"]["type"] = "string";
format["json_schema"]["schema"]["required"].append("answer");
format["json_schema"]["schema"]["additionalProperties"] = false;
auto parsed = co_await client.chat.completions.Parse({
    .messages = { { .role = "user", .content = "Say hello." } },
    .response_format = format,
});
auto final = co_await client.chat.completions.Stream(
    { .messages = { { .role = "user", .content = "Say hello." } } },
    [](drogon_openai::ChatCompletionChunk const& chunk) -> drogon_openai::Result<bool> {
        for (auto const& choice : chunk.choices) {
            if (choice.delta.content) LOG_INFO << *choice.delta.content;
        }
        return true;
    }
);
```

`Stream(params, on_chunk, on_event)` sets stream=true and delivers chunks as bytes arrive. `ChatCompletionChunk` contains indexed choice deltas (content, refusal, reasoning_content, tool_calls), finish reasons, logprobs, raw JSON and usage, including a final usage-only chunk when the provider uses one. Tool fragments accumulate by index, including parallel function/custom calls. The event callback receives chunk, content.delta/done, refusal.delta/done, reasoning_content.delta/done, tool_calls.function.arguments.delta/done, tool_calls.custom.input.delta/done, and logprobs.content/refusal.delta/done, with an accumulated snapshot. The returned completion is the equivalent of Python's get_final_completion(). Strict tools and structured output are parsed at completion.

Callbacks return `Result<bool>`: true continues, false closes the response and returns the current snapshot without synthetic done events, and an error aborts. Exceptions from callbacks become `InvalidRequest`. A stream is never retried after any successful response bytes have been consumed. A malformed SSE event or response ending before [DONE] fails validation. SSE comments, multiline data, CR/LF/CRLF and fragmented UTF-8 are supported.

## Stored completions

`chat.completions.Retrieve(id)`, `Update(id, { .metadata = ... })`, `Delete(id)`, `List(ChatCompletionListParams, options)`, and `messages.List(id, ChatCompletionMessageListParams, options)` manage completions created with store=true. List filters include after, limit, order, model and metadata; message lists use after, limit and order. Completion lists return Page<ChatCompletion>, message lists Page<ChatCompletionStoreMessage>. These endpoints are OpenAI-specific; compatible providers need not implement them.

## Errors, threads, and limitations

`Error` carries kind, message, status_code, body, type, code, param, request_id and lowercase headers. Status messages retain the HTTP status prefix and API error.message, falling back to raw text. `IsBadRequest`, `IsAuthentication`, `IsPermissionDenied`, `IsNotFound`, `IsConflict`, `IsUnprocessableEntity`, `IsRateLimit`, and `IsInternalServer` classify 400/401/403/404/409/422/429/>=500. ErrorKind includes Connection, Timeout, Status, ResponseValidation, LengthFinishReason, ContentFilterFinishReason, InvalidRequest and InvalidWebhookSignature. Headers and request IDs remain available when a response has arrived, including parsing failures and mid-stream failures; the body of a streamed response is not buffered.

Each request attempt uses a new Trantor TCP/TLS connection on the calling event loop, or the main loop for a thread without one. Requests remain concurrent and resume on the same awaiting loop. DNS callbacks are marshalled back to that loop; retry and polling delays are asynchronous. The TLS policy validates the certificate chain and hostname against system roots. The transport requests identity encoding and supports HTTP/1.1 Content-Length, chunked and EOF-delimited bodies; it does not provide redirects, HTTP/2, decompression, proxy configuration or pooling. Write completion means bytes were accepted by the socket/TLS layer. Use sync_wait only on a thread without an event loop. Stream and page callbacks must not block.

The 23 root resources beyond Chat Completions are implemented. See [API resources](api-resources.md) for every endpoint, field convention, streams, upload polling, WebSocket and webhook examples. Authentication and deployment extensions remain deferred in TODO.md; admin, deprecated Assistants and other beta resources remain excluded. Deprecated functions/function_call and the function message role are not added. Features such as custom tools, json_schema, audio and stored completions depend on the provider; forwarding a field does not guarantee the provider supports it.

## Tests and reference behavior

`just test` builds and runs all programs; `ctest --preset linux-debug -LE live` runs offline checks. Each tests/test_*.cpp is registered separately with CTest, using Drogon's test framework. Use -r to select a case, -s for passing assertions, and -l to list cases.

| Program | Coverage |
|---|---|
| test_config | Required configuration, environment keys and request defaults, including rejection paths. |
| test_chat_completions | Existing fake API requests/responses, errors, concurrency and continuation thread. |
| test_chat_json | All request fields, multimodal/tool messages, nested response fields, raw JSON, structured parsing and validation. |
| test_client | Defaults/copies, methods, raw/binary responses, multipart parsing, retries, timeouts, polling deadlines and create/upload polling helpers. |
| test_chat_features | Incremental SSE, events, cancellation/errors, structured parsing, stored methods and pagination. |
| test_http_exchange | Raw TCP framing, split body delivery, TLS rejection, phase deadlines, concurrency and loop affinity. |
| test_resources | Contracts for all 146 HTTP methods: path, method, result kind, headers and metadata. |
| test_resource_wire | Independent fixtures for query/body separation, null, multiple files, SDP/JSON form parts, cursors, text and errors. |
| test_resource_helpers | Responses Parse/SSE, upload polling, chunked uploads and an HMAC signature vector. |
| test_websocket | Six connection routes, JSON/binary frames, loop affinity, deadlines, closing and exclusive receive. |
| test_deepseek | Real API success and bad-key 401; requires DEEPSEEK_API_KEY, otherwise skips with code 77; labelled live. |

Behavior was checked against [openai-python 3.41.0](https://github.com/openai/openai-python/tree/v3.41.0/src/openai), the [OpenAI Chat Completions reference](https://developers.openai.com/api/reference/resources/chat/subresources/completions/methods/create), and [DeepSeek's Chat Completions documentation](https://api-docs.deepseek.com/api/create-chat-completion/).
