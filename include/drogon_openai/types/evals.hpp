/**
 * @file evals.hpp
 * @brief Request and response types for evals; generated from openai-python 3.41.0.
 */
#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drogon_openai/types/resource.hpp"

namespace drogon_openai::types::evals {

    /**
     * @brief Response EvalCreateResponse, retaining SDK union fields as JSON.
     */
    struct EvalCreateResponse {
        std::string id{};                      ///< Wire field id.
        std::int64_t created_at{};             ///< Wire field created_at.
        Json::Value data_source_config{};      ///< Wire field data_source_config.
        std::optional<Json::Value> metadata{}; ///< Wire field metadata.
        std::string name{};                    ///< Wire field name.
        std::string object{};                  ///< Wire field object.
        Json::Value testing_criteria{};        ///< Wire field testing_criteria.
        Json::Value raw{};                     ///< Complete JSON including provider extensions.
        std::string _request_id;               ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<EvalCreateResponse>;
    };

    /**
     * @brief Response EvalRetrieveResponse, retaining SDK union fields as JSON.
     */
    struct EvalRetrieveResponse {
        std::string id{};                      ///< Wire field id.
        std::int64_t created_at{};             ///< Wire field created_at.
        Json::Value data_source_config{};      ///< Wire field data_source_config.
        std::optional<Json::Value> metadata{}; ///< Wire field metadata.
        std::string name{};                    ///< Wire field name.
        std::string object{};                  ///< Wire field object.
        Json::Value testing_criteria{};        ///< Wire field testing_criteria.
        Json::Value raw{};                     ///< Complete JSON including provider extensions.
        std::string _request_id;               ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<EvalRetrieveResponse>;
    };

    /**
     * @brief Response EvalUpdateResponse, retaining SDK union fields as JSON.
     */
    struct EvalUpdateResponse {
        std::string id{};                      ///< Wire field id.
        std::int64_t created_at{};             ///< Wire field created_at.
        Json::Value data_source_config{};      ///< Wire field data_source_config.
        std::optional<Json::Value> metadata{}; ///< Wire field metadata.
        std::string name{};                    ///< Wire field name.
        std::string object{};                  ///< Wire field object.
        Json::Value testing_criteria{};        ///< Wire field testing_criteria.
        Json::Value raw{};                     ///< Complete JSON including provider extensions.
        std::string _request_id;               ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<EvalUpdateResponse>;
    };

    /**
     * @brief Response EvalListResponse, retaining SDK union fields as JSON.
     */
    struct EvalListResponse {
        std::string id{};                      ///< Wire field id.
        std::int64_t created_at{};             ///< Wire field created_at.
        Json::Value data_source_config{};      ///< Wire field data_source_config.
        std::optional<Json::Value> metadata{}; ///< Wire field metadata.
        std::string name{};                    ///< Wire field name.
        std::string object{};                  ///< Wire field object.
        Json::Value testing_criteria{};        ///< Wire field testing_criteria.
        Json::Value raw{};                     ///< Complete JSON including provider extensions.
        std::string _request_id;               ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<EvalListResponse>;
    };

    /**
     * @brief Response EvalDeleteResponse, retaining SDK union fields as JSON.
     */
    struct EvalDeleteResponse {
        bool deleted{};          ///< Wire field deleted.
        std::string eval_id{};   ///< Wire field eval_id.
        std::string object{};    ///< Wire field object.
        Json::Value raw{};       ///< Complete JSON including provider extensions.
        std::string _request_id; ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<EvalDeleteResponse>;
    };

