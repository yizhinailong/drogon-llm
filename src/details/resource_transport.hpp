/**
 * @file resource_transport.hpp
 * @brief Shared serialization and typed dispatch for API resource bindings.
 */
#pragma once

#include <type_traits>
#include <utility>

#include "details/transport.hpp"
#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::details {

    /**
     * @brief Serialize a scalar or JSON field.
     * @tparam T Field type.
     * @param value Field value.
     * @return Wire JSON value.
     */
    template <typename T>
    auto resource_json(T const& value) -> Json::Value {
        if constexpr (std::is_same_v<T, std::int64_t>) {
            return Json::Value{ Json::Int64{ value } };
        } else {
            return Json::Value{ value };
        }
    }

    /**
     * @brief Serialize a collection without restricting SDK union elements.
     * @tparam T Element type.
     * @param value Collection value.
     * @return JSON array.
     */
    template <typename T>
    auto resource_json(std::vector<T> const& value) -> Json::Value {
        Json::Value result{ Json::arrayValue };
        for (auto const& item : value) {
            result.append(resource_json(item));
        }
        return result;
    }

    /**
     * @brief Add file bytes or a JSON reference to an owned payload.
     * @param payload Destination request.
     * @param name API field name.
     * @param input File, files, or JSON reference.
     */
    auto resource_file(ResourcePayload& payload, std::string name, FileInput const& input) -> void;

    /**
     * @brief Apply body overrides and request defaults without changing caller state.
     * @param body API body or query fields.
     * @param extra Overriding fields.
     */
    auto resource_merge(Json::Value& body, Json::Value const& extra) -> void;

    /**
     * @brief Build a path by encoding each identifier as one segment.
     * @param pattern Path template.
     * @param identifiers Template names and values.
     * @return Relative path or invalid-request error for an empty identifier.
     */
    auto resource_path(
        std::string pattern,
        std::initializer_list<std::pair<std::string_view, std::string_view>> identifiers
    ) -> Result<std::string>;

    /**
     * @brief Dispatch a resource request and retain original bytes.
     * @param transport Shared endpoint settings.
     * @param method HTTP method.
     * @param path Relative API path.
     * @param payload JSON or multipart data.
     * @param options Per-request overrides.
     * @param defaults Resource-specific HTTP headers.
     * @param on_data Optional incremental body callback.
     * @return Response bytes and HTTP metadata.
     */
    auto resource_bytes(
        std::shared_ptr<Transport> transport,
        std::string method,
        std::string path,
        ResourcePayload payload,
        RequestOptions options,
        Headers defaults = {},
        BodyCallback on_data = {}
    ) -> drogon::Task<Result<RawResponse<std::string>>>;

    /**
     * @brief Dispatch a resource JSON request.
     * @param transport Shared endpoint settings.
     * @param method HTTP method.
     * @param path Relative API path.
     * @param payload JSON or multipart data.
     * @param options Per-request overrides.
     * @param defaults Resource-specific HTTP headers.
     * @return JSON with HTTP metadata.
     */
    auto resource_request(
        std::shared_ptr<Transport> transport,
        std::string method,
        std::string path,
        ResourcePayload payload,
        RequestOptions options,
        Headers defaults = {}
    ) -> drogon::Task<Result<RawResponse<Json::Value>>>;

    /**
     * @brief Decode a scalar response field with range and type checks.
     * @tparam T Destination field type.
     * @param value Wire value.
     * @param output Destination.
     * @return Whether the field matches its C++ type.
     */
    template <typename T>
    auto resource_read(Json::Value const& value, T& output) -> bool {
        if constexpr (std::is_same_v<T, Json::Value>) {
            output = value;
        } else if constexpr (std::is_same_v<T, std::string>) {
            if (!value.isString()) {
                return false;
            }
            output = value.asString();
        } else if constexpr (std::is_same_v<T, std::int64_t>) {
            if (!value.isInt64()) {
                return false;
            }
            output = value.asInt64();
        } else if constexpr (std::is_same_v<T, double>) {
            if (!value.isNumeric()) {
                return false;
            }
            output = value.asDouble();
        } else if constexpr (std::is_same_v<T, bool>) {
            if (!value.isBool()) {
                return false;
            }
            output = value.asBool();
        } else {
            if (!value.isArray()) {
                return false;
            }
            for (auto const& item : value) {
                typename T::value_type element{};
                if (!resource_read(item, element)) {
                    return false;
                }
                output.push_back(std::move(element));
            }
        }
        return true;
    }

    /**
     * @brief Decode one named resource or preserve a JSON union response.
     * @tparam T Result type.
     * @param value Response JSON.
     * @return Typed value or an error.
     */
    template <typename T>
    auto resource_decode(Json::Value const& value) -> Result<T> {
        if constexpr (std::is_same_v<T, Json::Value>) {
            return value;
        } else {
            return T::FromJson(value);
        }
    }

    /**
     * @brief Decode a typed response while retaining HTTP error metadata.
     * @tparam T Result type.
     * @param response JSON HTTP response.
     * @return Typed raw response or enriched validation error.
     */
    template <typename T>
    auto resource_decode_response(RawResponse<Json::Value> response) -> Result<RawResponse<T>> {
        auto parsed{ resource_decode<T>(response.parsed) };
        if (!parsed) {
            auto error{ std::move(parsed.error()) };
            error.status_code = response.status_code;
            error.headers = response.headers;
            error.body = response.body;
            error.request_id = response.request_id;
            return std::unexpected{ std::move(error) };
        }
        if constexpr (!std::is_same_v<T, Json::Value>) {
            parsed->_request_id = response.request_id;
        }
        return RawResponse<T>{ response.status_code, std::move(response.headers), std::move(response.body), std::move(*parsed), std::move(response.request_id) };
    }

    /**
     * @brief Fetch a typed page and retain owned continuation state.
     * @tparam T Item type.
     * @param transport Shared endpoint settings.
     * @param method HTTP method, including POST search pages.
     * @param path Relative collection path.
     * @param payload Request body or query parameters.
     * @param options Request overrides.
     * @param defaults Resource-specific headers.
     * @param cursor Whether this collection supports cursor traversal.
     * @param cursor_field Explicit response cursor field; empty uses the last item ID.
     * @return Owned page or request/validation error.
     */
    template <typename T>
    auto resource_page(
        std::shared_ptr<Transport> transport,
        std::string method,
        std::string path,
        ResourcePayload payload,
        RequestOptions options,
        Headers defaults,
        bool cursor,
        std::string cursor_field = {}
    ) -> drogon::Task<Result<Page<T>>> {
        auto response{ co_await resource_request(transport, method, path, payload, options, defaults) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto next{ [transport, method, path, payload, options, defaults, cursor, cursor_field](std::string after) mutable {
            auto request_payload{ payload };
            (method == "GET" ? request_payload.query : request_payload.body)["after"] = std::move(after);
            auto request_options{ options };
            if (request_options.extra_query.isObject()) {
                request_options.extra_query.removeMember("after");
            }
            return resource_page<T>(transport, method, path, std::move(request_payload), std::move(request_options), defaults, cursor, cursor_field);
        } };
        co_return Page<T>::FromResponse(std::move(*response), resource_decode<T>, std::move(next), cursor, std::move(cursor_field));
    }

    /**
     * @brief Stream arbitrary API SSE events without buffering the response.
     * @param transport Shared endpoint settings.
     * @param method HTTP method.
     * @param path API path.
     * @param payload Request body or multipart data.
     * @param options Request overrides.
     * @param defaults Resource-specific headers.
     * @param callback Event consumer on the awaiting loop.
     * @return Stream metadata and final event or an error.
     */
    auto resource_stream(
        std::shared_ptr<Transport> transport,
        std::string method,
        std::string path,
        ResourcePayload payload,
        RequestOptions options,
        Headers defaults,
        ApiEventCallback callback
    ) -> drogon::Task<Result<ApiStreamResult>>;

} // namespace drogon_openai::details
