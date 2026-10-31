/**
 * @file pagination.hpp
 * @brief Provide owned cursor pages with manual and asynchronous automatic traversal.
 */
#pragma once

#include <functional>
#include <optional>
#include <set>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <drogon/utils/coroutine.h>

#include "drogon_openai/error.hpp"

namespace drogon_openai {

    /**
     * @brief Own one JSON or typed cursor page and the request needed for its successor.
     * @tparam T Item type; JSON pages retain arbitrary API fields.
     * @note Callbacks run on the awaiting event loop and must not block it.
     */
    template <typename T = Json::Value>
    class Page {
    public:
        using Decoder = std::function<Result<T>(Json::Value const&)>;             ///< Decode one item.
        using FetchNext = std::function<drogon::Task<Result<Page>>(std::string)>; ///< Fetch an owned cursor.
        using ItemCallback = std::function<Result<bool>(T const&)>;               ///< False stops traversal.

    private:
        FetchNext m_fetch_next;    ///< Owns the transport and original request settings.
        bool m_cursor{ true };     ///< Cursor pagination; false is a single page.
        std::string m_next_cursor; ///< Cursor from the response's last item.

    public:
        std::vector<T> data;                   ///< Items in this page.
        std::string object;                    ///< Usually `list`.
        std::optional<bool> has_more{};        ///< Missing means infer continuation from the cursor.
        std::optional<std::string> first_id{}; ///< First item ID, when supplied.
        std::optional<std::string> last_id{};  ///< Last item ID, when supplied.
        Json::Value raw{};                     ///< Complete original JSON page.
        int status_code{};                     ///< HTTP status for this page.
        Headers headers;                       ///< Response headers for this page.
        std::string body;                      ///< Original response bytes.
        std::string _request_id;               ///< Response `x-request-id` for this page.

        /**
         * @brief Decode a response page and bind its continuation request.
         * @param response Parsed HTTP response, taken by value.
         * @param decoder Item decoder; omitted for JSON pages.
         * @param fetch_next Callback receiving the next `after` cursor.
         * @param cursor False for an SDK-style single page that cannot continue.
         * @param cursor_field Explicit response cursor field; empty uses the last item ID.
         * @return Page or a response validation error with HTTP metadata.
         */
        [[nodiscard]] static auto FromResponse(
            RawResponse<Json::Value> response,
            Decoder decoder = {},
            FetchNext fetch_next = {},
            bool cursor = true,
            std::string cursor_field = {}
        ) -> Result<Page> {
            auto invalid = [&](std::string message) -> Result<Page> {
                return std::unexpected{ Error{
                    .kind = ErrorKind::ResponseValidation,
                    .message = std::move(message),
                    .status_code = response.status_code,
                    .body = response.body,
                    .request_id = response.request_id,
                    .headers = response.headers,
                } };
            };
            auto const& root{ response.parsed };
            if (!root.isObject() || !root["data"].isArray()) {
                return invalid("page has no data array");
            }
            if (!root["has_more"].isNull() && !root["has_more"].isBool()) {
                return invalid("page has_more must be a boolean");
            }
            Page page;
            page.m_fetch_next = std::move(fetch_next);
            page.m_cursor = cursor;
            page.object = root["object"].isString() ? root["object"].asString() : "list";
            if (root["has_more"].isBool()) {
                page.has_more = root["has_more"].asBool();
            }
            for (auto const* key : { "first_id", "last_id" }) {
                if (!root[key].isNull() && !root[key].isString()) {
                    return invalid(std::string{ "page " } + key + " must be a string or null");
                }
            }
            if (root["first_id"].isString()) {
                page.first_id = root["first_id"].asString();
            }
            if (root["last_id"].isString()) {
                page.last_id = root["last_id"].asString();
            }
            for (auto const& item : root["data"]) {
                if constexpr (std::is_same_v<T, Json::Value>) {
                    if (!decoder) {
                        page.data.push_back(item);
                        continue;
                    }
                }
                if (!decoder) {
                    return invalid("typed page requires an item decoder");
                }
                Result<T> parsed;
                try {
                    parsed = decoder(item);
                } catch (std::exception const& error) {
                    return invalid(std::string{ "page decoder: " } + error.what());
                } catch (...) {
                    return invalid("page decoder threw an exception");
                }
                if (!parsed) {
                    auto error{ std::move(parsed.error()) };
                    error.status_code = response.status_code;
                    error.body = response.body;
                    error.request_id = response.request_id;
                    error.headers = response.headers;
                    return std::unexpected{ std::move(error) };
                }
                page.data.push_back(std::move(*parsed));
            }
            if (!cursor_field.empty()) {
                if (!root[cursor_field].isNull() && !root[cursor_field].isString()) {
                    return invalid("page cursor must be a string or null");
                }
                if (root[cursor_field].isString()) {
                    page.m_next_cursor = root[cursor_field].asString();
                }
            } else if (!page.data.empty()) {
                auto const& last{ root["data"][root["data"].size() - 1] };
                if (last.isObject() && last["id"].isString()) {
                    page.m_next_cursor = last["id"].asString();
                } else if (page.last_id) {
                    page.m_next_cursor = *page.last_id;
                }
            }
            if (page.has_more.value_or(false) && page.m_next_cursor.empty()) {
                return invalid("page has_more is true without a last item cursor");
            }
            page.raw = std::move(response.parsed);
            page.status_code = response.status_code;
            page.headers = std::move(response.headers);
            page.body = std::move(response.body);
            page._request_id = std::move(response.request_id);
            return page;
        }

