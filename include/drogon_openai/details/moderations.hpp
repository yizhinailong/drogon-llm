/**
 * @file moderations.hpp
 * @brief moderations API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/moderations.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK moderations resource.
     */
    class Moderations {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Moderations(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /moderations.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::moderations::ModerationCreateParams params = {}
        ) const -> drogon::Task<Result<types::moderations::ModerationCreateResponse>>;

        /**
         * @brief POST /moderations.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::moderations::ModerationCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::moderations::ModerationCreateResponse>>>;
    };
} // namespace drogon_openai::details
