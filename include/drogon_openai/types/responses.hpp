/**
 * @file responses.hpp
 * @brief Request and response types for responses; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::responses {

    /**
     * @brief Response InputTokenCountResponse, retaining SDK union fields as JSON.
     */
    struct InputTokenCountResponse {
        std::int64_t input_tokens{}; ///< Wire field input_tokens.
        std::string object{};        ///< Wire field object.
        Json::Value raw{};           ///< Complete JSON including provider extensions.
        std::string _request_id;     ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<InputTokenCountResponse>;
    };

    /**
     * @brief Response Response, retaining SDK union fields as JSON.
     */
    struct Response {
        std::string id{};                                      ///< Wire field id.
        std::optional<Json::Value> access_programs{};          ///< Wire field access_programs.
        double created_at{};                                   ///< Wire field created_at.
        std::optional<Json::Value> error{};                    ///< Wire field error.
        std::optional<Json::Value> incomplete_details{};       ///< Wire field incomplete_details.
        std::optional<Json::Value> instructions{};             ///< Wire field instructions.
        std::optional<Json::Value> metadata{};                 ///< Wire field metadata.
        std::string model{};                                   ///< Wire field model.
        std::string object{};                                  ///< Wire field object.
        Json::Value output{};                                  ///< Wire field output.
        bool parallel_tool_calls{};                            ///< Wire field parallel_tool_calls.
        std::optional<Json::Value> temperature{};              ///< Wire field temperature.
        Json::Value tool_choice{};                             ///< Wire field tool_choice.
        Json::Value tools{};                                   ///< Wire field tools.
        std::optional<Json::Value> top_p{};                    ///< Wire field top_p.
        std::optional<Json::Value> background{};               ///< Wire field background.
        std::optional<Json::Value> completed_at{};             ///< Wire field completed_at.
        std::optional<Json::Value> conversation{};             ///< Wire field conversation.
        std::optional<Json::Value> max_output_tokens{};        ///< Wire field max_output_tokens.
        std::optional<Json::Value> max_tool_calls{};           ///< Wire field max_tool_calls.
        std::optional<Json::Value> moderation{};               ///< Wire field moderation.
        std::optional<Json::Value> previous_response_id{};     ///< Wire field previous_response_id.
        std::optional<Json::Value> prompt{};                   ///< Wire field prompt.
        std::optional<Json::Value> prompt_cache_diagnostics{}; ///< Wire field prompt_cache_diagnostics.
        std::optional<Json::Value> prompt_cache_key{};         ///< Wire field prompt_cache_key.
        std::optional<Json::Value> prompt_cache_options{};     ///< Wire field prompt_cache_options.
        std::optional<Json::Value> prompt_cache_retention{};   ///< Wire field prompt_cache_retention.
        std::optional<Json::Value> reasoning{};                ///< Wire field reasoning.
        std::optional<Json::Value> safety_identifier{};        ///< Wire field safety_identifier.
        std::optional<Json::Value> service_tier{};             ///< Wire field service_tier.
        std::optional<Json::Value> status{};                   ///< Wire field status.
        std::optional<Json::Value> text{};                     ///< Wire field text.
        std::optional<Json::Value> tool_usage{};               ///< Wire field tool_usage.
        std::optional<Json::Value> top_logprobs{};             ///< Wire field top_logprobs.
        std::optional<Json::Value> truncation{};               ///< Wire field truncation.
        std::optional<Json::Value> usage{};                    ///< Wire field usage.
        std::optional<Json::Value> user{};                     ///< Wire field user.
        std::string output_text;                               ///< Concatenated output_text content.
        std::optional<Json::Value> output_parsed{};            ///< First structured final answer decoded by Parse.
        Json::Value raw{};                                     ///< Complete JSON including provider extensions.
        std::string _request_id;                               ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<Response>;
    };

    /**
     * @brief Response CompactedResponse, retaining SDK union fields as JSON.
     */
    struct CompactedResponse {
        std::string id{};          ///< Wire field id.
        std::int64_t created_at{}; ///< Wire field created_at.
        std::string object{};      ///< Wire field object.
        Json::Value output{};      ///< Wire field output.
        Json::Value usage{};       ///< Wire field usage.
        Json::Value raw{};         ///< Complete JSON including provider extensions.
        std::string _request_id;   ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<CompactedResponse>;
    };

    /**
     * @brief Parameters for InputItemListParams; omitted optionals are absent on the wire.
     */
    struct InputItemListParams {
        std::optional<std::string> after{};   ///< Optional SDK field after.
        std::optional<Json::Value> include{}; ///< Optional SDK field include.
        std::optional<std::int64_t> limit{};  ///< Optional SDK field limit.
        std::optional<std::string> order{};   ///< Optional SDK field order.
        Json::Value extra_body{};             ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};     ///< Override phase timeouts.
        std::optional<int> max_retries{};     ///< Override retries.
        Headers extra_headers{};              ///< Override request headers.
        Json::Value extra_query{};            ///< Override query parameters.

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

    /**
     * @brief Parameters for InputTokenCountParams; omitted optionals are absent on the wire.
     */
    struct InputTokenCountParams {
        std::optional<Json::Value> conversation{};         ///< Optional SDK field conversation.
        std::optional<Json::Value> input{};                ///< Optional SDK field input.
        std::optional<Json::Value> instructions{};         ///< Optional SDK field instructions.
        std::optional<Json::Value> model{};                ///< Optional SDK field model.
        std::optional<Json::Value> parallel_tool_calls{};  ///< Optional SDK field parallel_tool_calls.
        std::optional<std::string> personality{};          ///< Optional SDK field personality.
        std::optional<Json::Value> previous_response_id{}; ///< Optional SDK field previous_response_id.
        std::optional<Json::Value> reasoning{};            ///< Optional SDK field reasoning.
        std::optional<Json::Value> text{};                 ///< Optional SDK field text.
        std::optional<Json::Value> tool_choice{};          ///< Optional SDK field tool_choice.
        std::optional<Json::Value> tools{};                ///< Optional SDK field tools.
        std::optional<std::string> truncation{};           ///< Optional SDK field truncation.
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

    /**
     * @brief Parameters for ResponsesCreateParams; omitted optionals are absent on the wire.
     */
    struct ResponsesCreateParams {
        std::optional<Json::Value> access_programs{};        ///< Optional SDK field access_programs.
        std::optional<Json::Value> background{};             ///< Optional SDK field background.
        std::optional<Json::Value> context_management{};     ///< Optional SDK field context_management.
        std::optional<Json::Value> conversation{};           ///< Optional SDK field conversation.
        std::optional<Json::Value> include{};                ///< Optional SDK field include.
        std::optional<Json::Value> input{};                  ///< Optional SDK field input.
        std::optional<Json::Value> instructions{};           ///< Optional SDK field instructions.
        std::optional<Json::Value> max_output_tokens{};      ///< Optional SDK field max_output_tokens.
        std::optional<Json::Value> max_tool_calls{};         ///< Optional SDK field max_tool_calls.
        std::optional<Json::Value> metadata{};               ///< Optional SDK field metadata.
        std::optional<std::string> model{};                  ///< Optional SDK field model.
        std::optional<Json::Value> moderation{};             ///< Optional SDK field moderation.
        std::optional<Json::Value> parallel_tool_calls{};    ///< Optional SDK field parallel_tool_calls.
        std::optional<Json::Value> previous_response_id{};   ///< Optional SDK field previous_response_id.
        std::optional<Json::Value> prompt{};                 ///< Optional SDK field prompt.
        std::optional<Json::Value> prompt_cache_key{};       ///< Optional SDK field prompt_cache_key.
        std::optional<Json::Value> prompt_cache_options{};   ///< Optional SDK field prompt_cache_options.
        std::optional<Json::Value> prompt_cache_retention{}; ///< Optional SDK field prompt_cache_retention.
        std::optional<Json::Value> reasoning{};              ///< Optional SDK field reasoning.
        std::optional<Json::Value> safety_identifier{};      ///< Optional SDK field safety_identifier.
        std::optional<Json::Value> service_tier{};           ///< Optional SDK field service_tier.
        std::optional<Json::Value> store{};                  ///< Optional SDK field store.
        std::optional<Json::Value> stream{};                 ///< Optional SDK field stream.
        std::optional<Json::Value> stream_options{};         ///< Optional SDK field stream_options.
        std::optional<Json::Value> temperature{};            ///< Optional SDK field temperature.
        std::optional<Json::Value> text{};                   ///< Optional SDK field text.
        std::optional<Json::Value> tool_choice{};            ///< Optional SDK field tool_choice.
        std::optional<Json::Value> tools{};                  ///< Optional SDK field tools.
        std::optional<Json::Value> top_logprobs{};           ///< Optional SDK field top_logprobs.
        std::optional<Json::Value> top_p{};                  ///< Optional SDK field top_p.
        std::optional<Json::Value> truncation{};             ///< Optional SDK field truncation.
        std::optional<std::string> user{};                   ///< Optional SDK field user.
        Json::Value extra_body{};                            ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};                    ///< Override phase timeouts.
        std::optional<int> max_retries{};                    ///< Override retries.
        Headers extra_headers{};                             ///< Override request headers.
        Json::Value extra_query{};                           ///< Override query parameters.

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

    /**
     * @brief Parameters for ResponseRetrieveParams; omitted optionals are absent on the wire.
     */
    struct ResponseRetrieveParams {
        std::optional<Json::Value> include{};         ///< Optional SDK field include.
        std::optional<bool> include_obfuscation{};    ///< Optional SDK field include_obfuscation.
        std::optional<std::int64_t> starting_after{}; ///< Optional SDK field starting_after.
        std::optional<Json::Value> stream{};          ///< Optional SDK field stream.
        Json::Value extra_body{};                     ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};             ///< Override phase timeouts.
        std::optional<int> max_retries{};             ///< Override retries.
        Headers extra_headers{};                      ///< Override request headers.
        Json::Value extra_query{};                    ///< Override query parameters.

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

    /**
     * @brief Parameters for ResponsesDeleteParams; omitted optionals are absent on the wire.
     */
    struct ResponsesDeleteParams {
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

    /**
     * @brief Parameters for ResponsesCancelParams; omitted optionals are absent on the wire.
     */
    struct ResponsesCancelParams {
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

    /**
     * @brief Parameters for ResponseCompactParams; omitted optionals are absent on the wire.
     */
    struct ResponseCompactParams {
        Json::Value model{};                                 ///< Required SDK field model.
        std::optional<Json::Value> input{};                  ///< Optional SDK field input.
        std::optional<Json::Value> instructions{};           ///< Optional SDK field instructions.
        std::optional<Json::Value> previous_response_id{};   ///< Optional SDK field previous_response_id.
        std::optional<Json::Value> prompt_cache_key{};       ///< Optional SDK field prompt_cache_key.
        std::optional<Json::Value> prompt_cache_options{};   ///< Optional SDK field prompt_cache_options.
        std::optional<Json::Value> prompt_cache_retention{}; ///< Optional SDK field prompt_cache_retention.
        std::optional<Json::Value> service_tier{};           ///< Optional SDK field service_tier.
        Json::Value extra_body{};                            ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};                    ///< Override phase timeouts.
        std::optional<int> max_retries{};                    ///< Override retries.
        Headers extra_headers{};                             ///< Override request headers.
        Json::Value extra_query{};                           ///< Override query parameters.

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
} // namespace drogon_openai::types::responses