        /**
         * @brief Check whether this response supplies a next cursor.
         * @return False after an explicit final or empty page.
         */
        [[nodiscard]] auto HasNextPage() const noexcept -> bool {
            return m_cursor && static_cast<bool>(m_fetch_next) && has_more.value_or(true) && !m_next_cursor.empty();
        }

        /**
         * @brief Fetch the next page while retaining the original query and request overrides.
         * @return Next page, or an error when no continuation exists.
         */
        [[nodiscard]] auto GetNextPage() const -> drogon::Task<Result<Page>> {
            auto fetch_next{ m_fetch_next };
            auto cursor{ m_next_cursor };
            if (!HasNextPage() || !fetch_next) {
                co_return std::unexpected{ Error{
                    .kind = ErrorKind::InvalidRequest,
                    .message = "page has no next request",
                } };
            }
            co_return co_await fetch_next(std::move(cursor));
        }

        /**
         * @brief Visit items across pages without blocking the calling event loop.
         * @param callback Item consumer; false stops successfully, an error aborts traversal.
         * @return Success or the first callback, pagination, or HTTP error.
         */
        [[nodiscard]] auto ForEach(ItemCallback callback) const -> drogon::Task<Result<void>> {
            auto page{ *this };
            if (!callback) {
                co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "page callback is empty" } };
            }
            std::set<std::string> visited;
            for (;;) {
                for (auto const& item : page.data) {
                    Result<bool> keep_going;
                    try {
                        keep_going = callback(item);
                    } catch (std::exception const& error) {
                        co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = std::string{ "page callback: " } + error.what() } };
                    } catch (...) {
                        co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "page callback threw an exception" } };
                    }
                    if (!keep_going) {
                        co_return std::unexpected{ std::move(keep_going.error()) };
                    }
                    if (!*keep_going) {
                        co_return Result<void>{};
                    }
                }
                if (!page.HasNextPage()) {
                    co_return Result<void>{};
                }
                if (!visited.insert(page.m_next_cursor).second) {
                    co_return std::unexpected{ Error{
                        .kind = ErrorKind::ResponseValidation,
                        .message = "pagination repeated a cursor",
                        .status_code = page.status_code,
                        .body = page.body,
                        .request_id = page._request_id,
                        .headers = page.headers,
                    } };
                }
                auto next{ co_await page.GetNextPage() };
                if (!next) {
                    co_return std::unexpected{ std::move(next.error()) };
                }
                page = std::move(*next);
            }
        }

        /**
         * @brief Collect this page and every successor into one vector.
         * @return All items or the first pagination/HTTP error.
         */
        [[nodiscard]] auto All() const -> drogon::Task<Result<std::vector<T>>> {
            auto page{ *this };
            std::vector<T> items;
            auto result{ co_await page.ForEach([&items](T const& item) -> Result<bool> {
                items.push_back(item);
                return true;
            }) };
            if (!result) {
                co_return std::unexpected{ std::move(result.error()) };
            }
            co_return items;
        }
    };

    /**
     * @brief JSON page for unwrapped API paths.
     */
    using JsonPage = Page<Json::Value>;

    /**
     * @brief Cursor page alias for typed API groups.
     * @tparam T Item type.
     */
    template <typename T>
    using CursorPage = Page<T>;

} // namespace drogon_openai
