/**
 * @file chat.hpp
 * @brief Declare the chat API group that `OpenAI::chat` exposes.
 */
#pragma once

#include <memory>

#include "drogon_openai/details/completions.hpp"

namespace drogon_openai::details {

    /**
     * @brief Chat API group, reached through `client.chat`.
     */
    class Chat {
    public:
        /**
         * @brief Bind the group to an endpoint.
         * @param transport Connection shared by the API groups of one `OpenAI` client.
         */
        explicit Chat(std::shared_ptr<Transport> transport);

        Completions completions; ///< Chat completions API group.
    };

} // namespace drogon_openai::details
