# API resources

English · [简体中文](../zh-CN/api-resources.md)

[Project home](../../README.md) · [Plugin guide](drogon-plugin.md)

These bindings follow openai-python 3.41.0: 23 root resources, 57 resource classes and 146 HTTP methods in addition to Chat Completions. The SDK source is pinned to commit `05992039b9f7b78a48cf1e5a35727ba27d96db6e`. Maintenance tools extract paths, HTTP methods, fields, pagination kinds and special headers from that source; building and using the library requires no Python.

Authentication and deployment extensions remain deferred in `TODO.md`. The excluded resources are `admin.*`, deprecated Assistants APIs and other `beta.*`. Availability depends on the provider, model and account permissions.

## Types and calling conventions

Resources are plain value members of `OpenAI`, using Python names such as `client.vector_stores.files`; methods use PascalCase, such as `CreateAndPoll`. HTTP methods return `drogon::Task<Result<T>>` and take coroutine parameters by value. Signature verification performs synchronous local computation and returns `Result<T>`.

Each group has request types in `<drogon_openai/types/<group>.hpp>`, under `drogon_openai::types::<group>`. Primitive fields use C++ scalars and containers; nested objects, unions and extensions use `Json::Value`. An unset `std::optional` omits a field; assigning JSON null to `std::optional<Json::Value>` sends explicit null. Responses retain complete `raw` JSON and `_request_id`. Missing fields use defaults; present primitive fields are type checked. Nested JSON is not subjected to full schema validation.

Every parameter struct supports `extra_body`, `extra_query`, `extra_headers`, `timeout` and `max_retries`. Request overrides take precedence over named fields and client defaults. Query parameters and bodies are serialized separately; GET `extra_body` remains a body. Path IDs are encoded as single segments. Public headers and the `tools/sdk_resources.json` list every parameter, response field and SDK type annotation.

Typed JSON methods also expose `CreateRaw`, `RetrieveRaw` and similar variants, retaining status, headers, original bytes and parsed data. Binary endpoints return `RawResponse<std::string>` directly, with bytes in `parsed`. Union responses, such as JSON/text transcriptions, return `Json::Value`; text formats are JSON strings. Lists return `Page<T>` with `HasNextPage`, `GetNextPage` and `ForEach`. SDK single-page APIs, such as vector search, have no continuation. Conversations, Videos and checkpoint permissions use the response `last_id` cursor.

```cpp
auto models{ co_await client.models.List() };
auto embedding{ co_await client.embeddings.Create({ .input = "hello", .model = "text-embedding-3-small" }) };
auto response{ co_await client.responses.Create({ .input = "hello", .model = "gpt-4.1-mini" }) };
if (response) { LOG_INFO << response->output_text; }
```

Chat and Realtime can use the configured model default. Set model explicitly for other resources that require it, so each resource can select an appropriate model. Example model strings illustrate parameter placement.

## Responses parsing and streams

`responses.Parse(params)` accepts a JSON Schema in `text.format`, decodes final-answer `output_text` into each content part’s `parsed` field and sets `output_parsed`. It skips `phase=commentary` and refusals. Strict function arguments become `parsed_arguments`; non-strict function tools are rejected before sending. Schema enforcement is performed by the server. The library does not generate schemas from C++ types or provide a full local schema validator.

`CreateStream`, `RetrieveStream`, `GenerateStream` and related methods deliver `ApiStreamEvent {type, id, data}` incrementally. Returning false cancels successfully; returning an error aborts. `ApiStreamResult` retains event count, the last event, terminal response when present, cancellation and HTTP metadata; it does not retain all events. Started streams are never retried. Responses streams require a terminal event and report truncation otherwise; ordinary SSE also supports `[DONE]` and clean EOF. `responses.Stream` additionally accumulates output and returns the final `Response`, or the accumulated snapshot on cancellation. Callbacks run on the awaiting loop and must not block.

```cpp
auto stream{ co_await client.responses.CreateStream(
    { .input = "hello", .model = "gpt-4.1-mini" },
    [](drogon_openai::ApiStreamEvent const& event) -> drogon_openai::Result<bool> {
        LOG_INFO << event.type;
        return true;
    }) };
```

## Files, polling and chunked uploads

`UploadFile` owns content, filename and content_type, preserving NUL bytes. `FileInput` accepts one file, multiple files or a documented JSON file reference. Multipart uses SDK bracket notation and names such as `image[]` for multiple files. Realtime calls use `application/sdp` for a lone SDP field; with session present, SDP and JSON session parts have their own Content-Type.

Helpers include `files.WaitForProcessing`, `videos.Poll/CreateAndPoll`, `vector_stores.files.Poll/CreateAndPoll/Upload/UploadAndPoll` and `vector_stores.file_batches.Poll/CreateAndPoll/UploadAndPoll`. Polling returns terminal resources; inspect status and last_error. Failed/cancelled resource states are not automatically transport errors. PollOptions controls asynchronous intervals and the total deadline, respecting openai-poll-after-ms. Batch upload max_concurrency defaults to 5, must be positive and preserves existing file_ids.

