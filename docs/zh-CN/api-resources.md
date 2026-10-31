# API 资源

[English](../en/api-resources.md) · 简体中文

[项目主页](../../README.zh-CN.md) · [插件指南](drogon-plugin.md)

这些接口对照 openai-python 3.41.0，覆盖 Chat Completions 以外的 23 个根资源、57 个资源类和 146 个 HTTP 方法。SDK 源码固定在提交 `05992039b9f7b78a48cf1e5a35727ba27d96db6e`。资源路径、HTTP 方法、字段、分页类型和特殊请求头由维护工具从源码提取；正常编译和使用不需要 Python。

认证与部署扩展仍按 `TODO.md` 暂缓。`admin.*`、已弃用的 Assistants API 和其余 `beta.*` 不在范围内。接口是否可用取决于服务商、模型和账号权限。

## 类型和调用约定

`OpenAI` 的资源是普通值成员，使用 Python 的小写名称，例如 `client.vector_stores.files`；方法使用 PascalCase，例如 `CreateAndPoll`。HTTP 方法返回 `drogon::Task<Result<T>>`，协程参数按值传入。验签是同步本地计算，返回 `Result<T>`。

每组请求类型位于 `<drogon_openai/types/<group>.hpp>`，命名空间为 `drogon_openai::types::<group>`。基本字段使用 C++ 标量和容器，嵌套对象、联合类型及扩展字段使用 `Json::Value`。`std::optional` 未赋值表示省略；`std::optional<Json::Value>` 赋 JSON null 表示显式 null。响应保留完整 `raw` 和 `_request_id`；缺失字段采用默认值，已出现的基本字段会校验类型。嵌套 JSON 不做完整 Schema 校验。

所有参数结构都支持 `extra_body`、`extra_query`、`extra_headers`、`timeout` 和 `max_retries`。请求级覆盖优先于具名参数和客户端默认值。查询参数和请求体分开序列化，GET 的 `extra_body` 仍是请求体。路径 ID 编码为单个路径段。各接口的完整参数、响应字段和 SDK 类型名见头文件及 `tools/sdk_resources.json`。

类型化 JSON 方法另有 `CreateRaw`、`RetrieveRaw` 等方法，保留状态码、响应头、原始字节及解析结果。二进制接口直接返回 `RawResponse<std::string>`，`parsed` 为原始字节。联合响应（例如转录的 JSON/文本）返回 `Json::Value`；文本格式使用 JSON 字符串。列表返回 `Page<T>`，通过 `HasNextPage`、`GetNextPage` 和 `ForEach` 遍历；SDK 单页接口（例如向量搜索）不创建下一页。Conversations、Videos 和 checkpoint permissions 使用响应的 `last_id` 游标。

```cpp
auto models{ co_await client.models.List() };
auto embedding{ co_await client.embeddings.Create({ .input = "hello", .model = "text-embedding-3-small" }) };
auto response{ co_await client.responses.Create({ .input = "hello", .model = "gpt-4.1-mini" }) };
if (response) { LOG_INFO << response->output_text; }
```

Chat 和 Realtime 可使用插件配置的默认模型；其他资源显式设置其参数中的 model（接口需要时），以便不同资源选择合适的模型。示例模型仅演示参数位置。

## Responses 解析和流

`responses.Parse(params)` 接受 `text.format` 的 JSON Schema，解码最终回答的 `output_text` 到内容项的 `parsed`，并设置 `output_parsed`。`phase=commentary` 不解析；拒答保持原样。strict function 工具的 arguments 解码为 `parsed_arguments`；发送前拒绝非 strict 的 function 工具。Schema 由服务端约束，库不从 C++ 类型生成 Schema，也不提供本地完整 Schema 校验器。

`CreateStream`、`RetrieveStream`、`GenerateStream` 等流式方法逐个交付 `ApiStreamEvent {type, id, data}`。回调返回 false 成功取消、返回错误终止。`ApiStreamResult` 保留事件数、最后事件、终态 response（若有）、取消标记和 HTTP 元数据；底层不保存全部事件。已开始的流不自动重试。Responses 流必须收到终态事件，否则报截断；普通 SSE 流也支持 `[DONE]` 和正常 EOF。`responses.Stream` 额外累积输出并返回最终 `Response`；取消时返回已累积快照。回调在等待该请求的事件循环上执行，不得阻塞。

