/**
 * @file resource.hpp
 * @brief Common payloads and events used by OpenAI API resources.
 */
#pragma once

#include <cstddef>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "drogon_openai/error.hpp"
#include "drogon_openai/request_options.hpp"

namespace drogon_openai {

    /**
     * @brief Owned file bytes for a replayable multipart upload.
     */
    struct UploadFile {
        std::string content;                                    ///< File bytes, including embedded NULs.
        std::string filename{ "upload" };                       ///< Filename sent to the API.
        std::string content_type{ "application/octet-stream" }; ///< Media type.
    };

    /**
     * @brief A file, multiple files, or a documented JSON file reference.
     */
    using FileInput = std::variant<UploadFile, std::vector<UploadFile>, Json::Value>;

    /**
     * @brief Owned request fields and files prior to HTTP serialization.
     */
    struct ResourcePayload {
        Json::Value extra_body{};                                                ///< Body overrides validated and merged at dispatch.
        Json::Value body{};                                                      ///< JSON or form fields; null means no request body.
        Json::Value query{};                                                     ///< Named SDK query parameters, separate from extra_body.
        std::map<std::string, std::pair<std::string, bool>> multipart_encodings; ///< Media type and JSON encoding for explicit form parts.
        std::optional<std::string> raw_body_field{};                             ///< Single string field eligible for a raw request body.
        std::vector<MultipartPart> files;                                        ///< Files, named according to the API schema.
        bool multipart{};                                                        ///< Force multipart even when no files are present.
    };

    /**
     * @brief One decoded SSE event, retaining arbitrary provider fields.
     */
    struct ApiStreamEvent {
        std::string type; ///< JSON type, falling back to the SSE event name.
        std::string id;   ///< SSE event ID, when supplied.
        Json::Value data; ///< Complete event JSON.
    };

    /**
     * @brief Progress and the final JSON event from a streamed API operation.
     */
    struct ApiStreamResult {
        std::size_t event_count{};             ///< Number of decoded events.
        Json::Value last_event;                ///< Last event delivered to the consumer.
        std::optional<Json::Value> response{}; ///< Final Responses resource, when supplied.
        bool cancelled{};                      ///< Consumer stopped delivery before completion.
        Headers headers;                       ///< HTTP response headers.
        std::string _request_id;               ///< Response x-request-id.
    };

    /**
     * @brief Consume an event on the awaiting loop; false cancels delivery.
     */
    using ApiEventCallback = std::function<Result<bool>(ApiStreamEvent const&)>;

} // namespace drogon_openai
