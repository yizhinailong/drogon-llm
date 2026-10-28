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
| `base_url` | 是 | 接口地址，可带路径前缀，例如 `https://api.openai.com/v1`。请求发往 `<base_url>/chat/completions`，末尾的 `/` 会被忽略。 |
| `model` | 是 | 请求没有指定 `model` 时使用的模型名。 |
| `api_key_env` | 否 | 存放 API key 的**环境变量名**。省略时不发送 `Authorization` 头，适用于不需要鉴权的接口，例如本地部署的模型。 |

- API key 不写进 config.json，只从环境变量读取，以 `Authorization: Bearer <key>` 发送。
- 出现以下任一情况时，`initAndStart` 抛 `std::invalid_argument`：
  - `base_url` 或 `model` 缺失或为空；
  - 设置了 `api_key_env`，但对应的环境变量未设置或为空。

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

## 调用 chat completions

- `OpenAI` 是值句柄：`chat` 和 `chat.completions` 是普通成员，唯一的共享状态是一个 `std::shared_ptr<details::Transport>`。拷贝只是引用计数加一，传进协程时按值传递。
- `Create(ChatCompletionCreateParams)` 返回 `drogon::Task<Result<ChatCompletion>>`，`Result<T>` 是 `std::expected<T, Error>`。请求失败**不会抛异常**。

请求参数（`ChatCompletionCreateParams`，用指定初始化器填写，字段顺序与声明一致）：

| 字段 | 类型 | 说明 |
|---|---|---|
| `messages` | `std::vector<ChatCompletionMessageParam>` | 对话，按时间先后排列。每条是 `role`（`system`、`developer`、`user` 或 `assistant`）加文本 `content`。 |
| `model` | `std::optional<std::string>` | 不设置时使用配置的 `model`。 |
| `frequency_penalty` | `std::optional<double>` | -2.0 到 2.0，正值减少重复的 token。 |
| `max_tokens` | `std::optional<std::int64_t>` | 生成 token 数的上限。 |
| `presence_penalty` | `std::optional<double>` | -2.0 到 2.0，正值鼓励新话题。 |
| `temperature` | `std::optional<double>` | 采样温度，0 到 2。 |
| `top_p` | `std::optional<double>` | 核采样的概率质量。 |
| `extra_body` | `Json::Value` | 对象的成员原样加进请求体，并覆盖上面的同名字段。 |
| `timeout` | `double` | 等待响应的秒数，默认 600；0 表示一直等。 |

- 未设置的可选字段不写进请求体，由接口使用自己的默认值。
- `extra_body` 用来传参数结构体里还没有的字段，或厂商的扩展字段，例如 `extra_body["seed"] = 42;`。它不是对象时被忽略。

响应（`ChatCompletion`）：

| 字段 | 说明 |
|---|---|
| `id`、`object`、`created`、`model` | 补全的 ID、类型（`chat.completion`）、创建时间（Unix 秒）和实际使用的模型 |
| `choices` | 生成的候选，通常只有一个。每个有 `index`、`finish_reason`（如 `stop`、`length`）和 `message` |
| `choices[i].message` | `role`（`assistant`）、`content`（回复文本）、`reasoning_content`（DeepSeek 等推理模型的思考过程） |
| `usage` | `prompt_tokens`、`completion_tokens`、`total_tokens` |

- 响应缺少的字符串字段为空串，整数字段为 0。
- `content`、`reasoning_content` 缺失或为 null 时不设置；响应没有 `usage` 时 `usage` 不设置。

## 错误处理

`ErrorKind` 对应 OpenAI Python SDK 的异常类：

| `ErrorKind` | 含义 | `status_code` | `body` |
|---|---|---|---|
| `Connection` | 没有收到响应：DNS、TLS、连接被拒等（`APIConnectionError`） | 0 | 空 |
| `Timeout` | `timeout` 秒内没有收到响应（`APITimeoutError`） | 0 | 空 |
| `Status` | 非 2xx，例如 key 无效（401）、限流（429）、服务过载（503）（`APIStatusError`） | HTTP 状态码 | 原始响应体 |
| `ResponseValidation` | 2xx 响应不是 JSON、没有 `choices` 数组，或某个 choice 没有 `message` 对象（`APIResponseValidationError`） | 0 | 响应体 |

