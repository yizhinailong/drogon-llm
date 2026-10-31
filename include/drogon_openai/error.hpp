/**
 * @file error.hpp
 * @brief Declare the error type that failed requests return through `Result`.
 */
#pragma once

#include <expected>
#include <optional>
#include <string>

#include "drogon_openai/request_options.hpp"

namespace drogon_openai {

    /**
     * @brief Category of a failed request, after the OpenAI Python SDK's exception classes.
     */
    enum class ErrorKind {
        Connection,                ///< No response arrived, e.g. DNS, TLS, or network failure (`APIConnectionError`).
        Timeout,                   ///< No response arrived within the timeout (`APITimeoutError`).
        Status,                    ///< The endpoint answered with a non-2xx status (`APIStatusError`).
        ResponseValidation,        ///< A 2xx response did not have the expected shape (`APIResponseValidationError`).
        LengthFinishReason,        ///< Structured parsing stopped at the token limit.
        ContentFilterFinishReason, ///< Structured parsing was blocked by content filtering.
        InvalidWebhookSignature,   ///< Webhook signature or replay window verification failed.
        InvalidRequest,            ///< Invalid local request options or parsing configuration.
    };

    /**
     * @brief Describe why a request failed.
     */
    struct Error {
        ErrorKind kind;                     ///< Failure category.
        std::string message;                ///< Human-readable description; for `Status`, the endpoint's `error.message` when present.
        int status_code{};                  ///< HTTP status when response headers arrived; 0 before that.
        std::string body;                   ///< Buffered response body or an offending stream event; streams are not buffered.
        std::optional<std::string> type{};  ///< API error type.
        std::optional<std::string> code{};  ///< API error code.
        std::optional<std::string> param{}; ///< Invalid API parameter.
        std::string request_id;             ///< Response `x-request-id`.
        Headers headers;                    ///< Response headers, with lowercase names.

        /**
         * @brief Test for a bad request (400).
         * @return Whether this is that HTTP status error.
         */
        [[nodiscard]] auto IsBadRequest() const noexcept -> bool { return kind == ErrorKind::Status && status_code == 400; }

        /**
         * @brief Test for an authentication error (401).
         * @return Whether this is that HTTP status error.
         */
        [[nodiscard]] auto IsAuthentication() const noexcept -> bool { return kind == ErrorKind::Status && status_code == 401; }

        /**
         * @brief Test for permission denial (403).
         * @return Whether this is that HTTP status error.
         */
        [[nodiscard]] auto IsPermissionDenied() const noexcept -> bool { return kind == ErrorKind::Status && status_code == 403; }

        /**
         * @brief Test for a missing resource (404).
         * @return Whether this is that HTTP status error.
         */
        [[nodiscard]] auto IsNotFound() const noexcept -> bool { return kind == ErrorKind::Status && status_code == 404; }

        /**
         * @brief Test for a conflict (409).
         * @return Whether this is that HTTP status error.
         */
        [[nodiscard]] auto IsConflict() const noexcept -> bool { return kind == ErrorKind::Status && status_code == 409; }

        /**
         * @brief Test for an unprocessable entity (422).
         * @return Whether this is that HTTP status error.
         */
        [[nodiscard]] auto IsUnprocessableEntity() const noexcept -> bool { return kind == ErrorKind::Status && status_code == 422; }

        /**
         * @brief Test for rate limiting (429).
         * @return Whether this is that HTTP status error.
         */
        [[nodiscard]] auto IsRateLimit() const noexcept -> bool { return kind == ErrorKind::Status && status_code == 429; }

        /**
         * @brief Test for an upstream server error (500 and higher).
         * @return Whether this is a server HTTP status error.
         */
        [[nodiscard]] auto IsInternalServer() const noexcept -> bool { return kind == ErrorKind::Status && status_code >= 500; }
    };

    /**
     * @brief Value of a successful request, or the error that prevented it.
     * @tparam T Value type on success.
     */
    template <typename T>
    using Result = std::expected<T, Error>;

} // namespace drogon_openai