```cpp
auto stream{ co_await client.responses.CreateStream(
    { .input = "hello", .model = "gpt-4.1-mini" },
    [](drogon_openai::ApiStreamEvent const& event) -> drogon_openai::Result<bool> {
        LOG_INFO << event.type;
        return true;
    }) };
```

## 文件、轮询和分块上传

`UploadFile` 拥有 content、filename 和 content_type，保留 NUL 字节。`FileInput` 接受单文件、多文件数组或接口定义的 JSON 文件引用。multipart 使用 SDK 的括号字段格式，多文件使用 `image[]` 等名称。Realtime calls 的 SDP 单字段请求使用 `application/sdp`；包含 session 时，SDP 和 JSON session 使用各自的 Content-Type。

提供 `files.WaitForProcessing`、`videos.Poll/CreateAndPoll`、`vector_stores.files.Poll/CreateAndPoll/Upload/UploadAndPoll` 和 `vector_stores.file_batches.Poll/CreateAndPoll/UploadAndPoll`。轮询返回终态资源，调用方检查 status 及 last_error；failed/cancelled 是资源终态，不自动转换为传输错误。PollOptions 控制异步间隔和总截止时间，保留服务端 openai-poll-after-ms 提示。批量上传的 max_concurrency 默认为 5，必须大于 0，并保留已有 file_ids。

`uploads.UploadFileChunked(file, purpose, part_size, options)` 创建上传、按顺序上传分片并按 ID 顺序完成。part_size 默认 64 MiB，范围为 1 到 64 MiB。文件和分片在内存中；失败时返回首个错误，不会自动删除已上传文件或取消服务端上传，可显式调用 Delete/Cancel 清理。

```cpp
auto file{ co_await client.files.Create({
    .file = drogon_openai::UploadFile{ .content = "hello", .filename = "notes.txt", .content_type = "text/plain" },
    .purpose = "assistants",
}) };
auto attached{ co_await client.vector_stores.files.UploadAndPoll(
    "vs_id", { .content = "hello", .filename = "notes.txt" }) };
```

## WebSocket

`responses.Connect()`、`realtime.Connect(model, options)`、`realtime.translations.Connect(model, options)`、`live.Connect()`、`live.sideband.Connect(session_id)` 和 `live.forks.Connect(session_id)` 返回 `WebSocketSession`。通过额外查询参数传 call_id 可连接已有 Realtime 通话；此时不自动补默认模型。请求头、组织、项目和查询覆盖与 HTTP 一致。TLS 校验证书链和主机名，connect 超时控制握手。

会话支持 `Send(Json::Value)`、`Receive(timeout)`、`SendBytes`、`ReceiveBytes` 和 `Close`，均为协程。一次只能有一个 Receive；timeout 为秒，默认 0 表示无限等待，超时后仍可继续接收。会话副本共享连接和 Close，操作会切到连接所属 loop，完成后恢复等待者的 loop。发送成功表示帧已交给 socket 队列，不表示远端确认。未读数据队列上限为 16 MiB。错误事件按原始 JSON 交付；断开返回 Connection，不会自动重连或重放。应在 drogon 停止前关闭并释放会话。

Drogon 1.9.13 的 WebSocketClient 会丢弃非 101 的握手响应，因此被拒绝的握手只能返回 Connection，无法提供 HTTP 状态、响应头或响应体。常规 HTTP 请求不受此限制。

```cpp
auto session{ co_await client.realtime.Connect("gpt-realtime") };
if (session) {
    Json::Value event;
    event["type"] = "response.create";
    auto sent{ co_await session->Send(event) };
    auto received{ co_await session->Receive(30) };
    auto closed{ co_await session->Close() };
}
```

## Webhook

插件配置可设置 `webhook_secret_env`，读取含原始 secret 或 `whsec_` 前缀 base64 secret 的环境变量；该变量未设置或为空会阻止启动。直接构造客户端可设置 `OpenAIConfig.webhook_secret`，也可在 VerifySignature/Unwrap 中覆盖。密钥不写入 config.json。

`webhooks.VerifySignature(payload, headers, secret, tolerance)` 使用 webhook-id、webhook-timestamp、webhook-signature，对原始请求字节执行 HMAC-SHA256 常量时间比较；默认重放窗口 300 秒。签名或时间戳无效时返回 `InvalidWebhookSignature`。`Unwrap` 先验签，再解码事件 JSON。保留原始 body，不能先重新序列化 JSON 再验签。远程 webhooks CRUD 与本地验签共用同一资源。

