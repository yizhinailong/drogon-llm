#!/usr/bin/env python3
"""Generate C++ resource bindings from the pinned openai-python source tree.

This is a development tool; building or using the library does not require Python.
The SDK checkout is read as Python syntax, without importing or executing it.
"""
from __future__ import annotations
import argparse
import ast
import json
from pathlib import Path
import re
import subprocess

GROUPS = 'models embeddings completions responses conversations moderations files uploads images audio batches vector_stores webhooks realtime live fine_tuning evals containers videos skills decisions content_provenance_checks safety'.split()
ROOT = Path(__file__).resolve().parents[1]
COMMON = {'extra_headers', 'extra_query', 'extra_body', 'timeout'}
SDK_COMMIT = '05992039b9f7b78a48cf1e5a35727ba27d96db6e'


def pascal(name):
    return ''.join(part[0].upper() + part[1:] for part in name.split('_'))


def literal(node):
    return node.value if isinstance(node, ast.Constant) else None


def values(node):
    return node.elts if isinstance(node, ast.Tuple) else [node]


def flatten(node):
    if isinstance(node, ast.BinOp) and isinstance(node.op, ast.BitOr):
        return flatten(node.left) + flatten(node.right)
    if isinstance(node, ast.Subscript) and ast.unparse(node.value) in ('Union', 'Optional'):
        return sum((flatten(x) for x in values(node.slice)), []) + ([ast.Constant(None)] if ast.unparse(node.value) == 'Optional' else [])
    return [node]


def cpp_identifier(name):
    keywords = {'default','class','template','operator','namespace','private','public','protected','delete','new','switch','case','return','requires','concept','co_await','const','volatile','register','extern','export','import','module','typename','union','struct','enum','auto','void','char','double','float','int','long','short','signed','unsigned','bool','break','continue','for','while','if','else','do','try','catch','throw','using','friend','this','static','virtual','mutable','explicit','inline','constexpr','consteval','constinit','noexcept','alignas','alignof','sizeof','typeid','decltype','and','or','not'}
    return name + '_' if name in keywords else name


def cpp_type(node):
    parts = [x for x in flatten(node) if ast.unparse(x) not in ('Omit', 'NotGiven')]
    names = [ast.unparse(x) for x in parts]
    if any('FileTypes' in name or 'FileContent' in name for name in names):
        return 'FileInput'
    if len(parts) > 1:
        kinds = {cpp_type(x) for x in parts}
        if len(kinds) == 1:
            return kinds.pop()
        if kinds <= {'std::int64_t', 'double'}:
            return 'double'
        return 'Json::Value'
    if not parts:
        return 'Json::Value'
    node = parts[0]
    name = ast.unparse(node)
    if name in ('str', 'builtins.str') or name.endswith(('Model', 'ModelParam')):
        return 'std::string'
    if name in ('int', 'builtins.int'):
        return 'std::int64_t'
    if name == 'float':
        return 'double'
    if name == 'bool':
        return 'bool'
    if isinstance(node, ast.Subscript):
        head = ast.unparse(node.value)
        if head == 'Literal':
            vs = [literal(x) for x in values(node.slice)]
            return 'std::string' if all(isinstance(x, str) for x in vs) else 'Json::Value'
        if head in ('Iterable', 'List', 'list', 'Sequence', 'SequenceNotStr'):
            kind = cpp_type(node.slice)
            return f'std::vector<{kind}>' if kind != 'Json::Value' else 'Json::Value'
        if head in ('Annotated', 'Required'):
            return cpp_type(values(node.slice)[0])
    return 'Json::Value'


def import_map(tree, module):
    result = {}
    for item in tree.body:
        if isinstance(item, ast.ImportFrom) and item.module:
            prefix = module.split('.')[:-item.level] if item.level else []
            for name in item.names:
                result[name.asname or name.name] = '.'.join(prefix + item.module.split('.') + [name.name])
    return result


