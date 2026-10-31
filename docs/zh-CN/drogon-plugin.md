# drogon 插件：`drogon_openai::OpenAIClient`

[English](../en/drogon-plugin.md) · 简体中文

本文整理 drogon-openai 作为 drogon 插件的接入方式、配置、生命周期和使用方法，以及与 drogon 线程模型相关的设计约束。对接的是 OpenAI 格式的 HTTP API：OpenAI 本身，或 DeepSeek 等使用相同请求格式的接口。

## 概览

`drogon_openai::OpenAIClient` 继承 `drogon::Plugin<OpenAIClient>`，负责两件事：

1. 在 drogon 启动时从 config.json 读取并校验配置（`OpenAIConfig`）。
2. 通过 `NewOpenAIClient()` 生成绑定该配置的 `OpenAI` 客户端。

客户端的布局与 OpenAI Python SDK 的 `AsyncOpenAI` 一致：

| OpenAI Python SDK | drogon-openai |
|---|---|
| `client = AsyncOpenAI(base_url=..., api_key=...)` | `auto const client{ plugin->NewOpenAIClient() };` |
| `await client.chat.completions.create(messages=[...])` | `co_await client.chat.completions.Create({ .messages = { ... } })` |
| `openai.types.chat` 中的类型 | `types/chat.hpp` 中同名的结构体，如 `ChatCompletion` |
| `extra_body={...}` | `.extra_body = Json::Value` |
| `APIConnectionError` 等异常 | 返回值里的 `ErrorKind`，不抛异常 |

```cpp
auto const client{ drogon::app().getPlugin<drogon_openai::OpenAIClient>()->NewOpenAIClient() };
auto const completion{ co_await client.chat.completions.Create({
    .messages = { { .role = "user", .content = "Hello!" } },
}) };
if (completion && !completion->choices.empty()) {
    LOG_INFO << completion->choices.front().message.content.value_or("");
}
```

## 接入

库需要 C++23（公开头文件使用 `std::expected`），并通过 `drogon-openai::drogon-openai` 传递 `Drogon::Drogon` 依赖，消费方不需要再 `find_package(Drogon)`。两种方式任选：

```cmake
# 源码方式
add_subdirectory(path/to/drogon-openai)

# 安装方式：先在 drogon-openai 中执行 just publish，再把 publish/ 加进 CMAKE_PREFIX_PATH
find_package(drogon-openai CONFIG REQUIRED)

target_link_libraries(app PRIVATE drogon-openai::drogon-openai)
```

## 注册与配置

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

| 配置项 | 必填 | 说明 |
|---|---|---|
| `base_url` | 是 | 接口地址，可带路径前缀，例如 `https://api.openai.com/v1`。请求追加资源路径，例如 `/chat/completions`；末尾的 `/` 会被忽略。 |
| `model` | 是 | Chat Completions 和 Realtime 未指定模型时的默认值；其他资源按接口要求显式指定。 |
| `api_key_env` | 否 | 存放 API key 的**环境变量名**。省略时不发送 `Authorization` 头，适用于不需要鉴权的接口，例如本地部署的模型。 |
| `max_retries` | 否 | 首次请求之后的重试次数，默认 2，0 禁用重试。 |
| `timeout` | 否 | 所有阶段的秒数，或 `{ "connect": 5, "read": 600, "write": 600 }`；遗漏的阶段采用这些默认值。0 禁用该阶段的超时。 |
| `default_headers`、`default_query` | 否 | 合并到每次请求的对象；请求头的值必须是字符串。 |
| `organization`、`project` | 否 | `OpenAI-Organization` 和 `OpenAI-Project`；空字符串不发送对应请求头。 |
| `webhook_secret_env` | 否 | webhook 签名 secret 的环境变量名；省略或空名称表示不配置本地验签密钥。设置名称后，该变量必须存在且非空。 |

- API key 不写进 config.json，只从环境变量读取，以 `Authorization: Bearer <key>` 发送。
- 出现以下任一情况时，`initAndStart` 抛 `std::invalid_argument`：
  - `base_url` 或 `model` 缺失或为空；
  - 设置了 `api_key_env` 或 `webhook_secret_env`，但对应的环境变量未设置或为空；
  - 请求默认设置的类型无效、重试次数为负数，或超时为负数或非有限值。

  异常从 `app().run()` 抛出，不捕获时程序终止。
