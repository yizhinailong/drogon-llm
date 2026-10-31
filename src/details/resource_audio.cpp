/**
 * @file resource_audio.cpp
 * @brief Implement audio resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/audio.hpp"

namespace drogon_openai::types::audio {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto VoiceConsent::FromJson(Json::Value const& value) -> Result<VoiceConsent> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        VoiceConsent result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VoiceConsent.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VoiceConsent.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("language")) {
            if (!details::resource_read(value["language"], result.language)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VoiceConsent.language", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VoiceConsent.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VoiceConsent.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto VoiceConsentDeleted::FromJson(Json::Value const& value) -> Result<VoiceConsentDeleted> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        VoiceConsentDeleted result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VoiceConsentDeleted.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("deleted")) {
            if (!details::resource_read(value["deleted"], result.deleted)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VoiceConsentDeleted.deleted", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VoiceConsentDeleted.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto Voice::FromJson(Json::Value const& value) -> Result<Voice> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        Voice result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Voice.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Voice.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Voice.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Voice.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("type")) {
            if (!details::resource_read(value["type"], result.type)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Voice.type", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("prompt") && !value["prompt"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["prompt"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Voice.prompt", .body = value.toStyledString() } };
            }
            result.prompt = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto VoiceDeleted::FromJson(Json::Value const& value) -> Result<VoiceDeleted> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        VoiceDeleted result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VoiceDeleted.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("deleted")) {
            if (!details::resource_read(value["deleted"], result.deleted)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VoiceDeleted.deleted", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid VoiceDeleted.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto SpeechCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["input"] = details::resource_json(input);
        payload.body["model"] = details::resource_json(model);
        payload.body["voice"] = details::resource_json(voice);
        if (instructions) {
            payload.body["instructions"] = details::resource_json(*instructions);
        }
        if (response_format) {
            payload.body["response_format"] = details::resource_json(*response_format);
        }
        if (speed) {
            payload.body["speed"] = details::resource_json(*speed);
        }
        if (stream_format) {
            payload.body["stream_format"] = details::resource_json(*stream_format);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto SpeechCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto AudioTranscriptionsCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        details::resource_file(payload, "file", file);
        payload.body["model"] = details::resource_json(model);
        if (chunking_strategy) {
            payload.body["chunking_strategy"] = details::resource_json(*chunking_strategy);
        }
        if (include) {
            payload.body["include"] = details::resource_json(*include);
        }
        if (keywords) {
            payload.body["keywords"] = details::resource_json(*keywords);
        }
        if (known_speaker_names) {
            payload.body["known_speaker_names"] = details::resource_json(*known_speaker_names);
        }
        if (known_speaker_references) {
            payload.body["known_speaker_references"] = details::resource_json(*known_speaker_references);
        }
        if (language) {
            payload.body["language"] = details::resource_json(*language);
        }
        if (languages) {
            payload.body["languages"] = details::resource_json(*languages);
        }
        if (prompt) {
            payload.body["prompt"] = details::resource_json(*prompt);
        }
        if (response_format) {
            payload.body["response_format"] = details::resource_json(*response_format);
        }
        if (stream) {
            payload.body["stream"] = details::resource_json(*stream);
        }
        if (temperature) {
            payload.body["temperature"] = details::resource_json(*temperature);
        }
        if (timestamp_granularities) {
            payload.body["timestamp_granularities"] = details::resource_json(*timestamp_granularities);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto AudioTranscriptionsCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto TranslationCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        details::resource_file(payload, "file", file);
        payload.body["model"] = details::resource_json(model);
        if (prompt) {
            payload.body["prompt"] = details::resource_json(*prompt);
        }
        if (response_format) {
            payload.body["response_format"] = details::resource_json(*response_format);
        }
        if (temperature) {
            payload.body["temperature"] = details::resource_json(*temperature);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto TranslationCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VoiceConsentCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["language"] = details::resource_json(language);
        payload.body["name"] = details::resource_json(name);
        details::resource_file(payload, "recording", recording);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VoiceConsentCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto AudioVoiceConsentsRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto AudioVoiceConsentsRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VoiceConsentUpdateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["name"] = details::resource_json(name);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VoiceConsentUpdateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VoiceConsentListParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (after) {
            payload.query["after"] = details::resource_json(*after);
        }
        if (limit) {
            payload.query["limit"] = details::resource_json(*limit);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VoiceConsentListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto AudioVoiceConsentsDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto AudioVoiceConsentsDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto VoiceCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (audio_sample) {
            details::resource_file(payload, "audio_sample", *audio_sample);
        }
        if (consent) {
            payload.body["consent"] = details::resource_json(*consent);
        }
        payload.body["name"] = details::resource_json(name);
        if (type) {
            payload.body["type"] = details::resource_json(*type);
        }
        if (prompt) {
            payload.body["prompt"] = details::resource_json(*prompt);
        }
        if (model) {
            payload.body["model"] = details::resource_json(*model);
        }
        if (script_hint) {
            payload.body["script_hint"] = details::resource_json(*script_hint);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto VoiceCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto AudioVoicesDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto AudioVoicesDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::audio

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    AudioSpeech::AudioSpeech(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /audio/speech.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioSpeech::Create(
        types::audio::SpeechCreateParams params
    ) const -> drogon::Task<Result<RawResponse<std::string>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/audio/speech", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_bytes(transport, "POST", std::move(*path), std::move(payload), params.Options(), { { "Accept", "application/octet-stream" } }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        co_return std::move(*response);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    AudioTranscriptions::AudioTranscriptions(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /audio/transcriptions.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioTranscriptions::Create(
        types::audio::AudioTranscriptionsCreateParams params
    ) const -> drogon::Task<Result<Json::Value>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/audio/transcriptions", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        if ((payload.extra_body.isObject() ? payload.extra_body.get("stream", payload.body.get("stream", false)) : payload.body.get("stream", false)) == true) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "use CreateStream for SSE delivery" } };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<Json::Value>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /audio/transcriptions.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioTranscriptions::CreateRaw(
        types::audio::AudioTranscriptionsCreateParams params
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/audio/transcriptions", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        if ((payload.extra_body.isObject() ? payload.extra_body.get("stream", payload.body.get("stream", false)) : payload.body.get("stream", false)) == true) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "use CreateStream for SSE delivery" } };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<Json::Value>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Stream SSE from POST /audio/transcriptions.
     * @param params Owned SDK parameters.
     * @param callback Event consumer; false stops delivery.
     * @return Final event and stream metadata or an error.
     */
    auto AudioTranscriptions::CreateStream(
        types::audio::AudioTranscriptionsCreateParams params,
        ApiEventCallback callback
    ) const -> drogon::Task<Result<ApiStreamResult>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.body["stream"] = true;
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/audio/transcriptions", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_stream(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}, std::move(callback));
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    AudioTranslations::AudioTranslations(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /audio/translations.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioTranslations::Create(
        types::audio::TranslationCreateParams params
    ) const -> drogon::Task<Result<Json::Value>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/audio/translations", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<Json::Value>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /audio/translations.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioTranslations::CreateRaw(
        types::audio::TranslationCreateParams params
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/audio/translations", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<Json::Value>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    AudioVoiceConsents::AudioVoiceConsents(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /audio/voice_consents.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioVoiceConsents::Create(
        types::audio::VoiceConsentCreateParams params
    ) const -> drogon::Task<Result<types::audio::VoiceConsent>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/audio/voice_consents", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::audio::VoiceConsent>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /audio/voice_consents.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioVoiceConsents::CreateRaw(
        types::audio::VoiceConsentCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::audio::VoiceConsent>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/audio/voice_consents", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::audio::VoiceConsent>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /audio/voice_consents/{consent_id}.
     * @param consent_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioVoiceConsents::Retrieve(
        std::string consent_id,
        types::audio::AudioVoiceConsentsRetrieveParams params
    ) const -> drogon::Task<Result<types::audio::VoiceConsent>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/audio/voice_consents/{consent_id}", { { "consent_id", consent_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::audio::VoiceConsent>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /audio/voice_consents/{consent_id}.
     * @param consent_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioVoiceConsents::RetrieveRaw(
        std::string consent_id,
        types::audio::AudioVoiceConsentsRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::audio::VoiceConsent>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/audio/voice_consents/{consent_id}", { { "consent_id", consent_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::audio::VoiceConsent>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /audio/voice_consents/{consent_id}.
     * @param consent_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioVoiceConsents::Update(
        std::string consent_id,
        types::audio::VoiceConsentUpdateParams params
    ) const -> drogon::Task<Result<types::audio::VoiceConsent>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/audio/voice_consents/{consent_id}", { { "consent_id", consent_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::audio::VoiceConsent>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /audio/voice_consents/{consent_id}.
     * @param consent_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioVoiceConsents::UpdateRaw(
        std::string consent_id,
        types::audio::VoiceConsentUpdateParams params
    ) const -> drogon::Task<Result<RawResponse<types::audio::VoiceConsent>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/audio/voice_consents/{consent_id}", { { "consent_id", consent_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::audio::VoiceConsent>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /audio/voice_consents.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioVoiceConsents::List(
        types::audio::VoiceConsentListParams params
    ) const -> drogon::Task<Result<Page<types::audio::VoiceConsent>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/audio/voice_consents", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::audio::VoiceConsent>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true);
    }

    /**
     * @brief DELETE /audio/voice_consents/{consent_id}.
     * @param consent_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioVoiceConsents::Delete(
        std::string consent_id,
        types::audio::AudioVoiceConsentsDeleteParams params
    ) const -> drogon::Task<Result<types::audio::VoiceConsentDeleted>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/audio/voice_consents/{consent_id}", { { "consent_id", consent_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::audio::VoiceConsentDeleted>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief DELETE /audio/voice_consents/{consent_id}.
     * @param consent_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioVoiceConsents::DeleteRaw(
        std::string consent_id,
        types::audio::AudioVoiceConsentsDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<types::audio::VoiceConsentDeleted>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/audio/voice_consents/{consent_id}", { { "consent_id", consent_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::audio::VoiceConsentDeleted>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    AudioVoices::AudioVoices(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /audio/voices.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioVoices::Create(
        types::audio::VoiceCreateParams params
    ) const -> drogon::Task<Result<types::audio::Voice>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/audio/voices", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::audio::Voice>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /audio/voices.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioVoices::CreateRaw(
        types::audio::VoiceCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::audio::Voice>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = true || !payload.files.empty();
        auto path{ resource_path("/audio/voices", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::audio::Voice>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief DELETE /audio/voices/{voice_id}.
     * @param voice_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioVoices::Delete(
        std::string voice_id,
        types::audio::AudioVoicesDeleteParams params
    ) const -> drogon::Task<Result<types::audio::VoiceDeleted>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/audio/voices/{voice_id}", { { "voice_id", voice_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::audio::VoiceDeleted>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief DELETE /audio/voices/{voice_id}.
     * @param voice_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto AudioVoices::DeleteRaw(
        std::string voice_id,
        types::audio::AudioVoicesDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<types::audio::VoiceDeleted>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/audio/voices/{voice_id}", { { "voice_id", voice_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::audio::VoiceDeleted>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Audio::Audio(std::shared_ptr<Transport> transport) : m_transport{ transport }, transcriptions{ transport }, translations{ transport }, speech{ transport }, voices{ transport }, voice_consents{ transport } {}
} // namespace drogon_openai::details
