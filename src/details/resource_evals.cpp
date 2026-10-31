/**
 * @file resource_evals.cpp
 * @brief Implement evals resource bindings generated from openai-python 3.41.0.
 */
#include <utility>

#include "details/resource_transport.hpp"
#include "drogon_openai/details/evals.hpp"

namespace drogon_openai::types::evals {

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto EvalCreateResponse::FromJson(Json::Value const& value) -> Result<EvalCreateResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        EvalCreateResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalCreateResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalCreateResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("data_source_config")) {
            if (!details::resource_read(value["data_source_config"], result.data_source_config)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalCreateResponse.data_source_config", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("metadata") && !value["metadata"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["metadata"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalCreateResponse.metadata", .body = value.toStyledString() } };
            }
            result.metadata = std::move(field);
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalCreateResponse.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalCreateResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("testing_criteria")) {
            if (!details::resource_read(value["testing_criteria"], result.testing_criteria)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalCreateResponse.testing_criteria", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto EvalRetrieveResponse::FromJson(Json::Value const& value) -> Result<EvalRetrieveResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        EvalRetrieveResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalRetrieveResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalRetrieveResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("data_source_config")) {
            if (!details::resource_read(value["data_source_config"], result.data_source_config)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalRetrieveResponse.data_source_config", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("metadata") && !value["metadata"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["metadata"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalRetrieveResponse.metadata", .body = value.toStyledString() } };
            }
            result.metadata = std::move(field);
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalRetrieveResponse.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalRetrieveResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("testing_criteria")) {
            if (!details::resource_read(value["testing_criteria"], result.testing_criteria)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalRetrieveResponse.testing_criteria", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto EvalUpdateResponse::FromJson(Json::Value const& value) -> Result<EvalUpdateResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        EvalUpdateResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalUpdateResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalUpdateResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("data_source_config")) {
            if (!details::resource_read(value["data_source_config"], result.data_source_config)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalUpdateResponse.data_source_config", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("metadata") && !value["metadata"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["metadata"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalUpdateResponse.metadata", .body = value.toStyledString() } };
            }
            result.metadata = std::move(field);
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalUpdateResponse.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalUpdateResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("testing_criteria")) {
            if (!details::resource_read(value["testing_criteria"], result.testing_criteria)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalUpdateResponse.testing_criteria", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto EvalListResponse::FromJson(Json::Value const& value) -> Result<EvalListResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        EvalListResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalListResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalListResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("data_source_config")) {
            if (!details::resource_read(value["data_source_config"], result.data_source_config)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalListResponse.data_source_config", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("metadata") && !value["metadata"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["metadata"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalListResponse.metadata", .body = value.toStyledString() } };
            }
            result.metadata = std::move(field);
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalListResponse.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalListResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("testing_criteria")) {
            if (!details::resource_read(value["testing_criteria"], result.testing_criteria)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalListResponse.testing_criteria", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto EvalDeleteResponse::FromJson(Json::Value const& value) -> Result<EvalDeleteResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        EvalDeleteResponse result;
        result.raw = value;
        if (value.isMember("deleted")) {
            if (!details::resource_read(value["deleted"], result.deleted)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalDeleteResponse.deleted", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("eval_id")) {
            if (!details::resource_read(value["eval_id"], result.eval_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalDeleteResponse.eval_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid EvalDeleteResponse.object", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto OutputItemRetrieveResponse::FromJson(Json::Value const& value) -> Result<OutputItemRetrieveResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        OutputItemRetrieveResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemRetrieveResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemRetrieveResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("datasource_item")) {
            if (!details::resource_read(value["datasource_item"], result.datasource_item)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemRetrieveResponse.datasource_item", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("datasource_item_id")) {
            if (!details::resource_read(value["datasource_item_id"], result.datasource_item_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemRetrieveResponse.datasource_item_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("eval_id")) {
            if (!details::resource_read(value["eval_id"], result.eval_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemRetrieveResponse.eval_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemRetrieveResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("results")) {
            if (!details::resource_read(value["results"], result.results)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemRetrieveResponse.results", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("run_id")) {
            if (!details::resource_read(value["run_id"], result.run_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemRetrieveResponse.run_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("sample")) {
            if (!details::resource_read(value["sample"], result.sample)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemRetrieveResponse.sample", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemRetrieveResponse.status", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto OutputItemListResponse::FromJson(Json::Value const& value) -> Result<OutputItemListResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        OutputItemListResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemListResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemListResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("datasource_item")) {
            if (!details::resource_read(value["datasource_item"], result.datasource_item)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemListResponse.datasource_item", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("datasource_item_id")) {
            if (!details::resource_read(value["datasource_item_id"], result.datasource_item_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemListResponse.datasource_item_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("eval_id")) {
            if (!details::resource_read(value["eval_id"], result.eval_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemListResponse.eval_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemListResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("results")) {
            if (!details::resource_read(value["results"], result.results)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemListResponse.results", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("run_id")) {
            if (!details::resource_read(value["run_id"], result.run_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemListResponse.run_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("sample")) {
            if (!details::resource_read(value["sample"], result.sample)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemListResponse.sample", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid OutputItemListResponse.status", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto RunCreateResponse::FromJson(Json::Value const& value) -> Result<RunCreateResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        RunCreateResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCreateResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCreateResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("data_source")) {
            if (!details::resource_read(value["data_source"], result.data_source)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCreateResponse.data_source", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("error")) {
            if (!details::resource_read(value["error"], result.error)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCreateResponse.error", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("eval_id")) {
            if (!details::resource_read(value["eval_id"], result.eval_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCreateResponse.eval_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("metadata") && !value["metadata"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["metadata"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCreateResponse.metadata", .body = value.toStyledString() } };
            }
            result.metadata = std::move(field);
        }
        if (value.isMember("model")) {
            if (!details::resource_read(value["model"], result.model)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCreateResponse.model", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCreateResponse.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCreateResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("per_model_usage")) {
            if (!details::resource_read(value["per_model_usage"], result.per_model_usage)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCreateResponse.per_model_usage", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("per_testing_criteria_results")) {
            if (!details::resource_read(value["per_testing_criteria_results"], result.per_testing_criteria_results)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCreateResponse.per_testing_criteria_results", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("report_url")) {
            if (!details::resource_read(value["report_url"], result.report_url)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCreateResponse.report_url", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("result_counts")) {
            if (!details::resource_read(value["result_counts"], result.result_counts)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCreateResponse.result_counts", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCreateResponse.status", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto RunRetrieveResponse::FromJson(Json::Value const& value) -> Result<RunRetrieveResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        RunRetrieveResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunRetrieveResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunRetrieveResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("data_source")) {
            if (!details::resource_read(value["data_source"], result.data_source)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunRetrieveResponse.data_source", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("error")) {
            if (!details::resource_read(value["error"], result.error)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunRetrieveResponse.error", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("eval_id")) {
            if (!details::resource_read(value["eval_id"], result.eval_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunRetrieveResponse.eval_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("metadata") && !value["metadata"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["metadata"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunRetrieveResponse.metadata", .body = value.toStyledString() } };
            }
            result.metadata = std::move(field);
        }
        if (value.isMember("model")) {
            if (!details::resource_read(value["model"], result.model)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunRetrieveResponse.model", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunRetrieveResponse.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunRetrieveResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("per_model_usage")) {
            if (!details::resource_read(value["per_model_usage"], result.per_model_usage)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunRetrieveResponse.per_model_usage", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("per_testing_criteria_results")) {
            if (!details::resource_read(value["per_testing_criteria_results"], result.per_testing_criteria_results)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunRetrieveResponse.per_testing_criteria_results", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("report_url")) {
            if (!details::resource_read(value["report_url"], result.report_url)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunRetrieveResponse.report_url", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("result_counts")) {
            if (!details::resource_read(value["result_counts"], result.result_counts)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunRetrieveResponse.result_counts", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunRetrieveResponse.status", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto RunListResponse::FromJson(Json::Value const& value) -> Result<RunListResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        RunListResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunListResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunListResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("data_source")) {
            if (!details::resource_read(value["data_source"], result.data_source)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunListResponse.data_source", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("error")) {
            if (!details::resource_read(value["error"], result.error)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunListResponse.error", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("eval_id")) {
            if (!details::resource_read(value["eval_id"], result.eval_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunListResponse.eval_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("metadata") && !value["metadata"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["metadata"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunListResponse.metadata", .body = value.toStyledString() } };
            }
            result.metadata = std::move(field);
        }
        if (value.isMember("model")) {
            if (!details::resource_read(value["model"], result.model)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunListResponse.model", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunListResponse.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunListResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("per_model_usage")) {
            if (!details::resource_read(value["per_model_usage"], result.per_model_usage)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunListResponse.per_model_usage", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("per_testing_criteria_results")) {
            if (!details::resource_read(value["per_testing_criteria_results"], result.per_testing_criteria_results)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunListResponse.per_testing_criteria_results", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("report_url")) {
            if (!details::resource_read(value["report_url"], result.report_url)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunListResponse.report_url", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("result_counts")) {
            if (!details::resource_read(value["result_counts"], result.result_counts)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunListResponse.result_counts", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunListResponse.status", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto RunDeleteResponse::FromJson(Json::Value const& value) -> Result<RunDeleteResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        RunDeleteResponse result;
        result.raw = value;
        if (value.isMember("deleted") && !value["deleted"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["deleted"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunDeleteResponse.deleted", .body = value.toStyledString() } };
            }
            result.deleted = std::move(field);
        }
        if (value.isMember("object") && !value["object"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["object"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunDeleteResponse.object", .body = value.toStyledString() } };
            }
            result.object = std::move(field);
        }
        if (value.isMember("run_id") && !value["run_id"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["run_id"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunDeleteResponse.run_id", .body = value.toStyledString() } };
            }
            result.run_id = std::move(field);
        }
        return result;
    }

    /**
     * @brief Decode a resource without discarding unknown fields.
     * @param value Resource JSON.
     * @return Typed resource or validation error.
     */
    auto RunCancelResponse::FromJson(Json::Value const& value) -> Result<RunCancelResponse> {
        if (!value.isObject()) {
            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } };
        }
        RunCancelResponse result;
        result.raw = value;
        if (value.isMember("id")) {
            if (!details::resource_read(value["id"], result.id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCancelResponse.id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("created_at")) {
            if (!details::resource_read(value["created_at"], result.created_at)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCancelResponse.created_at", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("data_source")) {
            if (!details::resource_read(value["data_source"], result.data_source)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCancelResponse.data_source", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("error")) {
            if (!details::resource_read(value["error"], result.error)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCancelResponse.error", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("eval_id")) {
            if (!details::resource_read(value["eval_id"], result.eval_id)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCancelResponse.eval_id", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("metadata") && !value["metadata"].isNull()) {
            Json::Value field{};
            if (!details::resource_read(value["metadata"], field)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCancelResponse.metadata", .body = value.toStyledString() } };
            }
            result.metadata = std::move(field);
        }
        if (value.isMember("model")) {
            if (!details::resource_read(value["model"], result.model)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCancelResponse.model", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("name")) {
            if (!details::resource_read(value["name"], result.name)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCancelResponse.name", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("object")) {
            if (!details::resource_read(value["object"], result.object)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCancelResponse.object", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("per_model_usage")) {
            if (!details::resource_read(value["per_model_usage"], result.per_model_usage)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCancelResponse.per_model_usage", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("per_testing_criteria_results")) {
            if (!details::resource_read(value["per_testing_criteria_results"], result.per_testing_criteria_results)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCancelResponse.per_testing_criteria_results", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("report_url")) {
            if (!details::resource_read(value["report_url"], result.report_url)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCancelResponse.report_url", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("result_counts")) {
            if (!details::resource_read(value["result_counts"], result.result_counts)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCancelResponse.result_counts", .body = value.toStyledString() } };
            }
        }
        if (value.isMember("status")) {
            if (!details::resource_read(value["status"], result.status)) {
                return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "invalid RunCancelResponse.status", .body = value.toStyledString() } };
            }
        }
        return result;
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto EvalCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["data_source_config"] = details::resource_json(data_source_config);
        payload.body["testing_criteria"] = details::resource_json(testing_criteria);
        if (metadata) {
            payload.body["metadata"] = details::resource_json(*metadata);
        }
        if (name) {
            payload.body["name"] = details::resource_json(*name);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto EvalCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto EvalsRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto EvalsRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto EvalUpdateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (metadata) {
            payload.body["metadata"] = details::resource_json(*metadata);
        }
        if (name) {
            payload.body["name"] = details::resource_json(*name);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto EvalUpdateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto EvalListParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (after) {
            payload.query["after"] = details::resource_json(*after);
        }
        if (limit) {
            payload.query["limit"] = details::resource_json(*limit);
        }
        if (order) {
            payload.query["order"] = details::resource_json(*order);
        }
        if (order_by) {
            payload.query["order_by"] = details::resource_json(*order_by);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto EvalListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto EvalsDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto EvalsDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto EvalsRunsOutputItemsRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto EvalsRunsOutputItemsRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto OutputItemListParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (after) {
            payload.query["after"] = details::resource_json(*after);
        }
        if (limit) {
            payload.query["limit"] = details::resource_json(*limit);
        }
        if (order) {
            payload.query["order"] = details::resource_json(*order);
        }
        if (status) {
            payload.query["status"] = details::resource_json(*status);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto OutputItemListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto RunCreateParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.body["data_source"] = details::resource_json(data_source);
        if (metadata) {
            payload.body["metadata"] = details::resource_json(*metadata);
        }
        if (name) {
            payload.body["name"] = details::resource_json(*name);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto RunCreateParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto EvalsRunsRetrieveParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto EvalsRunsRetrieveParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto RunListParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        if (after) {
            payload.query["after"] = details::resource_json(*after);
        }
        if (limit) {
            payload.query["limit"] = details::resource_json(*limit);
        }
        if (order) {
            payload.query["order"] = details::resource_json(*order);
        }
        if (status) {
            payload.query["status"] = details::resource_json(*status);
        }
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto RunListParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto EvalsRunsDeleteParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto EvalsRunsDeleteParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }

    /**
     * @brief Serialize fields and files for transport.
     * @return Owned payload.
     */
    auto EvalsRunsCancelParams::Payload() const -> ResourcePayload {
        ResourcePayload payload;
        payload.extra_body = extra_body;
        return payload;
    }

    /**
     * @brief Extract per-request HTTP settings.
     * @return Request overrides.
     */
    auto EvalsRunsCancelParams::Options() const -> RequestOptions {
        return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };
    }
} // namespace drogon_openai::types::evals

namespace drogon_openai::details {

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    EvalsRunsOutputItems::EvalsRunsOutputItems(std::shared_ptr<Transport> transport) : m_transport{ transport } {}

    /**
     * @brief GET /evals/{eval_id}/runs/{run_id}/output_items/{output_item_id}.
     * @param output_item_id Owned path identifier, encoded as one segment.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param run_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto EvalsRunsOutputItems::Retrieve(
        std::string output_item_id,
        std::string eval_id,
        std::string run_id,
        types::evals::EvalsRunsOutputItemsRetrieveParams params
    ) const -> drogon::Task<Result<types::evals::OutputItemRetrieveResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}/runs/{run_id}/output_items/{output_item_id}", { { "output_item_id", output_item_id }, { "eval_id", eval_id }, { "run_id", run_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::OutputItemRetrieveResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /evals/{eval_id}/runs/{run_id}/output_items/{output_item_id}.
     * @param output_item_id Owned path identifier, encoded as one segment.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param run_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto EvalsRunsOutputItems::RetrieveRaw(
        std::string output_item_id,
        std::string eval_id,
        std::string run_id,
        types::evals::EvalsRunsOutputItemsRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::evals::OutputItemRetrieveResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}/runs/{run_id}/output_items/{output_item_id}", { { "output_item_id", output_item_id }, { "eval_id", eval_id }, { "run_id", run_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::OutputItemRetrieveResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /evals/{eval_id}/runs/{run_id}/output_items.
     * @param run_id Owned path identifier, encoded as one segment.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto EvalsRunsOutputItems::List(
        std::string run_id,
        std::string eval_id,
        types::evals::OutputItemListParams params
    ) const -> drogon::Task<Result<Page<types::evals::OutputItemListResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}/runs/{run_id}/output_items", { { "run_id", run_id }, { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::evals::OutputItemListResponse>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    EvalsRuns::EvalsRuns(std::shared_ptr<Transport> transport) : m_transport{ transport }, output_items{ transport } {}

    /**
     * @brief POST /evals/{eval_id}/runs.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto EvalsRuns::Create(
        std::string eval_id,
        types::evals::RunCreateParams params
    ) const -> drogon::Task<Result<types::evals::RunCreateResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}/runs", { { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::RunCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /evals/{eval_id}/runs.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto EvalsRuns::CreateRaw(
        std::string eval_id,
        types::evals::RunCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::evals::RunCreateResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}/runs", { { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::RunCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /evals/{eval_id}/runs/{run_id}.
     * @param run_id Owned path identifier, encoded as one segment.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto EvalsRuns::Retrieve(
        std::string run_id,
        std::string eval_id,
        types::evals::EvalsRunsRetrieveParams params
    ) const -> drogon::Task<Result<types::evals::RunRetrieveResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}/runs/{run_id}", { { "run_id", run_id }, { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::RunRetrieveResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /evals/{eval_id}/runs/{run_id}.
     * @param run_id Owned path identifier, encoded as one segment.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto EvalsRuns::RetrieveRaw(
        std::string run_id,
        std::string eval_id,
        types::evals::EvalsRunsRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::evals::RunRetrieveResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}/runs/{run_id}", { { "run_id", run_id }, { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::RunRetrieveResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /evals/{eval_id}/runs.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto EvalsRuns::List(
        std::string eval_id,
        types::evals::RunListParams params
    ) const -> drogon::Task<Result<Page<types::evals::RunListResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}/runs", { { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::evals::RunListResponse>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true);
    }

    /**
     * @brief DELETE /evals/{eval_id}/runs/{run_id}.
     * @param run_id Owned path identifier, encoded as one segment.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto EvalsRuns::Delete(
        std::string run_id,
        std::string eval_id,
        types::evals::EvalsRunsDeleteParams params
    ) const -> drogon::Task<Result<types::evals::RunDeleteResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}/runs/{run_id}", { { "run_id", run_id }, { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::RunDeleteResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief DELETE /evals/{eval_id}/runs/{run_id}.
     * @param run_id Owned path identifier, encoded as one segment.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto EvalsRuns::DeleteRaw(
        std::string run_id,
        std::string eval_id,
        types::evals::EvalsRunsDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<types::evals::RunDeleteResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}/runs/{run_id}", { { "run_id", run_id }, { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::RunDeleteResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /evals/{eval_id}/runs/{run_id}/cancel.
     * @param run_id Owned path identifier, encoded as one segment.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto EvalsRuns::Cancel(
        std::string run_id,
        std::string eval_id,
        types::evals::EvalsRunsCancelParams params
    ) const -> drogon::Task<Result<types::evals::RunCancelResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}/runs/{run_id}/cancel", { { "run_id", run_id }, { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::RunCancelResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /evals/{eval_id}/runs/{run_id}/cancel.
     * @param run_id Owned path identifier, encoded as one segment.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto EvalsRuns::CancelRaw(
        std::string run_id,
        std::string eval_id,
        types::evals::EvalsRunsCancelParams params
    ) const -> drogon::Task<Result<RawResponse<types::evals::RunCancelResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}/runs/{run_id}/cancel", { { "run_id", run_id }, { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::RunCancelResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief Bind this resource and its children to an endpoint.
     * @param transport Shared endpoint settings.
     */
    Evals::Evals(std::shared_ptr<Transport> transport) : m_transport{ transport }, runs{ transport } {}

    /**
     * @brief POST /evals.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Evals::Create(
        types::evals::EvalCreateParams params
    ) const -> drogon::Task<Result<types::evals::EvalCreateResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::EvalCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /evals.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Evals::CreateRaw(
        types::evals::EvalCreateParams params
    ) const -> drogon::Task<Result<RawResponse<types::evals::EvalCreateResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::EvalCreateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /evals/{eval_id}.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Evals::Retrieve(
        std::string eval_id,
        types::evals::EvalsRetrieveParams params
    ) const -> drogon::Task<Result<types::evals::EvalRetrieveResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}", { { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::EvalRetrieveResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief GET /evals/{eval_id}.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Evals::RetrieveRaw(
        std::string eval_id,
        types::evals::EvalsRetrieveParams params
    ) const -> drogon::Task<Result<RawResponse<types::evals::EvalRetrieveResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}", { { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::EvalRetrieveResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief POST /evals/{eval_id}.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Evals::Update(
        std::string eval_id,
        types::evals::EvalUpdateParams params
    ) const -> drogon::Task<Result<types::evals::EvalUpdateResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}", { { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::EvalUpdateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief POST /evals/{eval_id}.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Evals::UpdateRaw(
        std::string eval_id,
        types::evals::EvalUpdateParams params
    ) const -> drogon::Task<Result<RawResponse<types::evals::EvalUpdateResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}", { { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "POST", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::EvalUpdateResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }

    /**
     * @brief GET /evals.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Evals::List(
        types::evals::EvalListParams params
    ) const -> drogon::Task<Result<Page<types::evals::EvalListResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals", {}) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        co_return co_await resource_page<types::evals::EvalListResponse>(transport, "GET", std::move(*path), std::move(payload), params.Options(), {}, true);
    }

    /**
     * @brief DELETE /evals/{eval_id}.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Evals::Delete(
        std::string eval_id,
        types::evals::EvalsDeleteParams params
    ) const -> drogon::Task<Result<types::evals::EvalDeleteResponse>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}", { { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::EvalDeleteResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(parsed->parsed);
    }

    /**
     * @brief DELETE /evals/{eval_id}.
     * @param eval_id Owned path identifier, encoded as one segment.
     * @param params Owned SDK parameters and request overrides.
     * @return API result or request/validation error.
     */
    auto Evals::DeleteRaw(
        std::string eval_id,
        types::evals::EvalsDeleteParams params
    ) const -> drogon::Task<Result<RawResponse<types::evals::EvalDeleteResponse>>> {
        auto transport{ m_transport };
        auto payload{ params.Payload() };
        payload.multipart = false || !payload.files.empty();
        auto path{ resource_path("/evals/{eval_id}", { { "eval_id", eval_id } }) };
        if (!path) {
            co_return std::unexpected{ std::move(path.error()) };
        }
        auto response{ co_await resource_request(transport, "DELETE", std::move(*path), std::move(payload), params.Options(), {}) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ resource_decode_response<types::evals::EvalDeleteResponse>(std::move(*response)) };
        if (!parsed) {
            co_return std::unexpected{ std::move(parsed.error()) };
        }
        co_return std::move(*parsed);
    }
} // namespace drogon_openai::details
