/**
 * @file responses.hpp
 * @brief responses API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/responses.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK responses.input_items resource.
     */
    class ResponsesInputItems {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit ResponsesInputItems(std::shared_ptr<Transport> transport);

        /**
         * @brief GET /responses/{response_id}/input_items.
         * @param response_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            std::string response_id,
            types::responses::InputItemListParams params = {}
        ) const -> drogon::Task<Result<Page<Json::Value>>>;
    };

    /**
     * @brief Value handle for the SDK responses.input_tokens resource.
     */
    class ResponsesInputTokens {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit ResponsesInputTokens(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /responses/input_tokens.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Count(
            types::responses::InputTokenCountParams params = {}
        ) const -> drogon::Task<Result<types::responses::InputTokenCountResponse>>;

        /**
         * @brief POST /responses/input_tokens.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CountRaw(
            types::responses::InputTokenCountParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::responses::InputTokenCountResponse>>>;
    };

    /**
     * @brief Value handle for the SDK responses resource.
     */
    class Responses {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Responses(std::shared_ptr<Transport> transport);
        ResponsesInputItems input_items;   ///< Nested SDK resource input_items.
        ResponsesInputTokens input_tokens; ///< Nested SDK resource input_tokens.

        /**
         * @brief POST /responses.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::responses::ResponsesCreateParams params = {}
        ) const -> drogon::Task<Result<types::responses::Response>>;

        /**
         * @brief POST /responses.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::responses::ResponsesCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::responses::Response>>>;

        /**
         * @brief Stream SSE from POST /responses.
         * @param params Owned SDK parameters.
         * @param callback Event consumer; false stops delivery.
         * @return Final event and stream metadata or an error.
         */
        [[nodiscard]] auto CreateStream(
            types::responses::ResponsesCreateParams params,
            ApiEventCallback callback = {}
        ) const -> drogon::Task<Result<ApiStreamResult>>;

        /**
         * @brief GET /responses/{response_id}.
         * @param response_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string response_id,
            types::responses::ResponseRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::responses::Response>>;

        /**
         * @brief GET /responses/{response_id}.
         * @param response_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string response_id,
            types::responses::ResponseRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::responses::Response>>>;

        /**
         * @brief Stream SSE from GET /responses/{response_id}.
         * @param response_id Owned path identifier.
         * @param params Owned SDK parameters.
         * @param callback Event consumer; false stops delivery.
         * @return Final event and stream metadata or an error.
         */
        [[nodiscard]] auto RetrieveStream(
            std::string response_id,
            types::responses::ResponseRetrieveParams params,
            ApiEventCallback callback = {}
        ) const -> drogon::Task<Result<ApiStreamResult>>;

        /**
         * @brief DELETE /responses/{response_id}.
         * @param response_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string response_id,
            types::responses::ResponsesDeleteParams params = {}
        ) const -> drogon::Task<Result<void>>;

        /**
         * @brief DELETE /responses/{response_id}.
         * @param response_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string response_id,
            types::responses::ResponsesDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<Json::Value>>>;

        /**
         * @brief POST /responses/{response_id}/cancel.
         * @param response_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Cancel(
            std::string response_id,
            types::responses::ResponsesCancelParams params = {}
        ) const -> drogon::Task<Result<types::responses::Response>>;

        /**
         * @brief POST /responses/{response_id}/cancel.
         * @param response_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CancelRaw(
            std::string response_id,
            types::responses::ResponsesCancelParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::responses::Response>>>;

        /**
         * @brief POST /responses/compact.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Compact(
            types::responses::ResponseCompactParams params = {}
        ) const -> drogon::Task<Result<types::responses::CompactedResponse>>;

        /**
         * @brief POST /responses/compact.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CompactRaw(
            types::responses::ResponseCompactParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::responses::CompactedResponse>>>;

        /**
         * @brief Create and parse JSON Schema output and strict function arguments.
         * @param params Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto Parse(
            types::responses::ResponsesCreateParams params
        ) const -> drogon::Task<Result<types::responses::Response>>;
    };
} // namespace drogon_openai::details
