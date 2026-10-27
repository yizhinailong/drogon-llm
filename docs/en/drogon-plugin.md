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
| `base_url` | Yes | The API address. It may include a path prefix, such as `https://api.openai.com/v1`. Requests go to `<base_url>/chat/completions`; a trailing `/` is ignored. |
| `model` | Yes | The model name used when a request sets no `model`. |
| `api_key_env` | No | The **name of the environment variable** that holds the API key. Without it, no `Authorization` header is sent, which suits endpoints without auth, such as a locally deployed model. |

- The API key is never written in config.json. It is read only from the environment variable and sent as `Authorization: Bearer <key>`.
- `initAndStart` throws `std::invalid_argument` in any of these cases:
  - `base_url` or `model` is missing or empty;
  - `api_key_env` is set, but the variable it names is unset or empty.

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

## Calling chat completions

- `OpenAI` is a value handle: `chat` and `chat.completions` are plain members, and the only shared state is one `std::shared_ptr<details::Transport>`. A copy only increments a reference count, so pass the client to coroutines by value.
- `Create(ChatCompletionCreateParams)` returns `drogon::Task<Result<ChatCompletion>>`, where `Result<T>` is `std::expected<T, Error>`. A failed request **does not throw**.

Request parameters (`ChatCompletionCreateParams`; fill them in with designated initializers, in declaration order):

| Field | Type | Description |
|---|---|---|
| `messages` | `std::vector<ChatCompletionMessageParam>` | The conversation, oldest first. Each message is a `role` (`system`, `developer`, `user`, or `assistant`) plus a text `content`. |
| `model` | `std::optional<std::string>` | When unset, the configured `model` is used. |
| `frequency_penalty` | `std::optional<double>` | -2.0 to 2.0. Positive values reduce repeated tokens. |
| `max_tokens` | `std::optional<std::int64_t>` | The maximum number of tokens to generate. |
| `presence_penalty` | `std::optional<double>` | -2.0 to 2.0. Positive values encourage new topics. |
| `temperature` | `std::optional<double>` | Sampling temperature, 0 to 2. |
| `top_p` | `std::optional<double>` | The probability mass for nucleus sampling. |
| `extra_body` | `Json::Value` | The members of this object are added to the request body as they are, and override the fields above with the same names. |
| `timeout` | `double` | Seconds to wait for the response, 600 by default; 0 waits forever. |

- Optional fields that are not set are left out of the request body, so the API uses its own defaults.
- `extra_body` carries fields that the params struct does not have yet, or vendor extensions, e.g. `extra_body["seed"] = 42;`. It is ignored when it is not an object.

Response (`ChatCompletion`):

| Field | Description |
|---|---|
| `id`, `object`, `created`, `model` | The completion's ID, its type (`chat.completion`), its creation time (Unix seconds), and the model actually used |
| `choices` | The generated candidates, usually just one. Each has an `index`, a `finish_reason` (such as `stop` or `length`), and a `message` |
| `choices[i].message` | `role` (`assistant`), `content` (the reply text), and `reasoning_content` (the reasoning of reasoning models such as DeepSeek's) |
| `usage` | `prompt_tokens`, `completion_tokens`, `total_tokens` |

- String fields missing from the response are empty strings, and integer fields are 0.
- `content` and `reasoning_content` are left unset when they are missing or null; `usage` is left unset when the response has no `usage`.

## Error handling

Each `ErrorKind` corresponds to an exception class of the OpenAI Python SDK:

| `ErrorKind` | Meaning | `status_code` | `body` |
|---|---|---|---|
| `Connection` | No response arrived: DNS, TLS, a refused connection, and so on (`APIConnectionError`) | 0 | Empty |
| `Timeout` | No response arrived within `timeout` seconds (`APITimeoutError`) | 0 | Empty |
| `Status` | A non-2xx status, such as an invalid key (401), rate limiting (429), or an overloaded service (503) (`APIStatusError`) | The HTTP status code | The raw response body |
| `ResponseValidation` | A 2xx response that is not JSON, has no `choices` array, or has a choice without a `message` object (`APIResponseValidationError`) | 0 | The response body |

- The `message` of a `Status` error looks like `HTTP 401: <error.message>`. When the response body is not an OpenAI-format error, the raw body takes the place of `error.message`.
- The `message` of a `Connection` or `Timeout` error looks like `POST <base_url>/chat/completions: <the reason drogon gives>`.
- Failed requests are not retried automatically.

## Threads and event loops

- Only coroutine APIs are provided. drogon's documentation recommends asynchronous APIs or coroutines. A synchronous API would block the IO thread, and waiting synchronously on the client's own loop would deadlock.
- `Transport::Post` creates a new `HttpClient` for each request on the calling thread's event loop (on the main loop when the calling thread runs no loop), and keeps it in the coroutine frame until the response arrives. So after a `co_await` in a request handler, the coroutine resumes on the same IO thread.
- Don't cache a shared `HttpClient`. drogon's `HttpClient` defaults to a pipelining depth of 0 and sends one request at a time, so a shared client would make concurrent requests queue.
- `drogon::sync_wait(client.chat.completions.Create(...))` can only be used on a thread that runs no event loop, such as the main thread of a test program.
- Coroutines take their parameters by value. `Create` copies `m_transport` into the coroutine frame before the first `co_await`, so a request completes even if the `OpenAI` client is destroyed while it is in flight.

## Known limitations

- Only chat completions are implemented. Embeddings, models, and the other APIs are not.
- Streaming (`stream: true`) is not supported. drogon 1.9.13's `HttpClient` buffers the whole response, so streaming needs a different transport.
- Message content is text only. Images and other multimodal content are not supported, and neither are tool calls (`tools`, `tool_calls`).

## Tests

The tests use drogon's own test framework, `drogon/drogon_test.h`, which is installed with drogon and needs no extra dependency. Each `tests/test_*.cpp` is built into one program and registered with CTest.

- Run all tests: `just test`, which is equivalent to `ctest --preset linux-debug`.
- Run only the offline tests: `ctest --preset linux-debug -LE live`.
- Run one program: `just run test_chat_completions`.
- Run one test case: `build/linux-debug/tests/test_chat_completions -r CreateTimeout`. Add `-s` to print passing checks, or `-l` to list the test cases.

The tests that need a server use the same app as a stand-in for an OpenAI-format API, listening on `127.0.0.1:18081` from config.json.

| Program | What it covers |
|---|---|
| `test_config` | Calls `initAndStart` directly without starting the app: the required `base_url` and `model`, reading `api_key_env` and its errors (unset, empty), and that the config.json in the repository passes validation. |
| `test_chat_completions` | A fake OpenAI-format API. The request body: the default model, the messages, unset optional fields being left out, set optional fields, and `extra_body` merging and overriding. The `Authorization` header; `base_url` paths (no path, `/v1`, a trailing `/`); parsing of a full response and of a response with missing fields; all four `ErrorKind`s (401 with `error.message`, 503 plain text, 404, non-JSON, no `choices`, a choice without `message`, a timeout, a failed connection); concurrent requests not queuing; and the coroutine resuming on the same thread when called on an IO thread. |
| `test_deepseek` | Calls the real DeepSeek API: one normal request and one with a bad key (401). It needs network access and `DEEPSEEK_API_KEY`, and spends a few tokens; without the variable it is reported as skipped. It has the CTest label `live`. |