    /**
     * @brief Response OutputItemRetrieveResponse, retaining SDK union fields as JSON.
     */
    struct OutputItemRetrieveResponse {
        std::string id{};                  ///< Wire field id.
        std::int64_t created_at{};         ///< Wire field created_at.
        Json::Value datasource_item{};     ///< Wire field datasource_item.
        std::int64_t datasource_item_id{}; ///< Wire field datasource_item_id.
        std::string eval_id{};             ///< Wire field eval_id.
        std::string object{};              ///< Wire field object.
        Json::Value results{};             ///< Wire field results.
        std::string run_id{};              ///< Wire field run_id.
        Json::Value sample{};              ///< Wire field sample.
        std::string status{};              ///< Wire field status.
        Json::Value raw{};                 ///< Complete JSON including provider extensions.
        std::string _request_id;           ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<OutputItemRetrieveResponse>;
    };

    /**
     * @brief Response OutputItemListResponse, retaining SDK union fields as JSON.
     */
    struct OutputItemListResponse {
        std::string id{};                  ///< Wire field id.
        std::int64_t created_at{};         ///< Wire field created_at.
        Json::Value datasource_item{};     ///< Wire field datasource_item.
        std::int64_t datasource_item_id{}; ///< Wire field datasource_item_id.
        std::string eval_id{};             ///< Wire field eval_id.
        std::string object{};              ///< Wire field object.
        Json::Value results{};             ///< Wire field results.
        std::string run_id{};              ///< Wire field run_id.
        Json::Value sample{};              ///< Wire field sample.
        std::string status{};              ///< Wire field status.
        Json::Value raw{};                 ///< Complete JSON including provider extensions.
        std::string _request_id;           ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<OutputItemListResponse>;
    };

    /**
     * @brief Response RunCreateResponse, retaining SDK union fields as JSON.
     */
    struct RunCreateResponse {
        std::string id{};                           ///< Wire field id.
        std::int64_t created_at{};                  ///< Wire field created_at.
        Json::Value data_source{};                  ///< Wire field data_source.
        Json::Value error{};                        ///< Wire field error.
        std::string eval_id{};                      ///< Wire field eval_id.
        std::optional<Json::Value> metadata{};      ///< Wire field metadata.
        std::string model{};                        ///< Wire field model.
        std::string name{};                         ///< Wire field name.
        std::string object{};                       ///< Wire field object.
        Json::Value per_model_usage{};              ///< Wire field per_model_usage.
        Json::Value per_testing_criteria_results{}; ///< Wire field per_testing_criteria_results.
        std::string report_url{};                   ///< Wire field report_url.
        Json::Value result_counts{};                ///< Wire field result_counts.
        std::string status{};                       ///< Wire field status.
        Json::Value raw{};                          ///< Complete JSON including provider extensions.
        std::string _request_id;                    ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<RunCreateResponse>;
    };

    /**
     * @brief Response RunRetrieveResponse, retaining SDK union fields as JSON.
     */
    struct RunRetrieveResponse {
        std::string id{};                           ///< Wire field id.
        std::int64_t created_at{};                  ///< Wire field created_at.
        Json::Value data_source{};                  ///< Wire field data_source.
        Json::Value error{};                        ///< Wire field error.
        std::string eval_id{};                      ///< Wire field eval_id.
        std::optional<Json::Value> metadata{};      ///< Wire field metadata.
        std::string model{};                        ///< Wire field model.
        std::string name{};                         ///< Wire field name.
        std::string object{};                       ///< Wire field object.
        Json::Value per_model_usage{};              ///< Wire field per_model_usage.
        Json::Value per_testing_criteria_results{}; ///< Wire field per_testing_criteria_results.
        std::string report_url{};                   ///< Wire field report_url.
        Json::Value result_counts{};                ///< Wire field result_counts.
        std::string status{};                       ///< Wire field status.
        Json::Value raw{};                          ///< Complete JSON including provider extensions.
        std::string _request_id;                    ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<RunRetrieveResponse>;
    };

