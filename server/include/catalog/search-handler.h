#pragma once

#include "db.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

class SearchHandler {
public:
    SearchHandler(DB& db) : db_(db) {}

    nlohmann::json handle_search(const std::string& query) const;
    http::response<http::string_body> operator()(int http_version, const json& body) const;
private:
    DB& db_;
};