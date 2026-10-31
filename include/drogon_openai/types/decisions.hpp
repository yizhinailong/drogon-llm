/**
 * @file decisions.hpp
 * @brief Request and response types for decisions; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::decisions {

    /**
     * @brief Response Decision, retaining SDK union fields as JSON.
     */
    struct Decision {
        Json::Value answers{};   ///< Wire field answers.
        std::string model{};     ///< Wire field model.
        Json::Value usage{};     ///< Wire field usage.
        Json::Value raw{};       ///< Complete JSON including provider extensions.
        std::string _request_id; ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<Decision>;
    };

    /**
     * @brief Parameters for DecisionCreateParams; omitted optionals are absent on the wire.
     */
    struct DecisionCreateParams {
        Json::Value input{};                               ///< Required SDK field input.
        std::string model{};                               ///< Required SDK field model.
        Json::Value questions{};                           ///< Required SDK field questions.
        std::optional<Json::Value> prompt_cache_key{};     ///< Optional SDK field prompt_cache_key.
        std::optional<Json::Value> prompt_cache_options{}; ///< Optional SDK field prompt_cache_options.
        std::optional<Json::Value> safety_identifier{};    ///< Optional SDK field safety_identifier.
        Json::Value extra_body{};                          ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};                  ///< Override phase timeouts.
        std::optional<int> max_retries{};                  ///< Override retries.
        Headers extra_headers{};                           ///< Override request headers.
        Json::Value extra_query{};                         ///< Override query parameters.

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
} // namespace drogon_openai::types::decisions
