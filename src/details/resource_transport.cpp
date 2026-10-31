/**
 * @file resource_transport.cpp
 * @brief Serialize resource requests and incrementally decode arbitrary API SSE events.
 */
#include "details/resource_transport.hpp"

#include <algorithm>
#include <cctype>
#include <exception>
#include <sstream>
#include <utility>

#include <drogon/utils/Utilities.h>
#include <json/writer.h>

namespace drogon_openai::details {

    namespace {

        /**
         * @brief Return a compact JSON representation.
         * @param value JSON value.
         * @return Serialized JSON.
         */
        auto compact(Json::Value const& value) -> std::string {
            Json::StreamWriterBuilder writer;
            writer["indentation"] = "";
            return Json::writeString(writer, value);
        }

        /**
         * @brief Flatten nested form fields using SDK bracket notation.
         * @param parts Destination form fields.
         * @param name Field path.
         * @param value Field value.
         */
        auto form_fields(std::vector<MultipartPart>& parts, std::string name, Json::Value const& value) -> void {
            if (value.isObject()) {
                for (auto const& key : value.getMemberNames()) {
                    form_fields(parts, name.empty() ? key : name + "[" + key + "]", value[key]);
                }
            } else if (value.isArray()) {
                for (auto const& item : value) {
                    form_fields(parts, name + "[]", item);
                }
            } else if (!value.isNull()) {
                parts.push_back({ .name = std::move(name), .content = value.isString() ? value.asString() : compact(value) });
            }
        }

        /**
         * @brief Quote a multipart disposition parameter.
         * @param value Unescaped parameter.
         * @return Safe quoted value.
         */
        auto form_quote(std::string const& value) -> std::string {
            std::string result;
            for (char c : value) {
                if (c == '"') {
                    result += "%22";
                } else if (c == '\r') {
                    result += "%0D";
                } else if (c == '\n') {
                    result += "%0A";
                } else if (c == '\\') {
                    result += "\\\\";
                } else {
                    result += c;
                }
            }
            return result;
        }

