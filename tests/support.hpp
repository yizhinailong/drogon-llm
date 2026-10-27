/**
 * @file support.hpp
 * @brief Share the helpers of the test programs: environment variables, config.json, and the app thread.
 */
#pragma once

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <future>
#include <string_view>
#include <thread>

#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include <json/json.h>

namespace test_support {

    constexpr auto LISTEN_DELAY{ std::chrono::milliseconds{ 200 } };   ///< Wait for the listeners after the beginning advices.
    constexpr char const* SERVER_URL{ "http://127.0.0.1:18081" };      ///< Listener in config.json, which its `base_url` points at.
    constexpr char const* BASE_PATH{ "/v1" };                          ///< Path of `base_url` in config.json.
    constexpr char const* MODEL{ "fake-model" };                       ///< `model` in config.json.
    constexpr char const* API_KEY_ENV{ "DROGON_OPENAI_TEST_API_KEY" }; ///< `api_key_env` in config.json.
    constexpr char const* API_KEY{ "drogon-openai-test-key" };         ///< Value that the tests give `API_KEY_ENV`.

    /**
     * @brief Set an environment variable of this process.
     * @param name Variable name.
     * @param value New value.
     */
    inline auto set_env(char const* name, char const* value) -> void {
#ifdef _WIN32
        _putenv_s(name, value);
#else
        setenv(name, value, 1);
#endif
    }

    /**
     * @brief Remove an environment variable of this process.
     * @param name Variable name.
     */
    inline auto unset_env(char const* name) -> void {
#ifdef _WIN32
        _putenv_s(name, "");
#else
        unsetenv(name);
#endif
    }

    /**
     * @brief Parse config.json from the working directory, which CTest and `just run` set to the repo root.
     * @return The whole configuration.
     * @throws Json::RuntimeError If the file is missing or not valid JSON.
     */
    inline auto read_config() -> Json::Value {
        std::ifstream file{ "config.json" };
        Json::Value root{};
        file >> root;
        return root;
    }

    /**
     * @brief Tell whether the arguments make `drogon::test::run()` print and exit.
     * @param argc Argument count.
     * @param argv Arguments.
     * @return True when one of them is `-l`, `-h`, or `--help`.
     */
    inline auto prints_and_exits(int argc, char** argv) -> bool {
        for (int i{ 1 }; i < argc; ++i) {
            std::string_view const arg{ argv[i] };
            if (arg == "-l" || arg == "-h" || arg == "--help") {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Run the app on a background thread, run every `DROGON_TEST`, then stop the app.
     *
     * Follows drogon's own app-backed tests (`lib/tests/main_CookieSameSite.cc`). Beginning
     * advices run before `startListening()`, and each IO loop starts listening
     * asynchronously, so the tests start `LISTEN_DELAY` after the advices. The test thread
     * runs no event loop, so tests may block on `drogon::sync_wait()`.
     *
     * `drogon::test::run()` answers `-l` and `-h` with `exit()` (`drogon_test.cc`), which
     * would end the process while the app thread runs, so those run without the app.
     * @param argc Argument count, forwarded to `drogon::test::run()`.
     * @param argv Arguments, forwarded to `drogon::test::run()`: `-s` prints passing checks,
     * `-r <name>` runs one test, and `-l` lists them.
     * @return 0 if every check passed, 1 otherwise.
     */
    inline auto run_tests_with_app(int argc, char** argv) -> int {
        if (prints_and_exits(argc, argv)) {
            return drogon::test::run(argc, argv);
        }

        std::promise<void> started{};
        drogon::app().registerBeginningAdvice([&started] { started.set_value(); });
        std::thread server{ [] { drogon::app().run(); } };
        started.get_future().wait();
        std::this_thread::sleep_for(LISTEN_DELAY);

        int const status{ drogon::test::run(argc, argv) };
        drogon::app().getLoop()->queueInLoop([] { drogon::app().quit(); });
        server.join();
        return status;
    }

} // namespace test_support
