/**
 * @file decisions.hpp
 * @brief decisions API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/decisions.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK decisions resource.
     */
    class Decisions {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Decisions(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /decisions.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::decisions::DecisionCreateParams params = {}
        ) const -> drogon::Task<Result<types::decisions::Decision>>;

        /**
         * @brief POST /decisions.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::decisions::DecisionCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::decisions::Decision>>>;
    };
} // namespace drogon_openai::details
