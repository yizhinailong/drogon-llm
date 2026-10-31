/**
 * @file realtime.hpp
 * @brief Request and response types for realtime; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::realtime {

    /**
     * @brief Response ClientSecretCreateResponse, retaining SDK union fields as JSON.
     */
    struct ClientSecretCreateResponse {
        std::int64_t expires_at{}; ///< Wire field expires_at.
        Json::Value session{};     ///< Wire field session.
        std::string value{};       ///< Wire field value.
        Json::Value raw{};         ///< Complete JSON including provider extensions.
        std::string _request_id;   ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<ClientSecretCreateResponse>;
    };

    /**
     * @brief Response RealtimeTranslationClientSecretCreateResponse, retaining SDK union fields as JSON.
     */
    struct RealtimeTranslationClientSecretCreateResponse {
        std::int64_t expires_at{}; ///< Wire field expires_at.
        Json::Value session{};     ///< Wire field session.
        std::string value{};       ///< Wire field value.
        Json::Value raw{};         ///< Complete JSON including provider extensions.
        std::string _request_id;   ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<RealtimeTranslationClientSecretCreateResponse>;
    };

    /**
     * @brief Parameters for CallCreateParams; omitted optionals are absent on the wire.
     */
    struct CallCreateParams {
        std::string sdp{};                    ///< Required SDK field sdp.
        std::optional<Json::Value> session{}; ///< Optional SDK field session.
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
     * @brief Parameters for CallAcceptParams; omitted optionals are absent on the wire.
     */
    struct CallAcceptParams {
        std::string type{};                                          ///< Required SDK field type.
        std::optional<Json::Value> audio{};                          ///< Optional SDK field audio.
        std::optional<std::vector<std::string>> include{};           ///< Optional SDK field include.
        std::optional<std::string> instructions{};                   ///< Optional SDK field instructions.
        std::optional<Json::Value> max_output_tokens{};              ///< Optional SDK field max_output_tokens.
        std::optional<std::string> model{};                          ///< Optional SDK field model.
        std::optional<std::vector<std::string>> output_modalities{}; ///< Optional SDK field output_modalities.
        std::optional<bool> parallel_tool_calls{};                   ///< Optional SDK field parallel_tool_calls.
        std::optional<Json::Value> prompt{};                         ///< Optional SDK field prompt.
        std::optional<Json::Value> reasoning{};                      ///< Optional SDK field reasoning.
        std::optional<Json::Value> tool_choice{};                    ///< Optional SDK field tool_choice.
        std::optional<Json::Value> tools{};                          ///< Optional SDK field tools.
        std::optional<Json::Value> tracing{};                        ///< Optional SDK field tracing.
        std::optional<Json::Value> truncation{};                     ///< Optional SDK field truncation.
        Json::Value extra_body{};                                    ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};                            ///< Override phase timeouts.
        std::optional<int> max_retries{};                            ///< Override retries.
        Headers extra_headers{};                                     ///< Override request headers.
        Json::Value extra_query{};                                   ///< Override query parameters.

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
     * @brief Parameters for RealtimeCallsHangupParams; omitted optionals are absent on the wire.
     */
    struct RealtimeCallsHangupParams {
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
     * @brief Parameters for CallReferParams; omitted optionals are absent on the wire.
     */
    struct CallReferParams {
        std::string target_uri{};         ///< Required SDK field target_uri.
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
     * @brief Parameters for CallRejectParams; omitted optionals are absent on the wire.
     */
    struct CallRejectParams {
        std::optional<std::int64_t> status_code{}; ///< Optional SDK field status_code.
        Json::Value extra_body{};                  ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};          ///< Override phase timeouts.
        std::optional<int> max_retries{};          ///< Override retries.
        Headers extra_headers{};                   ///< Override request headers.
        Json::Value extra_query{};                 ///< Override query parameters.

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
     * @brief Parameters for ClientSecretCreateParams; omitted optionals are absent on the wire.
     */
    struct ClientSecretCreateParams {
        std::optional<Json::Value> expires_after{}; ///< Optional SDK field expires_after.
        std::optional<Json::Value> session{};       ///< Optional SDK field session.
        Json::Value extra_body{};                   ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};           ///< Override phase timeouts.
        std::optional<int> max_retries{};           ///< Override retries.
        Headers extra_headers{};                    ///< Override request headers.
        Json::Value extra_query{};                  ///< Override query parameters.

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
     * @brief Parameters for RealtimeTranslationsClientSecretsCreateParams; omitted optionals are absent on the wire.
     */
    struct RealtimeTranslationsClientSecretsCreateParams {
        Json::Value session{};                      ///< Required SDK field session.
        std::optional<Json::Value> expires_after{}; ///< Optional SDK field expires_after.
        Json::Value extra_body{};                   ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};           ///< Override phase timeouts.
        std::optional<int> max_retries{};           ///< Override retries.
        Headers extra_headers{};                    ///< Override request headers.
        Json::Value extra_query{};                  ///< Override query parameters.

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
} // namespace drogon_openai::types::realtime