def resolve_type(node, imports, sdk, depth=0):
    kind=cpp_type(node)
    if kind != 'Json::Value' or depth > 8:
        return kind
    parts=[p for p in flatten(node) if ast.unparse(p) not in ('Omit','NotGiven')]
    if len(parts)>1:
        kinds={resolve_type(p,imports,sdk,depth+1) for p in parts}
        if 'FileInput' in kinds: return 'FileInput'
        if len(kinds)==1: return kinds.pop()
        return kind
    if not parts:return kind
    item=parts[0]
    if isinstance(item,ast.Name): qualified=imports.get(item.id)
    elif isinstance(item,ast.Attribute) and isinstance(item.value,ast.Name): qualified=imports.get(item.value.id,'')+'.'+item.attr
    else:return kind
    if not qualified or not qualified.startswith('openai.'):return kind
    module,_,name=qualified.rpartition('.')
    file=sdk/'src'/(module.replace('.','/')+'.py')
    if not file.exists():return kind
    tree=ast.parse(file.read_text())
    for assignment in tree.body:
        target=assignment.target if isinstance(assignment,ast.AnnAssign) else assignment.targets[0] if isinstance(assignment,ast.Assign) and len(assignment.targets)==1 else None
        if isinstance(target,ast.Name) and target.id==name and assignment.value is not None:
            return resolve_type(assignment.value,import_map(tree,module),sdk,depth+1)
    return kind