## 接口目录

下面省略 Raw 变体和共用请求覆盖字段。params 列省略命名空间前缀 `drogon_openai::types::<group>::`；结果中的 Page 和 RawResponse 均位于 drogon_openai。参数字段和返回字段见对应类型头文件。

### `models`

`<drogon_openai/types/models.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `models.Retrieve(model, params)` | `GET /models/{model}` | `ModelsRetrieveParams` | `Model` |
| `models.List(params)` | `GET /models` | `ModelsListParams` | `Page<Model>` |
| `models.Delete(model, params)` | `DELETE /models/{model}` | `ModelsDeleteParams` | `ModelDeleted` |

### `embeddings`

`<drogon_openai/types/embeddings.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `embeddings.Create(params)` | `POST /embeddings` | `EmbeddingCreateParams` | `CreateEmbeddingResponse` |

### `completions`

`<drogon_openai/types/completions.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `completions.Create / CreateStream(params)` | `POST /completions` | `CompletionsCreateParams` | `Completion` |

### `responses`

`<drogon_openai/types/responses.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `responses.input_items.List(response_id, params)` | `GET /responses/{response_id}/input_items` | `InputItemListParams` | `Page<Json::Value>` |
| `responses.input_tokens.Count(params)` | `POST /responses/input_tokens` | `InputTokenCountParams` | `InputTokenCountResponse` |
| `responses.Create / CreateStream(params)` | `POST /responses` | `ResponsesCreateParams` | `Response` |
| `responses.Retrieve / RetrieveStream(response_id, params)` | `GET /responses/{response_id}` | `ResponseRetrieveParams` | `Response` |
| `responses.Delete(response_id, params)` | `DELETE /responses/{response_id}` | `ResponsesDeleteParams` | `void` |
| `responses.Cancel(response_id, params)` | `POST /responses/{response_id}/cancel` | `ResponsesCancelParams` | `Response` |
| `responses.Compact(params)` | `POST /responses/compact` | `ResponseCompactParams` | `CompactedResponse` |

### `conversations`

`<drogon_openai/types/conversations.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `conversations.Create(params)` | `POST /conversations` | `ConversationCreateParams` | `Conversation` |
| `conversations.Retrieve(conversation_id, params)` | `GET /conversations/{conversation_id}` | `ConversationsRetrieveParams` | `Conversation` |
| `conversations.Update(conversation_id, params)` | `POST /conversations/{conversation_id}` | `ConversationUpdateParams` | `Conversation` |
| `conversations.Delete(conversation_id, params)` | `DELETE /conversations/{conversation_id}` | `ConversationsDeleteParams` | `ConversationDeletedResource` |
| `conversations.items.Create(conversation_id, params)` | `POST /conversations/{conversation_id}/items` | `ItemCreateParams` | `ConversationItemList` |
| `conversations.items.Retrieve(item_id, conversation_id, params)` | `GET /conversations/{conversation_id}/items/{item_id}` | `ItemRetrieveParams` | `Json::Value` |
| `conversations.items.List(conversation_id, params)` | `GET /conversations/{conversation_id}/items` | `ItemListParams` | `Page<Json::Value>` |
| `conversations.items.Delete(item_id, conversation_id, params)` | `DELETE /conversations/{conversation_id}/items/{item_id}` | `ConversationsItemsDeleteParams` | `Conversation` |

### `moderations`

`<drogon_openai/types/moderations.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `moderations.Create(params)` | `POST /moderations` | `ModerationCreateParams` | `ModerationCreateResponse` |

### `files`

`<drogon_openai/types/files.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `files.Create(params)` | `POST /files` | `FileCreateParams` | `FileObject` |
| `files.Retrieve(file_id, params)` | `GET /files/{file_id}` | `FilesRetrieveParams` | `FileObject` |
| `files.List(params)` | `GET /files` | `FileListParams` | `Page<FileObject>` |
| `files.Delete(file_id, params)` | `DELETE /files/{file_id}` | `FilesDeleteParams` | `FileDeleted` |
| `files.Content(file_id, params)` | `GET /files/{file_id}/content` | `FilesContentParams` | `RawResponse<std::string>` |
| `files.RetrieveContent(file_id, params)` | `GET /files/{file_id}/content` | `FilesRetrieveContentParams` | `std::string` |

### `uploads`

`<drogon_openai/types/uploads.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `uploads.parts.Create(upload_id, params)` | `POST /uploads/{upload_id}/parts` | `PartCreateParams` | `UploadPart` |
| `uploads.Create(params)` | `POST /uploads` | `UploadCreateParams` | `Upload` |
| `uploads.Cancel(upload_id, params)` | `POST /uploads/{upload_id}/cancel` | `UploadsCancelParams` | `Upload` |
| `uploads.Complete(upload_id, params)` | `POST /uploads/{upload_id}/complete` | `UploadCompleteParams` | `Upload` |

