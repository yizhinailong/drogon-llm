/**
 * @file chat.cpp
 * @brief Implement the chat API group that `OpenAI::chat` exposes.
 */
#include "drogon_openai/details/chat.hpp"

#include <utility>

namespace drogon_openai::details {

    /**
     * @brief Bind the group to an endpoint.
     * @param transport Connection shared by the API groups of one `OpenAI` client.
     */
    Chat::Chat(std::shared_ptr<Transport> transport)
        : completions{ std::move(transport) } {}

} // namespace drogon_openai::details
