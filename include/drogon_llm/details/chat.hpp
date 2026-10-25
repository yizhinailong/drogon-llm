/**
 * @file chat.hpp
 * @brief Declare the chat API group that `Llm::chat` exposes.
 */
#pragma once

#include <memory>

#include "drogon_llm/details/completions.hpp"

namespace drogon_llm::details {

    /**
     * @brief Chat API group, reached through `llm.chat`.
     */
    class Chat {
    public:
        /**
         * @brief Bind the group to an endpoint.
         * @param transport Connection shared by the API groups of one `Llm`.
         */
        explicit Chat(std::shared_ptr<Transport> transport);

        Completions completions; ///< Chat completions API group.
    };

} // namespace drogon_llm::details
