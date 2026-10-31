/**
 * @file audio.hpp
 * @brief Request and response types for audio; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::audio {

    /**
     * @brief Response VoiceConsent, retaining SDK union fields as JSON.
     */
    struct VoiceConsent {
        std::string id{};          ///< Wire field id.
        std::int64_t created_at{}; ///< Wire field created_at.
        std::string language{};    ///< Wire field language.
        std::string name{};        ///< Wire field name.
        std::string object{};      ///< Wire field object.
        Json::Value raw{};         ///< Complete JSON including provider extensions.
        std::string _request_id;   ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<VoiceConsent>;
    };

    /**
     * @brief Response VoiceConsentDeleted, retaining SDK union fields as JSON.
     */
    struct VoiceConsentDeleted {
        std::string id{};        ///< Wire field id.
        bool deleted{};          ///< Wire field deleted.
        std::string object{};    ///< Wire field object.
        Json::Value raw{};       ///< Complete JSON including provider extensions.
        std::string _request_id; ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<VoiceConsentDeleted>;
    };

    /**
     * @brief Response Voice, retaining SDK union fields as JSON.
     */
    struct Voice {
        std::string id{};                    ///< Wire field id.
        std::int64_t created_at{};           ///< Wire field created_at.
        std::string name{};                  ///< Wire field name.
        std::string object{};                ///< Wire field object.
        std::string type{};                  ///< Wire field type.
        std::optional<Json::Value> prompt{}; ///< Wire field prompt.
        Json::Value raw{};                   ///< Complete JSON including provider extensions.
        std::string _request_id;             ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<Voice>;
    };

    /**
     * @brief Response VoiceDeleted, retaining SDK union fields as JSON.
     */
    struct VoiceDeleted {
        std::string id{};        ///< Wire field id.
        bool deleted{};          ///< Wire field deleted.
        std::string object{};    ///< Wire field object.
        Json::Value raw{};       ///< Complete JSON including provider extensions.
        std::string _request_id; ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<VoiceDeleted>;
    };

    /**
     * @brief Parameters for SpeechCreateParams; omitted optionals are absent on the wire.
     */
    struct SpeechCreateParams {
        std::string input{};                          ///< Required SDK field input.
        std::string model{};                          ///< Required SDK field model.
        Json::Value voice{};                          ///< Required SDK field voice.
        std::optional<std::string> instructions{};    ///< Optional SDK field instructions.
        std::optional<std::string> response_format{}; ///< Optional SDK field response_format.
        std::optional<double> speed{};                ///< Optional SDK field speed.
        std::optional<std::string> stream_format{};   ///< Optional SDK field stream_format.
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
     * @brief Parameters for AudioTranscriptionsCreateParams; omitted optionals are absent on the wire.
     */
    struct AudioTranscriptionsCreateParams {
        FileInput file{};                                                   ///< Required SDK field file.
        std::string model{};                                                ///< Required SDK field model.
        std::optional<Json::Value> chunking_strategy{};                     ///< Optional SDK field chunking_strategy.
        std::optional<Json::Value> include{};                               ///< Optional SDK field include.
        std::optional<std::vector<std::string>> keywords{};                 ///< Optional SDK field keywords.
        std::optional<std::vector<std::string>> known_speaker_names{};      ///< Optional SDK field known_speaker_names.
        std::optional<std::vector<std::string>> known_speaker_references{}; ///< Optional SDK field known_speaker_references.
        std::optional<std::string> language{};                              ///< Optional SDK field language.
        std::optional<std::vector<std::string>> languages{};                ///< Optional SDK field languages.
        std::optional<std::string> prompt{};                                ///< Optional SDK field prompt.
        std::optional<Json::Value> response_format{};                       ///< Optional SDK field response_format.
        std::optional<Json::Value> stream{};                                ///< Optional SDK field stream.
        std::optional<double> temperature{};                                ///< Optional SDK field temperature.
        std::optional<std::vector<std::string>> timestamp_granularities{};  ///< Optional SDK field timestamp_granularities.
        Json::Value extra_body{};                                           ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};                                   ///< Override phase timeouts.
        std::optional<int> max_retries{};                                   ///< Override retries.
        Headers extra_headers{};                                            ///< Override request headers.
        Json::Value extra_query{};                                          ///< Override query parameters.

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
     * @brief Parameters for TranslationCreateParams; omitted optionals are absent on the wire.
     */
    struct TranslationCreateParams {
        FileInput file{};                             ///< Required SDK field file.
        std::string model{};                          ///< Required SDK field model.
        std::optional<std::string> prompt{};          ///< Optional SDK field prompt.
        std::optional<std::string> response_format{}; ///< Optional SDK field response_format.
        std::optional<double> temperature{};          ///< Optional SDK field temperature.
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
     * @brief Parameters for VoiceConsentCreateParams; omitted optionals are absent on the wire.
     */
    struct VoiceConsentCreateParams {
        std::string language{};           ///< Required SDK field language.
        std::string name{};               ///< Required SDK field name.
        FileInput recording{};            ///< Required SDK field recording.
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
     * @brief Parameters for AudioVoiceConsentsRetrieveParams; omitted optionals are absent on the wire.
     */
    struct AudioVoiceConsentsRetrieveParams {
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
     * @brief Parameters for VoiceConsentUpdateParams; omitted optionals are absent on the wire.
     */
    struct VoiceConsentUpdateParams {
        std::string name{};               ///< Required SDK field name.
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
     * @brief Parameters for VoiceConsentListParams; omitted optionals are absent on the wire.
     */
    struct VoiceConsentListParams {
        std::optional<std::string> after{};  ///< Optional SDK field after.
        std::optional<std::int64_t> limit{}; ///< Optional SDK field limit.
        Json::Value extra_body{};            ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};    ///< Override phase timeouts.
        std::optional<int> max_retries{};    ///< Override retries.
        Headers extra_headers{};             ///< Override request headers.
        Json::Value extra_query{};           ///< Override query parameters.

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
     * @brief Parameters for AudioVoiceConsentsDeleteParams; omitted optionals are absent on the wire.
     */
    struct AudioVoiceConsentsDeleteParams {
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
     * @brief Parameters for VoiceCreateParams; omitted optionals are absent on the wire.
     */
    struct VoiceCreateParams {
        std::optional<FileInput> audio_sample{};  ///< Optional SDK field audio_sample.
        std::optional<std::string> consent{};     ///< Optional SDK field consent.
        std::string name{};                       ///< Required SDK field name.
        std::optional<std::string> type{};        ///< Optional SDK field type.
        std::optional<std::string> prompt{};      ///< Optional SDK field prompt.
        std::optional<std::string> model{};       ///< Optional SDK field model.
        std::optional<std::string> script_hint{}; ///< Optional SDK field script_hint.
        Json::Value extra_body{};                 ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};         ///< Override phase timeouts.
        std::optional<int> max_retries{};         ///< Override retries.
        Headers extra_headers{};                  ///< Override request headers.
        Json::Value extra_query{};                ///< Override query parameters.

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
     * @brief Parameters for AudioVoicesDeleteParams; omitted optionals are absent on the wire.
     */
    struct AudioVoicesDeleteParams {
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
} // namespace drogon_openai::types::audio
