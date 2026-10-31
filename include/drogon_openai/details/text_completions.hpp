/**
 * @file text_completions.hpp
 * @brief completions API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/completions.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK completions resource.
     */
    class TextCompletions {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit TextCompletions(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /completions.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::completions::CompletionsCreateParams params = {}
        ) const -> drogon::Task<Result<types::completions::Completion>>;

        /**
         * @brief POST /completions.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::completions::CompletionsCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::completions::Completion>>>;

        /**
         * @brief Stream SSE from POST /completions.
         * @param params Owned SDK parameters.
         * @param callback Event consumer; false stops delivery.
         * @return Final event and stream metadata or an error.
         */
        [[nodiscard]] auto CreateStream(
            types::completions::CompletionsCreateParams params,
            ApiEventCallback callback = {}
        ) const -> drogon::Task<Result<ApiStreamResult>>;
    };
} // namespace drogon_openai::details