    /**
     * @brief Response RunListResponse, retaining SDK union fields as JSON.
     */
    struct RunListResponse {
        std::string id{};                           ///< Wire field id.
        std::int64_t created_at{};                  ///< Wire field created_at.
        Json::Value data_source{};                  ///< Wire field data_source.
        Json::Value error{};                        ///< Wire field error.
        std::string eval_id{};                      ///< Wire field eval_id.
        std::optional<Json::Value> metadata{};      ///< Wire field metadata.
        std::string model{};                        ///< Wire field model.
        std::string name{};                         ///< Wire field name.
        std::string object{};                       ///< Wire field object.
        Json::Value per_model_usage{};              ///< Wire field per_model_usage.
        Json::Value per_testing_criteria_results{}; ///< Wire field per_testing_criteria_results.
        std::string report_url{};                   ///< Wire field report_url.
        Json::Value result_counts{};                ///< Wire field result_counts.
        std::string status{};                       ///< Wire field status.
        Json::Value raw{};                          ///< Complete JSON including provider extensions.
        std::string _request_id;                    ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<RunListResponse>;
    };

    /**
     * @brief Response RunDeleteResponse, retaining SDK union fields as JSON.
     */
    struct RunDeleteResponse {
        std::optional<Json::Value> deleted{}; ///< Wire field deleted.
        std::optional<Json::Value> object{};  ///< Wire field object.
        std::optional<Json::Value> run_id{};  ///< Wire field run_id.
        Json::Value raw{};                    ///< Complete JSON including provider extensions.
        std::string _request_id;              ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<RunDeleteResponse>;
    };

    /**
     * @brief Response RunCancelResponse, retaining SDK union fields as JSON.
     */
    struct RunCancelResponse {
        std::string id{};                           ///< Wire field id.
        std::int64_t created_at{};                  ///< Wire field created_at.
        Json::Value data_source{};                  ///< Wire field data_source.
        Json::Value error{};                        ///< Wire field error.
        std::string eval_id{};                      ///< Wire field eval_id.
        std::optional<Json::Value> metadata{};      ///< Wire field metadata.
        std::string model{};                        ///< Wire field model.
        std::string name{};                         ///< Wire field name.
        std::string object{};                       ///< Wire field object.
        Json::Value per_model_usage{};              ///< Wire field per_model_usage.
        Json::Value per_testing_criteria_results{}; ///< Wire field per_testing_criteria_results.
        std::string report_url{};                   ///< Wire field report_url.
        Json::Value result_counts{};                ///< Wire field result_counts.
        std::string status{};                       ///< Wire field status.
        Json::Value raw{};                          ///< Complete JSON including provider extensions.
        std::string _request_id;                    ///< HTTP x-request-id.

        /**
         * @brief Decode a resource without discarding unknown fields.
         * @param value Resource JSON.
         * @return Typed resource or validation error.
         */
        [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<RunCancelResponse>;
    };

