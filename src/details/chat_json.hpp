/**
 * @file chat_json.hpp
 * @brief Declare shared chat serialization and response decoding helpers.
 */
#pragma once

#include "drogon_openai/error.hpp"
#include "drogon_openai/types/chat.hpp"

namespace drogon_openai::details {

    /**
     * @brief Serialize a chat request, merging vendor fields last.
     * @param params Request fields.
     * @param default_model Configured default model.
     * @return JSON request body.
     */
    auto to_request_body(
        ChatCompletionCreateParams const& params,
        std::string const& default_model
    ) -> Json::Value;

    /**
     * @brief Decode a chat completion, retaining its original JSON.
     * @param root Response body.
     * @return Completion or a response validation error.
     */
    auto parse_chat_completion(Json::Value const& root) -> Result<ChatCompletion>;

    /**
     * @brief Decode one streaming chat chunk.
     * @param root Chunk body.
     * @return Chunk or a response validation error.
     */
    auto parse_chat_chunk(Json::Value const& root) -> Result<ChatCompletionChunk>;

    /**
     * @brief Check that automatic parsing uses only strict function tools.
     * @param params Request whose tools will be parsed.
     * @return Success or an invalid-request error, before sending the request.
     */
    auto validate_parse_tools(ChatCompletionCreateParams const& params) -> Result<void>;

    /**
     * @brief Decode structured response text and strict function arguments.
     * @param completion Completion to enrich.
     * @param params Original request, including schema and tools.
     * @return Success or a parsing/finish-reason error.
     */
    auto parse_structured(
        ChatCompletion& completion,
        ChatCompletionCreateParams const& params
    ) -> Result<void>;

} // namespace drogon_openai::details
