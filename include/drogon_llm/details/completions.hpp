/**
 * @file completions.hpp
 * @brief Declare the chat completions API group that `Chat::completions` exposes.
 */
#pragma once

#include <memory>

#include <drogon/utils/coroutine.h>

#include "drogon_llm/error.hpp"
#include "drogon_llm/types/chat.hpp"

namespace drogon_llm::details {

    class Transport;

    /**
     * @brief Chat completions API group, reached through `llm.chat.completions`.
     */
    class Completions {
    public:
        /**
         * @brief Bind the group to an endpoint.
         * @param transport Connection shared by the API groups of one `Llm`.
         */
        explicit Completions(std::shared_ptr<Transport> transport);

        /**
         * @brief Generate a reply to a conversation, like `await client.chat.completions.create()`.
         * @param params Request fields, taken by value so they outlive the caller's frame.
         * @return Task yielding the completion, or why the request failed.
         * @note The request runs on the calling thread's event loop (the main loop when the
         * thread runs none), so the coroutine resumes on the thread that awaited it. To block
         * on it with `drogon::sync_wait()`, call from a thread that runs no event loop.
         */
        [[nodiscard]] auto Create(
            ChatCompletionCreateParams params
        ) const -> drogon::Task<Result<ChatCompletion>>;

        // TODO: add streaming (`stream: true`).

    private:
        std::shared_ptr<Transport> m_transport; ///< Connection shared with the other API groups.
    };

} // namespace drogon_llm::details