- 不经过插件也可以直接构造客户端：`drogon_openai::OpenAI{ drogon_openai::OpenAIConfig{ ... } }`。这时 `base_url` 默认为 OpenAI 的地址。

## 生命周期

```
app().loadConfigFile("config.json")       读取 plugins 配置段
app().run()
 ├─ OpenAIClient::initAndStart(config)    解析并校验配置，失败时抛异常
 ├─ beginning advices                     从这里开始 getPlugin<OpenAIClient>() 可用
 ├─ startListening()                      各 IO loop 异步开始监听
 └─ 事件循环运行，处理请求
app().quit()                              drogon 调用 OpenAIClient::shutdown()
```

- `getPlugin` 返回 `OpenAIClient*`，由 drogon 持有，不要 delete。
- `NewOpenAIClient()` 每次返回一个新的客户端，可以在每个请求里调用，也可以保存一个拷贝反复使用。
- 静态链接时，应用代码必须引用 `OpenAIClient`（调用一次 `getPlugin<drogon_openai::OpenAIClient>()` 即可），否则链接器丢掉 `openai_client.o`，drogon 打印 `Plugin drogon_openai::OpenAIClient undefined!` 后继续运行。

## 客户端设置与 HTTP 操作

`OpenAI` 是轻量值句柄，副本共享不可变的传输设置；句柄和协程参数均应按值传递。`WithOptions(OpenAIOptions)` 和 `Copy(...)` 返回应用选定覆盖项的新客户端。请求头合并时忽略大小写，查询对象按键合并，单次请求的设置优先于客户端默认值。`Config()` 返回最终配置。

`Headers` 持有按名称排序的键值对，支持初始化列表、`at`/`find`/`contains` 查找、插入、删除和迭代。副本的字段相互独立，移动时转移存储且不分配内存。它是值容器，原先依赖它与 `std::map` 类型完全相同的代码应改用 `Headers`；已有的标准 map 可以通过 `Headers::Storage` 初始化该容器。

所有运行时方法均返回 `drogon::Task<Result<T>>`。未封装的相对路径可使用 `Get`、`Post`、`Put`、`Patch`、`Delete` 或 `Request(method, path, body, options)`；JSON 结果保留厂商字段。成功响应的空 body 返回 JSON null。`RequestRaw` 返回 `RawResponse<Json::Value>`，包含 `status_code`、小写键的 `headers`、`body`、`parsed` 和 `request_id`。`GetBinary` 在 `parsed` 和 `body` 中原样返回二进制内容及这些元数据。

`PostMultipart(path, vector<MultipartPart>, options)` 上传内存中的字段和文件。每项有 `name`、`content`、可选的 `filename` 和 `content_type`。文件字节（包括 NUL）保持不变；数据保留在内存中，以便重试时重放请求。

`RequestOptions` 包含可选的 `timeout`、`max_retries`，以及 `extra_headers`、`extra_query`。`Timeout{seconds}` 对所有阶段设置同一时长；默认构造采用 connect 5/read 600/write 600 秒，各成员也可单独修改。这些是无进展超时，不是请求总时长。connect 包括 DNS/TCP/TLS；read、write 计时器随进展重置。默认重试两次，适用于连接错误、超时、HTTP 408/409/429 和 >=500。指数退避从 0.5 秒开始、上限 8 秒，并加入随机抖动。`retry-after-ms`、以秒数或 HTTP 日期表示的 `retry-after` 以及 `x-should-retry` 可控制重试；有限的服务端等待时间超过 120 秒时不重试。`OPENAI_LOG=info` 或 `debug` 记录方法、主机、尝试次数和重试等待时间，不记录凭据或 body。

`List(path, options, cursor=true)` 返回 `JsonPage`；传 false 表示单页。`Page<T>`/`CursorPage<T>` 提供 `HasNextPage()`、`GetNextPage()`、`ForEach(callback)` 和 `All()`。后续请求持有传输对象并保留设置。显式 `has_more=false` 或空数据停止翻页，重复游标视为响应校验错误。回调返回 false 正常结束，返回错误则中止。

