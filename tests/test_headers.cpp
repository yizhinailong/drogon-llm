/**
 * @file test_headers.cpp
 * @brief Verify independent header values and portable coroutine result assignment.
 */
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include <drogon/drogon_test.h>
#include <drogon/utils/coroutine.h>

#include <drogon_openai/pagination.hpp>
#include <drogon_openai/types/chat.hpp>
#include <drogon_openai/types/resource.hpp>

namespace {

    using namespace drogon_openai;

    static_assert(std::is_nothrow_move_constructible_v<Headers>);
    static_assert(std::is_nothrow_move_assignable_v<Headers>);
    static_assert(std::is_nothrow_move_constructible_v<Error>);
    static_assert(std::is_move_assignable_v<std::optional<Result<RawResponse<ChatCompletion>>>>);
    static_assert(std::is_move_assignable_v<std::optional<Result<Page<ChatCompletion>>>>);
    static_assert(std::is_move_assignable_v<std::optional<Result<ApiStreamResult>>>);

    /**
     * @brief Return both alternatives through Drogon's assigning promise.
     * @param fail Whether to return an HTTP error.
     * @return Raw response or error, retaining header fields.
     */
    auto raw_result(bool fail) -> drogon::Task<Result<RawResponse<std::string>>> {
        if (fail) {
            co_return std::unexpected{ Error{
                .kind = ErrorKind::Status,
                .message = "test failure",
                .status_code = 429,
                .headers = { { "retry-after", "1" } },
            } };
        }
        co_return RawResponse<std::string>{
            .status_code = 200,
            .headers = { { "x-request-id", "req-headers" } },
            .body = "response",
            .parsed = "response",
        };
    }

} // namespace

DROGON_TEST(HeadersCopyOwnsIndependentFields) {
    drogon_openai::Headers original{ { "x-mode", "original" } };
    auto copy{ original };
    copy["x-mode"] = "copy";
    copy.emplace("x-extra", "added");
    CHECK(original.at("x-mode") == "original");
    CHECK(!original.contains("x-extra"));

    drogon_openai::Headers assigned;
    assigned = original;
    assigned.at("x-mode") = "assigned";
    CHECK(original.at("x-mode") == "original");
    CHECK(assigned.at("x-mode") == "assigned");
    CHECK(copy.at("x-mode") == "copy");

    drogon_openai::Headers::Storage legacy{ { "x-mode", "map" } };
    drogon_openai::Headers imported{ legacy };
    imported["x-mode"] = "imported";
    CHECK(legacy.at("x-mode") == "map");
    CHECK(imported.at("x-mode") == "imported");
}

DROGON_TEST(HeadersMovesPreserveStorageAndAllowReuse) {
    drogon_openai::Headers original{ { "x-mode", "value" } };
    auto const* address{ &original.at("x-mode") };
    auto moved{ std::move(original) };
    CHECK(&moved.at("x-mode") == address);
    CHECK(original.empty());
    CHECK(std::as_const(original).find("absent") == original.end());
    original["x-new"] = "reused";
    CHECK(!moved.contains("x-new"));

    drogon_openai::Headers assigned{ { "x-old", "discarded" } };
    assigned = std::move(moved);
    CHECK(&assigned.at("x-mode") == address);
    CHECK(!assigned.contains("x-old"));
    CHECK(moved.empty());
    moved = { { "x-replaced", "replacement" } };
    CHECK(moved.at("x-replaced") == "replacement");
}

DROGON_TEST(HeadersKeepOrderedLookupAndMutation) {
    drogon_openai::Headers fields{ { "z-last", "z" }, { "a-first", "a" } };
    CHECK(fields.begin()->first == "a-first");
    CHECK(fields.size() == 2);
    auto const inserted{ fields.insert({ "a-first", "unchanged" }) };
    CHECK(!inserted.second);
    CHECK(fields.at("a-first") == "a");
    CHECK(fields.erase("z-last") == 1);
    CHECK(fields.count("z-last") == 0);
    fields.erase(fields.begin());
    CHECK(fields.empty());
    fields.emplace("x-new", "new");
    fields.clear();
    CHECK(fields.cbegin() == fields.cend());
}

DROGON_TEST(CoroutineResultsPreserveBothAlternativesAndHeaders) {
    auto success{ drogon::sync_wait(raw_result(false)) };
    REQUIRE(success.has_value());
    CHECK(success->parsed == "response");
    CHECK(success->headers.at("x-request-id") == "req-headers");

    auto failure{ drogon::sync_wait(raw_result(true)) };
    REQUIRE(!failure.has_value());
    CHECK(failure.error().status_code == 429);
    CHECK(failure.error().headers.at("retry-after") == "1");

    success = std::move(failure);
    REQUIRE(!success.has_value());
    CHECK(success.error().headers.at("retry-after") == "1");
    success = drogon::sync_wait(raw_result(false));
    REQUIRE(success.has_value());
    CHECK(success->headers.at("x-request-id") == "req-headers");
}

/**
 * @brief Run header and coroutine checks without starting the application.
 * @param argc Argument count.
 * @param argv Argument values.
 * @return Test framework exit code.
 */
auto main(int argc, char** argv) -> int {
    return drogon::test::run(argc, argv);
}