    /**
     * @brief Parameters for EvalCreateParams; omitted optionals are absent on the wire.
     */
    struct EvalCreateParams {
        Json::Value data_source_config{};      ///< Required SDK field data_source_config.
        Json::Value testing_criteria{};        ///< Required SDK field testing_criteria.
        std::optional<Json::Value> metadata{}; ///< Optional SDK field metadata.
        std::optional<std::string> name{};     ///< Optional SDK field name.
        Json::Value extra_body{};              ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};      ///< Override phase timeouts.
        std::optional<int> max_retries{};      ///< Override retries.
        Headers extra_headers{};               ///< Override request headers.
        Json::Value extra_query{};             ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for EvalsRetrieveParams; omitted optionals are absent on the wire.
     */
    struct EvalsRetrieveParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for EvalUpdateParams; omitted optionals are absent on the wire.
     */
    struct EvalUpdateParams {
        std::optional<Json::Value> metadata{}; ///< Optional SDK field metadata.
        std::optional<std::string> name{};     ///< Optional SDK field name.
        Json::Value extra_body{};              ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};      ///< Override phase timeouts.
        std::optional<int> max_retries{};      ///< Override retries.
        Headers extra_headers{};               ///< Override request headers.
        Json::Value extra_query{};             ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for EvalListParams; omitted optionals are absent on the wire.
     */
    struct EvalListParams {
        std::optional<std::string> after{};    ///< Optional SDK field after.
        std::optional<std::int64_t> limit{};   ///< Optional SDK field limit.
        std::optional<std::string> order{};    ///< Optional SDK field order.
        std::optional<std::string> order_by{}; ///< Optional SDK field order_by.
        Json::Value extra_body{};              ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};      ///< Override phase timeouts.
        std::optional<int> max_retries{};      ///< Override retries.
        Headers extra_headers{};               ///< Override request headers.
        Json::Value extra_query{};             ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for EvalsDeleteParams; omitted optionals are absent on the wire.
     */
    struct EvalsDeleteParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for EvalsRunsOutputItemsRetrieveParams; omitted optionals are absent on the wire.
     */
    struct EvalsRunsOutputItemsRetrieveParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for OutputItemListParams; omitted optionals are absent on the wire.
     */
    struct OutputItemListParams {
        std::optional<std::string> after{};  ///< Optional SDK field after.
        std::optional<std::int64_t> limit{}; ///< Optional SDK field limit.
        std::optional<std::string> order{};  ///< Optional SDK field order.
        std::optional<std::string> status{}; ///< Optional SDK field status.
        Json::Value extra_body{};            ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};    ///< Override phase timeouts.
        std::optional<int> max_retries{};    ///< Override retries.
        Headers extra_headers{};             ///< Override request headers.
        Json::Value extra_query{};           ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for RunCreateParams; omitted optionals are absent on the wire.
     */
    struct RunCreateParams {
        Json::Value data_source{};             ///< Required SDK field data_source.
        std::optional<Json::Value> metadata{}; ///< Optional SDK field metadata.
        std::optional<std::string> name{};     ///< Optional SDK field name.
        Json::Value extra_body{};              ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};      ///< Override phase timeouts.
        std::optional<int> max_retries{};      ///< Override retries.
        Headers extra_headers{};               ///< Override request headers.
        Json::Value extra_query{};             ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for EvalsRunsRetrieveParams; omitted optionals are absent on the wire.
     */
    struct EvalsRunsRetrieveParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for RunListParams; omitted optionals are absent on the wire.
     */
    struct RunListParams {
        std::optional<std::string> after{};  ///< Optional SDK field after.
        std::optional<std::int64_t> limit{}; ///< Optional SDK field limit.
        std::optional<std::string> order{};  ///< Optional SDK field order.
        std::optional<std::string> status{}; ///< Optional SDK field status.
        Json::Value extra_body{};            ///< Body extensions, merged last.
        std::optional<Timeout> timeout{};    ///< Override phase timeouts.
        std::optional<int> max_retries{};    ///< Override retries.
        Headers extra_headers{};             ///< Override request headers.
        Json::Value extra_query{};           ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for EvalsRunsDeleteParams; omitted optionals are absent on the wire.
     */
    struct EvalsRunsDeleteParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };

    /**
     * @brief Parameters for EvalsRunsCancelParams; omitted optionals are absent on the wire.
     */
    struct EvalsRunsCancelParams {
        Json::Value extra_body{};         ///< Body extensions, merged last.
        std::optional<Timeout> timeout{}; ///< Override phase timeouts.
        std::optional<int> max_retries{}; ///< Override retries.
        Headers extra_headers{};          ///< Override request headers.
        Json::Value extra_query{};        ///< Override query parameters.

        /**
         * @brief Serialize fields and files for transport.
         * @return Owned payload.
         */
        [[nodiscard]] auto Payload() const -> ResourcePayload;

        /**
         * @brief Extract per-request HTTP settings.
         * @return Request overrides.
         */
        [[nodiscard]] auto Options() const -> RequestOptions;
    };
} // namespace drogon_openai::types::evals