`Poll(path, PollOptions, options)` 通过 GET 轮询资源，直到状态为 completed、failed、cancelled、expired 或 incomplete。可修改状态字段、终态列表、间隔和总超时（默认 600 秒；0 禁用）。`openai-poll-after-ms` 覆盖间隔，总截止时间涵盖请求、重试等待和轮询等待。`PostAndPoll(path, body, poll, options)` 和 `UploadAndPoll(path, parts, poll, options)` 创建资源后，使用返回的字符串 ID 轮询 `<path>/<id>`。它们保留集合路径的查询参数，将 ID 编码为单个路径段；轮询总超时从创建完成后开始计算。检索路径不同的资源可以将创建调用与 `Poll` 组合使用。`PollRaw` 额外保留终态 HTTP 元数据。Files、Videos 和 Vector Stores 的类型化辅助方法见 [API 资源](api-resources.md)。

## 其他 API 资源

客户端还包含 models、embeddings、completions、responses、conversations、moderations、files、uploads、images、audio、batches、vector_stores、webhooks、realtime、live、fine_tuning、evals、containers、videos、skills、decisions、content_provenance_checks 和 safety。对应的 146 个 HTTP 方法及参数类型逐项列于 [API 资源](api-resources.md)；返回类型的字段文档由 Doxygen 生成。

## 调用 chat completions

`Create(ChatCompletionCreateParams)` 返回 `Result<ChatCompletion>`。`CreateRaw` 保留 HTTP 元数据，补全的 `_request_id` 来自 `x-request-id`。未设置的可选请求字段不会发送；JSON 类型的联合字段采用 SDK 的协议格式并允许厂商扩展。指定初始化器按 `types/chat.hpp` 的声明顺序填写。

| 字段 | C++ 类型（省略 std::） | 含义 |
|---|---|---|
| `messages` | `vector<ChatCompletionMessageParam>` | 按时间顺序排列的对话消息。 |
| `model` | `optional<string>` | 未设置时使用配置中的模型。 |
| `frequency_penalty, presence_penalty, temperature, top_p` | `optional<double>` | 采样设置。 |
| `max_tokens, max_completion_tokens` | `optional<int64_t>` | token 上限；max_tokens 已弃用。 |
| `tools` | `optional<vector<ChatCompletionToolParam>>` | function 或 custom 定义；function 支持 strict 和 parameters JSON Schema。 |
| `tool_choice` | `optional<Json::Value>` | none、auto、required、指定 function/custom 工具，或 allowed_tools。 |
| `parallel_tool_calls` | `optional<bool>` | 允许并行调用工具。 |
| `response_format` | `optional<Json::Value>` | text、json_object 或 json_schema 对象。 |
| `stop` | `optional<Json::Value>` | 字符串或字符串数组。 |
| `n, seed, top_logprobs` | `optional<int64_t>` | 候选数量、随机种子和备选 token 数量。 |
| `logprobs` | `optional<bool>` | 返回 token 概率。 |
| `logit_bias` | `optional<Json::Value>` | token ID 到概率偏置的映射。 |
| `reasoning_effort, verbosity` | `optional<string>` | 推理强度和回答详略。 |
| `safety_identifier, prompt_cache_key, user` | `optional<string>` | 终端用户和缓存标识；user 已弃用。 |
| `metadata, moderation` | `optional<Json::Value>` | 元数据和输入输出审核设置。 |
| `store` | `optional<bool>` | 存储补全，以便通过管理接口操作。 |
| `service_tier` | `optional<string>` | auto、default、flex、scale、priority、fast；是否支持取决于服务商。 |
| `modalities` | `optional<vector<string>>` | 请求 text/audio 模态。 |
| `audio, prediction, web_search_options` | `optional<Json::Value>` | 音频输出、预测输出和联网搜索配置。 |
| `prompt_cache_retention` | `optional<string>` | 缓存保留策略。 |
| `prompt_cache_options` | `optional<Json::Value>` | 缓存断点及兼容选项。 |
| `stream_options` | `optional<Json::Value>` | include_usage 等流式选项；与 Stream 一起使用。 |
| `extra_body` | `Json::Value` | 最后合并的对象，可覆盖具名字段。 |
| `timeout, max_retries, extra_headers, extra_query` | `request overrides` | 与 RequestOptions 相同；不会写入 JSON 请求体。 |
| `schema_validator` | `function<Result<void>(Json const&, Json const&)>` | 可选的本地校验器，接收解码后的结构化内容和所提供的 Schema。 |

