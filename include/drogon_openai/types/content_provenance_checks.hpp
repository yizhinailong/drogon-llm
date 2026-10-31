/**
 * @file content_provenance_checks.hpp
 * @brief Request and response types for content_provenance_checks; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::content_provenance_checks {

    /**
     * @brief Response ContentProvenanceCheck, retaining SDK union fields as JSON.
     */
    struct ContentProvenanceCheck {
        std::int64_t created_at{}; ///< Wire field created_at.
        std::string object{};      ///< Wire field object.
        Json::Value results{};     ///< Wire field results.
        Json::Value raw{};         ///< Complete JSON including provider extensions.
        std::string _request_id;   ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<ContentProvenanceCheck>;
    };

    /**
     * @brief Parameters for ContentProvenanceCheckCreateParams; omitted optionals are absent on the wire.
     */
    struct ContentProvenanceCheckCreateParams {
        FileInput file{};                 ///< Required SDK field file.
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };
} // namespace drogon_openai::types::content_provenance_checks
