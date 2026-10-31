/**
 * @file completions.hpp
 * @brief Request and response types for completions; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::completions {

    /**
     * @brief Response Completion, retaining SDK union fields as JSON.
     */
    struct Completion {
        std::string id{};                                ///< Wire field id.
        Json::Value choices{};                           ///< Wire field choices.
        std::int64_t created{};                          ///< Wire field created.
        std::string model{};                             ///< Wire field model.
        std::string object{};                            ///< Wire field object.
        std::optional<Json::Value> system_fingerprint{}; ///< Wire field system_fingerprint.
        std::optional<Json::Value> usage{};              ///< Wire field usage.
        Json::Value raw{};                               ///< Complete JSON including provider extensions.
        std::string _request_id;                         ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<Completion>;
    };

    /**
     * @brief Parameters for CompletionsCreateParams; omitted optionals are absent on the wire.
     */
    struct CompletionsCreateParams {
        std::string model{};                            ///< Required SDK field model.
        Json::Value prompt{};                           ///< Required SDK field prompt.
        std::optional<Json::Value> best_of{};           ///< Optional SDK field best_of.
        std::optional<Json::Value> echo{};              ///< Optional SDK field echo.
        std::optional<Json::Value> frequency_penalty{}; ///< Optional SDK field frequency_penalty.
        std::optional<Json::Value> logit_bias{};        ///< Optional SDK field logit_bias.
        std::optional<Json::Value> logprobs{};          ///< Optional SDK field logprobs.
        std::optional<Json::Value> max_tokens{};        ///< Optional SDK field max_tokens.
        std::optional<Json::Value> n{};                 ///< Optional SDK field n.
        std::optional<Json::Value> presence_penalty{};  ///< Optional SDK field presence_penalty.
        std::optional<Json::Value> seed{};              ///< Optional SDK field seed.
        std::optional<Json::Value> stop{};              ///< Optional SDK field stop.
        std::optional<Json::Value> stream{};            ///< Optional SDK field stream.
        std::optional<Json::Value> stream_options{};    ///< Optional SDK field stream_options.
        std::optional<Json::Value> suffix{};            ///< Optional SDK field suffix.
        std::optional<Json::Value> temperature{};       ///< Optional SDK field temperature.
        std::optional<Json::Value> top_p{};             ///< Optional SDK field top_p.
        std::optional<std::string> user{};              ///< Optional SDK field user.
        Json::Value extra_body{};                       ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};               ///< Override phase timeouts.
        std::optional<int> max_retries{};               ///< Override retries.
        Headers extra_headers{};                        ///< Override request headers.
        Json::Value extra_query{};                      ///< Override query parameters.

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
} // namespace drogon_openai::types::completions