- `Status` 的 `message` 形如 `HTTP 401: <error.message>`。响应体不是 OpenAI 格式的错误时，用原始响应体代替 `error.message`。
- `Connection` 和 `Timeout` 的 `message` 形如 `POST <base_url>/chat/completions: <drogon 给出的原因>`。
- 在 Windows 上，连接被拒要约 2 秒才报告，因为系统会先重试连接。`timeout` 比这更短时，请求会以 `Timeout` 而不是 `Connection` 失败。Linux 上连接被拒会立即失败。
- 失败的请求不会自动重试。

## 线程与事件循环

- 只提供协程接口。drogon 文档建议优先使用异步接口或协程，同步 API 会阻塞 IO 线程，在 client 所在的 loop 上同步等待还会死锁。
- `Transport::Post` 为每个请求在调用线程的事件循环上新建 `HttpClient`（调用线程没有 loop 时用主 loop），并在响应到达前一直留在协程帧里。因此在请求处理函数里 `co_await` 之后，协程仍回到原来的 IO 线程。
- 不要缓存共享的 `HttpClient`：drogon 的 `HttpClient` 默认 pipelining 深度为 0，同一时间只发一个请求，共享一个 client 会让并发请求排队。
- `drogon::sync_wait(client.chat.completions.Create(...))` 只能在不运行事件循环的线程上使用，例如测试程序的主线程。
- 协程按值接收参数；`Create` 在第一次 `co_await` 之前把 `m_transport` 拷进协程帧，即使 `OpenAI` 在请求途中被销毁，请求也能完成。

## 已知限制

- 只实现了 chat completions，没有 embeddings、models 等其他接口。
- 不支持流式输出（`stream: true`）：drogon 1.9.13 的 `HttpClient` 会缓冲整个响应，流式输出需要另一种传输方式。
- 消息内容只有文本，不支持图片等多模态内容，也不支持工具调用（`tools`、`tool_calls`）。

## 测试

测试使用 drogon 自带的测试框架 `drogon/drogon_test.h`（随 drogon 安装，不需要额外依赖）。每个 `tests/test_*.cpp` 编译成一个程序，并注册进 CTest。

- 运行全部测试：`just test`，等价于 `ctest --preset linux-debug`。
- 只运行离线测试：`ctest --preset linux-debug -LE live`。
- 运行单个程序：`just run test_chat_completions`。
- 运行单个用例：`build/linux-debug/tests/test_chat_completions -r CreateTimeout`。加 `-s` 打印通过的断言，加 `-l` 列出用例。

需要服务端的测试由同一个 app 充当 OpenAI 格式接口，监听 config.json 中的 `127.0.0.1:18081`。

| 程序 | 覆盖内容 |
|---|---|
| `test_config` | 不启动 app，直接调用 `initAndStart`：必填项 `base_url` 和 `model`、`api_key_env` 的读取与报错（未设置、为空），以及仓库里的 config.json 能通过校验。 |
| `test_chat_completions` | 假的 OpenAI 格式接口。请求体：默认模型、消息、未设置的可选字段不出现、已设置的可选字段、`extra_body` 的合并与覆盖。`Authorization` 头；`base_url` 的路径（无路径、`/v1`、末尾 `/`）；完整响应与缺字段响应的解析；全部四种 `ErrorKind`（401 带 `error.message`、503 纯文本、404、非 JSON、没有 `choices`、choice 没有 `message`、超时、连接失败）；并发请求不排队；在 IO 线程上调用时协程回到同一线程。 |
| `test_deepseek` | 真实调用 DeepSeek：一次正常请求，一次错误 key（401）。需要网络和 `DEEPSEEK_API_KEY`，会消耗少量 token；未设置该变量时报告为跳过。带 CTest 标签 `live`。 |
