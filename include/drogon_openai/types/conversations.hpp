/**
 * @file conversations.hpp
 * @brief Request and response types for conversations; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::conversations {

    /**
     * @brief Response Conversation, retaining SDK union fields as JSON.
     */
    struct Conversation {
        std::string id{};          ///< Wire field id.
        std::int64_t created_at{}; ///< Wire field created_at.
        Json::Value metadata{};    ///< Wire field metadata.
        std::string object{};      ///< Wire field object.
        Json::Value raw{};         ///< Complete JSON including provider extensions.
        std::string _request_id;   ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<Conversation>;
    };

    /**
     * @brief Response ConversationDeletedResource, retaining SDK union fields as JSON.
     */
    struct ConversationDeletedResource {
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
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<ConversationDeletedResource>;
    };

    /**
     * @brief Response ConversationItemList, retaining SDK union fields as JSON.
     */
    struct ConversationItemList {
        Json::Value data{};      ///< Wire field data.
        std::string first_id{};  ///< Wire field first_id.
        bool has_more{};         ///< Wire field has_more.
        std::string last_id{};   ///< Wire field last_id.
        std::string object{};    ///< Wire field object.
        Json::Value raw{};       ///< Complete JSON including provider extensions.
        std::string _request_id; ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<ConversationItemList>;
    };

    /**
     * @brief Parameters for ConversationCreateParams; omitted optionals are absent on the wire.
     */
    struct ConversationCreateParams {
        std::optional<Json::Value> items{};    ///< Optional SDK field items.
        std::optional<Json::Value> metadata{}; ///< Optional SDK field metadata.
        Json::Value extra_body{};              ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};      ///< Override phase timeouts.
        std::optional<int> max_retries{};      ///< Override retries.
        Headers extra_headers{};               ///< Override request headers.
        Json::Value extra_query{};             ///< Override query parameters.

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
     * @brief Parameters for ConversationsRetrieveParams; omitted optionals are absent on the wire.
     */
    struct ConversationsRetrieveParams {
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
     * @brief Parameters for ConversationUpdateParams; omitted optionals are absent on the wire.
     */
    struct ConversationUpdateParams {
        Json::Value metadata{};           ///< Required SDK field metadata.
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
     * @brief Parameters for ConversationsDeleteParams; omitted optionals are absent on the wire.
     */
    struct ConversationsDeleteParams {
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
     * @brief Parameters for ItemCreateParams; omitted optionals are absent on the wire.
     */
    struct ItemCreateParams {
        Json::Value items{};                  ///< Required SDK field items.
        std::optional<Json::Value> include{}; ///< Optional SDK field include.
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
     * @brief Parameters for ItemRetrieveParams; omitted optionals are absent on the wire.
     */
    struct ItemRetrieveParams {
        std::optional<Json::Value> include{}; ///< Optional SDK field include.
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
     * @brief Parameters for ItemListParams; omitted optionals are absent on the wire.
     */
    struct ItemListParams {
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
     * @brief Parameters for ConversationsItemsDeleteParams; omitted optionals are absent on the wire.
     */
    struct ConversationsItemsDeleteParams {
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
} // namespace drogon_openai::types::conversations