def catalog(sdk):
    base = sdk / 'src/openai'
    resource_root = base / 'resources'
    groups = []
    for group in GROUPS:
        files = [resource_root / (group + '.py')] if (resource_root / (group + '.py')).exists() else sorted((resource_root / group).rglob('*.py'))
        resources = []
        for file in files:
            tree = ast.parse(file.read_text())
            module = 'openai.resources.' + file.relative_to(resource_root).with_suffix('').as_posix().replace('/', '.')
            imports = import_map(tree, module)
            for cls in tree.body:
                if not isinstance(cls, ast.ClassDef) or not any(isinstance(b, ast.Name) and b.id == 'AsyncAPIResource' for b in cls.bases):
                    continue
                parts = file.relative_to(resource_root).with_suffix('').parts
                if len(parts) > 1 and parts[-1] == parts[-2]:
                    parts = parts[:-1]
                methods = []
                for method in cls.body:
                    if not isinstance(method, (ast.FunctionDef, ast.AsyncFunctionDef)) or method.name.startswith('_'):
                        continue
                    if any(isinstance(d, ast.Name) and d.id in ('overload', 'cached_property') for d in method.decorator_list):
                        continue
                    calls = [n for n in ast.walk(method) if isinstance(n, ast.Call) and isinstance(n.func, ast.Attribute) and n.func.attr in ('_get', '_post', '_put', '_patch', '_delete', '_get_api_list')]
                    if not calls:
                        continue
                    call = calls[-1]
                    path = call.args[0]
                    path = literal(path) if isinstance(path, ast.Constant) else literal(path.args[0])
                    assert path and path.startswith('/'), (file, method.name, ast.unparse(call))
                    path_names = re.findall(r'{([^}]+)}', path)
                    args = method.args.posonlyargs + method.args.args
                    pos_defaults = [None] * (len(args) - len(method.args.defaults)) + method.args.defaults
                    args_and_defaults = list(zip(args, pos_defaults)) + list(zip(method.args.kwonlyargs, method.args.kw_defaults))
                    fields = []
                    path_args = []
                    for arg, default in args_and_defaults:
                        if arg.arg in path_names:
                            path_args.append(arg.arg)
                        elif arg.arg not in COMMON and arg.arg != 'self':
                            fields.append({'name': arg.arg, 'type': resolve_type(arg.annotation, imports, sdk), 'required': default is None, 'annotation': ast.unparse(arg.annotation)})
                    kw = {k.arg: k.value for k in call.keywords}
                    query_names = set()
                    for node in ast.walk(method):
                        if isinstance(node, ast.Call):
                            for keyword in node.keywords:
                                if keyword.arg == 'query':
                                    for mapping in ast.walk(keyword.value):
                                        if isinstance(mapping, ast.Dict):
                                            query_names.update(literal(key) for key in mapping.keys)
                    for field in fields:
                        field['query'] = field['name'] in query_names
                    encodings = {}
                    raw_body_field = None
                    for node in ast.walk(method):
                        if isinstance(node, ast.Call) and isinstance(node.func, ast.Name) and node.func.id == 'encode_multipart':
                            encodings = ast.literal_eval(node.args[2])
                            raw_body_field = literal(node.args[3]) if len(node.args) > 3 else None
                    verb = call.func.attr[1:].upper()
                    page = verb == 'GET_API_LIST'
                    if page:
                        verb = (literal(kw.get('method')) or 'GET').upper()
                    cast = ast.unparse(kw.get('cast_to', ast.Constant(None)))
                    result_name = cast
                    if page:
                        assert 'model' in kw, (file, method.name)
                        result_name = ast.unparse(kw['model'])
                    response_module = imports.get(result_name)
                    if response_module and response_module.startswith('openai.types.'):
                        response_path, response_class = response_module.rsplit('.', 1)
                        response_file = sdk / 'src' / (response_path.replace('.', '/') + '.py')
                    else:
                        response_file, response_class = None, None
                    fields_response = []
                    if response_file and response_file.exists():
                        rt = ast.parse(response_file.read_text())
                        classes = {c.name: c for c in rt.body if isinstance(c, ast.ClassDef)}
                        if response_class in classes:
                            current = classes[response_class]
                            seen = set()
                            def add_fields(c):
                                for b in c.bases:
                                    if isinstance(b, ast.Name) and b.id in classes:
                                        add_fields(classes[b.id])
                                for member in c.body:
                                    if isinstance(member, ast.AnnAssign) and isinstance(member.target, ast.Name) and member.target.id not in seen:
                                        seen.add(member.target.id)
                                        annotation = member.annotation
                                        fields_response.append({'name': member.target.id, 'type': cpp_type(annotation), 'optional': 'Optional[' in ast.unparse(annotation) or 'None' in ast.unparse(annotation), 'annotation': ast.unparse(annotation)})
                            add_fields(current)
                    extra_headers = {}
                    for n in ast.walk(method):
                        if isinstance(n, ast.Assign) and any(isinstance(t, ast.Name) and t.id == 'extra_headers' for t in n.targets) and isinstance(n.value, ast.Dict):
                            for key, value in zip(n.value.keys, n.value.values):
                                if isinstance(key, ast.Constant) and isinstance(key.value, str) and isinstance(value, ast.Constant) and isinstance(value.value, str) and key.value.lower() != 'content-type':
                                    extra_headers[key.value] = value.value
                    param_names = [n.args[-1].attr for n in ast.walk(method) if isinstance(n,ast.Call) and isinstance(n.func,ast.Name) and n.func.id in ('maybe_transform','async_maybe_transform') and n.args and isinstance(n.args[-1],ast.Attribute) and 'Params' in n.args[-1].attr]
                    methods.append({'param_name': param_names[-1] if param_names else ''.join(pascal(p) for p in parts) + pascal(method.name) + 'Params', 'name': method.name, 'cpp_name': pascal(method.name), 'path': path, 'path_args': path_args, 'verb': verb, 'page': page, 'cursor': 'CursorPage' in ast.unparse(method.returns), 'cursor_field':'last_id' if 'ConversationCursorPage' in ast.unparse(method.returns) else '', 'fields': fields, 'result': result_name, 'response_fields': fields_response, 'response_source': str(response_file.relative_to(sdk)) if response_file and response_file.exists() else None, 'binary': 'BinaryResponse' in result_name, 'text': result_name in ('str','builtins.str'), 'void': result_name in ('NoneType','None'), 'multipart': 'files' in kw, 'multipart_encodings':encodings, 'raw_body_field':raw_body_field, 'stream': 'stream' in kw, 'headers': extra_headers})
                # Keep resources with children or custom connection methods too.
                children = []
                for item in cls.body:
                    if isinstance(item, ast.FunctionDef) and any(isinstance(d, ast.Name) and d.id == 'cached_property' for d in item.decorator_list) and item.name not in ('with_raw_response','with_streaming_response'):
                        children.append(item.name)
                resources.append({'parts':list(parts),'class': 'TextCompletions' if parts == ('completions',) else ''.join(pascal(p) for p in parts), 'source':file.relative_to(resource_root).as_posix(),'children':children,'methods':methods})
        groups.append({'name':group,'resources':resources})
    return {'sdk':'openai-python 3.41.0','commit':SDK_COMMIT,'groups':groups}



