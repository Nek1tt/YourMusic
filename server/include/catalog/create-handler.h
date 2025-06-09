#pragma once

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <optional>
#include "db.h"

namespace beast = boost::beast;
namespace http  = beast::http;
using json = nlohmann::json;

// Хендлер “CREATE ALBUM / CREATE TRACK”
class CreateHandler {
public:
    CreateHandler(DB& db)
        : db_(db) {}

    http::response<http::string_body> operator()(int http_version, const json& body_json);

private:
    DB& db_;

    static std::vector<uint8_t> decode_base64(const std::string& b64);
};
