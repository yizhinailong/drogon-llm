/**
 * @file moderations.hpp
 * @brief Request and response types for moderations; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::moderations {

    /**
     * @brief Response ModerationCreateResponse, retaining SDK union fields as JSON.
     */
    struct ModerationCreateResponse {
        std::string id{};        ///< Wire field id.
        std::string model{};     ///< Wire field model.
        Json::Value results{};   ///< Wire field results.
        Json::Value raw{};       ///< Complete JSON including provider extensions.
        std::string _request_id; ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<ModerationCreateResponse>;
    };

    /**
     * @brief Parameters for ModerationCreateParams; omitted optionals are absent on the wire.
     */
    struct ModerationCreateParams {
        Json::Value input{};                ///< Required SDK field input.
        std::optional<std::string> model{}; ///< Optional SDK field model.
        Json::Value extra_body{};           ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};   ///< Override phase timeouts.
        std::optional<int> max_retries{};   ///< Override retries.
        Headers extra_headers{};            ///< Override request headers.
        Json::Value extra_query{};          ///< Override query parameters.

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
} // namespace drogon_openai::types::moderations
