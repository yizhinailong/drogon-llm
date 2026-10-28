# AGENTS.md

## Project state

Early-stage Drogon plugin library for OpenAI-compatible APIs: OpenAI itself, or any endpoint that speaks the same format, such as DeepSeek. It is consumed by other Drogon apps (e.g. `../assistantbot`). Only chat completions are implemented.

- The library target is `drogon-openai` (alias `drogon-openai::drogon-openai`). It is static by default; pass `-DBUILD_SHARED_LIBS=ON` for a shared build.
- Public headers live in `include/drogon_openai/` and are installed through the `HEADERS` file set. Consumers include `<drogon_openai/...>`. Private headers go in `src/`, which is only on the library's own include path.
- The plugin class is `drogon_openai::OpenAIClient` (`drogon::Plugin<OpenAIClient>`). Apps register it by class name under `plugins` in their config.json. Drogon calls `initAndStart(config)` inside `app().run()`, before beginning advices, so `app().getPlugin<drogon_openai::OpenAIClient>()` only works after `run()` starts.
- Registration relies on drogon's static `DrObject<T>::alloc_` initializer. With the static library, the linker only pulls in `openai_client.o` when the app references the class. If the app lists the plugin in config.json but never calls `getPlugin<OpenAIClient>()` (or otherwise uses the class), drogon logs `Plugin drogon_openai::OpenAIClient undefined!` and keeps running without it.
- Plugin config keys: `base_url` and `model` are required, and `initAndStart` throws `std::invalid_argument` if either is empty. The API key is never read from config.json. The optional `api_key_env` names the environment variable that holds it, and `initAndStart` throws if that variable is unset or empty. Without `api_key_env`, `api_key` stays empty (endpoints without auth). Add new keys to `OpenAIConfig`, `initAndStart`, the class doc comment, `config.json`, and the tables in `docs/*/drogon-plugin.md`.
- The client API mirrors the OpenAI Python SDK's `AsyncOpenAI`: `OpenAIClient::NewOpenAIClient()` returns an `OpenAI` client by value, and `co_await client.chat.completions.Create(ChatCompletionCreateParams)` yields `Result<ChatCompletion>` (`std::expected<T, Error>`, in `error.hpp`). `Create` returns `drogon::Task<...>`. There is no sync API, because drogon's docs advise against sync calls on event loops. `OpenAI` is a value handle: `chat` and `completions` are plain members, and the only shared state is one `std::shared_ptr<details::Transport>`. Copies are cheap and share the transport, so pass an `OpenAI` client to coroutines by value. Don't wrap API groups in pointers unless one needs polymorphism or non-copyable state. Request and response structs live in `types/chat.hpp`, named after `openai.types.chat`. The API groups (`details::Chat`, `details::Completions`) share one `details::Transport` (private, `src/details/`), which adds the auth header and maps failures, including the `drogon::HttpException` that `sendRequestCoro` throws, to `ErrorKind`. Add new request fields to the params struct, to `to_request_body` in `src/details/completions.cpp`, to the parameter tables in `docs/*/drogon-plugin.md`, and to `CreateSendsSetOptionalFields` in `tests/test_chat_completions.cpp`.
- Coroutines take their parameters by value (drogon's coroutine guidance), and `Completions::Create` copies `m_transport` into its frame before the first `co_await`. Keep both rules in new API methods.
- `Transport::Post` creates a new `HttpClient` for each request on the calling thread's loop (`trantor::EventLoop::getEventLoopOfCurrentThread()`), or on the main loop when the thread runs no loop. Don't cache one client for reuse. drogon's `HttpClient` defaults to pipelining depth 0, so it allows one in-flight request and queues the rest (`HttpClientImpl.cc`). A shared client would serialize concurrent requests. Using the caller's loop also means the coroutine resumes on the thread that awaited it. `drogon::sync_wait(Create(...))` works only on a thread that runs no event loop. `test_chat_completions` checks both properties (`ConcurrentRequestsDoNotQueue`, `CreateResumesOnCallingThread`).
- Streaming is not implemented yet. drogon 1.9.13's `HttpClient` buffers the whole response, so streaming needs a different transport.

## Consuming the library

The library supports two ways to consume it. Keep both working when you change `CMakeLists.txt`.

- From source: `add_subdirectory(<path>)`, or CPM with `SOURCE_DIR`/`GIT_REPOSITORY`. `PROJECT_IS_TOP_LEVEL` is false then, so `DROGON_OPENAI_BUILD_TESTS` and `DROGON_OPENAI_INSTALL` default to OFF. Neither the tests nor `enable_testing()` reach the consumer, and nothing is installed into the consumer's prefix.
- Installed: `just publish` installs into `publish/` (`lib/`, `include/`, `lib/cmake/drogon-openai/`). Consumers set `CMAKE_PREFIX_PATH` to that directory and call `find_package(drogon-openai CONFIG REQUIRED)`. The package config calls `find_dependency(Drogon CONFIG)`, so consumers don't need their own `find_package(Drogon)`. Version compatibility is `SameMinorVersion`.
- Either way, link `drogon-openai::drogon-openai`. It carries `Drogon::Drogon` publicly.

## Tests

The tests use drogon's own framework, `drogon/drogon_test.h`, which is installed with drogon, so the tests add no dependency. They follow `../drogon-onebot`'s setup.

- `tests/CMakeLists.txt` globs `tests/test_*.cpp` (non-recursive, `CONFIGURE_DEPENDS`) and builds each file into its own executable named after the file stem (`build/<preset>/tests/test_chat_completions`). Each one links `drogon-openai::drogon-openai` and gets `cxx_std_23` (for `<print>`). Adding a `tests/test_*.cpp` file is enough. Removing one leaves its old binary until `just clean`.
- Each executable is registered with CTest:
  - `WORKING_DIRECTORY` is the repo root (for `config.json`), and `TIMEOUT` is 60.
  - `RESOURCE_LOCK config_json_listener` serializes them under `ctest -j`, because the app-backed tests share the listener port.
  - `SKIP_RETURN_CODE 77` reports a test as skipped.
  - A block after the loop gives `test_deepseek` the `live` label and `TIMEOUT 300`. It names the test, so renaming the file means updating that block.
- The executables take drogon's test options: `-r <name>` runs one `DROGON_TEST`, `-s` prints passing checks, and `-l` lists the tests.
- `tests/support.hpp` holds the shared helpers:
  - `run_tests_with_app` follows drogon's `lib/tests/main_CookieSameSite.cc`. It runs `app().run()` on a background thread and waits for a beginning advice plus `LISTEN_DELAY`, because listeners start after the advices. Then it runs the tests on the main thread and quits the app.
  - `drogon::test::run()` answers `-l` and `-h` by calling `exit()` (`drogon_test.cc:171`, `:205`). With the app thread running, the process then dies with a trantor `FATAL` and exit code 1, so `run_tests_with_app` passes those options straight to `drogon::test::run()` without starting the app (`prints_and_exits`). `../drogon-onebot`'s copy does not do this yet.
  - The test thread runs no event loop, so tests may block on `drogon::sync_wait` (`Create`) or `HttpClient::sendRequest`. `sendRequestCoro` returns an awaiter, not an awaitable, so `sync_wait` rejects it.
- `config.json` (tracked, no secrets) is the shared fixture:
  - It listens on `127.0.0.1:18081` (`../drogon-onebot` uses 18080, so both suites can run at once). `base_url` is `http://127.0.0.1:18081/v1`, so the plugin's client exercises the path prefix.
  - `model` is `fake-model`, and `api_key_env` is `DROGON_OPENAI_TEST_API_KEY`. The tests set that variable themselves.
  - `document_root` is `/tmp/drogon-openai-example`. Drogon creates `uploads/tmp/00` to `FF` under it on startup (`HttpAppFrameworkImpl.cc:535`), so every test that runs the app loads `config.json`, `test_deepseek` included, to keep those directories out of the repo.

| Test | What it covers |
| --- | --- |
| `test_config` | Calls `initAndStart` on test-built plugins without running the app: the required `base_url` and `model` (missing or empty), `api_key_env` (read, unset, empty), and the tracked `config.json`. |
| `test_chat_completions` | Serves a fake OpenAI-format API. Covers the request body (configured model, messages, unset optional fields left out, every set optional field, `extra_body` merging and overriding), `Authorization`, and `base_url` paths (none, `/v1`, trailing `/`); parsing of a full and a minimal response; every `ErrorKind` (401 with `error.message`, 503 plain text, 404, non-JSON, no `choices`, a choice without `message`, timeout, unreachable); concurrent requests not queuing; and resuming on the calling IO thread. The responses that are not JSON (the 503, the 404 page, and the non-JSON 2xx) each log a drogon JSON `Syntax error` line; that is expected. |
| `test_deepseek` | Calls the real DeepSeek API (`https://api.deepseek.com`, `deepseek-flash`): one request that must return a reply and usage, and one with a bad key that must fail with `Status` 401. It builds its client from constants instead of the plugin, because `config.json` points at the fake API. Without `DEEPSEEK_API_KEY` it prints a note and exits 77 (skipped). With the key it needs network access, spends a few tokens, and takes a few seconds. |

- The fake endpoints in `test_chat_completions` record each request in `g_last_request` on the IO thread before answering, so a test reads its own request after `sync_wait` returns. Path prefixes select the fake's behavior (`/minimal`, `/unauthorized`, `/slow`, ...), and each successful endpoint uses its own completion `id`, so a test can tell which one answered.
- `ConcurrentRequestsDoNotQueue` sends two requests to `/slow` (1 s each) from two threads and requires them to finish within 1.8 s. `CreateResumesOnCallingThread` calls `Create` from a coroutine handler on the IO thread and compares `std::this_thread::get_id()` before and after the `co_await`. Each fails when the transport shares one client, or always uses the main loop, respectively.
- `ctest --preset linux-debug -LE live` runs only the offline tests.

## Commands

`just` drives the build. Recipes map onto CMake presets named `<os>-<profile>` (`linux-debug`, `linux-release`, `windows-debug`, `windows-release`), with build trees in `build/<preset>/`.

- `just init`: run `vcpkg install` for the packages in `vcpkg_deps` (currently `drogon`), in classic mode (no `vcpkg.json`; `VCPKG_ROOT` must be set). On Windows it passes `--triplet x64-windows-static-md --host-triplet x64-windows-static-md`, so the dependencies are static libraries linked against the dynamic CRT, and the `windows-base` preset sets `VCPKG_TARGET_TRIPLET` to the same triplet so `find_package` looks in that installed tree. Keep the two in sync: the vcpkg toolchain ignores `VCPKG_DEFAULT_TRIPLET` and would fall back to `x64-windows`. Other platforms use vcpkg's default triplet.
- `just build [debug|release]`: configure only if `build/<preset>/CMakeCache.txt` is missing, then build, then copy `compile_commands.json` to `build/`. A cache left by a failed configure makes this skip configure and fail with `loading 'build.ninja'`; fix it with `just configure debug --fresh`.
- `just test [profile]`: build, then `ctest --preset <os>-<profile>` (output on failure; no tests is an error). It runs `test_deepseek` too when `DEEPSEEK_API_KEY` is set.
- `just run <target> [profile]`: build everything, then run `build/<os>-<profile>/tests/<target>`, e.g. `just run test_chat_completions` or `just run test_config release`. `<target>` is required.
- `just publish [profile]`: build (default `release`) and `cmake --install` into `publish/`.
- `just clean [profile]`: delete `build/<os>-<profile>/`, or `build/` and `publish/` when no profile is given.
- `just docs`: create `build/doxygen` (Doxygen does not create the output path) and run `doxygen Doxyfile`; open `build/doxygen/html/index.html`.
- Without just: `cmake --workflow --preset linux-debug` configures, builds, and tests.

## CI, version tags, and docs

The workflows are copies of `../drogon-onebot`'s, which follow `yizhinailong/mcr` (pinned action SHAs with the version in a comment, checksum-verified tool downloads, logic inline in the YAML). Only the tag message, the Test step comment, and the `Doxyfile` project name and brief differ; keep the two repos in step when changing them. `docs/*/ci.md` and `docs/*/doxygen.md` describe them for users; keep those pages in sync.

- `.github/workflows/linux-ci.yml` (Linux CI) runs on every branch push, PR, manual dispatch, and on version tags. It runs a `debug` and a `release` matrix leg on `ubuntu-24.04`.
  - Tools: CMake 4.2.3 (Kitware tarball, SHA-256 pinned) and LLVM 21 from apt.llvm.org (signing key fingerprint checked). `/usr/lib/llvm-21/bin` goes on `PATH`, so the presets' plain `clang++` resolves to LLVM 21.
  - Steps: `clang-format --dry-run -Werror` (debug leg only), then `vcpkg install drogon`, then `cmake --preset`, `cmake --build --preset`, and `ctest --preset`.
  - CI sets no `DEEPSEEK_API_KEY`, so `test_deepseek` reports skipped and CI never calls DeepSeek.
  - vcpkg is a shallow fetch of the pinned commit `67972e92`, the checkout that provides drogon 1.9.13 port-version 2, the version these docs cite. Bump `VCPKG_COMMIT` only after verifying locally on the same commit.
  - The vcpkg binary cache is restored and saved with `actions/cache/restore` and `actions/cache/save` right after the install, so a failing build still keeps the dependencies. The key includes `ImageOS`, the vcpkg commit, `gcc -dumpfullversion` (vcpkg builds the ports with the system GCC), and the CMake version.
  - On a tag, a step after configure compares the tag with `CMAKE_PROJECT_VERSION` from `CMakeCache.txt`. CMake parses that value itself, which double-checks the tag workflow's text parsing.
  - The ubuntu-24.04 image has GCC 12 to 14. Clang 21 uses the newest libstdc++ it finds (14).
- `.github/workflows/version-tag.yml` runs on pushes to `master` that touch `CMakeLists.txt`, and on manual dispatch.
  - It reads `VERSION` from the first `project()` call at the push's before and after commits. It strips `#` line comments, and the value must be a literal `major[.minor[.patch[.tweak]]]`. On a change, it creates the annotated tag `v<VERSION>` (`drogon-openai <VERSION>`) on the pushed commit.
  - A before commit without a usable version is only a baseline. A tag that already points at another commit fails the run; one on the same commit is reused.
  - Tags pushed with `GITHUB_TOKEN` do not trigger push workflows, so it then runs `gh workflow run linux-ci.yml --ref <tag>`. That is why Linux CI keeps `workflow_dispatch`.
  - There is no Release workflow. Consumers build from source (CPM `GIT_TAG v<VERSION>`), so the tag is the release.
- `.github/workflows/docs.yml` builds the Doxygen site with Doxygen 1.18.0 (SHA-256 pinned) on pushes to `master` that touch the documented paths, then deploys it to GitHub Pages. Pages must use the "GitHub Actions" source in the repository settings.
  - It appends `WARN_AS_ERROR = FAIL_ON_WARNINGS` through stdin (`( cat Doxyfile; echo ... ) | doxygen -`), so any comment warning fails the run.
  - The `Doxyfile` itself keeps `NO`, because older local Doxygen versions warn about unknown 1.18 tags.
- Check workflow edits with `actionlint` (with `shellcheck` on `PATH`, so the `run:` scripts are checked too).

## Documentation

- `README.md` is always the English README. GitHub shows it by default, and it is the Doxygen main page. `README.zh-CN.md` is its Simplified Chinese version, with the same sections, examples, and links (anchor ids in each file's language). Each links to the other on the line below its title. Change both together, and keep any new translation out of `README.md`.
- The user guides live in two mirrored directories, `docs/en/` (English) and `docs/zh-CN/` (Simplified Chinese), with identifiers in English in both; `docs/*/<page>` below means both copies. Every page exists in both with the same file name, and each links to its counterpart on the line below its title. Project-home links go to `../../README.md` from `docs/en/` and to `../../README.zh-CN.md` from `docs/zh-CN/`. Add a new page to both directories and both `README.md` indexes, and change the two copies together. `AGENTS.md` stays in English.
- `docs/*/drogon-plugin.md` is the user guide: configuration, request and response fields, `ErrorKind`, threads, limitations, and the tests. Update it with every behavior change.
- `Doxyfile` is the Doxygen 1.18.0 template, with the values that differ from it listed in `docs/*/doxygen.md` (`doxygen -x Doxyfile` prints them).
  - `INPUT` is `include src README.md README.zh-CN.md docs AGENTS.md` with `*.hpp *.cpp *.md`. A top-level Markdown file added to `INPUT` also goes into the `paths` of `docs.yml` and the lists in `docs/*/doxygen.md`.
  - `STRIP_FROM_INC_PATH = include` makes class pages show `#include <drogon_openai/...>`.
  - `OUTPUT_LANGUAGE = English` and the English `PROJECT_BRIEF` match the English main page. Only the navigation and built-in titles follow `OUTPUT_LANGUAGE`; `README.zh-CN.md` and `docs/zh-CN/` stay Chinese.
  - Tests are not documented.
- The repeated Doxygen block on an out-of-line definition must match the declaration's. When they differ, Doxygen warns `has multiple @param documentation sections`, which fails the docs workflow.
- Doxygen does not convert Markdown links with a fragment (`page.md#section`), so link to the page without one. `docs/en/README.md` and `docs/zh-CN/README.md` become the directory pages of `docs/en/` and `docs/zh-CN/` (`IMPLICIT_DIR_DOCS`), and links to them resolve there.
- The example program in `README.md` and `README.zh-CN.md` must compile against the library; the two copies differ only in their comments. When the public API changes, rebuild it as a small `add_subdirectory` consumer.

## Build notes

- Requires CMake >= 4.0, Ninja, and ccache (hard-wired as `CMAKE_CXX_COMPILER_LAUNCHER`). Compiler is `clang++` on Linux, `clang-cl` on Windows.
- Don't set `CMAKE_CXX_STANDARD` here. The library declares `target_compile_features(PUBLIC cxx_std_23)` (the public headers use `std::expected`), which is exported to `find_package` consumers. A parent project's `CMAKE_CXX_STANDARD` still applies in `add_subdirectory` mode, so under a C++26 parent the library builds with `-std=gnu++26`. The test executables add `cxx_std_23` for `<print>`.
- Drogon 1.9.13's `FindFilesystem.cmake` (run by `find_package(Drogon)`) leaves an unset `CMAKE_CXX_STANDARD` defined as `""`, and `target_compile_features` then fails with `CXX_STANDARD property ... contained an invalid value: ""`. Both `CMakeLists.txt` and `cmake/drogon-openai-config.cmake.in` unset the empty value right after finding Drogon. Keep those blocks.
- Sources and public headers are collected with `file(GLOB_RECURSE ... CONFIGURE_DEPENDS)`, so adding a file triggers a re-configure on the next build.

## Code style

New C++ code follows the `cpp-style-ref` skill (CMake + vcpkg variant), the same as `../assistantbot`: types and public methods PascalCase, private members `m_`, namespaces lowercase, `/** @file ... @brief ... */` at the top of every file, Doxygen blocks repeated on out-of-line definitions, `T const&`, brace initialization, and trailing return types. Drogon's virtual overrides (`initAndStart`, `shutdown`) keep their upstream names.

Format with `clang-format -i <files>` (root `.clang-format`, same as `../assistantbot`). Check with `clang-format --dry-run -Werror <files>`.

`.clang-format` sets `LineEnding: LF`, so the dry run flags every line of a CRLF file. `.gitattributes` (`* text=auto eol=lf`) overrides `core.autocrlf` and checks every file out with LF, Windows included. A working tree from before `.gitattributes` keeps its CRLF files until they are converted or checked out again.