`ChatCompletionMessageParam` 支持 system/developer/user/assistant/tool 角色。`content` 是 `Json::Value`：字符串、null，或包含 text、image_url（含 detail）、input_audio、file、refusal 的片段数组。消息还支持 `name`、`tool_call_id`、`tool_calls`、`refusal`、`audio`（引用之前回复的 ID）、`reasoning_content` 和最后合并的 `extra_body`。使用 `completion.choices[0].message.ToParam()` 可保留工具调用和未知厂商字段，继续下一轮对话；修改类型化响应字段后，转换得到的请求也会反映修改。

响应在补全及嵌套对象的 `raw` 中保留原始 JSON。除 ID/model/object/created 外，choices 包含 finish_reason、index、message、logprobs。消息包含 content、reasoning_content、refusal、function/custom tool_calls、URL annotations 和 audio（id、expires_at、data、transcript）。logprobs 的 content/refusal 数组包含 token、logprob、bytes、top_logprobs。usage 包含三个总数、completion_tokens_details（reasoning_tokens、accepted_prediction_tokens、rejected_prediction_tokens、audio_tokens、text_tokens）和 prompt_tokens_details（cached_tokens、cache_write_tokens、audio_tokens、image_tokens、text_tokens）。另保留 system_fingerprint、service_tier、metadata、moderation。DeepSeek 的 prompt_cache_hit_tokens 等未知计数可通过 usage.raw 读取。可空字段使用 optional；缺失的普通标量采用空字符串或 0。

## 结构化解析与流式输出

`Parse(params)` 发送调用方提供的 `response_format` JSON Schema，并将内容解码到 `message.parsed`；strict 函数的参数解码到 `function.parsed_arguments`。仅接受 strict=true 的 function 工具，发送前进行检查。finish_reason 为 length 或 content_filter 时返回 `LengthFinishReason` 或 `ContentFilterFinishReason`。拒答不生成 parsed 内容。Schema 由服务端约束；调用方可提供 `schema_validator(decoded, schema)` 接入完整的本地 JSON Schema 校验器。库不会从 C++ 类型生成 Schema，也不实现不完整的 Schema 校验器。

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

`Stream(params, on_chunk, on_event)` 设置 stream=true，数据到达后即交付分块。`ChatCompletionChunk` 包含按 index 索引的 choice delta（content、refusal、reasoning_content、tool_calls）、finish_reason、logprobs、raw 和 usage；也支持厂商使用的最终 usage-only 分块。工具片段按 index 累积，支持并行 function/custom 调用。事件回调收到 chunk、content.delta/done、refusal.delta/done、reasoning_content.delta/done、tool_calls.function.arguments.delta/done、tool_calls.custom.input.delta/done 和 logprobs.content/refusal.delta/done，同时附带累积快照。返回的补全相当于 Python 的 get_final_completion()。strict 工具和结构化内容在结束时解析。

回调返回 `Result<bool>`：true 继续，false 关闭响应并返回当前快照，不合成 done 事件；返回错误则中止，抛出的异常转为 `InvalidRequest`。一旦开始消费成功响应字节，就不会自动重试。SSE 事件损坏或响应在 [DONE] 前结束时返回校验错误。支持 SSE 注释、多行 data、CR/LF/CRLF 和跨片段 UTF-8。

## 存储补全

`chat.completions.Retrieve(id)`、`Update(id, { .metadata = ... })`、`Delete(id)`、`List(ChatCompletionListParams, options)` 和 `messages.List(id, ChatCompletionMessageListParams, options)` 管理 store=true 创建的补全。补全列表过滤项包括 after、limit、order、model、metadata；消息列表包括 after、limit、order。两者分别返回 Page<ChatCompletion> 和 Page<ChatCompletionStoreMessage>。这些接口属于 OpenAI，兼容厂商不一定实现。

