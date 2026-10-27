/**
 * @file test_config.cpp
 * @brief Check how `OpenAIClient::initAndStart` reads and validates the plugin config.
 *
 * Calls `initAndStart` on plugins built by the test, without running the app.
 */
#include <stdexcept>
#include <string>

#include <drogon/drogon_test.h>

#include <drogon_openai/openai_client.hpp>

#include "support.hpp"

namespace {

    constexpr char const* UNSET_ENV{ "DROGON_OPENAI_TEST_UNSET" }; ///< Variable that no test sets.
    constexpr char const* EMPTY_ENV{ "DROGON_OPENAI_TEST_EMPTY" }; ///< Variable that a test sets to an empty value.

    /**
     * @brief Build a plugin config that has only the required keys.
     * @return Config object with DeepSeek's `base_url` and a `model`.
     */
    auto minimal_config() -> Json::Value {
        Json::Value config{ Json::objectValue };
        config["base_url"] = "https://api.deepseek.com";
        config["model"] = "deepseek-flash";
        return config;
    }

    /**
     * @brief Initialize a new plugin with `config` and report how it was rejected.
     * @param config Plugin config to pass to `initAndStart`.
     * @return The `std::invalid_argument` message, or `(accepted)` if nothing was thrown.
     */
    auto rejection(Json::Value const& config) -> std::string {
        drogon_openai::OpenAIClient plugin{};
        try {
            plugin.initAndStart(config);
        } catch (std::invalid_argument const& e) {
            return e.what();
        }
        return "(accepted)";
    }

} // namespace

DROGON_TEST(RequiresBaseUrlAndModel) {
    CHECK(rejection(Json::Value{ Json::objectValue }).contains("non-empty base_url and model"));

    for (auto const* key : { "base_url", "model" }) {
        auto missing{ minimal_config() };
        missing.removeMember(key);
        CHECK(rejection(missing).contains("non-empty base_url and model"));

        auto empty{ minimal_config() };
        empty[key] = "";
        CHECK(rejection(empty).contains("non-empty base_url and model"));
    }
}

DROGON_TEST(ReadsConfigWithoutApiKey) {
    drogon_openai::OpenAIClient plugin{};
    plugin.initAndStart(minimal_config());
    auto const& config{ plugin.Config() };
    CHECK(config.base_url == "https://api.deepseek.com");
    CHECK(config.model == "deepseek-flash");
    CHECK(config.api_key.empty());
}

DROGON_TEST(ReadsApiKeyFromEnv) {
    test_support::set_env(test_support::API_KEY_ENV, test_support::API_KEY);
    auto config_json{ minimal_config() };
    config_json["api_key_env"] = test_support::API_KEY_ENV;
    drogon_openai::OpenAIClient plugin{};
    plugin.initAndStart(config_json);
    CHECK(plugin.Config().api_key == test_support::API_KEY);
    test_support::unset_env(test_support::API_KEY_ENV);
}

DROGON_TEST(RejectsUnsetApiKeyEnv) {
    auto config{ minimal_config() };
    config["api_key_env"] = UNSET_ENV;
    CHECK(rejection(config).contains("DROGON_OPENAI_TEST_UNSET is unset or empty"));
}

DROGON_TEST(RejectsEmptyApiKeyEnv) {
    test_support::set_env(EMPTY_ENV, "");
    auto config{ minimal_config() };
    config["api_key_env"] = EMPTY_ENV;
    CHECK(rejection(config).contains("DROGON_OPENAI_TEST_EMPTY is unset or empty"));
    test_support::unset_env(EMPTY_ENV);
}

DROGON_TEST(AcceptsTrackedConfigJson) {
    test_support::set_env(test_support::API_KEY_ENV, test_support::API_KEY);
    auto const root{ test_support::read_config() };
    REQUIRE(root["plugins"][0]["name"].asString() == "drogon_openai::OpenAIClient");

    drogon_openai::OpenAIClient plugin{};
    plugin.initAndStart(root["plugins"][0]["config"]);
    CHECK(plugin.Config().base_url == std::string{ test_support::SERVER_URL } + test_support::BASE_PATH);
    CHECK(plugin.Config().model == test_support::MODEL);
    CHECK(plugin.Config().api_key == test_support::API_KEY);
    test_support::unset_env(test_support::API_KEY_ENV);
}

auto main(int argc, char** argv) -> int {
    return drogon::test::run(argc, argv);
}
