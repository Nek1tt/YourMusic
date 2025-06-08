#pragma once

#include "db.h"
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <functional>

namespace http = boost::beast::http;
using json = nlohmann::json;

class UserActionHandler {
public:
    explicit UserActionHandler(DB& db);

    http::response<http::string_body> operator()(int http_version, const json& body) const;

private:
    DB& db_;
    std::unordered_map<std::string, std::function<json(const json&)>> actions_;
};