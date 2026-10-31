/**
 * @file safety.hpp
 * @brief safety API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/safety.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK safety.alerts resource.
     */
    class SafetyAlerts {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit SafetyAlerts(std::shared_ptr<Transport> transport);

        /**
         * @brief GET /safety/alerts/{id}.
         * @param id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string id,
            types::safety::SafetyAlertsRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::safety::SafetyAlert>>;

        /**
         * @brief GET /safety/alerts/{id}.
         * @param id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string id,
            types::safety::SafetyAlertsRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::safety::SafetyAlert>>>;
    };

    /**
     * @brief Value handle for the SDK safety.cases resource.
     */
    class SafetyCases {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit SafetyCases(std::shared_ptr<Transport> transport);

        /**
         * @brief GET /safety/cases/{id}.
         * @param id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string id,
            types::safety::SafetyCasesRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::safety::SafetyCase>>;

        /**
         * @brief GET /safety/cases/{id}.
         * @param id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string id,
            types::safety::SafetyCasesRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::safety::SafetyCase>>>;
    };

    /**
     * @brief Value handle for the SDK safety resource.
     */
    class Safety {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Safety(std::shared_ptr<Transport> transport);
        SafetyCases cases;   ///< Nested SDK resource cases.
        SafetyAlerts alerts; ///< Nested SDK resource alerts.
    };
} // namespace drogon_openai::details