## 错误、线程与限制

`Error` 包含 kind、message、status_code、body、type、code、param、request_id 和小写键的 headers。状态错误消息保留 HTTP 状态前缀和 API error.message，无法解析时使用原始文本。`IsBadRequest`、`IsAuthentication`、`IsPermissionDenied`、`IsNotFound`、`IsConflict`、`IsUnprocessableEntity`、`IsRateLimit`、`IsInternalServer` 分别判断 400/401/403/404/409/422/429/>=500。ErrorKind 包括 Connection、Timeout、Status、ResponseValidation、LengthFinishReason、ContentFilterFinishReason、InvalidRequest、InvalidWebhookSignature。只要已收到响应，解析错误和流中错误也保留响应头与请求 ID；流式响应不会缓冲整个 body。

每次尝试都在调用线程的事件循环上创建新的 Trantor TCP/TLS 连接；调用线程没有 loop 时使用主 loop。请求并发执行，协程恢复到等待它的原 loop。DNS 回调会转回该 loop；重试和轮询等待均不阻塞。TLS 根据系统根证书校验证书链和主机名。传输请求 identity 编码，支持 HTTP/1.1 的 Content-Length、chunked 和 EOF 分帧；当前不提供重定向、HTTP/2、解压缩、代理设置或连接池。写入完成表示字节已被 socket/TLS 层接受。sync_wait 仅用于没有事件循环的线程；流和分页回调不得阻塞。

Chat Completions 之外的 23 个根资源已实现，完整接口、字段约定、流、上传轮询、WebSocket 和 webhook 示例见 [API 资源](api-resources.md)。仅认证部署扩展按 TODO.md 暂缓；admin、已弃用的 Assistants 和其余 beta 接口仍不在范围内。不会添加已弃用的 functions/function_call 或 function 消息角色。custom 工具、json_schema、音频和存储补全等能力取决于服务商；转发字段不保证服务商支持它。

## 测试与参考行为

`just test` 构建并运行所有测试；`ctest --preset linux-debug -LE live` 运行离线测试。每个 tests/test_*.cpp 使用 Drogon 测试框架并独立注册为 CTest 测试。-r 选择用例，-s 显示通过的断言，-l 列出用例。

| 程序 | 覆盖内容 |
|---|---|
| test_config | 必需配置、环境变量及请求默认设置，包括拒绝路径。 |
| test_chat_completions | 原有假 API 的请求响应、错误、并发和协程恢复线程。 |
| test_chat_json | 全部请求字段、多模态与工具消息、嵌套响应字段、raw JSON 和结构化校验。 |
| test_client | 默认设置与副本、方法、原始和二进制响应、multipart 解析、重试、超时、轮询截止时间及创建/上传后轮询。 |
| test_headers | 头字段的独立复制、无分配移动、移动后复用，以及两种标准库下的协程结果赋值。 |
| test_chat_features | 增量 SSE、事件、取消与错误、结构化解析、存储方法及分页。 |
| test_http_exchange | 原始 TCP 分帧、分段交付、TLS 拒绝、各阶段截止时间、并发和 loop。 |
| test_resources | 全部 146 个 HTTP 方法的路径、方法、类型、请求头和元数据契约。 |
| test_resource_wire | 独立报文：查询与请求体、null、多文件、SDP/JSON 表单项、游标、文本和错误。 |
| test_resource_helpers | Responses Parse/SSE、上传轮询、分块上传和 HMAC 签名向量。 |
| test_websocket | 六种连接路由、JSON/二进制帧、loop 恢复、超时、关闭及并发接收限制。 |
| test_deepseek | 真实接口成功和错误 key 的 401；需要 DEEPSEEK_API_KEY，否则以 77 跳过；带 live 标签。 |

行为对照 [openai-python 3.41.0](https://github.com/openai/openai-python/tree/v3.41.0/src/openai)、[OpenAI Chat Completions 官方参考](https://developers.openai.com/api/reference/resources/chat/subresources/completions/methods/create) 和 [DeepSeek Chat Completions 文档](https://api-docs.deepseek.com/api/create-chat-completion/)。
