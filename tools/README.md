# Resource binding maintenance

The bindings target [openai-python 3.41.0](https://github.com/openai/openai-python/tree/05992039b9f7b78a48cf1e5a35727ba27d96db6e), commit `05992039b9f7b78a48cf1e5a35727ba27d96db6e`. The source SDK is licensed under Apache-2.0; its license is retained in [OPENAI-PYTHON-LICENSE](OPENAI-PYTHON-LICENSE). Installation includes this license under `share/licenses/drogon-openai/`. This project extracts API signatures, field names and wire metadata from that source and implements its own C++ transport and helpers.

These are development tools. Normal builds require neither Python nor an SDK checkout. The generated C++ and catalog are checked in. Read the checkout as Python AST only; never import or execute its modules.

```sh
git init /tmp/openai-python
git -C /tmp/openai-python remote add origin https://github.com/openai/openai-python.git
git -C /tmp/openai-python fetch --depth 1 origin 05992039b9f7b78a48cf1e5a35727ba27d96db6e
git -C /tmp/openai-python checkout --detach FETCH_HEAD
python3 -B tools/generate_resources.py --sdk-root /tmp/openai-python
python3 -B tools/generate_resource_tests.py
clang-format -i tests/test_resources.cpp
python3 -B tools/generate_resource_docs.py
```

`generate_resources.py` generates `include/drogon_openai/types/<group>.hpp`, `include/drogon_openai/details/<group>.hpp`, `src/details/resource_<group>.cpp` and `sdk_resources.json`. The legacy text completions resource uses `details/text_completions.hpp`; `details/completions.hpp` belongs to Chat Completions. Generation formats C++ with the repository's clang-format and only rewrites changed files.

`resource_extensions.json` adds helper declarations. Their implementations in resource_helpers.cpp, resource_connections.cpp and resource_webhook_signatures.cpp are handwritten and are never overwritten. Keep repeated Doxygen blocks synchronized. Changes to helper declarations also require updating these implementations and both user guides.

`generate_resource_tests.py` checks the complete HTTP endpoint catalog against a local server. Maintain independent behavioral tests in test_resource_wire.cpp, test_resource_helpers.cpp and test_websocket.cpp: generated method/path checks cannot establish correct multipart encodings, query/body placement, streaming, polling or signatures by themselves. Verify generation is deterministic, run the full tests and strict Doxygen, and rebuild source/installed consumers after changing public headers.

Only the groups explicitly listed in `GROUPS` are generated. `admin.*`, deprecated Assistants and other `beta.*` are excluded. Keep SDK updates deliberate: update the pin, review signature, body/query and response changes, rerun generation and independent tests, and synchronize the two documentation languages.
