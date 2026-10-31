# TODO

OpenAI Python SDK 功能清单，全部实现后删除本文件。对照的是 openai-python 3.41.0（2026-10-30 的 `main`）的 `AsyncOpenAI`。

已完成客户端基础能力、错误信息、Chat Completions 和下列其他 API：对照 openai-python 3.41.0，新增 23 个根资源、57 个资源类、146 个 HTTP 方法，并提供 Responses 解析/流、上传轮询、WebSocket 与 webhook 验签。仅认证部署扩展仍暂缓，因此保留本文件。

请求扩展字段可放进 `extra_body`，响应扩展字段保留在 `raw`。行为已对照 openai-python 的 `src/openai/types/` 和 DeepSeek 等厂商的文档；使用说明见 [用户指南](docs/zh-CN/drogon-plugin.md)。

## 客户端

- [x] `max_retries`：默认重试 2 次，对连接错误、408、409、429 和 ≥500 指数退避，并遵守 `retry-after-ms`、`retry-after`、`x-should-retry`。可以按客户端设置，也可以按请求覆盖。
- [x] 客户端级的 `timeout`：默认 connect 5 秒、read/write 600 秒，可以分别设置。支持客户端设置和单次请求覆盖。
- [x] `default_headers`、`default_query`：客户端级的请求头和查询参数
- [x] `extra_headers`、`extra_query`：单次请求的请求头和查询参数
- [x] `organization`、`project`：发送 `OpenAI-Organization`、`OpenAI-Project` 请求头
- [x] `with_options()` / `copy()`：对应 `WithOptions` / `Copy`，复制客户端并覆盖部分设置
- [x] `with_raw_response`：对应 `CreateRaw` / `RequestRaw`，读取响应头，如 `x-ratelimit-*`
- [x] `_request_id`：响应对象带上 `x-request-id`
- [x] `client.get()`、`client.post()` 等：请求没有封装的路径，返回 `Result<Json::Value>`
- [x] GET、DELETE、`multipart/form-data` 上传和二进制响应：通过通用 HTTP 方法、`PostMultipart` 和 `GetBinary` 提供。
- [x] 分页：`AsyncCursorPage` / `AsyncPage`，可以自动翻页，也可以用 `has_next_page()`、`get_next_page()` 手动翻页
- [x] `*_and_poll` 的通用基础：`Poll`、`PostAndPoll`、`UploadAndPoll` 轮询到终态再返回；另有 Files、Videos 和 Vector Stores 的类型化轮询/上传封装。
- [x] `OPENAI_LOG`：记录请求和重试

## 认证和部署

> 先不实现，接入对应平台时再加

- [ ] `api_key` 传异步回调：动态刷新 token
- [ ] workload identity：Kubernetes、Azure、GCP、X.509 mTLS
- [ ] `AzureOpenAI`：`api_version` 查询参数、`api-key` 请求头、deployment 路径、Entra ID token
- [ ] Amazon Bedrock provider：SigV4 签名
- [ ] `data_residency`：区域端点
- [ ] `http_client`：代理、mTLS 和连接池。至少开放 TLS 证书校验开关和客户端证书。

## 错误

已有 `kind`、`message`、`status_code`、`body`。

- [x] `type`、`code`、`param`：从错误体的 `error` 对象读取，对应 Python `APIError` 的同名属性
- [x] `request_id`：取自 `x-request-id`，对应 `APIStatusError.request_id`
- [x] 错误响应的响应头，如 `retry-after`
- [x] 按状态码判断的函数，如 `IsRateLimit()`：对应 Python 的 `BadRequestError`（400）、`AuthenticationError`（401）、`PermissionDeniedError`（403）、`NotFoundError`（404）、`ConflictError`（409）、`UnprocessableEntityError`（422）、`RateLimitError`（429）、`InternalServerError`（≥500）

## 流式输出

drogon 1.9.13 的 `HttpClient` 会缓冲整个响应，已新增基于 Trantor 的增量传输层，再在上面解析 SSE。已经开始读取的流不自动重试，以免输出重复，与 Python 一致。

