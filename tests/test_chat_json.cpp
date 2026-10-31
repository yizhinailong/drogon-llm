/**
 * @file test_chat_json.cpp
 * @brief Check complete chat wire fields, round trips, and structured parsing contracts.
 */
#include <stdexcept>

#include <drogon/drogon_test.h>

#include "../src/details/chat_json.hpp"
#include "../src/details/transport.hpp"

using namespace drogon_openai;
namespace chat_json = drogon_openai::details;

namespace {
    /**
     * @brief Decode a literal JSON test fixture.
     * @param value Fixture bytes.
     * @return Parsed value, throwing for a broken fixture.
     */
    auto json(std::string const& value) -> Json::Value {
        return chat_json::decode_json(value).value();
    }

    /**
     * @brief Build a response fixture containing every supported response field.
     * @return Complete completion JSON.
     */
    auto full_completion() -> Json::Value {
        return json(R"JSON({
          "id":"chat-test","created":123,"model":"test-model","object":"chat.completion",
          "system_fingerprint":"fp","service_tier":"priority","metadata":{"test":"yes"},"moderation":{"flagged":false},"vendor":7,
          "choices":[{"index":0,"finish_reason":"stop","message":{
            "role":"assistant","content":"{\"answer\":42}","reasoning_content":"reason","refusal":null,
            "tool_calls":[{"id":"call-1","type":"function","function":{"name":"weather","arguments":"{\"city\":\"Paris\"}"}},
                          {"id":"call-2","type":"custom","custom":{"name":"code","input":"print(1)"}
}],
            "annotations":[{"type":"url_citation","url_citation":{"url":"https://example.com","title":"Example","start_index":0,"end_index":4}}],
            "audio":{"id":"audio-1","expires_at":456,"data":"YQ==","transcript":"hi"},"vendor_message":"kept"
}
, "logprobs" : {
    "content" : [ { "token" : "x", "logprob" : -0.5, "bytes" : [120], "top_logprobs" : [ { "token" : "y", "logprob" : -1.0, "bytes" : null } ] } ],
                "refusal" : [ { "token" : "no", "logprob" : -2, "bytes" : [ 110, 111 ], "top_logprobs" : [] } ]
}
}],
          "usage":{
    "prompt_tokens" : 10, "completion_tokens" : 20, "total_tokens" : 30, "prompt_cache_hit_tokens" : 9,
        "completion_tokens_details" : { "reasoning_tokens" : 1, "accepted_prediction_tokens" : 2, "rejected_prediction_tokens" : 3, "audio_tokens" : 4, "text_tokens" : 5 },
                                      "prompt_tokens_details" : { "cached_tokens" : 6, "cache_write_tokens" : 7, "audio_tokens" : 8, "image_tokens" : 9, "text_tokens" : 10 }
}
})JSON");
    }
} // namespace

DROGON_TEST(AllRequestFieldsSerializeAndUnsetStayOmitted) {
    ChatCompletionCreateParams params{ .messages = { { .role = "user", .content = "hello" } } };
    auto empty{ chat_json::to_request_body(params, "configured") };
    CHECK(empty.size() == 2);
    CHECK(empty["model"] == "configured");
    params.model = "override";
    params.frequency_penalty = 0.5;
    params.max_tokens = 10;
    params.presence_penalty = 0.6;
    params.temperature = 0.7;
    params.top_p = 0.8;
    params.tools = std::vector<ChatCompletionToolParam>{ { .function = json(R"JSON({"name":"weather","strict":true,"parameters":{"type":"object"}})JSON") }, { .type = "custom", .custom = json(R"JSON({"name":"code","format":{"type":"text"}})JSON") } };
    params.tool_choice = json(R"JSON({"type":"allowed_tools","allowed_tools":{"mode":"auto","tools":[{"type":"function","function":{"name":"weather"}}]}})JSON");
    params.parallel_tool_calls = false;
    params.response_format = json(R"JSON({"type":"json_schema","json_schema":{"name":"reply","strict":true,"schema":{"type":"object"}}})JSON");
    params.stop = json(R"JSON(["END","STOP"])JSON");
    params.max_completion_tokens = 100;
    params.n = 2;
    params.seed = 123;
    params.logprobs = true;
    params.top_logprobs = 5;
    params.logit_bias = json(R"JSON({"42":-2})JSON");
    params.reasoning_effort = "high";
    params.verbosity = "low";
    params.safety_identifier = "safe";
    params.prompt_cache_key = "cache";
    params.user = "user";
    params.metadata = json(R"JSON({"tag":"test"})JSON");
    params.store = true;
    params.service_tier = "fast";
    params.modalities = std::vector<std::string>{ "text", "audio" };
    params.audio = json(R"JSON({"format":"wav","voice":"alloy"})JSON");
    params.prediction = json(R"JSON({"type":"content","content":"guess"})JSON");
    params.web_search_options = json(R"JSON({"search_context_size":"low"})JSON");
    params.prompt_cache_retention = "24h";
    params.prompt_cache_options = json(R"JSON({"breakpoints":[{"message_index":0}]})JSON");
    params.moderation = json(R"JSON({"input":"on","output":"on"})JSON");
    params.stream_options = json(R"JSON({"include_usage":true})JSON");
    params.timeout = Timeout{ 1 };
    params.max_retries = 0;
    params.extra_headers["x-test"] = "local";
    params.extra_query["query"] = "local";
    auto actual{ chat_json::to_request_body(params, "configured") };
    auto expected{ json(R"JSON({"model":"override","messages":[{"role":"user","content":"hello"}],
      "frequency_penalty":0.5,"max_tokens":10,"presence_penalty":0.6,"temperature":0.7,"top_p":0.8,
      "tools":[{"type":"function","function":{"name":"weather","strict":true,"parameters":{"type":"object"}}},{"type":"custom","custom":{"name":"code","format":{"type":"text"}}}],
      "tool_choice":{"type":"allowed_tools","allowed_tools":{"mode":"auto","tools":[{"type":"function","function":{"name":"weather"}}]}},
      "parallel_tool_calls":false,"response_format":{"type":"json_schema","json_schema":{"name":"reply","strict":true,"schema":{"type":"object"}}},
      "stop":["END","STOP"],"max_completion_tokens":100,"n":2,"seed":123,"logprobs":true,"top_logprobs":5,"logit_bias":{"42":-2},
      "reasoning_effort":"high","verbosity":"low","safety_identifier":"safe","prompt_cache_key":"cache","user":"user","metadata":{"tag":"test"},
      "store":true,"service_tier":"fast","modalities":["text","audio"],"audio":{"format":"wav","voice":"alloy"},"prediction":{"type":"content","content":"guess"},
      "web_search_options":{"search_context_size":"low"},"prompt_cache_retention":"24h","prompt_cache_options":{"breakpoints":[{"message_index":0}]},
      "moderation":{"input":"on","output":"on"},"stream_options":{"include_usage":true}})JSON") };
    CHECK(actual == expected);
    params.extra_body["model"] = "vendor";
    params.extra_body["temperature"] = 0;
    CHECK(chat_json::to_request_body(params, "configured")["model"] == "vendor");
    params.stop = "STOP";
    CHECK(chat_json::to_request_body(params, "configured")["stop"] == "STOP");
}

