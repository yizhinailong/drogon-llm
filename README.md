# drogon-openai

English · [简体中文](README.zh-CN.md)

A plugin library for OpenAI-format APIs, built on [drogon](https://github.com/drogonframework/drogon) and used as a plugin by other drogon apps. It works with OpenAI and with any endpoint that uses the same request format, such as DeepSeek.

The API follows the `AsyncOpenAI` client of the [OpenAI Python SDK](https://github.com/openai/openai-python): `co_await client.chat.completions.Create(...)` returns `drogon::Task<Result<ChatCompletion>>`, where `Result<T>` is `std::expected<T, Error>`. Chat completions are implemented so far; streaming is not supported yet.

[Quick start](#quick-start) · [Documentation index](docs/en/README.md) · [Plugin guide](docs/en/drogon-plugin.md) · [Version tags and Linux CI](docs/en/ci.md) · [Doxygen guide](docs/en/doxygen.md)

## Requirements

| Dependency | Version | Notes |
| --- | --- | --- |
| CMake | ≥ 4.0 | The build uses the `<os>-<profile>` presets in `CMakePresets.json` |
| Ninja, ccache | Any recent version | The presets always use the Ninja generator and the ccache compiler launcher |
| Compiler | `clang++` on Linux, `clang-cl` on Windows | The public headers use `std::expected`, which needs C++23 |
| [vcpkg](https://github.com/microsoft/vcpkg) | Classic mode | Provides drogon; `VCPKG_ROOT` must be set |
| drogon | 1.9.13 | Installed by vcpkg; `just init` runs `vcpkg install drogon` |

[Linux CI](.github/workflows/linux-ci.yml) runs a debug and a release configuration on Ubuntu 24.04:

- Tools: CMake 4.2.3 and LLVM 21, with vcpkg pinned to the commit that provides drogon 1.9.13.
- Steps: check the formatting, build, and run the offline tests.

See [Version tags and Linux CI](docs/en/ci.md).

<a id="quick-start"></a>

## Quick start

From the repository root, install the dependencies, build, and run the tests:

```sh
just init
just build
just test
```

`just test` uses CTest to run every test program built from `tests/test_*.cpp`. The tests use drogon's own test framework. The tests that need a server use the same app as a stand-in for an OpenAI-format API, so they need no network access. The one exception is `test_deepseek`: it calls the real DeepSeek API and runs only when `DEEPSEEK_API_KEY` is set; otherwise it is reported as skipped. Without just, `cmake --workflow --preset linux-debug` configures, builds, and tests in one go.

## Using it in a project

From source (CPM shown here; `add_subdirectory` works too):

```cmake
CPMAddPackage(
    NAME drogon-openai
    GIT_REPOSITORY https://github.com/yizhinailong/drogon-openai.git
    GIT_TAG master
)

target_link_libraries(app PRIVATE drogon-openai::drogon-openai)
```

- `GIT_TAG` can also name a version tag. When `VERSION` in the `project()` call of `CMakeLists.txt` changes, a `v<VERSION>` tag is created automatically.
- As a subproject, the tests and install rules are off by default.
- Alternatively, run `just publish` first, then use `find_package(drogon-openai CONFIG REQUIRED)` in the consuming project.

Register the plugin in the app's config.json. This example connects to DeepSeek, and the API key is read only from an environment variable:

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

For OpenAI, change `base_url` to `https://api.openai.com/v1` and set a matching model name.

Here is a complete program that sends the body of `POST /chat` to the model as a user message and returns the model's reply:

- The handler is a coroutine, so waiting for the reply with `co_await` does not block the IO thread.
- A failed request does not throw; `Error::message` explains what went wrong.

```cpp
/**
 * @file main.cpp
 * @brief Send the body of POST /chat to the model and return the model's reply.
 */
#include <string>

#include <drogon/drogon.h>

#include <drogon_openai/openai_client.hpp>

/**
 * @brief Register the /chat route and start drogon.
 * @return Zero after drogon exits.
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

The [plugin guide](docs/en/drogon-plugin.md) covers the configuration keys, the request parameters, the error kinds, the lifecycle, and the threading model.

## Development and documentation

The user documentation comes in English (`docs/en/`) and Simplified Chinese (`docs/zh-CN/`), with matching file names, and API identifiers, commands, and protocol values left as they are. Write a new page in both languages and add it to each [documentation index](docs/en/README.md). [README.zh-CN.md](README.zh-CN.md) is the Simplified Chinese version of this file; when you change one, update the other. The development conventions are in [AGENTS.md](AGENTS.md):

- the code style;
- checking the formatting with `clang-format --dry-run -Werror`;
- updating the documentation with every behavior change.

With Doxygen installed, `just docs` generates the API documentation. Its home page is `build/doxygen/html/index.html`. On pushes to `master`, the Documentation workflow generates the documentation and publishes it to GitHub Pages. The [Doxygen guide](docs/en/doxygen.md) describes the settings needed the first time.

## License

Released under the [MIT License](LICENSE).
