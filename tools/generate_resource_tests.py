#!/usr/bin/env python3
"""Generate HTTP binding contract cases from the pinned SDK endpoint catalog."""
import json
from pathlib import Path
import re
from generate_resources import cpp_identifier, write_code

root=Path(__file__).resolve().parents[1]
spec=json.loads((root/'tools/sdk_resources.json').read_text())
lines=['''/**
 * @file test_resources.cpp
 * @brief Exercise every HTTP resource binding against the pinned SDK wire contract.
 */
#include <string>
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include <drogon_openai/openai_client.hpp>
#include "details/transport.hpp"
#include "support.hpp"

using namespace drogon_openai;
namespace {
    drogon::HttpRequestPtr g_request; ///< Last request, retained through its assertion.
    Json::Value g_reply; ///< Current resource fixture.
    bool g_binary{}; ///< Send binary content for this contract.
    bool g_text{}; ///< Send legacy text content for this contract.
    bool g_void{}; ///< Send a no-content response for this contract.

    /**
     * @brief Create the isolated contract client with a base path.
     * @return Owned client.
     */
    auto client() -> OpenAI {
        return OpenAI{ { .base_url = std::string{ test_support::SERVER_URL } + "/resources", .api_key = "resource-key", .model = "fake", .max_retries = 0 } };
    }

    /**
     * @brief Decode fixture JSON without exposing request data in logs.
     * @param text Fixture bytes.
     * @return JSON fixture.
     */
    auto json(std::string const& text) -> Json::Value {
        return details::decode_json(text).value();
    }

    /**
     * @brief Serve arbitrary SDK routes while retaining method, body and headers.
     */
    auto register_fixture() -> void {
        drogon::app().registerHandlerViaRegex("/resources/.*", [](drogon::HttpRequestPtr const& req, std::function<void(drogon::HttpResponsePtr const&)>&& callback) {
            g_request = req;
            auto response{ drogon::HttpResponse::newHttpResponse() };
            if (g_void) { response->setStatusCode(drogon::k204NoContent); }
            else if (g_binary || g_text) {
                response->setContentTypeString(g_text ? "text/plain" : "application/octet-stream");
                response->setBody(g_text ? "legacy text" : std::string{ "byte\\0data", 9 });
            } else { response = drogon::HttpResponse::newHttpJsonResponse(g_reply); }
            response->addHeader("x-request-id", "req-resource");
            callback(response);
        });
    }
} // namespace

''']

def fixture(m):
    fields={}
    for f in m['response_fields']:
        t=f['type'];name=f['name']
        if t=='std::string':v='resource'
        elif t=='std::int64_t':v=7
        elif t=='double':v=.5
        elif t=='bool':v=True
        elif t.startswith('std::vector<'):v=[]
        elif 'List[' in f['annotation']:v=[]
        else:v={}
        fields[name]=v
    fields.setdefault('id','resource')
    fields['provider_extension']={'preserved':True}
    return {'data':[fields],'has_more':False,'object':'list'} if m['page'] else fields

for group in spec['groups']:
    g=group['name']
    for r in group['resources']:
        for m in r['methods']:
            title='.'.join(r['parts'])+'.'+m['name']
            lines += [f'DROGON_TEST({r["class"]}{m["cpp_name"]}Contract) {{',f'        // SDK contract: {title}.',f'        types::{g}::{m["param_name"]} params;']
            for f in m['fields']:
                name=cpp_identifier(f['name']);t=f['type']
                if f['name']=='stream':v='false'
                elif t=='std::string':v='"value"'
                elif t=='std::int64_t':v='7'
                elif t=='double':v='0.5'
                elif t=='bool':v='true'
                elif t=='FileInput':v='UploadFile{ .content = std::string{ "file\\0data", 9 }, .filename = "input.bin" }'
                elif t.startswith('std::vector<'):v=t+'{}'
                else:v='json(R"JSON({"vendor":"value"})JSON")'
                lines.append(f'        params.{name} = {v};')
            lines += ['        params.extra_headers["x-resource-contract"] = "yes";',f'        g_reply = json(R"JSON({json.dumps(fixture(m),separators=(",",":"))})JSON");',f'        g_binary = {str(m["binary"]).lower()};', f'        g_text = {str(m["text"]).lower()};',f'        g_void = {str(m["void"]).lower()};']
            args=['"'+arg+'-value"' for arg in m['path_args']]+['params']
            lines += [f'        auto result{{ drogon::sync_wait(client().{".".join(r["parts"])}.{m["cpp_name"]}({", ".join(args)})) }};', '        REQUIRE(result);', '        REQUIRE(g_request);']
            path=m['path']
            for n in m['path_args']:path=path.replace('{'+n+'}',n+'-value')
            lines += [f'        CHECK(g_request->getOriginalPath() == "/resources{path}");',f'        CHECK(std::string_view(g_request->methodString()) == "{m["verb"]}");','        CHECK(g_request->getHeader("authorization") == "Bearer resource-key");','        CHECK(g_request->getHeader("x-resource-contract") == "yes");']
            for k,v in m['headers'].items():lines.append(f'        CHECK(g_request->getHeader("{k.lower()}") == "{v}");')
            if m['binary']:lines.append('        CHECK(result->parsed == std::string("byte\\0data", 9));')
            elif m['text']:lines.append('        CHECK(*result == "legacy text");')
            elif m['page']:lines += ['        CHECK(result->data.size() == 1);', '        CHECK(!result->HasNextPage());','        CHECK(result->_request_id == "req-resource");']
            elif m['response_fields']:lines += ['        CHECK(result->raw["provider_extension"]["preserved"] == true);','        CHECK(result->_request_id == "req-resource");']
            if m['multipart'] or any(f['type']=='FileInput' for f in m['fields']):
                lines += ['        CHECK(g_request->getHeader("content-type").starts_with("multipart/form-data; boundary="));',*(['        CHECK(g_request->body().find(std::string_view("file\\0data", 9)) != std::string_view::npos);'] if any(f['type']=='FileInput' for f in m['fields']) else [])]
            lines.append('    }')
lines += ['''
/**
 * @brief Run HTTP contract checks on the local fixture app.
 * @param argc Framework argument count.
 * @param argv Framework options.
 * @return Test status.
 */
auto main(int argc, char** argv) -> int {
    test_support::set_env(test_support::API_KEY_ENV, test_support::API_KEY);
    drogon::app().loadConfigFile("config.json");
    register_fixture();
    return test_support::run_tests_with_app(argc, argv);
}
''']
write_code(root/'tests/test_resources.cpp', '\n'.join(lines))
print('Generated contract coverage for',sum(len(r['methods']) for g in spec['groups'] for r in g['resources']),'HTTP methods.')