`uploads.UploadFileChunked(file, purpose, part_size, options)` creates an upload, sends ordered parts and completes with ordered IDs. part_size defaults to 64 MiB and must be 1–64 MiB. Files and parts are held in memory. Failure returns the first error; uploaded files and incomplete server uploads are not automatically removed. Use Delete/Cancel for cleanup.

```cpp
auto file{ co_await client.files.Create({
    .file = drogon_openai::UploadFile{ .content = "hello", .filename = "notes.txt", .content_type = "text/plain" },
    .purpose = "assistants",
}) };
auto attached{ co_await client.vector_stores.files.UploadAndPoll(
    "vs_id", { .content = "hello", .filename = "notes.txt" }) };
```

## WebSocket

`responses.Connect()`, `realtime.Connect(model, options)`, `realtime.translations.Connect(model, options)`, `live.Connect()`, `live.sideband.Connect(session_id)` and `live.forks.Connect(session_id)` return `WebSocketSession`. Pass call_id in extra_query to attach to a Realtime call; this suppresses the default model. Header, organization, project and query overrides follow HTTP conventions. TLS validates certificate chains and hostnames; connect timeout bounds the handshake.

Sessions provide coroutine methods `Send(Json::Value)`, `Receive(timeout)`, `SendBytes`, `ReceiveBytes` and `Close`. Only one receive may be pending. Receive timeout is seconds, defaults to zero for no deadline, and leaves the session usable after timeout. Copies share the connection and Close. Operations move to its owner loop and resume on the awaiting loop. Successful send means the frame entered the socket queue, without remote acknowledgement. Unread data is bounded to 16 MiB. Error events remain JSON events; disconnect returns Connection, without automatic reconnect or replay. Close and release sessions before stopping drogon.

Drogon 1.9.13 WebSocketClient discards non-101 handshake responses. Rejected handshakes therefore return Connection without HTTP status, headers or body. Ordinary HTTP requests retain that metadata.

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

Set plugin `webhook_secret_env` to the name of an environment variable containing a raw secret or a `whsec_` base64 secret. An unset or empty variable prevents startup. Direct clients can use `OpenAIConfig.webhook_secret`; VerifySignature/Unwrap can override it per call. Secrets are never read directly from config.json.

`webhooks.VerifySignature(payload, headers, secret, tolerance)` uses webhook-id, webhook-timestamp and webhook-signature to verify HMAC-SHA256 over the original bytes with constant-time comparison and a default 300-second replay window. Invalid signatures or timestamps return `InvalidWebhookSignature`. `Unwrap` verifies before decoding the event JSON. Preserve the original body; do not reserialize JSON before verifying. Remote webhook CRUD and local verification share the same resource.

## Endpoint catalog

The tables omit Raw variants and common request overrides. Parameter names omit the namespace prefix `drogon_openai::types::<group>::`. Page and RawResponse belong to drogon_openai. See each type header for request and response fields.

### `models`

`<drogon_openai/types/models.hpp>`

| Resource and method | HTTP | Params | Result |
| --- | --- | --- | --- |
| `models.Retrieve(model, params)` | `GET /models/{model}` | `ModelsRetrieveParams` | `Model` |
| `models.List(params)` | `GET /models` | `ModelsListParams` | `Page<Model>` |
| `models.Delete(model, params)` | `DELETE /models/{model}` | `ModelsDeleteParams` | `ModelDeleted` |

### `embeddings`

`<drogon_openai/types/embeddings.hpp>`

| Resource and method | HTTP | Params | Result |
| --- | --- | --- | --- |
| `embeddings.Create(params)` | `POST /embeddings` | `EmbeddingCreateParams` | `CreateEmbeddingResponse` |

### `completions`

`<drogon_openai/types/completions.hpp>`

| Resource and method | HTTP | Params | Result |
| --- | --- | --- | --- |
| `completions.Create / CreateStream(params)` | `POST /completions` | `CompletionsCreateParams` | `Completion` |

### `responses`

`<drogon_openai/types/responses.hpp>`

| Resource and method | HTTP | Params | Result |
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

| Resource and method | HTTP | Params | Result |
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

| Resource and method | HTTP | Params | Result |
| --- | --- | --- | --- |
| `moderations.Create(params)` | `POST /moderations` | `ModerationCreateParams` | `ModerationCreateResponse` |

### `files`

`<drogon_openai/types/files.hpp>`

| Resource and method | HTTP | Params | Result |
| --- | --- | --- | --- |
| `files.Create(params)` | `POST /files` | `FileCreateParams` | `FileObject` |
| `files.Retrieve(file_id, params)` | `GET /files/{file_id}` | `FilesRetrieveParams` | `FileObject` |
| `files.List(params)` | `GET /files` | `FileListParams` | `Page<FileObject>` |
| `files.Delete(file_id, params)` | `DELETE /files/{file_id}` | `FilesDeleteParams` | `FileDeleted` |
| `files.Content(file_id, params)` | `GET /files/{file_id}/content` | `FilesContentParams` | `RawResponse<std::string>` |
| `files.RetrieveContent(file_id, params)` | `GET /files/{file_id}/content` | `FilesRetrieveContentParams` | `std::string` |