### `images`

`<drogon_openai/types/images.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `images.CreateVariation(params)` | `POST /images/variations` | `ImageCreateVariationParams` | `ImagesResponse` |
| `images.Edit / EditStream(params)` | `POST /images/edits` | `ImagesEditParams` | `ImagesResponse` |
| `images.Generate / GenerateStream(params)` | `POST /images/generations` | `ImagesGenerateParams` | `ImagesResponse` |

### `audio`

`<drogon_openai/types/audio.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `audio.speech.Create(params)` | `POST /audio/speech` | `SpeechCreateParams` | `RawResponse<std::string>` |
| `audio.transcriptions.Create / CreateStream(params)` | `POST /audio/transcriptions` | `AudioTranscriptionsCreateParams` | `Json::Value` |
| `audio.translations.Create(params)` | `POST /audio/translations` | `TranslationCreateParams` | `Json::Value` |
| `audio.voice_consents.Create(params)` | `POST /audio/voice_consents` | `VoiceConsentCreateParams` | `VoiceConsent` |
| `audio.voice_consents.Retrieve(consent_id, params)` | `GET /audio/voice_consents/{consent_id}` | `AudioVoiceConsentsRetrieveParams` | `VoiceConsent` |
| `audio.voice_consents.Update(consent_id, params)` | `POST /audio/voice_consents/{consent_id}` | `VoiceConsentUpdateParams` | `VoiceConsent` |
| `audio.voice_consents.List(params)` | `GET /audio/voice_consents` | `VoiceConsentListParams` | `Page<VoiceConsent>` |
| `audio.voice_consents.Delete(consent_id, params)` | `DELETE /audio/voice_consents/{consent_id}` | `AudioVoiceConsentsDeleteParams` | `VoiceConsentDeleted` |
| `audio.voices.Create(params)` | `POST /audio/voices` | `VoiceCreateParams` | `Voice` |
| `audio.voices.Delete(voice_id, params)` | `DELETE /audio/voices/{voice_id}` | `AudioVoicesDeleteParams` | `VoiceDeleted` |

### `batches`