- [x] `stream`：返回 `ChatCompletionChunk` 流。C++ 接口可以是逐块回调，也可以是能 `co_await` 下一块的流对象。
- [x] `ChatCompletionChunk`：`choices[i].delta` 的 `content`、`refusal`、`tool_calls`（按 `index` 增量拼接），以及 DeepSeek 等的 `reasoning_content`
- [x] `stream_options`：`include_usage` 让最后一块带上 `usage`
- [x] `chat.completions.stream()`：把分块聚合成 `content.delta`、`content.done`、`refusal.*`、`tool_calls.function.arguments.*`、`logprobs.*` 等事件，C++ 的 `Stream` 返回完整的 `ChatCompletion`，对应 `get_final_completion()`

## 请求参数

已有 `messages`、`model`、`frequency_penalty`、`max_tokens`、`presence_penalty`、`temperature`、`top_p`、`extra_body`、`timeout`。已弃用的 `functions`、`function_call` 不实现。

- [x] `tools`：工具定义，有 `function`（可设 `strict`）和 `custom` 两种
- [x] `tool_choice`：`none`、`auto`、`required`、指定某个工具，或用 `allowed_tools` 限定范围
- [x] `parallel_tool_calls`：允许并行调用工具
- [x] `response_format`：`text`、`json_object`、`json_schema`
- [x] `stop`：一个或多个停止序列
- [x] `max_completion_tokens`：取代已弃用的 `max_tokens`
- [x] `n`：候选数量
- [x] `seed`：可复现采样
- [x] `logprobs`、`top_logprobs`：返回对数概率
- [x] `logit_bias`：调整指定 token 的概率
- [x] `reasoning_effort`：推理模型的思考强度
- [x] `verbosity`：回答的详略
- [x] `safety_identifier`、`prompt_cache_key`、`user`：终端用户标识，`user` 已被前两个字段取代
- [x] `metadata`、`store`：存储补全
- [x] `service_tier`：`auto`、`default`、`flex`、`scale`、`priority`、`fast`
- [x] `modalities`、`audio`：音频输出
- [x] `prediction`：Predicted Outputs
- [x] `web_search_options`：联网搜索
- [x] `prompt_cache_retention`、`prompt_cache_options`：提示缓存的保留时长和断点
- [x] `moderation`：对输入和输出做内容审核

## 请求消息

支持字符串或片段数组形式的 `content`。已弃用的 `function` 角色不实现。

- [x] `tool` 角色：`tool_call_id` 和工具结果
- [x] assistant 消息的 `tool_calls`
- [x] 从 `ChatCompletionMessage` 转成请求消息：多轮对话和工具调用循环要把回复原样放回 `messages`，Python 可以直接传 `completion.choices[0].message`
- [x] 内容片段数组：`text`、`image_url`（含 `detail`）、`input_audio`、`file`
- [x] `name`：区分同一角色的不同参与者
- [x] assistant 消息的 `refusal`
- [x] assistant 消息的 `audio`：按 id 引用之前的音频回复

## 响应字段

已有 `id`、`object`、`created`、`model`，`choices` 的 `index`、`finish_reason`、`message.role`、`message.content`、`message.reasoning_content`，以及 `usage` 的三个总数。

- [x] 原始 JSON：保留没有解析的字段，例如 DeepSeek 的 `usage.prompt_cache_hit_tokens`，对应 Python 的 `model_extra`、`to_dict()`
- [x] `message.tool_calls`：`function`（`id`、`function.name`、`function.arguments`）和 `custom` 两种
- [x] `message.refusal`：拒答说明
- [x] `message.annotations`：联网搜索的 `url_citation`
- [x] `message.audio`：音频回复
- [x] `choices[i].logprobs`：`content` 和 `refusal` 两组，每项有 `token`、`logprob`、`bytes`、`top_logprobs`
- [x] `usage.completion_tokens_details`：`reasoning_tokens`、`accepted_prediction_tokens`、`rejected_prediction_tokens`、`audio_tokens`、`text_tokens`
- [x] `usage.prompt_tokens_details`：`cached_tokens`、`cache_write_tokens`、`audio_tokens`、`image_tokens`、`text_tokens`
- [x] `system_fingerprint`、`service_tier`
- [x] `metadata`、`moderation`

## Chat Completions 的其他方法

已有非流式的 `create`。

