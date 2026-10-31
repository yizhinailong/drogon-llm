/**
 * @file audio.hpp
 * @brief audio API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/audio.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK audio.speech resource.
     */
    class AudioSpeech {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit AudioSpeech(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /audio/speech.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::audio::SpeechCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<std::string>>>;
    };

    /**
     * @brief Value handle for the SDK audio.transcriptions resource.
     */
    class AudioTranscriptions {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit AudioTranscriptions(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /audio/transcriptions.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::audio::AudioTranscriptionsCreateParams params = {}
        ) const -> drogon::Task<Result<Json::Value>>;

        /**
         * @brief POST /audio/transcriptions.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::audio::AudioTranscriptionsCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;

        /**
         * @brief Stream SSE from POST /audio/transcriptions.
         * @param params Owned SDK parameters.
         * @param callback Event consumer; false stops delivery.
         * @return Final event and stream metadata or an error.
         */
        [[nodiscard]] auto CreateStream(
            types::audio::AudioTranscriptionsCreateParams params,
            ApiEventCallback callback = {}
        ) const -> drogon::Task<Result<ApiStreamResult>>;
    };

    /**
     * @brief Value handle for the SDK audio.translations resource.
     */
    class AudioTranslations {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit AudioTranslations(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /audio/translations.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::audio::TranslationCreateParams params = {}
        ) const -> drogon::Task<Result<Json::Value>>;

        /**
         * @brief POST /audio/translations.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::audio::TranslationCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;
    };

    /**
     * @brief Value handle for the SDK audio.voice_consents resource.
     */
    class AudioVoiceConsents {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit AudioVoiceConsents(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /audio/voice_consents.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::audio::VoiceConsentCreateParams params = {}
        ) const -> drogon::Task<Result<types::audio::VoiceConsent>>;

        /**
         * @brief POST /audio/voice_consents.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::audio::VoiceConsentCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::audio::VoiceConsent>>>;

        /**
         * @brief GET /audio/voice_consents/{consent_id}.
         * @param consent_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string consent_id,
            types::audio::AudioVoiceConsentsRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::audio::VoiceConsent>>;

        /**
         * @brief GET /audio/voice_consents/{consent_id}.
         * @param consent_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string consent_id,
            types::audio::AudioVoiceConsentsRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::audio::VoiceConsent>>>;

        /**
         * @brief POST /audio/voice_consents/{consent_id}.
         * @param consent_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Update(
            std::string consent_id,
            types::audio::VoiceConsentUpdateParams params = {}
        ) const -> drogon::Task<Result<types::audio::VoiceConsent>>;

        /**
         * @brief POST /audio/voice_consents/{consent_id}.
         * @param consent_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto UpdateRaw(
            std::string consent_id,
            types::audio::VoiceConsentUpdateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::audio::VoiceConsent>>>;

        /**
         * @brief GET /audio/voice_consents.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            types::audio::VoiceConsentListParams params = {}
        ) const -> drogon::Task<Result<Page<types::audio::VoiceConsent>>>;

        /**
         * @brief DELETE /audio/voice_consents/{consent_id}.
         * @param consent_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string consent_id,
            types::audio::AudioVoiceConsentsDeleteParams params = {}
        ) const -> drogon::Task<Result<types::audio::VoiceConsentDeleted>>;

        /**
         * @brief DELETE /audio/voice_consents/{consent_id}.
         * @param consent_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string consent_id,
            types::audio::AudioVoiceConsentsDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::audio::VoiceConsentDeleted>>>;
    };

    /**
     * @brief Value handle for the SDK audio.voices resource.
     */
    class AudioVoices {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit AudioVoices(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /audio/voices.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::audio::VoiceCreateParams params = {}
        ) const -> drogon::Task<Result<types::audio::Voice>>;

        /**
         * @brief POST /audio/voices.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::audio::VoiceCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::audio::Voice>>>;

        /**
         * @brief DELETE /audio/voices/{voice_id}.
         * @param voice_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string voice_id,
            types::audio::AudioVoicesDeleteParams params = {}
        ) const -> drogon::Task<Result<types::audio::VoiceDeleted>>;

        /**
         * @brief DELETE /audio/voices/{voice_id}.
         * @param voice_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string voice_id,
            types::audio::AudioVoicesDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::audio::VoiceDeleted>>>;
    };

    /**
     * @brief Value handle for the SDK audio resource.
     */
    class Audio {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Audio(std::shared_ptr<Transport> transport);
        AudioTranscriptions transcriptions; ///< Nested SDK resource transcriptions.
        AudioTranslations translations;     ///< Nested SDK resource translations.
        AudioSpeech speech;                 ///< Nested SDK resource speech.
        AudioVoices voices;                 ///< Nested SDK resource voices.
        AudioVoiceConsents voice_consents;  ///< Nested SDK resource voice_consents.
    };
} // namespace drogon_openai::details