`<drogon_openai/types/batches.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `batches.Create(params)` | `POST /batches` | `BatchCreateParams` | `Batch` |
| `batches.Retrieve(batch_id, params)` | `GET /batches/{batch_id}` | `BatchesRetrieveParams` | `Batch` |
| `batches.List(params)` | `GET /batches` | `BatchListParams` | `Page<Batch>` |
| `batches.Cancel(batch_id, params)` | `POST /batches/{batch_id}/cancel` | `BatchesCancelParams` | `Batch` |

### `vector_stores`

`<drogon_openai/types/vector_stores.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `vector_stores.file_batches.Create(vector_store_id, params)` | `POST /vector_stores/{vector_store_id}/file_batches` | `FileBatchCreateParams` | `VectorStoreFileBatch` |
| `vector_stores.file_batches.Retrieve(batch_id, vector_store_id, params)` | `GET /vector_stores/{vector_store_id}/file_batches/{batch_id}` | `VectorStoresFileBatchesRetrieveParams` | `VectorStoreFileBatch` |
| `vector_stores.file_batches.Cancel(batch_id, vector_store_id, params)` | `POST /vector_stores/{vector_store_id}/file_batches/{batch_id}/cancel` | `VectorStoresFileBatchesCancelParams` | `VectorStoreFileBatch` |
| `vector_stores.file_batches.ListFiles(batch_id, vector_store_id, params)` | `GET /vector_stores/{vector_store_id}/file_batches/{batch_id}/files` | `FileBatchListFilesParams` | `Page<VectorStoreFile>` |
| `vector_stores.files.Create(vector_store_id, params)` | `POST /vector_stores/{vector_store_id}/files` | `FileCreateParams` | `VectorStoreFile` |
| `vector_stores.files.Retrieve(file_id, vector_store_id, params)` | `GET /vector_stores/{vector_store_id}/files/{file_id}` | `VectorStoresFilesRetrieveParams` | `VectorStoreFile` |
| `vector_stores.files.Update(file_id, vector_store_id, params)` | `POST /vector_stores/{vector_store_id}/files/{file_id}` | `FileUpdateParams` | `VectorStoreFile` |
| `vector_stores.files.List(vector_store_id, params)` | `GET /vector_stores/{vector_store_id}/files` | `FileListParams` | `Page<VectorStoreFile>` |
| `vector_stores.files.Delete(file_id, vector_store_id, params)` | `DELETE /vector_stores/{vector_store_id}/files/{file_id}` | `VectorStoresFilesDeleteParams` | `VectorStoreFileDeleted` |
| `vector_stores.files.Content(file_id, vector_store_id, params)` | `GET /vector_stores/{vector_store_id}/files/{file_id}/content` | `VectorStoresFilesContentParams` | `Page<FileContentResponse>` |
| `vector_stores.Create(params)` | `POST /vector_stores` | `VectorStoreCreateParams` | `VectorStore` |
| `vector_stores.Retrieve(vector_store_id, params)` | `GET /vector_stores/{vector_store_id}` | `VectorStoresRetrieveParams` | `VectorStore` |
| `vector_stores.Update(vector_store_id, params)` | `POST /vector_stores/{vector_store_id}` | `VectorStoreUpdateParams` | `VectorStore` |
| `vector_stores.List(params)` | `GET /vector_stores` | `VectorStoreListParams` | `Page<VectorStore>` |
| `vector_stores.Delete(vector_store_id, params)` | `DELETE /vector_stores/{vector_store_id}` | `VectorStoresDeleteParams` | `VectorStoreDeleted` |
| `vector_stores.Search(vector_store_id, params)` | `POST /vector_stores/{vector_store_id}/search` | `VectorStoreSearchParams` | `Page<VectorStoreSearchResponse>` |

### `webhooks`

`<drogon_openai/types/webhooks.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `webhooks.event_types.List(params)` | `GET /webhook_event_types` | `WebhooksEventTypesListParams` | `WebhookEventTypeList` |
| `webhooks.Create(params)` | `POST /webhook_endpoints` | `WebhookCreateParams` | `WebhookEndpointWithSecret` |
| `webhooks.Retrieve(webhook_endpoint_id, params)` | `GET /webhook_endpoints/{webhook_endpoint_id}` | `WebhooksRetrieveParams` | `WebhookEndpoint` |
| `webhooks.Update(webhook_endpoint_id, params)` | `POST /webhook_endpoints/{webhook_endpoint_id}` | `WebhookUpdateParams` | `WebhookEndpoint` |
| `webhooks.List(params)` | `GET /webhook_endpoints` | `WebhookListParams` | `Page<WebhookEndpoint>` |
| `webhooks.Delete(webhook_endpoint_id, params)` | `DELETE /webhook_endpoints/{webhook_endpoint_id}` | `WebhooksDeleteParams` | `DeletedWebhookEndpoint` |
| `webhooks.RotateSecret(webhook_endpoint_id, params)` | `POST /webhook_endpoints/{webhook_endpoint_id}/rotate_secret` | `WebhookRotateSecretParams` | `WebhookEndpointWithSecret` |
| `webhooks.Test(webhook_endpoint_id, params)` | `POST /webhook_endpoints/{webhook_endpoint_id}/test` | `WebhookTestParams` | `WebhookEndpointTestResult` |

### `realtime`

`<drogon_openai/types/realtime.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `realtime.calls.Create(params)` | `POST /realtime/calls` | `CallCreateParams` | `RawResponse<std::string>` |
| `realtime.calls.Accept(call_id, params)` | `POST /realtime/calls/{call_id}/accept` | `CallAcceptParams` | `void` |
| `realtime.calls.Hangup(call_id, params)` | `POST /realtime/calls/{call_id}/hangup` | `RealtimeCallsHangupParams` | `void` |
| `realtime.calls.Refer(call_id, params)` | `POST /realtime/calls/{call_id}/refer` | `CallReferParams` | `void` |
| `realtime.calls.Reject(call_id, params)` | `POST /realtime/calls/{call_id}/reject` | `CallRejectParams` | `void` |
| `realtime.client_secrets.Create(params)` | `POST /realtime/client_secrets` | `ClientSecretCreateParams` | `ClientSecretCreateResponse` |
| `realtime.translations.client_secrets.Create(params)` | `POST /realtime/translations/client_secrets` | `RealtimeTranslationsClientSecretsCreateParams` | `RealtimeTranslationClientSecretCreateResponse` |

