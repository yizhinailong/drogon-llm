/**
 * @file images.hpp
 * @brief Request and response types for images; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::images {

    /**
     * @brief Response ImagesResponse, retaining SDK union fields as JSON.
     */
    struct ImagesResponse {
        std::int64_t created{};                         ///< Wire field created.
        std::optional<Json::Value> background{};        ///< Wire field background.
        std::optional<Json::Value> data{};              ///< Wire field data.
        std::optional<Json::Value> output_format{};     ///< Wire field output_format.
        std::optional<Json::Value> quality{};           ///< Wire field quality.
        std::optional<Json::Value> safety_identifier{}; ///< Wire field safety_identifier.
        std::optional<Json::Value> size{};              ///< Wire field size.
        std::optional<Json::Value> usage{};             ///< Wire field usage.
        Json::Value raw{};                              ///< Complete JSON including provider extensions.
        std::string _request_id;                        ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<ImagesResponse>;
    };

    /**
     * @brief Parameters for ImageCreateVariationParams; omitted optionals are absent on the wire.
     */
    struct ImageCreateVariationParams {
        FileInput image{};                            ///< Required SDK field image.
        std::optional<Json::Value> model{};           ///< Optional SDK field model.
        std::optional<Json::Value> n{};               ///< Optional SDK field n.
        std::optional<Json::Value> response_format{}; ///< Optional SDK field response_format.
        std::optional<Json::Value> size{};            ///< Optional SDK field size.
        std::optional<std::string> user{};            ///< Optional SDK field user.
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
     * @brief Parameters for ImagesEditParams; omitted optionals are absent on the wire.
     */
    struct ImagesEditParams {
        FileInput image{};                               ///< Required SDK field image.
        std::string prompt{};                            ///< Required SDK field prompt.
        std::optional<Json::Value> background{};         ///< Optional SDK field background.
        std::optional<Json::Value> input_fidelity{};     ///< Optional SDK field input_fidelity.
        std::optional<FileInput> markup{};               ///< Optional SDK field markup.
        std::optional<FileInput> mask{};                 ///< Optional SDK field mask.
        std::optional<Json::Value> model{};              ///< Optional SDK field model.
        std::optional<Json::Value> n{};                  ///< Optional SDK field n.
        std::optional<Json::Value> output_compression{}; ///< Optional SDK field output_compression.
        std::optional<Json::Value> output_format{};      ///< Optional SDK field output_format.
        std::optional<Json::Value> partial_images{};     ///< Optional SDK field partial_images.
        std::optional<Json::Value> quality{};            ///< Optional SDK field quality.
        std::optional<Json::Value> response_format{};    ///< Optional SDK field response_format.
        std::optional<Json::Value> safety_identifier{};  ///< Optional SDK field safety_identifier.
        std::optional<Json::Value> size{};               ///< Optional SDK field size.
        std::optional<Json::Value> stream{};             ///< Optional SDK field stream.
        std::optional<std::string> user{};               ///< Optional SDK field user.
        Json::Value extra_body{};                        ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};                ///< Override phase timeouts.
        std::optional<int> max_retries{};                ///< Override retries.
        Headers extra_headers{};                         ///< Override request headers.
        Json::Value extra_query{};                       ///< Override query parameters.

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
     * @brief Parameters for ImagesGenerateParams; omitted optionals are absent on the wire.
     */
    struct ImagesGenerateParams {
        std::string prompt{};                            ///< Required SDK field prompt.
        std::optional<Json::Value> background{};         ///< Optional SDK field background.
        std::optional<Json::Value> model{};              ///< Optional SDK field model.
        std::optional<Json::Value> moderation{};         ///< Optional SDK field moderation.
        std::optional<Json::Value> n{};                  ///< Optional SDK field n.
        std::optional<Json::Value> output_compression{}; ///< Optional SDK field output_compression.
        std::optional<Json::Value> output_format{};      ///< Optional SDK field output_format.
        std::optional<Json::Value> partial_images{};     ///< Optional SDK field partial_images.
        std::optional<Json::Value> quality{};            ///< Optional SDK field quality.
        std::optional<Json::Value> response_format{};    ///< Optional SDK field response_format.
        std::optional<Json::Value> safety_identifier{};  ///< Optional SDK field safety_identifier.
        std::optional<Json::Value> size{};               ///< Optional SDK field size.
        std::optional<Json::Value> stream{};             ///< Optional SDK field stream.
        std::optional<Json::Value> style{};              ///< Optional SDK field style.
        std::optional<std::string> user{};               ///< Optional SDK field user.
        Json::Value extra_body{};                        ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};                ///< Override phase timeouts.
        std::optional<int> max_retries{};                ///< Override retries.
        Headers extra_headers{};                         ///< Override request headers.
        Json::Value extra_query{};                       ///< Override query parameters.

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
} // namespace drogon_openai::types::images
