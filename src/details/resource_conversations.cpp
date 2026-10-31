/**
 * @file resource_conversations.cpp
 * @brief Implement conversations resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/conversations.hpp"

namespace drogon_openai::types::conversations {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto Conversation::FromJson(Json::Value const& value) -> Result<Conversation> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        Conversation result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Conversation.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Conversation.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("metadata")) {
            if (!details::resource_read(value["metadata"], result.metadata)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Conversation.metadata", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid Conversation.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto ConversationDeletedResource::FromJson(Json::Value const& value) -> Result<ConversationDeletedResource> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        ConversationDeletedResource result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ConversationDeletedResource.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("deleted")) {
            if (!details::resource_read(value["deleted"], result.deleted)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ConversationDeletedResource.deleted", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ConversationDeletedResource.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto ConversationItemList::FromJson(Json::Value const& value) -> Result<ConversationItemList> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        ConversationItemList result;
        result.raw = value;
        if (value.isMember("data")) {
            if (!details::resource_read(value["data"], result.data)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ConversationItemList.data", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("first_id")) {
            if (!details::resource_read(value["first_id"], result.first_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ConversationItemList.first_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("has_more")) {
            if (!details::resource_read(value["has_more"], result.has_more)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ConversationItemList.has_more", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("last_id")) {
            if (!details::resource_read(value["last_id"], result.last_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ConversationItemList.last_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid ConversationItemList.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ConversationCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (items) {
            payload.body["items"] = details::resource_json(*items);
        }
        if (metadata) {
            payload.body["metadata"] = details::resource_json(*metadata);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ConversationCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ConversationsRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ConversationsRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ConversationUpdateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["metadata"] = details::resource_json(metadata);
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ConversationUpdateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ConversationsDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ConversationsDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ItemCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["items"] = details::resource_json(items);
        if (include) {
            payload.query["include"] = details::resource_json(*include);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ItemCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ItemRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (include) {
            payload.query["include"] = details::resource_json(*include);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ItemRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ItemListParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (after) {
            payload.query["after"] = details::resource_json(*after);
        }
        if (include) {
            payload.query["include"] = details::resource_json(*include);
        }
        if (limit) {
            payload.query["limit"] = details::resource_json(*limit);
        }
        if (order) {
            payload.query["order"] = details::resource_json(*order);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ItemListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto ConversationsItemsDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto ConversationsItemsDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::conversations

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    ConversationsItems::ConversationsItems(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief POST /conversations/{conversation_id}/items.
     * @param conversation_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ConversationsItems::Create(
        std::string conversation_id,
        types::conversations::ItemCreateParams params
    ) const -> drogon::Task<Result<types::conversations::ConversationItemList>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations/{conversation_id}/items", { { "conversation_id", conversation_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::conversations::ConversationItemList>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /conversations/{conversation_id}/items.
     * @param conversation_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ConversationsItems::CreateRaw(
        std::string conversation_id,
        types::conversations::ItemCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::conversations::ConversationItemList>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations/{conversation_id}/items", { { "conversation_id", conversation_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::conversations::ConversationItemList>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /conversations/{conversation_id}/items/{item_id}.
     * @param item_id Owned path identifier, encoded as one segment.
     * @param conversation_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ConversationsItems::Retrieve(
        std::string item_id,
        std::string conversation_id,
        types::conversations::ItemRetrieveParams params
    ) const -> drogon::Task<Result<Json::Value>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations/{conversation_id}/items/{item_id}", { { "item_id", item_id }, { "conversation_id", conversation_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
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
     * @brief GET /conversations/{conversation_id}/items/{item_id}.
     * @param item_id Owned path identifier, encoded as one segment.
     * @param conversation_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ConversationsItems::RetrieveRaw(
        std::string item_id,
        std::string conversation_id,
        types::conversations::ItemRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations/{conversation_id}/items/{item_id}", { { "item_id", item_id }, { "conversation_id", conversation_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
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
     * @brief GET /conversations/{conversation_id}/items.
     * @param conversation_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ConversationsItems::List(
        std::string conversation_id,
        types::conversations::ItemListParams params
    ) const -> drogon::Task<Result<Page<Json::Value>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations/{conversation_id}/items", { { "conversation_id", conversation_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<Json::Value>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true, "last_id");
    }

    /**
     * @brief DELETE /conversations/{conversation_id}/items/{item_id}.
     * @param item_id Owned path identifier, encoded as one segment.
     * @param conversation_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ConversationsItems::Delete(
        std::string item_id,
        std::string conversation_id,
        types::conversations::ConversationsItemsDeleteParams params
    ) const -> drogon::Task<Result<types::conversations::Conversation>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations/{conversation_id}/items/{item_id}", { { "item_id", item_id }, { "conversation_id", conversation_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::conversations::Conversation>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief DELETE /conversations/{conversation_id}/items/{item_id}.
     * @param item_id Owned path identifier, encoded as one segment.
     * @param conversation_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto ConversationsItems::DeleteRaw(
        std::string item_id,
        std::string conversation_id,
        types::conversations::ConversationsItemsDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<types::conversations::Conversation>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations/{conversation_id}/items/{item_id}", { { "item_id", item_id }, { "conversation_id", conversation_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::conversations::Conversation>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Conversations::Conversations(std::shared_ptr<Transport> transport) : m_transport{ transport }, items{ transport } {}

    /**
     * @brief POST /conversations.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Conversations::Create(
        types::conversations::ConversationCreateParams params
    ) const -> drogon::Task<Result<types::conversations::Conversation>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::conversations::Conversation>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /conversations.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Conversations::CreateRaw(
        types::conversations::ConversationCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::conversations::Conversation>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::conversations::Conversation>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /conversations/{conversation_id}.
     * @param conversation_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Conversations::Retrieve(
        std::string conversation_id,
        types::conversations::ConversationsRetrieveParams params
    ) const -> drogon::Task<Result<types::conversations::Conversation>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations/{conversation_id}", { { "conversation_id", conversation_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::conversations::Conversation>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /conversations/{conversation_id}.
     * @param conversation_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Conversations::RetrieveRaw(
        std::string conversation_id,
        types::conversations::ConversationsRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::conversations::Conversation>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations/{conversation_id}", { { "conversation_id", conversation_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::conversations::Conversation>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /conversations/{conversation_id}.
     * @param conversation_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Conversations::Update(
        std::string conversation_id,
        types::conversations::ConversationUpdateParams params
    ) const -> drogon::Task<Result<types::conversations::Conversation>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations/{conversation_id}", { { "conversation_id", conversation_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::conversations::Conversation>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /conversations/{conversation_id}.
     * @param conversation_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Conversations::UpdateRaw(
        std::string conversation_id,
        types::conversations::ConversationUpdateParams params
    ) const -> drogon::Task<Result<RawResponse<types::conversations::Conversation>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations/{conversation_id}", { { "conversation_id", conversation_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::conversations::Conversation>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief DELETE /conversations/{conversation_id}.
     * @param conversation_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Conversations::Delete(
        std::string conversation_id,
        types::conversations::ConversationsDeleteParams params
    ) const -> drogon::Task<Result<types::conversations::ConversationDeletedResource>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations/{conversation_id}", { { "conversation_id", conversation_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::conversations::ConversationDeletedResource>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief DELETE /conversations/{conversation_id}.
     * @param conversation_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Conversations::DeleteRaw(
        std::string conversation_id,
        types::conversations::ConversationsDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<types::conversations::ConversationDeletedResource>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/conversations/{conversation_id}", { { "conversation_id", conversation_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::conversations::ConversationDeletedResource>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }
} // namespace drogon_openai::details