### `live`

`<drogon_openai/types/live.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `live.Create(params)` | `POST /live/sessions` | `LiveCreateParams` | `LiveCreateResponse` |
| `live.sessions.Accept(session_id, params)` | `POST /live/sessions/{session_id}/accept` | `SessionAcceptParams` | `void` |
| `live.sessions.DownloadRecording(session_id, params)` | `GET /live/sessions/{session_id}/content` | `LiveSessionsDownloadRecordingParams` | `RawResponse<std::string>` |
| `live.sessions.Fork(session_id, params)` | `POST /live/sessions/{session_id}/fork` | `SessionForkParams` | `SessionForkResponse` |
| `live.sessions.Hangup(session_id, params)` | `POST /live/sessions/{session_id}/hangup` | `LiveSessionsHangupParams` | `void` |
| `live.sessions.Refer(session_id, params)` | `POST /live/sessions/{session_id}/refer` | `SessionReferParams` | `void` |
| `live.sessions.Reject(session_id, params)` | `POST /live/sessions/{session_id}/reject` | `SessionRejectParams` | `void` |

### `fine_tuning`

`<drogon_openai/types/fine_tuning.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `fine_tuning.alpha.graders.Run(params)` | `POST /fine_tuning/alpha/graders/run` | `GraderRunParams` | `GraderRunResponse` |
| `fine_tuning.alpha.graders.Validate(params)` | `POST /fine_tuning/alpha/graders/validate` | `GraderValidateParams` | `GraderValidateResponse` |
| `fine_tuning.checkpoints.permissions.Create(fine_tuned_model_checkpoint, params)` | `POST /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions` | `PermissionCreateParams` | `Page<PermissionCreateResponse>` |
| `fine_tuning.checkpoints.permissions.Retrieve(fine_tuned_model_checkpoint, params)` | `GET /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions` | `PermissionRetrieveParams` | `PermissionRetrieveResponse` |
| `fine_tuning.checkpoints.permissions.List(fine_tuned_model_checkpoint, params)` | `GET /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions` | `PermissionListParams` | `Page<PermissionListResponse>` |
| `fine_tuning.checkpoints.permissions.Delete(permission_id, fine_tuned_model_checkpoint, params)` | `DELETE /fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions/{permission_id}` | `FineTuningCheckpointsPermissionsDeleteParams` | `PermissionDeleteResponse` |
| `fine_tuning.jobs.checkpoints.List(fine_tuning_job_id, params)` | `GET /fine_tuning/jobs/{fine_tuning_job_id}/checkpoints` | `CheckpointListParams` | `Page<FineTuningJobCheckpoint>` |
| `fine_tuning.jobs.Create(params)` | `POST /fine_tuning/jobs` | `JobCreateParams` | `FineTuningJob` |
| `fine_tuning.jobs.Retrieve(fine_tuning_job_id, params)` | `GET /fine_tuning/jobs/{fine_tuning_job_id}` | `FineTuningJobsRetrieveParams` | `FineTuningJob` |
| `fine_tuning.jobs.List(params)` | `GET /fine_tuning/jobs` | `JobListParams` | `Page<FineTuningJob>` |
| `fine_tuning.jobs.Cancel(fine_tuning_job_id, params)` | `POST /fine_tuning/jobs/{fine_tuning_job_id}/cancel` | `FineTuningJobsCancelParams` | `FineTuningJob` |
| `fine_tuning.jobs.ListEvents(fine_tuning_job_id, params)` | `GET /fine_tuning/jobs/{fine_tuning_job_id}/events` | `JobListEventsParams` | `Page<FineTuningJobEvent>` |
| `fine_tuning.jobs.Pause(fine_tuning_job_id, params)` | `POST /fine_tuning/jobs/{fine_tuning_job_id}/pause` | `FineTuningJobsPauseParams` | `FineTuningJob` |
| `fine_tuning.jobs.Resume(fine_tuning_job_id, params)` | `POST /fine_tuning/jobs/{fine_tuning_job_id}/resume` | `FineTuningJobsResumeParams` | `FineTuningJob` |

