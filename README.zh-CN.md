# drogon-openai

[English](README.md) · 简体中文

基于 [drogon](https://github.com/drogonframework/drogon) 的 OpenAI 格式 API 插件库，供其他 drogon 应用以插件形式使用。它可以对接 OpenAI，也可以对接 DeepSeek 等使用相同请求格式的接口。

接口按 [OpenAI Python SDK](https://github.com/openai/openai-python) 的 `AsyncOpenAI` 组织：`co_await client.chat.completions.Create(...)` 返回 `drogon::Task<Result<ChatCompletion>>`，其中 `Result<T>` 即 `std::expected<T, Error>`。Chat completions 已支持流式回调、结构化解析、工具、多模态消息和存储管理，另有 Responses、Embeddings、文件、图像、音频、视频、向量库、Realtime/Live 等 23 个根资源和 146 个 HTTP 方法，提供客户端默认设置、重试、原始响应、上传、二进制下载、分页、轮询、WebSocket 和 webhook 验签。

[快速开始](#快速开始) · [文档索引](docs/zh-CN/README.md) · [插件使用说明](docs/zh-CN/drogon-plugin.md) · [API 资源](docs/zh-CN/api-resources.md) · [版本标签、CI 与发布](docs/zh-CN/ci.md) · [Doxygen 使用说明](docs/zh-CN/doxygen.md)

## 环境与依赖

| 依赖 | 版本 | 说明 |
| --- | --- | --- |
| CMake | ≥ 4.0 | 构建使用 `CMakePresets.json` 中的 `<os>-<profile>` 预设 |
| Ninja、ccache | 任意较新版本 | 预设固定使用 Ninja 生成器和 ccache 编译器启动器 |
| 编译器 | Linux 上为 `clang++`，Windows 上为 `clang-cl` | 公开头文件使用 `std::expected`，需要 C++23 |
| [vcpkg](https://github.com/microsoft/vcpkg) | classic 模式 | 提供 drogon，需设置 `VCPKG_ROOT` |
| drogon | 1.9.13 | 由 vcpkg 安装，`just init` 执行 `vcpkg install drogon` |
| OpenSSL Crypto | 随 drogon 依赖安装 | 用于 webhook HMAC-SHA256；CMake 包自动查找 |

[Linux CI](.github/workflows/linux-ci.yml) 和 [Windows CI](.github/workflows/windows-ci.yml) 分别在 Ubuntu 24.04 和 Windows Server 2025 上运行 debug 和 release 两套配置：

- 工具：CMake 4.2.3，Linux 上用 LLVM 21，Windows 上用运行器镜像自带的 LLVM，vcpkg 固定在提供 drogon 1.9.13 的提交；
- 内容：检查格式（在 Linux 上）、构建并运行离线测试；
- 在版本标签上，[Release](.github/workflows/release.yml) 工作流运行这两个 CI，并把它们的 release 构建发布为 GitHub Release。

详见 [版本标签、CI 与发布](docs/zh-CN/ci.md)。

<a id="快速开始"></a>

## 快速开始

在仓库根目录安装依赖、构建并运行测试：

```sh
just init
just build
just test
```

`just test` 通过 CTest 运行 `tests/test_*.cpp` 编译出的全部测试程序。测试使用 drogon 自带的测试框架，需要服务端的测试由同一个 app 充当 OpenAI 格式接口，因此不需要网络。唯一的例外是 `test_deepseek`：它真实调用 DeepSeek，只在设置了 `DEEPSEEK_API_KEY` 时运行，否则报告为跳过。不使用 just 时，`cmake --workflow --preset linux-debug` 依次完成配置、构建和测试。

## 在项目中使用

源码方式（以 CPM 为例，也可以用 `add_subdirectory`）：

```cmake
CPMAddPackage(
    NAME drogon-openai
    GIT_REPOSITORY https://github.com/yizhinailong/drogon-openai.git
    GIT_TAG master
)

target_link_libraries(app PRIVATE drogon-openai::drogon-openai)
```

- `GIT_TAG` 也可以写版本标签：`CMakeLists.txt` 中 `project()` 的 `VERSION` 变化后，会自动创建 `v<VERSION>` 标签。
- 作为子项目时，测试和安装规则默认关闭。
- 也可以先执行 `just publish`，再在消费项目中用 `find_package(drogon-openai CONFIG REQUIRED)` 引入。
- 每个版本的 GitHub Release 上还有预编译的 Linux 和 Windows 包，只含 release 构建。解压后加入 `CMAKE_PREFIX_PATH`，即可用 `find_package` 引入；drogon 仍从 vcpkg 获取。使用要求见 [版本标签、CI 与发布](docs/zh-CN/ci.md) 的“发布”一节。

在应用的 config.json 中注册插件。下例对接 DeepSeek，API key 只从环境变量读取：

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

对接 OpenAI 时，把 `base_url` 换成 `https://api.openai.com/v1`，并填写相应的模型名。

下面是完整程序：把 `POST /chat` 的请求体作为用户消息发给模型，返回模型的回复。

- 处理函数是协程，`co_await` 等待模型回复时不阻塞 IO 线程。
- 请求失败时不抛异常，`Error::message` 说明原因。

```cpp
/**
 * @file main.cpp
 * @brief 把 POST /chat 的请求体发给模型，返回模型的回复。
 */
#include <string>

#include <drogon/drogon.h>

#include <drogon_openai/openai_client.hpp>

/**
 * @brief 注册 /chat 路由并启动 drogon。
 * @return drogon 退出后返回零。
 */
auto main() -> int {
    drogon::app()
        .loadConfigFile("config.json")
        .registerHandler(
            "/chat",
            [](drogon::HttpRequestPtr req) -> drogon::Task<drogon::HttpResponsePtr> {
                auto const client{ drogon::app().getPlugin<drogon_openai::OpenAIClient>()->NewOpenAIClient() };
                auto const completion{ co_await client.chat.completions.Create({
                    .messages = { { .role = "user", .content = std::string{ req->body() } } },
                }) };

                auto response{ drogon::HttpResponse::newHttpResponse(drogon::k200OK, drogon::CT_TEXT_PLAIN) };
                if (!completion) {
                    response->setStatusCode(drogon::k502BadGateway);
                    response->setBody(completion.error().message);
                } else if (!completion->choices.empty()) {
                    response->setBody(completion->choices.front().message.content.value_or(""));
                }
                co_return response;
            },
            { drogon::Post }
        )
        .run();
    return 0;
}
```

配置项、请求参数、错误分类、生命周期与线程模型见 [插件使用说明](docs/zh-CN/drogon-plugin.md)。

## 开发与文档

使用文档分为英文版（`docs/en/`）和简体中文版（`docs/zh-CN/`），两边的文件一一对应，API 标识符、命令和协议值保留原文；新增文档请在两边各写一份，并加入各自的 [文档索引](docs/zh-CN/README.md)。本文是 [README.md](README.md) 的简体中文版，修改其中一份时请同步更新另一份。开发约定见 [AGENTS.md](AGENTS.md)：

- 代码风格；
- 用 `clang-format --dry-run -Werror` 检查格式；
- 每次改动行为时同步更新文档。

安装 Doxygen 后，执行 `just docs` 生成 API 文档，首页为 `build/doxygen/html/index.html`。推送到 `master` 时，Documentation 工作流会自动生成文档并发布到 GitHub Pages。首次启用所需的设置见 [Doxygen 使用说明](docs/zh-CN/doxygen.md)。

## 许可证

采用 [MIT 许可证](LICENSE)。