### `uploads`

`<drogon_openai/types/uploads.hpp>`

| Resource and method | HTTP | Params | Result |
| --- | --- | --- | --- |
| `uploads.parts.Create(upload_id, params)` | `POST /uploads/{upload_id}/parts` | `PartCreateParams` | `UploadPart` |
| `uploads.Create(params)` | `POST /uploads` | `UploadCreateParams` | `Upload` |
| `uploads.Cancel(upload_id, params)` | `POST /uploads/{upload_id}/cancel` | `UploadsCancelParams` | `Upload` |
| `uploads.Complete(upload_id, params)` | `POST /uploads/{upload_id}/complete` | `UploadCompleteParams` | `Upload` |

### `images`

`<drogon_openai/types/images.hpp>`

| Resource and method | HTTP | Params | Result |
| --- | --- | --- | --- |
| `images.CreateVariation(params)` | `POST /images/variations` | `ImageCreateVariationParams` | `ImagesResponse` |
| `images.Edit / EditStream(params)` | `POST /images/edits` | `ImagesEditParams` | `ImagesResponse` |
| `images.Generate / GenerateStream(params)` | `POST /images/generations` | `ImagesGenerateParams` | `ImagesResponse` |

### `audio`

`<drogon_openai/types/audio.hpp>`

| Resource and method | HTTP | Params | Result |
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

| Resource and method | HTTP | Params | Result |
| --- | --- | --- | --- |
| `batches.Create(params)` | `POST /batches` | `BatchCreateParams` | `Batch` |
| `batches.Retrieve(batch_id, params)` | `GET /batches/{batch_id}` | `BatchesRetrieveParams` | `Batch` |
| `batches.List(params)` | `GET /batches` | `BatchListParams` | `Page<Batch>` |
| `batches.Cancel(batch_id, params)` | `POST /batches/{batch_id}/cancel` | `BatchesCancelParams` | `Batch` |

### `vector_stores`

`<drogon_openai/types/vector_stores.hpp>`

| Resource and method | HTTP | Params | Result |
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

| Resource and method | HTTP | Params | Result |
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

| Resource and method | HTTP | Params | Result |
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

| Resource and method | HTTP | Params | Result |
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

| Resource and method | HTTP | Params | Result |
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

| Resource and method | HTTP | Params | Result |
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

| Resource and method | HTTP | Params | Result |
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

| Resource and method | HTTP | Params | Result |
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

| Resource and method | HTTP | Params | Result |
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

| Resource and method | HTTP | Params | Result |
| --- | --- | --- | --- |
| `decisions.Create(params)` | `POST /decisions` | `DecisionCreateParams` | `Decision` |

### `content_provenance_checks`

`<drogon_openai/types/content_provenance_checks.hpp>`

| Resource and method | HTTP | Params | Result |
| --- | --- | --- | --- |
| `content_provenance_checks.Create(params)` | `POST /content_provenance_checks` | `ContentProvenanceCheckCreateParams` | `ContentProvenanceCheck` |

### `safety`

`<drogon_openai/types/safety.hpp>`

| Resource and method | HTTP | Params | Result |
| --- | --- | --- | --- |
| `safety.alerts.Retrieve(id, params)` | `GET /safety/alerts/{id}` | `SafetyAlertsRetrieveParams` | `SafetyAlert` |
| `safety.cases.Retrieve(id, params)` | `GET /safety/cases/{id}` | `SafetyCasesRetrieveParams` | `SafetyCase` |

## Maintenance and verification

`tools/generate_resources.py` reads the pinned SDK to generate types, resource bindings and the catalog. `tools/generate_resource_tests.py` generates contract coverage for all 146 HTTP methods; `tools/generate_resource_docs.py` synchronizes these tables. Extension declarations live in resource_extensions.json; handwritten implementations are not overwritten. See `tools/README.md`.

`test_resources` checks every HTTP method, path, result kind, special header and metadata. Independent fixtures in `test_resource_wire` check query/body separation, JSON null, multipart, text/binary, path encoding and cursors. `test_resource_helpers` checks Responses SSE/Parse, upload polling, chunk order and an independent HMAC signature vector. `test_websocket` checks routes, frames, loop affinity, exclusive receive, deadlines and closing. These tests use local fake APIs without OpenAI credentials; they do not claim live verification of every remote product.

[openai-python 3.41.0](https://github.com/openai/openai-python/tree/05992039b9f7b78a48cf1e5a35727ba27d96db6e/src/openai) · [OpenAI streaming](https://developers.openai.com/api/docs/guides/streaming-responses) · [OpenAI webhooks](https://developers.openai.com/api/docs/guides/webhooks)