### `evals`

`<drogon_openai/types/evals.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `evals.Create(params)` | `POST /evals` | `EvalCreateParams` | `EvalCreateResponse` |
| `evals.Retrieve(eval_id, params)` | `GET /evals/{eval_id}` | `EvalsRetrieveParams` | `EvalRetrieveResponse` |
| `evals.Update(eval_id, params)` | `POST /evals/{eval_id}` | `EvalUpdateParams` | `EvalUpdateResponse` |
| `evals.List(params)` | `GET /evals` | `EvalListParams` | `Page<EvalListResponse>` |
| `evals.Delete(eval_id, params)` | `DELETE /evals/{eval_id}` | `EvalsDeleteParams` | `EvalDeleteResponse` |
| `evals.runs.output_items.Retrieve(output_item_id, eval_id, run_id, params)` | `GET /evals/{eval_id}/runs/{run_id}/output_items/{output_item_id}` | `EvalsRunsOutputItemsRetrieveParams` | `OutputItemRetrieveResponse` |
| `evals.runs.output_items.List(run_id, eval_id, params)` | `GET /evals/{eval_id}/runs/{run_id}/output_items` | `OutputItemListParams` | `Page<OutputItemListResponse>` |
| `evals.runs.Create(eval_id, params)` | `POST /evals/{eval_id}/runs` | `RunCreateParams` | `RunCreateResponse` |
| `evals.runs.Retrieve(run_id, eval_id, params)` | `GET /evals/{eval_id}/runs/{run_id}` | `EvalsRunsRetrieveParams` | `RunRetrieveResponse` |
| `evals.runs.List(eval_id, params)` | `GET /evals/{eval_id}/runs` | `RunListParams` | `Page<RunListResponse>` |
| `evals.runs.Delete(run_id, eval_id, params)` | `DELETE /evals/{eval_id}/runs/{run_id}` | `EvalsRunsDeleteParams` | `RunDeleteResponse` |
| `evals.runs.Cancel(run_id, eval_id, params)` | `POST /evals/{eval_id}/runs/{run_id}/cancel` | `EvalsRunsCancelParams` | `RunCancelResponse` |

### `containers`

`<drogon_openai/types/containers.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `containers.Create(params)` | `POST /containers` | `ContainerCreateParams` | `ContainerCreateResponse` |
| `containers.Retrieve(container_id, params)` | `GET /containers/{container_id}` | `ContainersRetrieveParams` | `ContainerRetrieveResponse` |
| `containers.List(params)` | `GET /containers` | `ContainerListParams` | `Page<ContainerListResponse>` |
| `containers.Delete(container_id, params)` | `DELETE /containers/{container_id}` | `ContainersDeleteParams` | `void` |
| `containers.files.content.Retrieve(file_id, container_id, params)` | `GET /containers/{container_id}/files/{file_id}/content` | `ContainersFilesContentRetrieveParams` | `RawResponse<std::string>` |
| `containers.files.Create(container_id, params)` | `POST /containers/{container_id}/files` | `FileCreateParams` | `FileCreateResponse` |
| `containers.files.Retrieve(file_id, container_id, params)` | `GET /containers/{container_id}/files/{file_id}` | `ContainersFilesRetrieveParams` | `FileRetrieveResponse` |
| `containers.files.List(container_id, params)` | `GET /containers/{container_id}/files` | `FileListParams` | `Page<FileListResponse>` |
| `containers.files.Delete(file_id, container_id, params)` | `DELETE /containers/{container_id}/files/{file_id}` | `ContainersFilesDeleteParams` | `void` |

### `videos`

