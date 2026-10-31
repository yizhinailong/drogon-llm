/**
 * @file request_options.hpp
 * @brief Declare common request settings and raw HTTP responses.
 */
#pragma once

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <json/value.h>

namespace drogon_openai {

    /**
     * @brief HTTP header names and values; names are matched without case.
     */
    using Headers = std::map<std::string, std::string>;

    /**
     * @brief Per-phase inactivity timeouts, in seconds; zero disables a phase timeout.
     */
    struct Timeout {
        double connect{ 5.0 }; ///< DNS, TCP, and TLS establishment.
        double read{ 600.0 };  ///< Waiting for the next response bytes.
        double write{ 600.0 }; ///< Waiting for request bytes to drain.

        /**
         * @brief Use the SDK defaults.
         */
        Timeout() = default;

        /**
         * @brief Apply one timeout to all phases.
         * @param seconds Timeout in seconds; zero disables timeouts.
         */
        Timeout(double seconds) : connect{ seconds }, read{ seconds }, write{ seconds } {}
    };

    /**
     * @brief Settings overriding client defaults for one HTTP request.
     */
    struct RequestOptions {
        std::optional<Timeout> timeout{}; ///< Per-phase timeouts.
        std::optional<int> max_retries{}; ///< Retries after the initial attempt; zero disables them.
        Headers extra_headers{};          ///< Request headers overriding client defaults.
        Json::Value extra_query{};        ///< Query parameters overriding client defaults.
    };

    /**
     * @brief In-memory multipart upload part; binary content is retained verbatim.
     */
    struct MultipartPart {
        std::string name;                          ///< Form field name.
        std::string content;                       ///< Field text or file bytes.
        std::optional<std::string> filename{};     ///< Set for file parts.
        std::optional<std::string> content_type{}; ///< File media type.
    };

    /**
     * @brief HTTP response metadata paired with a parsed result.
     * @tparam T Parsed response type.
     */
    template <typename T>
    struct RawResponse {
        int status_code{};      ///< HTTP status.
        Headers headers;        ///< Response headers with lowercase names.
        std::string body;       ///< Raw response bytes.
        T parsed;               ///< Parsed response.
        std::string request_id; ///< Response `x-request-id`.
    };

    /**
     * @brief Conditions controlling a generic asynchronous poll operation.
     */
    struct PollOptions {
        double interval{ 1.0 };                                                                                  ///< Seconds between polls unless the response supplies `openai-poll-after-ms`.
        double timeout{ 600.0 };                                                                                 ///< Total polling duration; zero disables the deadline.
        std::string status_field{ "status" };                                                                    ///< Field containing the job state.
        std::vector<std::string> terminal_states{ "completed", "failed", "cancelled", "expired", "incomplete" }; ///< Terminal states.
    };

} // namespace drogon_openai
