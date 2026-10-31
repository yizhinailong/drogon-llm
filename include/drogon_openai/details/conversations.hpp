/**
 * @file conversations.hpp
 * @brief conversations API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/conversations.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK conversations.items resource.
     */
    class ConversationsItems {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit ConversationsItems(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /conversations/{conversation_id}/items.
         * @param conversation_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            std::string conversation_id,
            types::conversations::ItemCreateParams params = {}
        ) const -> drogon::Task<Result<types::conversations::ConversationItemList>>;

        /**
         * @brief POST /conversations/{conversation_id}/items.
         * @param conversation_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            std::string conversation_id,
            types::conversations::ItemCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::conversations::ConversationItemList>>>;

        /**
         * @brief GET /conversations/{conversation_id}/items/{item_id}.
         * @param item_id Owned path identifier, encoded as one segment.
         * @param conversation_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string item_id,
            std::string conversation_id,
            types::conversations::ItemRetrieveParams params = {}
        ) const -> drogon::Task<Result<Json::Value>>;

        /**
         * @brief GET /conversations/{conversation_id}/items/{item_id}.
         * @param item_id Owned path identifier, encoded as one segment.
         * @param conversation_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string item_id,
            std::string conversation_id,
            types::conversations::ItemRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;

        /**
         * @brief GET /conversations/{conversation_id}/items.
         * @param conversation_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            std::string conversation_id,
            types::conversations::ItemListParams params = {}
        ) const -> drogon::Task<Result<Page<Json::Value>>>;

        /**
         * @brief DELETE /conversations/{conversation_id}/items/{item_id}.
         * @param item_id Owned path identifier, encoded as one segment.
         * @param conversation_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string item_id,
            std::string conversation_id,
            types::conversations::ConversationsItemsDeleteParams params = {}
        ) const -> drogon::Task<Result<types::conversations::Conversation>>;

        /**
         * @brief DELETE /conversations/{conversation_id}/items/{item_id}.
         * @param item_id Owned path identifier, encoded as one segment.
         * @param conversation_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string item_id,
            std::string conversation_id,
            types::conversations::ConversationsItemsDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::conversations::Conversation>>>;
    };

    /**
     * @brief Value handle for the SDK conversations resource.
     */
    class Conversations {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Conversations(std::shared_ptr<Transport> transport);
        ConversationsItems items; ///< Nested SDK resource items.

        /**
         * @brief POST /conversations.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::conversations::ConversationCreateParams params = {}
        ) const -> drogon::Task<Result<types::conversations::Conversation>>;

        /**
         * @brief POST /conversations.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::conversations::ConversationCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::conversations::Conversation>>>;

        /**
         * @brief GET /conversations/{conversation_id}.
         * @param conversation_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string conversation_id,
            types::conversations::ConversationsRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::conversations::Conversation>>;

        /**
         * @brief GET /conversations/{conversation_id}.
         * @param conversation_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string conversation_id,
            types::conversations::ConversationsRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::conversations::Conversation>>>;

        /**
         * @brief POST /conversations/{conversation_id}.
         * @param conversation_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Update(
            std::string conversation_id,
            types::conversations::ConversationUpdateParams params = {}
        ) const -> drogon::Task<Result<types::conversations::Conversation>>;

        /**
         * @brief POST /conversations/{conversation_id}.
         * @param conversation_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto UpdateRaw(
            std::string conversation_id,
            types::conversations::ConversationUpdateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::conversations::Conversation>>>;

        /**
         * @brief DELETE /conversations/{conversation_id}.
         * @param conversation_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string conversation_id,
            types::conversations::ConversationsDeleteParams params = {}
        ) const -> drogon::Task<Result<types::conversations::ConversationDeletedResource>>;

        /**
         * @brief DELETE /conversations/{conversation_id}.
         * @param conversation_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string conversation_id,
            types::conversations::ConversationsDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::conversations::ConversationDeletedResource>>>;
    };
} // namespace drogon_openai::details
