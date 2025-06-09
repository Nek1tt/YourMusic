#pragma once

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include "db.h"
#include <boost/mysql.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast;
namespace http  = beast::http;
using json = nlohmann::json;

// Хендлер “What’s New + Recommendations”
class HomeHandler {
public:
    // Конструктор получает ссылку на DB и текущий usertag
    HomeHandler(const DB& db, std::string usertag)
        : db_(db), usertag_(std::move(usertag)) {}

    // Вызывается, когда нужно сформировать ответ
    http::response<http::string_body> operator()(int http_version) const;

private:
    const DB& db_;
    std::string usertag_;
};

