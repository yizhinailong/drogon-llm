/**
 * @file chat_management.hpp
 * @brief Declare stored chat completion management parameters and results.
 */
#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include <json/value.h>

namespace drogon_openai {

    /**
     * @brief Filters for stored chat completions.
     */
    struct ChatCompletionListParams {
        std::optional<std::string> after{};    ///< Last completion from the previous page.
        std::optional<std::int64_t> limit{};   ///< Requested page size.
        std::optional<Json::Value> metadata{}; ///< Metadata filters, encoded as bracketed query keys.
        std::optional<std::string> model{};    ///< Filter by model.
        std::optional<std::string> order{};    ///< `asc` or `desc`.
    };

    /**
     * @brief Changes to a stored chat completion.
     */
    struct ChatCompletionUpdateParams {
        Json::Value metadata{};   ///< Replacement metadata map, or null to clear it.
        Json::Value extra_body{}; ///< Additional body fields, merged last.
    };

    /**
     * @brief Pagination of the messages in a stored chat completion.
     */
    struct ChatCompletionMessageListParams {
        std::optional<std::string> after{};  ///< Last message from the previous page.
        std::optional<std::int64_t> limit{}; ///< Requested page size.
        std::optional<std::string> order{};  ///< `asc` or `desc`.
    };

    /**
     * @brief Original conversation message retained by a stored chat completion.
     */
    struct ChatCompletionStoreMessage {
        std::string id;                             ///< Stored message ID.
        std::string role;                           ///< Conversation role, including `user`, `assistant`, and `tool`.
        std::optional<std::string> content{};       ///< Text, when available.
        std::optional<Json::Value> content_parts{}; ///< Original multimodal content parts.
        std::optional<std::string> name{};          ///< Participant name.
        Json::Value raw{};                          ///< Complete original JSON, including vendor fields.
        std::string _request_id;                    ///< Response `x-request-id`.
    };

    /**
     * @brief Confirmation that a stored completion was deleted.
     */
    struct ChatCompletionDeleted {
        std::string id;          ///< Deleted completion ID.
        bool deleted{};          ///< Server confirmation.
        std::string object;      ///< `chat.completion.deleted`.
        Json::Value raw{};       ///< Complete response JSON.
        std::string _request_id; ///< Response `x-request-id`.
    };

} // namespace drogon_openai
