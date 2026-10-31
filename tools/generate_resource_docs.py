#!/usr/bin/env python3
"""Render mirrored resource reference tables from the checked-in SDK catalog."""
import json
from pathlib import Path

root = Path(__file__).resolve().parents[1]
spec = json.loads((root / 'tools/sdk_resources.json').read_text())
for lang in ('en', 'zh-CN'):
    zh = lang == 'zh-CN'
    lines = [
        '# ' + ('API 资源' if zh else 'API resources'), '',
        '[English](../en/api-resources.md) · 简体中文' if zh else 'English · [简体中文](../zh-CN/api-resources.md)', '',
        '[项目主页](../../README.zh-CN.md) · [插件指南](drogon-plugin.md)' if zh else '[Project home](../../README.md) · [Plugin guide](drogon-plugin.md)', '',
        '这些接口对照 openai-python 3.41.0，覆盖 Chat Completions 以外的 23 个根资源、57 个资源类和 146 个 HTTP 方法。SDK 源码固定在提交 `05992039b9f7b78a48cf1e5a35727ba27d96db6e`。资源路径、HTTP 方法、字段、分页类型和特殊请求头由维护工具从源码提取；正常编译和使用不需要 Python。' if zh else
        'These bindings follow openai-python 3.41.0: 23 root resources, 57 resource classes and 146 HTTP methods in addition to Chat Completions. The SDK source is pinned to commit `05992039b9f7b78a48cf1e5a35727ba27d96db6e`. Maintenance tools extract paths, HTTP methods, fields, pagination kinds and special headers from that source; building and using the library requires no Python.', '',
        '认证与部署扩展仍按 `TODO.md` 暂缓。`admin.*`、已弃用的 Assistants API 和其余 `beta.*` 不在范围内。接口是否可用取决于服务商、模型和账号权限。' if zh else
        'Authentication and deployment extensions remain deferred in `TODO.md`. The excluded resources are `admin.*`, deprecated Assistants APIs and other `beta.*`. Availability depends on the provider, model and account permissions.', '',
        '## ' + ('类型和调用约定' if zh else 'Types and calling conventions'), '',
        '`OpenAI` 的资源是普通值成员，使用 Python 的小写名称，例如 `client.vector_stores.files`；方法使用 PascalCase，例如 `CreateAndPoll`。HTTP 方法返回 `drogon::Task<Result<T>>`，协程参数按值传入。验签是同步本地计算，返回 `Result<T>`。' if zh else
        'Resources are plain value members of `OpenAI`, using Python names such as `client.vector_stores.files`; methods use PascalCase, such as `CreateAndPoll`. HTTP methods return `drogon::Task<Result<T>>` and take coroutine parameters by value. Signature verification performs synchronous local computation and returns `Result<T>`.', '',
        '每组请求类型位于 `<drogon_openai/types/<group>.hpp>`，命名空间为 `drogon_openai::types::<group>`。基本字段使用 C++ 标量和容器，嵌套对象、联合类型及扩展字段使用 `Json::Value`。`std::optional` 未赋值表示省略；`std::optional<Json::Value>` 赋 JSON null 表示显式 null。响应保留完整 `raw` 和 `_request_id`；缺失字段采用默认值，已出现的基本字段会校验类型。嵌套 JSON 不做完整 Schema 校验。' if zh else
        'Each group has request types in `<drogon_openai/types/<group>.hpp>`, under `drogon_openai::types::<group>`. Primitive fields use C++ scalars and containers; nested objects, unions and extensions use `Json::Value`. An unset `std::optional` omits a field; assigning JSON null to `std::optional<Json::Value>` sends explicit null. Responses retain complete `raw` JSON and `_request_id`. Missing fields use defaults; present primitive fields are type checked. Nested JSON is not subjected to full schema validation.', '',
        '所有参数结构都支持 `extra_body`、`extra_query`、`extra_headers`、`timeout` 和 `max_retries`。请求级覆盖优先于具名参数和客户端默认值。查询参数和请求体分开序列化，GET 的 `extra_body` 仍是请求体。路径 ID 编码为单个路径段。各接口的完整参数、响应字段和 SDK 类型名见头文件及 `tools/sdk_resources.json`。' if zh else
        'Every parameter struct supports `extra_body`, `extra_query`, `extra_headers`, `timeout` and `max_retries`. Request overrides take precedence over named fields and client defaults. Query parameters and bodies are serialized separately; GET `extra_body` remains a body. Path IDs are encoded as single segments. Public headers and the `tools/sdk_resources.json` list every parameter, response field and SDK type annotation.', '',
        '类型化 JSON 方法另有 `CreateRaw`、`RetrieveRaw` 等方法，保留状态码、响应头、原始字节及解析结果。二进制接口直接返回 `RawResponse<std::string>`，`parsed` 为原始字节。联合响应（例如转录的 JSON/文本）返回 `Json::Value`；文本格式使用 JSON 字符串。列表返回 `Page<T>`，通过 `HasNextPage`、`GetNextPage` 和 `ForEach` 遍历；SDK 单页接口（例如向量搜索）不创建下一页。Conversations、Videos 和 checkpoint permissions 使用响应的 `last_id` 游标。' if zh else
        'Typed JSON methods also expose `CreateRaw`, `RetrieveRaw` and similar variants, retaining status, headers, original bytes and parsed data. Binary endpoints return `RawResponse<std::string>` directly, with bytes in `parsed`. Union responses, such as JSON/text transcriptions, return `Json::Value`; text formats are JSON strings. Lists return `Page<T>` with `HasNextPage`, `GetNextPage` and `ForEach`. SDK single-page APIs, such as vector search, have no continuation. Conversations, Videos and checkpoint permissions use the response `last_id` cursor.', '',
        '```cpp',
        'auto models{ co_await client.models.List() };',
        'auto embedding{ co_await client.embeddings.Create({ .input = "hello", .model = "text-embedding-3-small" }) };',
        'auto response{ co_await client.responses.Create({ .input = "hello", .model = "gpt-4.1-mini" }) };',
        'if (response) { LOG_INFO << response->output_text; }',
        '```', '',
        'Chat 和 Realtime 可使用插件配置的默认模型；其他资源显式设置其参数中的 model（接口需要时），以便不同资源选择合适的模型。示例模型仅演示参数位置。' if zh else
        'Chat and Realtime can use the configured model default. Set model explicitly for other resources that require it, so each resource can select an appropriate model. Example model strings illustrate parameter placement.', '',
        '## ' + ('Responses 解析和流' if zh else 'Responses parsing and streams'), '',
        '`responses.Parse(params)` 接受 `text.format` 的 JSON Schema，解码最终回答的 `output_text` 到内容项的 `parsed`，并设置 `output_parsed`。`phase=commentary` 不解析；拒答保持原样。strict function 工具的 arguments 解码为 `parsed_arguments`；发送前拒绝非 strict 的 function 工具。Schema 由服务端约束，库不从 C++ 类型生成 Schema，也不提供本地完整 Schema 校验器。' if zh else
        '`responses.Parse(params)` accepts a JSON Schema in `text.format`, decodes final-answer `output_text` into each content part’s `parsed` field and sets `output_parsed`. It skips `phase=commentary` and refusals. Strict function arguments become `parsed_arguments`; non-strict function tools are rejected before sending. Schema enforcement is performed by the server. The library does not generate schemas from C++ types or provide a full local schema validator.', '',
        '`CreateStream`、`RetrieveStream`、`GenerateStream` 等流式方法逐个交付 `ApiStreamEvent {type, id, data}`。回调返回 false 成功取消、返回错误终止。`ApiStreamResult` 保留事件数、最后事件、终态 response（若有）、取消标记和 HTTP 元数据；底层不保存全部事件。已开始的流不自动重试。Responses 流必须收到终态事件，否则报截断；普通 SSE 流也支持 `[DONE]` 和正常 EOF。`responses.Stream` 额外累积输出并返回最终 `Response`；取消时返回已累积快照。回调在等待该请求的事件循环上执行，不得阻塞。' if zh else
        '`CreateStream`, `RetrieveStream`, `GenerateStream` and related methods deliver `ApiStreamEvent {type, id, data}` incrementally. Returning false cancels successfully; returning an error aborts. `ApiStreamResult` retains event count, the last event, terminal response when present, cancellation and HTTP metadata; it does not retain all events. Started streams are never retried. Responses streams require a terminal event and report truncation otherwise; ordinary SSE also supports `[DONE]` and clean EOF. `responses.Stream` additionally accumulates output and returns the final `Response`, or the accumulated snapshot on cancellation. Callbacks run on the awaiting loop and must not block.', '',
        '```cpp',
        'auto stream{ co_await client.responses.CreateStream(',
        '    { .input = "hello", .model = "gpt-4.1-mini" },',
        '    [](drogon_openai::ApiStreamEvent const& event) -> drogon_openai::Result<bool> {',
        '        LOG_INFO << event.type;',
        '        return true;',
        '    }) };',
        '```', '',
        '## ' + ('文件、轮询和分块上传' if zh else 'Files, polling and chunked uploads'), '',
        '`UploadFile` 拥有 content、filename 和 content_type，保留 NUL 字节。`FileInput` 接受单文件、多文件数组或接口定义的 JSON 文件引用。multipart 使用 SDK 的括号字段格式，多文件使用 `image[]` 等名称。Realtime calls 的 SDP 单字段请求使用 `application/sdp`；包含 session 时，SDP 和 JSON session 使用各自的 Content-Type。' if zh else
        '`UploadFile` owns content, filename and content_type, preserving NUL bytes. `FileInput` accepts one file, multiple files or a documented JSON file reference. Multipart uses SDK bracket notation and names such as `image[]` for multiple files. Realtime calls use `application/sdp` for a lone SDP field; with session present, SDP and JSON session parts have their own Content-Type.', '',
        '提供 `files.WaitForProcessing`、`videos.Poll/CreateAndPoll`、`vector_stores.files.Poll/CreateAndPoll/Upload/UploadAndPoll` 和 `vector_stores.file_batches.Poll/CreateAndPoll/UploadAndPoll`。轮询返回终态资源，调用方检查 status 及 last_error；failed/cancelled 是资源终态，不自动转换为传输错误。PollOptions 控制异步间隔和总截止时间，保留服务端 openai-poll-after-ms 提示。批量上传的 max_concurrency 默认为 5，必须大于 0，并保留已有 file_ids。' if zh else
        'Helpers include `files.WaitForProcessing`, `videos.Poll/CreateAndPoll`, `vector_stores.files.Poll/CreateAndPoll/Upload/UploadAndPoll` and `vector_stores.file_batches.Poll/CreateAndPoll/UploadAndPoll`. Polling returns terminal resources; inspect status and last_error. Failed/cancelled resource states are not automatically transport errors. PollOptions controls asynchronous intervals and the total deadline, respecting openai-poll-after-ms. Batch upload max_concurrency defaults to 5, must be positive and preserves existing file_ids.', '',
        '`uploads.UploadFileChunked(file, purpose, part_size, options)` 创建上传、按顺序上传分片并按 ID 顺序完成。part_size 默认 64 MiB，范围为 1 到 64 MiB。文件和分片在内存中；失败时返回首个错误，不会自动删除已上传文件或取消服务端上传，可显式调用 Delete/Cancel 清理。' if zh else
        '`uploads.UploadFileChunked(file, purpose, part_size, options)` creates an upload, sends ordered parts and completes with ordered IDs. part_size defaults to 64 MiB and must be 1–64 MiB. Files and parts are held in memory. Failure returns the first error; uploaded files and incomplete server uploads are not automatically removed. Use Delete/Cancel for cleanup.', '',
        '```cpp',
        'auto file{ co_await client.files.Create({',
        '    .file = drogon_openai::UploadFile{ .content = "hello", .filename = "notes.txt", .content_type = "text/plain" },',
        '    .purpose = "assistants",',
        '}) };',
        'auto attached{ co_await client.vector_stores.files.UploadAndPoll(',
        '    "vs_id", { .content = "hello", .filename = "notes.txt" }) };',
        '```', '',
        '## WebSocket', '',
        '`responses.Connect()`、`realtime.Connect(model, options)`、`realtime.translations.Connect(model, options)`、`live.Connect()`、`live.sideband.Connect(session_id)` 和 `live.forks.Connect(session_id)` 返回 `WebSocketSession`。通过额外查询参数传 call_id 可连接已有 Realtime 通话；此时不自动补默认模型。请求头、组织、项目和查询覆盖与 HTTP 一致。TLS 校验证书链和主机名，connect 超时控制握手。' if zh else
        '`responses.Connect()`, `realtime.Connect(model, options)`, `realtime.translations.Connect(model, options)`, `live.Connect()`, `live.sideband.Connect(session_id)` and `live.forks.Connect(session_id)` return `WebSocketSession`. Pass call_id in extra_query to attach to a Realtime call; this suppresses the default model. Header, organization, project and query overrides follow HTTP conventions. TLS validates certificate chains and hostnames; connect timeout bounds the handshake.', '',
        '会话支持 `Send(Json::Value)`、`Receive(timeout)`、`SendBytes`、`ReceiveBytes` 和 `Close`，均为协程。一次只能有一个 Receive；timeout 为秒，默认 0 表示无限等待，超时后仍可继续接收。会话副本共享连接和 Close，操作会切到连接所属 loop，完成后恢复等待者的 loop。发送成功表示帧已交给 socket 队列，不表示远端确认。未读数据队列上限为 16 MiB。错误事件按原始 JSON 交付；断开返回 Connection，不会自动重连或重放。应在 drogon 停止前关闭并释放会话。' if zh else
        'Sessions provide coroutine methods `Send(Json::Value)`, `Receive(timeout)`, `SendBytes`, `ReceiveBytes` and `Close`. Only one receive may be pending. Receive timeout is seconds, defaults to zero for no deadline, and leaves the session usable after timeout. Copies share the connection and Close. Operations move to its owner loop and resume on the awaiting loop. Successful send means the frame entered the socket queue, without remote acknowledgement. Unread data is bounded to 16 MiB. Error events remain JSON events; disconnect returns Connection, without automatic reconnect or replay. Close and release sessions before stopping drogon.', '',
        'Drogon 1.9.13 的 WebSocketClient 会丢弃非 101 的握手响应，因此被拒绝的握手只能返回 Connection，无法提供 HTTP 状态、响应头或响应体。常规 HTTP 请求不受此限制。' if zh else
        'Drogon 1.9.13 WebSocketClient discards non-101 handshake responses. Rejected handshakes therefore return Connection without HTTP status, headers or body. Ordinary HTTP requests retain that metadata.', '',
        '```cpp',
        'auto session{ co_await client.realtime.Connect("gpt-realtime") };',
        'if (session) {',
        '    Json::Value event;',
        '    event["type"] = "response.create";',
        '    auto sent{ co_await session->Send(event) };',
        '    auto received{ co_await session->Receive(30) };',
        '    auto closed{ co_await session->Close() };',
        '}',
        '```', '',
        '## Webhook', '',
        '插件配置可设置 `webhook_secret_env`，读取含原始 secret 或 `whsec_` 前缀 base64 secret 的环境变量；该变量未设置或为空会阻止启动。直接构造客户端可设置 `OpenAIConfig.webhook_secret`，也可在 VerifySignature/Unwrap 中覆盖。密钥不写入 config.json。' if zh else
        'Set plugin `webhook_secret_env` to the name of an environment variable containing a raw secret or a `whsec_` base64 secret. An unset or empty variable prevents startup. Direct clients can use `OpenAIConfig.webhook_secret`; VerifySignature/Unwrap can override it per call. Secrets are never read directly from config.json.', '',
        '`webhooks.VerifySignature(payload, headers, secret, tolerance)` 使用 webhook-id、webhook-timestamp、webhook-signature，对原始请求字节执行 HMAC-SHA256 常量时间比较；默认重放窗口 300 秒。签名或时间戳无效时返回 `InvalidWebhookSignature`。`Unwrap` 先验签，再解码事件 JSON。保留原始 body，不能先重新序列化 JSON 再验签。远程 webhooks CRUD 与本地验签共用同一资源。' if zh else
        '`webhooks.VerifySignature(payload, headers, secret, tolerance)` uses webhook-id, webhook-timestamp and webhook-signature to verify HMAC-SHA256 over the original bytes with constant-time comparison and a default 300-second replay window. Invalid signatures or timestamps return `InvalidWebhookSignature`. `Unwrap` verifies before decoding the event JSON. Preserve the original body; do not reserialize JSON before verifying. Remote webhook CRUD and local verification share the same resource.', '',
        '## ' + ('接口目录' if zh else 'Endpoint catalog'), '',
        '下面省略 Raw 变体和共用请求覆盖字段。params 列省略命名空间前缀 `drogon_openai::types::<group>::`；结果中的 Page 和 RawResponse 均位于 drogon_openai。参数字段和返回字段见对应类型头文件。' if zh else
        'The tables omit Raw variants and common request overrides. Parameter names omit the namespace prefix `drogon_openai::types::<group>::`. Page and RawResponse belong to drogon_openai. See each type header for request and response fields.', '',
    ]
    for group in spec['groups']:
        g = group['name']
        lines += [f'### `{g}`', '', f'`<drogon_openai/types/{g}.hpp>`', '',
                  '| ' + ('资源与方法 | HTTP | params | 结果 |' if zh else 'Resource and method | HTTP | Params | Result |'),
                  '| --- | --- | --- | --- |']
        for resource in group['resources']:
            for method in resource['methods']:
                result = method['cpp_result'].replace('types::' + g + '::', '')
                if method['binary']: result = 'RawResponse<std::string>'
                elif method['text']: result = 'std::string'
                elif method['void']: result = 'void'
                elif method['page']: result = 'Page<' + result + '>'
                path_args = ', '.join(method['path_args'] + ['params'])
                name = '.'.join(resource['parts']) + '.' + method['cpp_name']
                if method['stream']: name += ' / ' + method['cpp_name'] + 'Stream'
                lines.append(f'| `{name}({path_args})` | `{method["verb"]} {method["path"]}` | `{method["param_name"]}` | `{result}` |')
        lines += ['']
    lines += [
        '## ' + ('维护和验证' if zh else 'Maintenance and verification'), '',
        '`tools/generate_resources.py` 读取固定 SDK 源码生成类型、资源绑定和目录，`tools/generate_resource_tests.py` 生成 146 个接口契约用例，本文件的表格由 `tools/generate_resource_docs.py` 同步。扩展方法声明见 resource_extensions.json，实现为手写源码，不会被生成器覆盖。操作步骤见 `tools/README.md`。' if zh else
        '`tools/generate_resources.py` reads the pinned SDK to generate types, resource bindings and the catalog. `tools/generate_resource_tests.py` generates contract coverage for all 146 HTTP methods; `tools/generate_resource_docs.py` synchronizes these tables. Extension declarations live in resource_extensions.json; handwritten implementations are not overwritten. See `tools/README.md`.', '',
        '`test_resources` 核对全部 HTTP 方法、路径、类型、特殊请求头和元数据；`test_resource_wire` 使用独立报文核对查询、请求体、JSON null、multipart、文本、二进制、路径编码及游标；`test_resource_helpers` 验证 Responses SSE/Parse、上传轮询、分块顺序和独立 HMAC 签名向量；`test_websocket` 验证路由、帧、线程恢复、并发接收限制、超时和关闭。测试在本地 fake API 上执行，不需要 OpenAI 凭证，也不代表所有远程产品已进行实网验证。' if zh else
        '`test_resources` checks every HTTP method, path, result kind, special header and metadata. Independent fixtures in `test_resource_wire` check query/body separation, JSON null, multipart, text/binary, path encoding and cursors. `test_resource_helpers` checks Responses SSE/Parse, upload polling, chunk order and an independent HMAC signature vector. `test_websocket` checks routes, frames, loop affinity, exclusive receive, deadlines and closing. These tests use local fake APIs without OpenAI credentials; they do not claim live verification of every remote product.', '',
        '[openai-python 3.41.0](https://github.com/openai/openai-python/tree/05992039b9f7b78a48cf1e5a35727ba27d96db6e/src/openai) · [OpenAI streaming](https://developers.openai.com/api/docs/guides/streaming-responses) · [OpenAI webhooks](https://developers.openai.com/api/docs/guides/webhooks)', '',
    ]
    path = root / 'docs' / lang / 'api-resources.md'
    content = '\n'.join(lines)
    if not path.exists() or path.read_text() != content:
        path.write_text(content)
