/**
 * @file completions.hpp
 * @brief Declare the chat completions API group that `Chat::completions` exposes.
 */
#pragma once

#include <memory>

#include "drogon_llm/error.hpp"
#include "drogon_llm/types/chat.hpp"

namespace drogon_llm::details {

    class Transport;

    /**
     * @brief Chat completions API group, reached through `llm.chat->completions`.
     */
    class Completions {
    public:
        /**
         * @brief Bind the group to an endpoint.
         * @param transport Connection shared by the API groups of one `Llm`.
         */
        explicit Completions(std::shared_ptr<Transport> transport);

        /**
         * @brief Generate a reply to a conversation, like `client.chat.completions.create()`.
         * @param params Request fields.
         * @return The completion, or why the request failed.
         * @note Blocks the calling thread until the response arrives. The HTTP client runs on
         * the app's main event loop, so never call this on that loop's thread (e.g. from a
         * beginning advice): it would deadlock, and debug builds of drogon assert.
         */
        [[nodiscard]] auto Create(ChatCompletionCreateParams const& params) -> Result<ChatCompletion>;

        // TODO: add a coroutine variant and streaming (`stream: true`).

    private:
        std::shared_ptr<Transport> m_transport; ///< Connection shared with the other API groups.
    };

} // namespace drogon_llm::details