def doc(brief, params=(), ret=None):
    lines=['/**',f' * @brief {brief}']
    lines += [f' * @param {name} {description}' for name,description in params]
    if ret: lines.append(f' * @return {ret}')
    return '\n'+'\n'.join(lines+[' */'])


def member_doc(text):
    return '\n'+ '\n'.join('    '+line for line in text.lstrip().splitlines())


def signature(prefix, name, args, suffix):
    short=prefix+name+'('+', '.join(args)+')'+suffix
    return short if len(short)+8 <= 100 else prefix+name+'(\n'+',\n'.join('    '+a for a in args)+'\n)'+suffix


def write_code(path, text):
    formatted = subprocess.run(['clang-format', '--assume-filename='+str(path)], input=text, text=True, capture_output=True, check=True).stdout
    if not path.exists() or path.read_text() != formatted:
        path.write_text(formatted)


def generate(spec):
    extensions=json.loads((ROOT/'tools/resource_extensions.json').read_text()) if (ROOT/'tools/resource_extensions.json').exists() else {}
    for group in spec['groups']:
        g=group['name']; ns='drogon_openai::types::'+g
        header_name='text_completions' if g == 'completions' else g
        types=[f'/**\n * @file {g}.hpp\n * @brief Request and response types for {g}; generated from openai-python 3.41.0.\n */', '#pragma once', '#include <cstdint>\n#include <memory>\n#include <optional>\n#include <string>\n#include <vector>', '#include "drogon_openai/types/resource.hpp"',f'namespace {ns} {{']
        source=[f'/**\n * @file resource_{g}.cpp\n * @brief Implement {g} resource bindings generated from openai-python 3.41.0.\n */', f'#include "drogon_openai/details/{header_name}.hpp"', '#include "details/resource_transport.hpp"', '#include <utility>',f'namespace {ns} {{']
        responses={}; requests={}
        for r in group['resources']:
            for m in r['methods']:
                name=m['result']
                if m['response_fields']:
                    if name in responses: assert responses[name]==m['response_fields'],(g,name)
                    responses[name]=m['response_fields']
                    m['cpp_result']='types::'+g+'::'+name
                else: m['cpp_result']='Json::Value'
                pn=m['param_name']
                # Params of streaming union overloads share one generated superset.
                if pn in requests and requests[pn] != m['fields']:
                    pn=r['class']+m['cpp_name']+'Params';m['param_name']=pn
                requests[pn]=m['fields']
        for name,fields in responses.items():
            types += [doc(f'Response {name}, retaining SDK union fields as JSON.'), f'struct {name} {{']
            for f in fields:
                t='std::optional<'+f['type']+'>' if f['optional'] else f['type']
                types.append(f'    {t} {cpp_identifier(f["name"])}{{}}; ///< Wire field {f["name"]}.')
            if g=='responses' and name=='Response':
                types += ['    std::string output_text; ///< Concatenated output_text content.', '    std::optional<Json::Value> output_parsed{}; ///< First structured final answer decoded by Parse.']
            types += ['    Json::Value raw{}; ///< Complete JSON including provider extensions.', '    std::string _request_id; ///< HTTP x-request-id.', member_doc(doc('Decode a resource without discarding unknown fields.', [('value','Resource JSON.')], 'Typed resource or validation error.')), f'    [[nodiscard]] static auto FromJson(Json::Value const& value) -> Result<{name}>;', '};']
            source += [doc('Decode a resource without discarding unknown fields.', [('value','Resource JSON.')], 'Typed resource or validation error.'), f'auto {name}::FromJson(Json::Value const& value) -> Result<{name}> {{', '    if (!value.isObject()) { return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "expected a resource object", .body = value.toStyledString() } }; }', f'    {name} result;', '    result.raw = value;']
            for f in fields:
                k=f['name'];typ=f['type']; cpp_k=cpp_identifier(k)
                source.append(f'    if (value.isMember("{k}")'+(f' && !value["{k}"].isNull()' if f['optional'] else '')+') {')
                if f['optional']:
                    source.append(f'        {typ} field{{}};')
                    dest='field'
                else: dest='result.'+cpp_k
                source.append(f'        if (!details::resource_read(value["{k}"], {dest})) {{ return std::unexpected{{ Error{{ .kind = ErrorKind::ResponseValidation, .message = "invalid {name}.{k}", .body = value.toStyledString() }} }}; }}')
                if f['optional']:source.append(f'        result.{cpp_k} = std::move(field);')
                source.append('    }')
            if g=='responses' and name=='Response':
                source += ['    if (result.output.isArray()) { for (auto const& item : result.output) { if (item.isObject() && item["type"] == "message" && item["content"].isArray()) { for (auto const& part : item["content"]) { if (part.isObject() && part["type"] == "output_text" && part["text"].isString()) { result.output_text += part["text"].asString(); } } } } }']
            source += ['    return result;', '}']
        for name,fields in requests.items():
            types += [doc(f'Parameters for {name}; omitted optionals are absent on the wire.'),f'struct {name} {{']
            for f in fields:
                t=f['type'] if f['required'] else 'std::optional<'+f['type']+'>'
                types.append(f'    {t} {cpp_identifier(f["name"])}{{}}; ///< '+('Required' if f['required'] else 'Optional')+f' SDK field {f["name"]}.')
            types += ['    Json::Value extra_body{}; ///< Body extensions, merged last.', '    std::optional<Timeout> timeout{}; ///< Override phase timeouts.', '    std::optional<int> max_retries{}; ///< Override retries.', '    Headers extra_headers{}; ///< Override request headers.', '    Json::Value extra_query{}; ///< Override query parameters.', member_doc(doc('Serialize fields and files for transport.', ret='Owned payload.')), '    [[nodiscard]] auto Payload() const -> ResourcePayload;', member_doc(doc('Extract per-request HTTP settings.', ret='Request overrides.')), '    [[nodiscard]] auto Options() const -> RequestOptions;', '};']
            source += [doc('Serialize fields and files for transport.',ret='Owned payload.'),f'auto {name}::Payload() const -> ResourcePayload {{', '    ResourcePayload payload;']
            for f in fields:
                k=f['name']; cpp_k=cpp_identifier(k);v=cpp_k if f['required'] else '*'+cpp_k
                if not f['required']:source.append(f'    if ({cpp_k}) {{')
                if f['type']=='FileInput':source.append(f'    details::resource_file(payload, "{k}", {v});')
                else:source.append(f'    payload.{"query" if f["query"] else "body"}["{k}"] = details::resource_json({v});')
                if not f['required']:source.append('    }')
            source += ['    payload.extra_body = extra_body;','    return payload;','}',doc('Extract per-request HTTP settings.',ret='Request overrides.'),f'auto {name}::Options() const -> RequestOptions {{', '    return { .timeout = timeout, .max_retries = max_retries, .extra_headers = extra_headers, .extra_query = extra_query };','}']
        types.append(f'}} // namespace {ns}')
        write_code(ROOT/f'include/drogon_openai/types/{g}.hpp', '\n'.join(types)+'\n')
        source += [f'}} // namespace {ns}', 'namespace drogon_openai::details {']
        header=[f'/**\n * @file {header_name}.hpp\n * @brief {g} API groups generated from openai-python 3.41.0.\n */', '#pragma once', '#include <memory>', '#include "drogon_openai/websocket.hpp"', '#include "drogon_openai/pagination.hpp"',f'#include "drogon_openai/types/{g}.hpp"','namespace drogon_openai::details {', 'class Transport;']
        by_parts={tuple(r['parts']):r for r in group['resources']}
        for r in sorted(group['resources'],key=lambda r:-len(r['parts'])):
            cn=r['class']; children=[]
            for child in r['children']:
                sub=by_parts.get(tuple(r['parts'])+(child,))
                assert sub,(r,child)
                children.append((child,sub['class']))
            header += [doc(f'Value handle for the SDK {".".join(r["parts"])} resource.'),f'class {cn} {{','private:', '    std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.', 'public:', member_doc(doc('Bind this resource and its children to an endpoint.', [('transport','Shared endpoint settings.')])), f'    explicit {cn}(std::shared_ptr<Transport> transport);']
            source += [doc('Bind this resource and its children to an endpoint.', [('transport','Shared endpoint settings.')]), f'{cn}::{cn}(std::shared_ptr<Transport> transport) : m_transport{{ transport }}'+''.join(f', {name}{{ transport }}' for name,typ in children)+' {}']
            for child,typ in children:header.append(f'    {typ} {child}; ///< Nested SDK resource {child}.')
            for m in r['methods']:
                pn='types::'+g+'::'+m['param_name']; result=m['cpp_result']
                baseargs=['std::string '+name for name in m['path_args']]
                args=baseargs+[pn+' params = {}']
                argsdef=baseargs+[pn+' params']
                docs=doc(f'{m["verb"]} {m["path"]}.',[(n,'Owned path identifier, encoded as one segment.') for n in m['path_args']]+[('params','Owned SDK parameters and request overrides.')], 'API result or request/validation error.')
                if m['binary']: rt='RawResponse<std::string>'
                elif m['text']: rt='std::string'
                elif m['void']:rt='void'
                elif m['page']:rt='Page<'+result+'>'
                else:rt=result
                for raw in ([False] if m['page'] or m['binary'] else [False,True]):
                    suffix='Raw' if raw else '';name=m['cpp_name']+suffix
                    rettype='RawResponse<'+('std::string' if m['text'] else result)+'>' if raw else rt
                    header += [member_doc(docs),signature('    [[nodiscard]] auto ',name,args,f') const -> drogon::Task<Result<{rettype}>>;'.removeprefix(')'))]
                    source += [docs,signature('auto ',cn+'::'+name,argsdef,f' const -> drogon::Task<Result<{rettype}>> {{'), '    auto transport{ m_transport };', '    auto payload{ params.Payload() };',f'    payload.multipart = {str(m["multipart"]).lower()} || !payload.files.empty();']
                    pairs=', '.join('{ "'+n+'", '+n+' }' for n in m['path_args'])
                    source += [f'    auto path{{ resource_path("{m["path"]}", {{ {pairs} }}) }};', '    if (!path) { co_return std::unexpected{ std::move(path.error()) }; }']
                    for key,(media,as_json) in m['multipart_encodings'].items():
                        source.append(f'    payload.multipart_encodings["{key}"] = {{ "{media}", {str(as_json).lower()} }};')
                    if m['raw_body_field']:source.append(f'    payload.raw_body_field = "{m["raw_body_field"]}";')
                    stream_container='query' if m['verb']=='GET' else 'body'
                    stream_value = 'payload.query.get("stream", false)' if m['verb']=='GET' else '(payload.extra_body.isObject() ? payload.extra_body.get("stream", payload.body.get("stream", false)) : payload.body.get("stream", false))' if m['stream'] else ''
                    if m['stream']:source += [f'    if ({stream_value} == true) {{ co_return std::unexpected{{ Error{{ .kind = ErrorKind::InvalidRequest, .message = "use '+m['cpp_name']+'Stream for SSE delivery" } }; }']
                    defaults='{ '+', '.join('{ "'+k+'", "'+v+'" }' for k,v in m['headers'].items())+' }'
                    invoke=f'transport, "{m["verb"]}", std::move(*path), std::move(payload), params.Options(), {defaults}'
                    if m['page']:
                        cursor_field = ', "'+m['cursor_field']+'"' if m['cursor_field'] else ''
                        source += [f'    co_return co_await resource_page<{result}>({invoke}, {str(m["cursor"]).lower()}{cursor_field});']
                    elif m['binary'] or m['text']:
                        source += [f'    auto response{{ co_await resource_bytes({invoke}) }};', '    if (!response) { co_return std::unexpected{ std::move(response.error()) }; }', '    co_return std::move('+('*response' if m['binary'] or raw else 'response->body')+');']
                    else:
                        source += [f'    auto response{{ co_await resource_request({invoke}) }};', '    if (!response) { co_return std::unexpected{ std::move(response.error()) }; }']
                        if m['void'] and not raw:source += ['    co_return {};']
                        else:
                            source += [f'    auto parsed{{ resource_decode_response<{result}>(std::move(*response)) }};', '    if (!parsed) { co_return std::unexpected{ std::move(parsed.error()) }; }','    co_return std::move('+('*parsed' if raw else 'parsed->parsed')+');']
                    source.append('}')
                if m['stream']:
                    docs_stream=doc(f'Stream SSE from {m["verb"]} {m["path"]}.',[(n,'Owned path identifier.') for n in m['path_args']]+[('params','Owned SDK parameters.'),('callback','Event consumer; false stops delivery.')], 'Final event and stream metadata or an error.')
                    header += [member_doc(docs_stream),signature('    [[nodiscard]] auto ',m['cpp_name']+'Stream',baseargs+[pn+' params', 'ApiEventCallback callback = {}'],' const -> drogon::Task<Result<ApiStreamResult>>;')]
                    source += [docs_stream,signature('auto ',cn+'::'+m['cpp_name']+'Stream',argsdef+['ApiEventCallback callback'],' const -> drogon::Task<Result<ApiStreamResult>> {'), '    auto transport{ m_transport };','    auto payload{ params.Payload() };', f'    payload.{stream_container}["stream"] = true;',f'    payload.multipart = {str(m["multipart"]).lower()} || !payload.files.empty();',f'    auto path{{ resource_path("{m["path"]}", {{ {pairs} }}) }};','    if (!path) { co_return std::unexpected{ std::move(path.error()) }; }',f'    co_return co_await resource_stream({invoke}, std::move(callback));','}']
            for extra in extensions.get(cn,[]):
                ret='Result<'+extra['return']+'>' if extra['sync'] else 'drogon::Task<Result<'+extra['return']+'>>'
                header += [member_doc(extra['doc']), signature('    [[nodiscard]] auto ', extra['name'], extra['args'], ' const -> '+ret+';')]
            header.append('};')
        header.append('} // namespace drogon_openai::details')
        source.append('} // namespace drogon_openai::details')
        write_code(ROOT/f'include/drogon_openai/details/{header_name}.hpp', '\n'.join(header)+'\n')
        write_code(ROOT/f'src/details/resource_{g}.cpp', '\n'.join(source)+'\n')

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk-root',type=Path,required=True)
    args=parser.parse_args()
    revision = subprocess.run(['git', '-C', str(args.sdk_root), 'rev-parse', 'HEAD'], check=True, capture_output=True, text=True).stdout.strip()
    if revision != SDK_COMMIT:
        parser.error('SDK checkout must be at pinned commit ' + SDK_COMMIT)
    spec=catalog(args.sdk_root)
    generate(spec)
    catalog_path = ROOT/'tools/sdk_resources.json'
    catalog_text = json.dumps(spec,ensure_ascii=False,indent=2)+'\n'
    if not catalog_path.exists() or catalog_path.read_text() != catalog_text:
        catalog_path.write_text(catalog_text)
    print('Catalog:',sum(len(g['resources']) for g in spec['groups']),'resources,',sum(len(r['methods']) for g in spec['groups'] for r in g['resources']),'HTTP methods')
    for g in spec['groups']:
        unknown=[(r['class'],m['name'],m['result']) for r in g['resources'] for m in r['methods'] if not m['response_fields'] and not m['binary'] and not m['text'] and not m['void']]
        if unknown: print(g['name'],'union/alias responses:',unknown)


if __name__ == '__main__':
    main()
