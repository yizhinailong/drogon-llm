# TODO

尚未支持的 OpenAI Python SDK 功能，全部实现后删除本文件。对照的是 openai-python 3.41.0（2026-10-30 的 `main`）的 `AsyncOpenAI`。

没有类型化的请求字段现在可以放进 `extra_body`，响应里没有解析的字段目前读不到。实现前先对照 openai-python 的 `src/openai/types/` 和 DeepSeek 等厂商的文档确认实际行为。

## 客户端

- [ ] `max_retries`：默认重试 2 次，对连接错误、408、409、429 和 ≥500 指数退避，并遵守 `retry-after-ms`、`retry-after`、`x-should-retry`。可以按客户端设置，也可以按请求覆盖。
- [ ] 客户端级的 `timeout`：默认 600 秒，可以分别设 connect、read、write。现在只有每次请求的 `timeout`。
- [ ] `default_headers`、`default_query`：客户端级的请求头和查询参数
- [ ] `extra_headers`、`extra_query`：单次请求的请求头和查询参数
- [ ] `organization`、`project`：发送 `OpenAI-Organization`、`OpenAI-Project` 请求头
- [ ] `with_options()` / `copy()`：复制客户端并覆盖部分设置
- [ ] `with_raw_response`：读取响应头，如 `x-ratelimit-*`
- [ ] `_request_id`：响应对象带上 `x-request-id`
- [ ] `client.get()`、`client.post()` 等：请求没有封装的路径，返回 `Result<Json::Value>`
- [ ] GET、DELETE、`multipart/form-data` 上传和二进制响应：`Transport` 现在只有 JSON 的 `Post`，其他接口大多依赖这一项。multipart 可以先试 drogon 的 `HttpRequest::newFileUploadRequest`。
- [ ] 分页：`AsyncCursorPage` / `AsyncPage`，可以自动翻页，也可以用 `has_next_page()`、`get_next_page()` 手动翻页
- [ ] `*_and_poll`：轮询到终态再返回，如 `vector_stores.files.upload_and_poll`
- [ ] `OPENAI_LOG`：记录请求和重试

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

- [ ] `type`、`code`、`param`：从错误体的 `error` 对象读取，对应 Python `APIError` 的同名属性
- [ ] `request_id`：取自 `x-request-id`，对应 `APIStatusError.request_id`
- [ ] 错误响应的响应头，如 `retry-after`
- [ ] 按状态码判断的函数，如 `IsRateLimit()`：对应 Python 的 `BadRequestError`（400）、`AuthenticationError`（401）、`PermissionDeniedError`（403）、`NotFoundError`（404）、`ConflictError`（409）、`UnprocessableEntityError`（422）、`RateLimitError`（429）、`InternalServerError`（≥500）

## 流式输出

drogon 1.9.13 的 `HttpClient` 会缓冲整个响应，流式输出要另写能读分块响应的传输层，再在上面解析 SSE。已经开始读取的流不自动重试，以免输出重复，与 Python 一致。

- [ ] `stream`：返回 `ChatCompletionChunk` 流。C++ 接口可以是逐块回调，也可以是能 `co_await` 下一块的流对象。
- [ ] `ChatCompletionChunk`：`choices[i].delta` 的 `content`、`refusal`、`tool_calls`（按 `index` 增量拼接），以及 DeepSeek 等的 `reasoning_content`
- [ ] `stream_options`：`include_usage` 让最后一块带上 `usage`
- [ ] `chat.completions.stream()`：把分块聚合成 `content.delta`、`content.done`、`refusal.*`、`tool_calls.function.arguments.*`、`logprobs.*` 等事件，结束后用 `get_final_completion()` 取完整的 `ChatCompletion`

## 请求参数

已有 `messages`、`model`、`frequency_penalty`、`max_tokens`、`presence_penalty`、`temperature`、`top_p`、`extra_body`、`timeout`。已弃用的 `functions`、`function_call` 不实现。

- [ ] `tools`：工具定义，有 `function`（可设 `strict`）和 `custom` 两种
- [ ] `tool_choice`：`none`、`auto`、`required`、指定某个工具，或用 `allowed_tools` 限定范围
- [ ] `parallel_tool_calls`：允许并行调用工具
- [ ] `response_format`：`text`、`json_object`、`json_schema`
- [ ] `stop`：一个或多个停止序列
- [ ] `max_completion_tokens`：取代已弃用的 `max_tokens`
- [ ] `n`：候选数量
- [ ] `seed`：可复现采样
- [ ] `logprobs`、`top_logprobs`：返回对数概率
- [ ] `logit_bias`：调整指定 token 的概率
- [ ] `reasoning_effort`：推理模型的思考强度
- [ ] `verbosity`：回答的详略
- [ ] `safety_identifier`、`prompt_cache_key`、`user`：终端用户标识，`user` 已被前两个字段取代
- [ ] `metadata`、`store`：存储补全
- [ ] `service_tier`：`auto`、`default`、`flex`、`scale`、`priority`、`fast`
- [ ] `modalities`、`audio`：音频输出
- [ ] `prediction`：Predicted Outputs
- [ ] `web_search_options`：联网搜索
- [ ] `prompt_cache_retention`、`prompt_cache_options`：提示缓存的保留时长和断点
- [ ] `moderation`：对输入和输出做内容审核