        /**
         * @brief Convert owned fields and files into a replayable HTTP body.
         * @param payload Request fields and files.
         * @return Bytes and content type, or invalid-request error.
         */
        auto encode_payload(ResourcePayload payload) -> Result<std::pair<std::string, std::string>> {
            if (payload.raw_body_field && payload.files.empty() && payload.body.isObject() && payload.body.size() == 1 && payload.body.isMember(*payload.raw_body_field)) {
                auto const& value{ payload.body[*payload.raw_body_field] };
                if (!value.isString()) {
                    return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "raw form field must be a string" } };
                }
                return std::pair{ value.asString(), payload.multipart_encodings.at(*payload.raw_body_field).first };
            }
            for (auto const& [name, encoding] : payload.multipart_encodings) {
                if (!payload.body.isMember(name)) {
                    continue;
                }
                auto value{ payload.body[name] };
                payload.body.removeMember(name);
                if (!encoding.second && !value.isString()) {
                    return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "multipart text field must be a string" } };
                }
                payload.files.push_back({ .name = name, .content = encoding.second ? compact(value) : value.asString(), .content_type = encoding.first });
            }
            if (!payload.multipart && payload.files.empty()) {
                return std::pair{ payload.body.isNull() ? std::string{} : compact(payload.body), payload.body.isNull() ? std::string{} : std::string{ "application/json" } };
            }
            auto const boundary{ "drogon-openai-" + drogon::utils::genRandomString(32) };
            std::vector<MultipartPart> parts;
            form_fields(parts, "", payload.body);
            parts.insert(parts.end(), std::make_move_iterator(payload.files.begin()), std::make_move_iterator(payload.files.end()));
            std::string body;
            for (auto const& part : parts) {
                if (part.name.find('\0') != std::string::npos || (part.filename && part.filename->find('\0') != std::string::npos) || (part.content_type && part.content_type->find_first_of("\r\n") != std::string::npos)) {
                    return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "invalid multipart field metadata" } };
                }
                body += "--" + boundary + "\r\nContent-Disposition: form-data; name=\"" + form_quote(part.name) + "\"";
                if (part.filename) {
                    body += "; filename=\"" + form_quote(*part.filename) + "\"";
                }
                body += "\r\n";
                if (part.content_type) {
                    body += "Content-Type: " + *part.content_type + "\r\n";
                }
                body += "\r\n" + part.content + "\r\n";
            }
            body += "--" + boundary + "--\r\n";
            return std::pair{ std::move(body), "multipart/form-data; boundary=" + boundary };
        }

        /**
         * @brief Decode fragmented SSE without retaining the full stream.
         */
        class EventDecoder {
        private:
            std::string m_line;          ///< Partial line, including split UTF-8.
            std::string m_data;          ///< Multiline event data.
            std::string m_type;          ///< SSE event name.
            std::string m_id;            ///< Last SSE ID.
            bool m_first{ true };        ///< Strip an optional BOM.
            bool m_skip_lf{};            ///< Previous delimiter was CR.
            bool m_done{};               ///< DONE or a terminal resource event was received.
            ApiEventCallback m_callback; ///< User event consumer.

        public:
            ApiStreamResult result; ///< Latest event and resource metadata.

            /**
             * @brief Bind an owned event consumer.
             * @param callback Event consumer.
             */
            explicit EventDecoder(ApiEventCallback callback) : m_callback{ std::move(callback) } {}

            /**
             * @brief Feed a fragment on the awaiting loop.
             * @param bytes Arbitrarily split body bytes.
             * @return Whether to continue reading, or a validation/callback error.
             */
            auto Feed(std::string_view bytes) -> Result<bool> {
                for (char c : bytes) {
                    if (m_skip_lf) {
                        m_skip_lf = false;
                        if (c == '\n') {
                            continue;
                        }
                    }
                    if (c != '\r' && c != '\n') {
                        m_line += c;
                        continue;
                    }
                    m_skip_lf = c == '\r';
                    auto current{ std::exchange(m_line, {}) };
                    if (m_first) {
                        m_first = false;
                        if (current.starts_with("\xEF\xBB\xBF")) {
                            current.erase(0, 3);
                        }
                    }
                    if (current.empty()) {
                        if (m_data.empty()) {
                            m_type.clear();
                            continue;
                        }
                        m_data.pop_back();
                        if (m_data == "[DONE]") {
                            m_done = true;
                            m_data.clear();
                            return false;
                        }
                        auto parsed{ decode_json(std::exchange(m_data, {})) };
                        if (!parsed) {
                            return std::unexpected{ std::move(parsed.error()) };
                        }
                        if (!parsed->isObject()) {
                            return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "SSE event is not an object" } };
                        }
                        ApiStreamEvent event{ .type = (*parsed)["type"].isString() ? (*parsed)["type"].asString() : m_type, .id = m_id, .data = std::move(*parsed) };
                        m_type.clear();
                        if (event.type == "error" || !event.data["error"].isNull()) {
                            auto const error{ event.data["error"].isObject() ? event.data["error"] : event.data };
                            return std::unexpected{ Error{ .kind = ErrorKind::Status, .message = error["message"].isString() ? error["message"].asString() : "API stream error", .body = compact(event.data), .type = error["type"].isString() ? error["type"].asString() : "", .code = error["code"].isString() ? error["code"].asString() : "" } };
                        }
                        ++result.event_count;
                        result.last_event = event.data;
                        if (event.type == "response.completed" || event.type == "response.failed" || event.type == "response.incomplete") {
                            m_done = true;
                            if (event.data["response"].isObject()) {
                                result.response = event.data["response"];
                            }
                        }
                        if (m_callback) {
                            auto next{ m_callback(event) };
                            if (!next) {
                                return std::unexpected{ std::move(next.error()) };
                            }
                            if (!*next) {
                                result.cancelled = true;
                                return false;
                            }
                        }
                        continue;
                    }
                    if (current.front() == ':') {
                        continue;
                    }
                    auto const colon{ current.find(':') };
                    auto const field{ current.substr(0, colon) };
                    auto value{ colon == std::string::npos ? std::string{} : current.substr(colon + 1) };
                    if (value.starts_with(' ')) {
                        value.erase(0, 1);
                    }
                    if (field == "data") {
                        m_data += value + "\n";
                    } else if (field == "event") {
                        m_type = std::move(value);
                    } else if (field == "id" && value.find('\0') == std::string::npos) {
                        m_id = std::move(value);
                    }
                }
                return true;
            }

            /**
             * @brief Validate EOF and the terminal Responses event.
             * @param responses Whether this is a Responses resource stream.
             * @return Stream result or a truncation error.
             */
            auto Finish(bool responses) -> Result<ApiStreamResult> {
                if (!result.cancelled && (!m_line.empty() || !m_data.empty() || result.event_count == 0 || (responses && !result.response))) {
                    return std::unexpected{ Error{ .kind = ErrorKind::ResponseValidation, .message = "truncated API event stream" } };
                }
                return std::move(result);
            }
        };

    } // namespace

    /**
     * @brief Add file bytes or a JSON reference to an owned payload.
     * @param payload Destination request.
     * @param name API field name.
     * @param input File, files, or JSON reference.
     */
    auto resource_file(ResourcePayload& payload, std::string name, FileInput const& input) -> void {
        auto add{ [&](UploadFile const& file, std::string key) { payload.files.push_back({ .name = std::move(key), .content = file.content, .filename = file.filename, .content_type = file.content_type }); } };
        if (auto file{ std::get_if<UploadFile>(&input) }) {
            add(*file, name);
        } else if (auto files{ std::get_if<std::vector<UploadFile>>(&input) }) {
            for (auto const& file : *files) {
                add(file, name + "[]");
            }
        } else {
            payload.body[name] = std::get<Json::Value>(input);
        }
    }

    /**
     * @brief Apply body overrides and request defaults without changing caller state.
     * @param body API body or query fields.
     * @param extra Overriding fields.
     */
    auto resource_merge(Json::Value& body, Json::Value const& extra) -> void {
        if (extra.isObject()) {
            for (auto const& key : extra.getMemberNames()) {
                body[key] = extra[key];
            }
        }
    }

    /**
     * @brief Build a path by encoding each identifier as one segment.
     * @param pattern Path template.
     * @param identifiers Template names and values.
     * @return Relative path or invalid-request error for an empty identifier.
     */
    auto resource_path(
        std::string pattern,
        std::initializer_list<std::pair<std::string_view, std::string_view>> identifiers
    ) -> Result<std::string> {
        for (auto const& [key, value] : identifiers) {
            if (value.empty()) {
                return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = std::string{ key } + " must not be empty" } };
            }
            std::string const marker{ "{" + std::string{ key } + "}" };
            auto const at{ pattern.find(marker) };
            if (at != std::string::npos) {
                pattern.replace(at, marker.size(), encode_path_segment(std::string{ value }));
            }
        }
        return pattern;
    }

    /**
     * @brief Dispatch a resource request and retain original bytes.
     * @param transport Shared endpoint settings.
     * @param method HTTP method.
     * @param path Relative API path.
     * @param payload JSON or multipart data.
     * @param options Per-request overrides.
     * @param defaults Resource-specific HTTP headers.
     * @param on_data Optional incremental body callback.
     * @return Response bytes and HTTP metadata.
     */
    auto resource_bytes(
        std::shared_ptr<Transport> transport,
        std::string method,
        std::string path,
        ResourcePayload payload,
        RequestOptions options,
        Headers defaults,
        BodyCallback on_data
    ) -> drogon::Task<Result<RawResponse<std::string>>> {
        try {
            if ((!payload.extra_body.isNull() && !payload.extra_body.isObject()) || (!options.extra_query.isNull() && !options.extra_query.isObject())) {
                co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "extra_body and extra_query must be objects" } };
            }
            resource_merge(payload.body, payload.extra_body);
            for (auto const& [name, value] : defaults) {
                std::string normalized{ name };
                std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
                bool const present{ std::any_of(options.extra_headers.begin(), options.extra_headers.end(), [&](auto const& header) {
                    std::string key{ header.first };
                    std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
                    return key == normalized;
                }) };
                if (!present) {
                    options.extra_headers[name] = value;
                }
            }
            auto query{ std::move(payload.query) };
            resource_merge(query, options.extra_query);
            options.extra_query = std::move(query);
            auto encoded{ encode_payload(std::move(payload)) };
            if (!encoded) {
                co_return std::unexpected{ std::move(encoded.error()) };
            }
            co_return co_await transport->Send(std::move(method), std::move(path), std::move(encoded->first), std::move(encoded->second), std::move(options), std::move(on_data));
        } catch (std::exception const& error) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = error.what() } };
        } catch (...) {
            co_return std::unexpected{ Error{ .kind = ErrorKind::InvalidRequest, .message = "resource request threw an exception" } };
        }
    }

    /**
     * @brief Dispatch a resource JSON request.
     * @param transport Shared endpoint settings.
     * @param method HTTP method.
     * @param path Relative API path.
     * @param payload JSON or multipart data.
     * @param options Per-request overrides.
     * @param defaults Resource-specific HTTP headers.
     * @return JSON with HTTP metadata.
     */
    auto resource_request(
        std::shared_ptr<Transport> transport,
        std::string method,
        std::string path,
        ResourcePayload payload,
        RequestOptions options,
        Headers defaults
    ) -> drogon::Task<Result<RawResponse<Json::Value>>> {
        bool const audio_text{ path == "/audio/transcriptions" || path == "/audio/translations" };
        auto response{ co_await resource_bytes(std::move(transport), std::move(method), std::move(path), std::move(payload), std::move(options), std::move(defaults)) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto parsed{ decode_json(response->body) };
        if (!parsed && audio_text && response->headers.contains("content-type") && response->headers.at("content-type").starts_with("text/")) {
            parsed = Json::Value{ response->body };
        }
        if (!parsed) {
            auto error{ std::move(parsed.error()) };
            error.status_code = response->status_code;
            error.body = response->body;
            error.headers = response->headers;
            error.request_id = response->request_id;
            co_return std::unexpected{ std::move(error) };
        }
        co_return RawResponse<Json::Value>{ response->status_code, std::move(response->headers), std::move(response->body), std::move(*parsed), std::move(response->request_id) };
    }

    /**
     * @brief Stream arbitrary API SSE events without buffering the response.
     * @param transport Shared endpoint settings.
     * @param method HTTP method.
     * @param path API path.
     * @param payload Request body or multipart data.
     * @param options Request overrides.
     * @param defaults Resource-specific headers.
     * @param callback Event consumer on the awaiting loop.
     * @return Stream metadata and final event or an error.
     */
    auto resource_stream(
        std::shared_ptr<Transport> transport,
        std::string method,
        std::string path,
        ResourcePayload payload,
        RequestOptions options,
        Headers defaults,
        ApiEventCallback callback
    ) -> drogon::Task<Result<ApiStreamResult>> {
        if (payload.extra_body.isObject()) {
            payload.extra_body.removeMember("stream");
        }
        bool const responses{ path == "/responses" || path.starts_with("/responses/") };
        EventDecoder decoder{ std::move(callback) };
        auto response{ co_await resource_bytes(std::move(transport), std::move(method), std::move(path), std::move(payload), std::move(options), std::move(defaults), [&decoder](std::string_view bytes, Headers const&) { return decoder.Feed(bytes); }) };
        if (!response) {
            co_return std::unexpected{ std::move(response.error()) };
        }
        auto result{ decoder.Finish(responses) };
        if (!result) {
            auto error{ std::move(result.error()) };
            error.status_code = response->status_code;
            error.headers = response->headers;
            error.request_id = response->request_id;
            co_return std::unexpected{ std::move(error) };
        }
        result->headers = std::move(response->headers);
        result->_request_id = std::move(response->request_id);
        co_return std::move(*result);
    }

} // namespace drogon_openai::details