- [x] `parse()`：请求结构化输出并解析回复，`finish_reason` 为 `length` 或 `content_filter` 时失败（`LengthFinishReasonError`、`ContentFilterFinishReasonError`）；`strict: true` 的函数工具也解析调用参数。Python 从 Pydantic 模型生成 JSON Schema，C++ 没有反射，可以让调用方直接传 Schema。
- [x] `retrieve`、`update`、`list`、`delete`、`messages.list`：管理 `store: true` 存下的补全，只有 OpenAI 提供

## 其他接口

对照固定 SDK 提交实现，完整目录和示例见 [API 资源](docs/zh-CN/api-resources.md)。

`admin.*` 需要 admin key，Assistants API（`beta.assistants`、`beta.threads`）已弃用，其余 `beta.*` 变动频繁，这些都不实现。

兼容接口普遍提供：

- [x] `models`：`list`、`retrieve`、`delete`
- [x] `embeddings`：`create`
- [x] `completions`：`create`，旧版文本补全；DeepSeek 的 FIM 补全也用这个格式

OpenAI：

- [x] `responses`：`create`、`retrieve`、`delete`、`cancel`、`compact`、`input_items.list`、`input_tokens.count`、`parse`、`stream`，OpenAI 目前主推的接口
- [x] `conversations`：`create`、`retrieve`、`update`、`delete`、`items.*`，与 Responses 配套
- [x] `moderations`：`create`
- [x] `files`：`create`、`retrieve`、`list`、`delete`、`content`、`retrieve_content`
- [x] `uploads`：`create`、`complete`、`cancel`、`parts.create`，分片上传大文件
- [x] `images`：`generate`、`edit`、`create_variation`
- [x] `audio`：`speech`、`transcriptions`、`translations`、`voices`、`voice_consents`
- [x] `batches`：`create`、`retrieve`、`list`、`cancel`
- [x] `vector_stores`：增删改查、`search`、`files.*`、`file_batches.*`
- [x] `webhooks`：`unwrap`、`verify_signature` 用 `webhook_secret` 在本地校验签名（失败时 `InvalidWebhookSignatureError`），适合 drogon 服务端接收 webhook；另有 webhook 的增删改查、`event_types.list`、`rotate_secret`、`test`
- [x] `realtime`：WebSocket 连接（可以用 drogon 的 `WebSocketClient`）、`calls.*`、`client_secrets.create`、`translations.client_secrets.create`
- [x] `live`：`create`、`sessions.*`
- [x] `fine_tuning`：`jobs.*`、`checkpoints.permissions.*`、`alpha.graders.*`
- [x] `evals`：增删改查、`runs.*`、`runs.output_items.*`
- [x] `containers`：增删改查、`files.*`
- [x] `videos`：`create`、`retrieve`、`list`、`delete`、`download_content`、`remix`、`edit`、`extend`、`create_character`、`get_character`
- [x] `skills`：增删改查、`versions.*`、`content.retrieve`
- [x] `decisions`、`content_provenance_checks`：`create`
- [x] `safety`：`alerts.retrieve`、`cases.retrieve`

## 当前阶段验证

- `test_config`、`test_client`：配置、设置覆盖、HTTP 方法、上传、重试和轮询。
- `test_chat_completions`、`test_chat_json`：请求字段、嵌套响应、扩展字段和错误。
- `test_chat_features`、`test_http_exchange`：增量 SSE、事件、取消、结构化解析、存储管理、分页、HTTP 分帧、TLS 和阶段超时。
- `test_deepseek`：真实接口的正常回复与错误密钥 401。
- `test_resources`：146 个 HTTP 方法的接口契约。
- `test_resource_wire`：独立报文、multipart、SDP/JSON 表单项、路径编码、游标、文本和错误。
- `test_resource_helpers`、`test_websocket`：Responses、上传轮询、分片顺序、HMAC、六种连接路由、帧、超时、关闭和 loop 恢复。
- Linux Debug/Release 的 10 个离线测试程序已通过，DeepSeek 实网测试也已通过；新增远程 API 使用本地 fake 服务验证，未声称全部实网调用。
- 源码/安装包消费、静态/共享库、两份 README 与新指南示例、严格 Doxygen、格式检查及生成器确定性检查已通过。
