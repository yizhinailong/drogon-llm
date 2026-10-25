/**
 * @file chat.cpp
 * @brief Implement the chat API group that `Llm::chat` exposes.
 */
#include "drogon_llm/details/chat.hpp"

#include <utility>

namespace drogon_llm::details {

    /**
     * @brief Bind the group to an endpoint.
     * @param transport Connection shared by the API groups of one `Llm`.
     */
    Chat::Chat(std::shared_ptr<Transport> transport)
        : completions{ std::make_unique<Completions>(std::move(transport)) } {}

} // namespace drogon_llm::details
