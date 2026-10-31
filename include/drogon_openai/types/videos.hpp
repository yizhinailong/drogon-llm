/**
 * @file videos.hpp
 * @brief Request and response types for videos; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::videos {

    /**
     * @brief Response Video, retaining SDK union fields as JSON.
     */
    struct Video {
        std::string id{};                                   ///< Wire field id.
        std::optional<Json::Value> completed_at{};          ///< Wire field completed_at.
        std::int64_t created_at{};                          ///< Wire field created_at.
        std::optional<Json::Value> error{};                 ///< Wire field error.
        std::optional<Json::Value> expires_at{};            ///< Wire field expires_at.
        std::string model{};                                ///< Wire field model.
        std::string object{};                               ///< Wire field object.
        std::int64_t progress{};                            ///< Wire field progress.
        std::optional<Json::Value> prompt{};                ///< Wire field prompt.
        std::optional<Json::Value> remixed_from_video_id{}; ///< Wire field remixed_from_video_id.
        Json::Value seconds{};                              ///< Wire field seconds.
        Json::Value size{};                                 ///< Wire field size.
        std::string status{};                               ///< Wire field status.
        Json::Value raw{};                                  ///< Complete JSON including provider extensions.
        std::string _request_id;                            ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<Video>;
    };

    /**
     * @brief Response VideoDeleteResponse, retaining SDK union fields as JSON.
     */
    struct VideoDeleteResponse {
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
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<VideoDeleteResponse>;
    };

    /**
     * @brief Response VideoCreateCharacterResponse, retaining SDK union fields as JSON.
     */
    struct VideoCreateCharacterResponse {
        std::optional<Json::Value> id{};   ///< Wire field id.
        std::int64_t created_at{};         ///< Wire field created_at.
        std::optional<Json::Value> name{}; ///< Wire field name.
        Json::Value raw{};                 ///< Complete JSON including provider extensions.
        std::string _request_id;           ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<VideoCreateCharacterResponse>;
    };

    /**
     * @brief Response VideoGetCharacterResponse, retaining SDK union fields as JSON.
     */
    struct VideoGetCharacterResponse {
        std::optional<Json::Value> id{};   ///< Wire field id.
        std::int64_t created_at{};         ///< Wire field created_at.
        std::optional<Json::Value> name{}; ///< Wire field name.
        Json::Value raw{};                 ///< Complete JSON including provider extensions.
        std::string _request_id;           ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<VideoGetCharacterResponse>;
    };

    /**
     * @brief Parameters for VideoCreateParams; omitted optionals are absent on the wire.
     */
    struct VideoCreateParams {
        std::string prompt{};                       ///< Required SDK field prompt.
        std::optional<FileInput> input_reference{}; ///< Optional SDK field input_reference.
        std::optional<std::string> model{};         ///< Optional SDK field model.
        std::optional<std::string> seconds{};       ///< Optional SDK field seconds.
        std::optional<std::string> size{};          ///< Optional SDK field size.
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
     * @brief Parameters for VideosRetrieveParams; omitted optionals are absent on the wire.
     */
    struct VideosRetrieveParams {
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
     * @brief Parameters for VideoListParams; omitted optionals are absent on the wire.
     */
    struct VideoListParams {
        std::optional<std::string> after{};  ///< Optional SDK field after.
        std::optional<std::int64_t> limit{}; ///< Optional SDK field limit.
        std::optional<std::string> order{};  ///< Optional SDK field order.
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
     * @brief Parameters for VideosDeleteParams; omitted optionals are absent on the wire.
     */
    struct VideosDeleteParams {
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
     * @brief Parameters for VideoCreateCharacterParams; omitted optionals are absent on the wire.
     */
    struct VideoCreateCharacterParams {
        std::string name{};               ///< Required SDK field name.
        FileInput video{};                ///< Required SDK field video.
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
     * @brief Parameters for VideoDownloadContentParams; omitted optionals are absent on the wire.
     */
    struct VideoDownloadContentParams {
        std::optional<std::string> variant{}; ///< Optional SDK field variant.
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
     * @brief Parameters for VideoEditParams; omitted optionals are absent on the wire.
     */
    struct VideoEditParams {
        std::string prompt{};             ///< Required SDK field prompt.
        FileInput video{};                ///< Required SDK field video.
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
     * @brief Parameters for VideoExtendParams; omitted optionals are absent on the wire.
     */
    struct VideoExtendParams {
        std::string prompt{};             ///< Required SDK field prompt.
        std::string seconds{};            ///< Required SDK field seconds.
        FileInput video{};                ///< Required SDK field video.
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
     * @brief Parameters for VideosGetCharacterParams; omitted optionals are absent on the wire.
     */
    struct VideosGetCharacterParams {
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
     * @brief Parameters for VideoRemixParams; omitted optionals are absent on the wire.
     */
    struct VideoRemixParams {
        std::string prompt{};             ///< Required SDK field prompt.
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
} // namespace drogon_openai::types::videos