## 请求消息

现在只有字符串类型的 `role` 和 `content`。已弃用的 `function` 角色不实现。

- [ ] `tool` 角色：`tool_call_id` 和工具结果
- [ ] assistant 消息的 `tool_calls`
- [ ] 从 `ChatCompletionMessage` 转成请求消息：多轮对话和工具调用循环要把回复原样放回 `messages`，Python 可以直接传 `completion.choices[0].message`
- [ ] 内容片段数组：`text`、`image_url`（含 `detail`）、`input_audio`、`file`
- [ ] `name`：区分同一角色的不同参与者
- [ ] assistant 消息的 `refusal`
- [ ] assistant 消息的 `audio`：按 id 引用之前的音频回复

## 响应字段

已有 `id`、`object`、`created`、`model`，`choices` 的 `index`、`finish_reason`、`message.role`、`message.content`、`message.reasoning_content`，以及 `usage` 的三个总数。

- [ ] 原始 JSON：保留没有解析的字段，例如 DeepSeek 的 `usage.prompt_cache_hit_tokens`，对应 Python 的 `model_extra`、`to_dict()`
- [ ] `message.tool_calls`：`function`（`id`、`function.name`、`function.arguments`）和 `custom` 两种
- [ ] `message.refusal`：拒答说明
- [ ] `message.annotations`：联网搜索的 `url_citation`
- [ ] `message.audio`：音频回复
- [ ] `choices[i].logprobs`：`content` 和 `refusal` 两组，每项有 `token`、`logprob`、`bytes`、`top_logprobs`
- [ ] `usage.completion_tokens_details`：`reasoning_tokens`、`accepted_prediction_tokens`、`rejected_prediction_tokens`、`audio_tokens`、`text_tokens`
- [ ] `usage.prompt_tokens_details`：`cached_tokens`、`cache_write_tokens`、`audio_tokens`、`image_tokens`、`text_tokens`
- [ ] `system_fingerprint`、`service_tier`
- [ ] `metadata`、`moderation`

## Chat Completions 的其他方法

已有非流式的 `create`。

- [ ] `parse()`：请求结构化输出并解析回复，`finish_reason` 为 `length` 或 `content_filter` 时失败（`LengthFinishReasonError`、`ContentFilterFinishReasonError`）；`strict: true` 的函数工具也解析调用参数。Python 从 Pydantic 模型生成 JSON Schema，C++ 没有反射，可以让调用方直接传 Schema。
- [ ] `retrieve`、`update`、`list`、`delete`、`messages.list`：管理 `store: true` 存下的补全，只有 OpenAI 提供

## 其他接口

> 先不实现，先确保 chat completions 的功能已经实现

`admin.*` 需要 admin key，Assistants API（`beta.assistants`、`beta.threads`）已弃用，其余 `beta.*` 变动频繁，这些都不实现。

兼容接口普遍提供：

- [ ] `models`：`list`、`retrieve`、`delete`
- [ ] `embeddings`：`create`
- [ ] `completions`：`create`，旧版文本补全；DeepSeek 的 FIM 补全也用这个格式

OpenAI：

- [ ] `responses`：`create`、`retrieve`、`delete`、`cancel`、`compact`、`input_items.list`、`input_tokens.count`、`parse`、`stream`，OpenAI 目前主推的接口
- [ ] `conversations`：`create`、`retrieve`、`update`、`delete`、`items.*`，与 Responses 配套
- [ ] `moderations`：`create`
- [ ] `files`：`create`、`retrieve`、`list`、`delete`、`content`、`retrieve_content`
- [ ] `uploads`：`create`、`complete`、`cancel`、`parts.create`，分片上传大文件
- [ ] `images`：`generate`、`edit`、`create_variation`
- [ ] `audio`：`speech`、`transcriptions`、`translations`、`voices`、`voice_consents`
- [ ] `batches`：`create`、`retrieve`、`list`、`cancel`
- [ ] `vector_stores`：增删改查、`search`、`files.*`、`file_batches.*`
- [ ] `webhooks`：`unwrap`、`verify_signature` 用 `webhook_secret` 在本地校验签名（失败时 `InvalidWebhookSignatureError`），适合 drogon 服务端接收 webhook；另有 webhook 的增删改查、`event_types.list`、`rotate_secret`、`test`
- [ ] `realtime`：WebSocket 连接（可以用 drogon 的 `WebSocketClient`）、`calls.*`、`client_secrets.create`、`translations.client_secrets.create`
- [ ] `live`：`create`、`sessions.*`
- [ ] `fine_tuning`：`jobs.*`、`checkpoints.permissions.*`、`alpha.graders.*`
- [ ] `evals`：增删改查、`runs.*`、`runs.output_items.*`
- [ ] `containers`：增删改查、`files.*`
- [ ] `videos`：`create`、`retrieve`、`list`、`delete`、`download_content`、`remix`、`edit`、`extend`、`create_character`、`get_character`
- [ ] `skills`：增删改查、`versions.*`、`content.retrieve`
- [ ] `decisions`、`content_provenance_checks`：`create`
- [ ] `safety`：`alerts.retrieve`、`cases.retrieve`