`<drogon_openai/types/videos.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `videos.Create(params)` | `POST /videos` | `VideoCreateParams` | `Video` |
| `videos.Retrieve(video_id, params)` | `GET /videos/{video_id}` | `VideosRetrieveParams` | `Video` |
| `videos.List(params)` | `GET /videos` | `VideoListParams` | `Page<Video>` |
| `videos.Delete(video_id, params)` | `DELETE /videos/{video_id}` | `VideosDeleteParams` | `VideoDeleteResponse` |
| `videos.CreateCharacter(params)` | `POST /videos/characters` | `VideoCreateCharacterParams` | `VideoCreateCharacterResponse` |
| `videos.DownloadContent(video_id, params)` | `GET /videos/{video_id}/content` | `VideoDownloadContentParams` | `RawResponse<std::string>` |
| `videos.Edit(params)` | `POST /videos/edits` | `VideoEditParams` | `Video` |
| `videos.Extend(params)` | `POST /videos/extensions` | `VideoExtendParams` | `Video` |
| `videos.GetCharacter(character_id, params)` | `GET /videos/characters/{character_id}` | `VideosGetCharacterParams` | `VideoGetCharacterResponse` |
| `videos.Remix(video_id, params)` | `POST /videos/{video_id}/remix` | `VideoRemixParams` | `Video` |

### `skills`

`<drogon_openai/types/skills.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `skills.content.Retrieve(skill_id, params)` | `GET /skills/{skill_id}/content` | `SkillsContentRetrieveParams` | `RawResponse<std::string>` |
| `skills.Create(params)` | `POST /skills` | `SkillCreateParams` | `Skill` |
| `skills.Retrieve(skill_id, params)` | `GET /skills/{skill_id}` | `SkillsRetrieveParams` | `Skill` |
| `skills.Update(skill_id, params)` | `POST /skills/{skill_id}` | `SkillUpdateParams` | `Skill` |
| `skills.List(params)` | `GET /skills` | `SkillListParams` | `Page<Skill>` |
| `skills.Delete(skill_id, params)` | `DELETE /skills/{skill_id}` | `SkillsDeleteParams` | `DeletedSkill` |
| `skills.versions.content.Retrieve(version, skill_id, params)` | `GET /skills/{skill_id}/versions/{version}/content` | `SkillsVersionsContentRetrieveParams` | `RawResponse<std::string>` |
| `skills.versions.Create(skill_id, params)` | `POST /skills/{skill_id}/versions` | `VersionCreateParams` | `SkillVersion` |
| `skills.versions.Retrieve(version, skill_id, params)` | `GET /skills/{skill_id}/versions/{version}` | `SkillsVersionsRetrieveParams` | `SkillVersion` |
| `skills.versions.List(skill_id, params)` | `GET /skills/{skill_id}/versions` | `VersionListParams` | `Page<SkillVersion>` |
| `skills.versions.Delete(version, skill_id, params)` | `DELETE /skills/{skill_id}/versions/{version}` | `SkillsVersionsDeleteParams` | `DeletedSkillVersion` |

### `decisions`

`<drogon_openai/types/decisions.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `decisions.Create(params)` | `POST /decisions` | `DecisionCreateParams` | `Decision` |

### `content_provenance_checks`

`<drogon_openai/types/content_provenance_checks.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `content_provenance_checks.Create(params)` | `POST /content_provenance_checks` | `ContentProvenanceCheckCreateParams` | `ContentProvenanceCheck` |

### `safety`

`<drogon_openai/types/safety.hpp>`

| 资源与方法 | HTTP | params | 结果 |
| --- | --- | --- | --- |
| `safety.alerts.Retrieve(id, params)` | `GET /safety/alerts/{id}` | `SafetyAlertsRetrieveParams` | `SafetyAlert` |
| `safety.cases.Retrieve(id, params)` | `GET /safety/cases/{id}` | `SafetyCasesRetrieveParams` | `SafetyCase` |

## 维护和验证

`tools/generate_resources.py` 读取固定 SDK 源码生成类型、资源绑定和目录，`tools/generate_resource_tests.py` 生成 146 个接口契约用例，本文件的表格由 `tools/generate_resource_docs.py` 同步。扩展方法声明见 resource_extensions.json，实现为手写源码，不会被生成器覆盖。操作步骤见 `tools/README.md`。

`test_resources` 核对全部 HTTP 方法、路径、类型、特殊请求头和元数据；`test_resource_wire` 使用独立报文核对查询、请求体、JSON null、multipart、文本、二进制、路径编码及游标；`test_resource_helpers` 验证 Responses SSE/Parse、上传轮询、分块顺序和独立 HMAC 签名向量；`test_websocket` 验证路由、帧、线程恢复、并发接收限制、超时和关闭。测试在本地 fake API 上执行，不需要 OpenAI 凭证，也不代表所有远程产品已进行实网验证。

[openai-python 3.41.0](https://github.com/openai/openai-python/tree/05992039b9f7b78a48cf1e5a35727ba27d96db6e/src/openai) · [OpenAI streaming](https://developers.openai.com/api/docs/guides/streaming-responses) · [OpenAI webhooks](https://developers.openai.com/api/docs/guides/webhooks)
