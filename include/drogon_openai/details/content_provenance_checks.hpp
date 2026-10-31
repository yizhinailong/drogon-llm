/**
 * @file content_provenance_checks.hpp
 * @brief content_provenance_checks API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/content_provenance_checks.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK content_provenance_checks resource.
     */
    class ContentProvenanceChecks {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit ContentProvenanceChecks(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /content_provenance_checks.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::content_provenance_checks::ContentProvenanceCheckCreateParams params = {}
        ) const -> drogon::Task<Result<types::content_provenance_checks::ContentProvenanceCheck>>;

        /**
         * @brief POST /content_provenance_checks.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::content_provenance_checks::ContentProvenanceCheckCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::content_provenance_checks::ContentProvenanceCheck>>>;
    };
} // namespace drogon_openai::details
