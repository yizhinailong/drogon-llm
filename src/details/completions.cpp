/**
 * @file completions.cpp
 * @brief Implement the chat completions API group that `Chat::completions` exposes.
 */
#include "drogon_llm/details/completions.hpp"

#include <utility>

#include "details/transport.hpp"

namespace drogon_llm::details {

    namespace {

        /**
         * @brief Add `value` to `body` under `key` when it is set.
         * @tparam T Value type that `Json::Value` can hold.
         * @param body Request body to fill.
         * @param key Field name.
         * @param value Optional field value.
         */
        template <typename T>
        auto set_if(Json::Value& body, char const* key, std::optional<T> const& value) -> void {
            if (value) {
                body[key] = *value;
            }
        }

        /**
         * @brief Read a JSON string.
         * @param value Field that should hold a string.
         * @return The string, or unset if `value` is missing or not a string.
         */
        auto optional_string(Json::Value const& value) -> std::optional<std::string> {
            if (!value.isString()) {
                return std::nullopt;
            }
            return value.asString();
        }

        /**
         * @brief Read a JSON integer.
         * @param value Field that should hold an integer.
         * @return The integer, or 0 if `value` is missing or not a 64-bit integer.
         */
        auto int64_or_zero(Json::Value const& value) -> std::int64_t {
            return value.isInt64() ? value.asInt64() : 0;
        }

        /**
         * @brief Build the `/chat/completions` request body.
         * @param params Request fields.
         * @param default_model Model sent when `params.model` is unset.
         * @return OpenAI-format request body.
         */
        auto to_request_body(
            ChatCompletionCreateParams const& params,
            std::string const& default_model
        ) -> Json::Value {
            Json::Value body{ Json::objectValue };
            body["model"] = params.model.value_or(default_model);
            body["messages"] = Json::arrayValue;
            for (auto const& message : params.messages) {
                Json::Value item{ Json::objectValue };
                item["role"] = message.role;
                item["content"] = message.content;
                body["messages"].append(std::move(item));
            }
            set_if(body, "frequency_penalty", params.frequency_penalty);
            set_if(body, "max_tokens", params.max_tokens);
            set_if(body, "presence_penalty", params.presence_penalty);
            set_if(body, "temperature", params.temperature);
            set_if(body, "top_p", params.top_p);
            if (params.extra_body.isObject()) {
                for (auto const& name : params.extra_body.getMemberNames()) {
                    body[name] = params.extra_body[name];
                }
            }
            return body;
        }

        /**
         * @brief Build a `ChatCompletion` from the `/chat/completions` response body.
         * @param root Parsed response body.
         * @return The completion, or a `ResponseValidation` error if `choices` is not an
         * array of objects that each hold a `message` object.
         */
        auto parse_chat_completion(Json::Value const& root) -> Result<ChatCompletion> {
            auto const invalid{ [&root](char const* message) {
                return std::unexpected{ Error{
                    .kind = ErrorKind::ResponseValidation,
                    .message = message,
                    .body = root.toStyledString(),
                } };
            } };

            if (!root.isObject() || !root["choices"].isArray()) {
                return invalid("chat completion has no choices array");
            }
            ChatCompletion completion{
                .id = optional_string(root["id"]).value_or(""),
                .choices = {},
                .created = int64_or_zero(root["created"]),
                .model = optional_string(root["model"]).value_or(""),
                .object = optional_string(root["object"]).value_or(""),
            };
            for (auto const& choice : root["choices"]) {
                if (!choice.isObject() || !choice["message"].isObject()) {
                    return invalid("chat completion has a choice without a message object");
                }
                Json::Value const& message{ choice["message"] };
                completion.choices.push_back(ChatCompletionChoice{
                    .finish_reason = optional_string(choice["finish_reason"]).value_or(""),
                    .index = int64_or_zero(choice["index"]),
                    .message = {
                        .role = optional_string(message["role"]).value_or(""),
                        .content = optional_string(message["content"]),
                        .reasoning_content = optional_string(message["reasoning_content"]),
                    },
                });
            }
            if (Json::Value const& usage{ root["usage"] }; usage.isObject()) {
                completion.usage = CompletionUsage{
                    .completion_tokens = int64_or_zero(usage["completion_tokens"]),
                    .prompt_tokens = int64_or_zero(usage["prompt_tokens"]),
                    .total_tokens = int64_or_zero(usage["total_tokens"]),
                };
            }
            return completion;
        }

    } // namespace

    /**
     * @brief Bind the group to an endpoint.
     * @param transport Connection shared by the API groups of one `Llm`.
     */
    Completions::Completions(std::shared_ptr<Transport> transport)
        : m_transport{ std::move(transport) } {}

    /**
     * @brief Generate a reply to a conversation, like `await client.chat.completions.create()`.
     * @param params Request fields, taken by value so they outlive the caller's frame.
     * @return Task yielding the completion, or why the request failed.
     * @note The request runs on the calling thread's event loop (the main loop when the
     * thread runs none), so the coroutine resumes on the thread that awaited it. To block
     * on it with `drogon::sync_wait()`, call from a thread that runs no event loop.
     */
    auto Completions::Create(
        ChatCompletionCreateParams params
    ) const -> drogon::Task<Result<ChatCompletion>> {
        // Keep the transport in the coroutine frame in case this object dies mid-request.
        auto const transport{ m_transport };
        auto body{ to_request_body(params, transport->Config().model) };
        auto const response{ co_await transport->Post("/chat/completions", std::move(body), params.timeout) };
        co_return response.and_then(parse_chat_completion);
    }

} // namespace drogon_llm::details
