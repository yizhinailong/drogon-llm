/**
 * @file embeddings.hpp
 * @brief Request and response types for embeddings; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::embeddings {

    /**
     * @brief Response CreateEmbeddingResponse, retaining SDK union fields as JSON.
     */
    struct CreateEmbeddingResponse {
        Json::Value data{};      ///< Wire field data.
        std::string model{};     ///< Wire field model.
        std::string object{};    ///< Wire field object.
        Json::Value usage{};     ///< Wire field usage.
        Json::Value raw{};       ///< Complete JSON including provider extensions.
        std::string _request_id; ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<CreateEmbeddingResponse>;
    };

    /**
     * @brief Parameters for EmbeddingCreateParams; omitted optionals are absent on the wire.
     */
    struct EmbeddingCreateParams {
        Json::Value input{};                          ///< Required SDK field input.
        std::string model{};                          ///< Required SDK field model.
        std::optional<std::int64_t> dimensions{};     ///< Optional SDK field dimensions.
        std::optional<std::string> encoding_format{}; ///< Optional SDK field encoding_format.
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
} // namespace drogon_openai::types::embeddings
