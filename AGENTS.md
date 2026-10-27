# AGENTS.md

## Project state

Early-stage Drogon plugin library for OpenAI-compatible APIs: OpenAI itself, or any endpoint that speaks the same format, such as DeepSeek. It is consumed by other Drogon apps (e.g. `../assistantbot`). No tests or CI yet.

- The library target is `drogon-openai` (alias `drogon-openai::drogon-openai`). It is static by default; pass `-DBUILD_SHARED_LIBS=ON` for a shared build.
- Public headers live in `include/drogon_openai/` and are installed through the `HEADERS` file set. Consumers include `<drogon_openai/...>`. Private headers go in `src/`, which is only on the library's own include path.
- The plugin class is `drogon_openai::OpenAIClient` (`drogon::Plugin<OpenAIClient>`). Apps register it by class name under `plugins` in their config.json. Drogon calls `initAndStart(config)` inside `app().run()`, before beginning advices, so `app().getPlugin<drogon_openai::OpenAIClient>()` only works after `run()` starts.
- Registration relies on drogon's static `DrObject<T>::alloc_` initializer. With the static library, the linker only pulls in `openai_client.o` when the app references the class. If the app lists the plugin in config.json but never calls `getPlugin<OpenAIClient>()` (or otherwise uses the class), drogon logs `Plugin drogon_openai::OpenAIClient undefined!` and keeps running without it.
- Plugin config keys: `base_url` and `model` are required, and `initAndStart` throws `std::invalid_argument` if either is empty. The API key is never read from config.json. The optional `api_key_env` names the environment variable that holds it, and `initAndStart` throws if that variable is unset or empty. Without `api_key_env`, `api_key` stays empty (endpoints without auth). Add new keys to `OpenAIConfig`, `initAndStart`, the class doc comment, and `config.json`.
- The client API mirrors the OpenAI Python SDK's `AsyncOpenAI`: `OpenAIClient::NewOpenAIClient()` returns an `OpenAI` client by value, and `co_await client.chat.completions.Create(ChatCompletionCreateParams)` yields `Result<ChatCompletion>` (`std::expected<T, Error>`, in `error.hpp`). `Create` returns `drogon::Task<...>`. There is no sync API, because drogon's docs advise against sync calls on event loops. `OpenAI` is a value handle: `chat` and `completions` are plain members, and the only shared state is one `std::shared_ptr<details::Transport>`. Copies are cheap and share the transport, so pass an `OpenAI` client to coroutines by value. Don't wrap API groups in pointers unless one needs polymorphism or non-copyable state. Request and response structs live in `types/chat.hpp`, named after `openai.types.chat`. The API groups (`details::Chat`, `details::Completions`) share one `details::Transport` (private, `src/details/`), which adds the auth header and maps failures, including the `drogon::HttpException` that `sendRequestCoro` throws, to `ErrorKind`. Add new request fields to the params struct and to `to_request_body` in `src/details/completions.cpp`.
- Coroutines take their parameters by value (drogon's coroutine guidance), and `Completions::Create` copies `m_transport` into its frame before the first `co_await`. Keep both rules in new API methods.
- `Transport::Post` creates a new `HttpClient` for each request on the calling thread's loop (`trantor::EventLoop::getEventLoopOfCurrentThread()`), or on the main loop when the thread runs no loop. Don't cache one client for reuse. drogon's `HttpClient` defaults to pipelining depth 0, so it allows one in-flight request and queues the rest (`HttpClientImpl.cc`). A shared client would serialize concurrent requests. Using the caller's loop also means the coroutine resumes on the thread that awaited it. `drogon::sync_wait(Create(...))` works only on a thread that runs no event loop.
- Streaming is not implemented yet. drogon 1.9.13's `HttpClient` buffers the whole response, so streaming needs a different transport.

## Consuming the library

The library supports two ways to consume it. Keep both working when you change `CMakeLists.txt`.

- From source: `add_subdirectory(<path>)`, or CPM with `SOURCE_DIR`/`GIT_REPOSITORY`. `PROJECT_IS_TOP_LEVEL` is false then, so `DROGON_OPENAI_BUILD_TESTS` and `DROGON_OPENAI_INSTALL` default to OFF. The tests are not built, and nothing is installed into the consumer's prefix.
- Installed: `just publish` installs into `publish/` (`lib/`, `include/`, `lib/cmake/drogon-openai/`). Consumers set `CMAKE_PREFIX_PATH` to that directory and call `find_package(drogon-openai CONFIG REQUIRED)`. The package config calls `find_dependency(Drogon CONFIG)`, so consumers don't need their own `find_package(Drogon)`. Version compatibility is `SameMinorVersion`.
- Either way, link `drogon-openai::drogon-openai`. It carries `Drogon::Drogon` publicly.

## Tests

`tests/CMakeLists.txt` globs `tests/test_*.cpp` (non-recursive, `CONFIGURE_DEPENDS`) and builds each file into its own executable named after the file stem (`tests/test_config.cpp` → `build/<preset>/tests/test_config`). Each one links `drogon-openai::drogon-openai` and gets `cxx_std_23` (for `<print>`). Adding a `tests/test_*.cpp` file is enough. Removing one leaves its old binary in `build/<preset>/tests/` until `just clean`. They are plain programs: there is no test framework and no CTest registration yet.

`tests/test_config.cpp` loads `config.json` relative to the working directory (`just run` uses the repo root), prints the plugin's settings, then calls `app().quit()`.

`tests/test_http_client.cpp` sends one real chat completion request with `co_await NewOpenAIClient().chat.completions.Create(...)` inside `drogon::async_run` from a beginning advice. No extra thread is needed. It needs network access and a valid key, spends a few tokens, and takes a few seconds against DeepSeek. It prints the model, `reasoning_content` (present because DeepSeek flash thinks by default), `content`, and `usage`, and exits 1 with `Error::message` when the request fails (e.g. `HTTP 401: Authentication Fails, ...` for a bad key).

`config.json` (repo root, tracked, no secrets) points at DeepSeek: `base_url` `https://api.deepseek.com` (OpenAI format), model `deepseek-flash`, and the key from `DEEPSEEK_API_KEY`. `test_config` aborts with `environment variable DEEPSEEK_API_KEY is unset or empty` until that variable is exported. Drogon creates `uploads/` under `document_root` (`/tmp/drogon-openai-example`) on startup.

## Commands

`just` drives the build. Recipes map onto CMake presets named `<os>-<profile>` (`linux-debug`, `linux-release`, `windows-debug`, `windows-release`), with build trees in `build/<preset>/`.

- `just init`: `vcpkg install drogon` (classic mode, no `vcpkg.json`; `VCPKG_ROOT` must be set).
- `just build [debug|release]`: configure only if `build/<preset>/CMakeCache.txt` is missing, then build, then copy `compile_commands.json` to `build/`. A cache left by a failed configure makes this skip configure and fail with `loading 'build.ninja'`; fix it with `just configure debug --fresh`.
- `just run <target> [profile]`: build everything, then run `build/<os>-<profile>/tests/<target>`, e.g. `just run test_config` or `just run test_config release`. `<target>` is required.
- `just publish [profile]`: build (default `release`) and `cmake --install` into `publish/`.
- `just clean [profile]`: delete `build/<os>-<profile>/`, or `build/` and `publish/` when no profile is given.

## Build notes

- Requires CMake >= 4.0, Ninja, and ccache (hard-wired as `CMAKE_CXX_COMPILER_LAUNCHER`). Compiler is `clang++` on Linux, `clang-cl` on Windows.
- Don't set `CMAKE_CXX_STANDARD` here. The library declares `target_compile_features(PUBLIC cxx_std_23)` (the public headers use `std::expected`), which is exported to `find_package` consumers. A parent project's `CMAKE_CXX_STANDARD` still applies in `add_subdirectory` mode, so under a C++26 parent the library builds with `-std=gnu++26`. The test executables add `cxx_std_23` for `<print>`.
- Drogon 1.9.13's `FindFilesystem.cmake` (run by `find_package(Drogon)`) leaves an unset `CMAKE_CXX_STANDARD` defined as `""`, and `target_compile_features` then fails with `CXX_STANDARD property ... contained an invalid value: ""`. Both `CMakeLists.txt` and `cmake/drogon-openai-config.cmake.in` unset the empty value right after finding Drogon. Keep those blocks.
- Sources and public headers are collected with `file(GLOB_RECURSE ... CONFIGURE_DEPENDS)`, so adding a file triggers a re-configure on the next build.

## Code style

New C++ code follows the `cpp-style-ref` skill (CMake + vcpkg variant), the same as `../assistantbot`: types and public methods PascalCase, private members `m_`, namespaces lowercase, `/** @file ... @brief ... */` at the top of every file, Doxygen blocks repeated on out-of-line definitions, `T const&`, brace initialization, and trailing return types. Drogon's virtual overrides (`initAndStart`, `shutdown`) keep their upstream names.

Format with `clang-format -i <files>` (root `.clang-format`, same as `../assistantbot`). Check with `clang-format --dry-run -Werror <files>`.