DROGON_TEST(ResponsesRetainAllFieldsAndCanRoundTripMessages) {
    auto root{ full_completion() };
    auto result{ chat_json::parse_chat_completion(root) };
    REQUIRE(result.has_value());
    CHECK(result->raw == root);
    CHECK(result->system_fingerprint == "fp");
    CHECK(result->service_tier == "priority");
    CHECK((*result->metadata)["test"] == "yes");
    CHECK((*result->moderation)["flagged"] == false);
    auto& message{ result->choices[0].message };
    REQUIRE(message.tool_calls.has_value());
    CHECK(message.tool_calls->at(0).function.name == "weather");
    CHECK(message.tool_calls->at(1).custom.input == "print(1)");
    CHECK(message.annotations->at(0).url_citation["title"] == "Example");
    CHECK(message.audio->transcript == "hi");
    CHECK(message.audio->expires_at == 456);
    CHECK(result->choices[0].logprobs->content->at(0).top_logprobs[0].token == "y");
    CHECK(result->choices[0].logprobs->refusal->at(0).bytes->at(0) == 110);
    REQUIRE(result->usage.has_value());
    CHECK(result->usage->raw["prompt_cache_hit_tokens"] == 9);
    auto const& completion{ *result->usage->completion_tokens_details };
    CHECK(completion.reasoning_tokens == 1);
    CHECK(completion.accepted_prediction_tokens == 2);
    CHECK(completion.rejected_prediction_tokens == 3);
    CHECK(completion.audio_tokens == 4);
    CHECK(completion.text_tokens == 5);
    auto const& prompt{ *result->usage->prompt_tokens_details };
    CHECK(prompt.cached_tokens == 6);
    CHECK(prompt.cache_write_tokens == 7);
    CHECK(prompt.audio_tokens == 8);
    CHECK(prompt.image_tokens == 9);
    CHECK(prompt.text_tokens == 10);
    message.content = "edited";
    message.tool_calls->at(0).function.arguments = "{\"city\":\"Tokyo\"}";
    ChatCompletionCreateParams params{ .messages = { message.ToParam(), { .role = "tool", .content = "sunny", .name = "weather", .tool_call_id = "call-1" } } };
    auto body{ chat_json::to_request_body(params, "model") };
    CHECK(body["messages"][0]["content"] == "edited");
    CHECK(body["messages"][0]["tool_calls"][0]["function"]["arguments"] == "{\"city\":\"Tokyo\"}");
    CHECK(body["messages"][0]["vendor_message"] == "kept");
    CHECK(body["messages"][0]["audio"].size() == 1);
    CHECK(body["messages"][0]["audio"]["id"] == "audio-1");
    CHECK(body["messages"][1]["tool_call_id"] == "call-1");
    params.messages[0].content = json(R"JSON([{"type":"text","text":"hi"},{"type":"image_url","image_url":{"url":"data:image/png;base64,AA","detail":"high"}},{"type":"input_audio","input_audio":{"data":"AA","format":"wav"}},{"type":"file","file":{"file_id":"file-1"}}])JSON");
    CHECK(chat_json::to_request_body(params, "model")["messages"][0]["content"] == params.messages[0].content);
}

/**
 * @brief Run serialization and decoding tests without starting an application.
 * @param argc Framework argument count.
 * @param argv Framework options.
 * @return Test status.
 */
auto main(int argc, char** argv) -> int {
    return drogon::test::run(argc, argv);
}
