#pragma once
#include "db.h"
#include <iostream>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

class SearchUserHandler {
public:
    explicit SearchUserHandler(DB& db) : db_(db) {}

    http::response<http::string_body> operator()(int http_version, const json& body) const;

private:
    DB& db_;

    std::vector<std::string> handle_search_user(const std::string& query) const;
};