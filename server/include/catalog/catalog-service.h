#pragma once

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <boost/beast/http/vector_body.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include "db.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
using json = nlohmann::json;

using HandlerFunc = std::function<http::response<http::string_body>(const json&)>;

// -------------------- CatalogSession --------------------
// Отвечает за один HTTP-запрос: читает, парсит JSON, диспатчит по action, отвечает
class CatalogSession : public std::enable_shared_from_this<CatalogSession> {
public:
    CatalogSession(boost::beast::tcp_stream&& stream, DB& db);

    void run();

private:
    boost::beast::tcp_stream stream_;
    DB& db_;
    beast::flat_buffer buffer_;

    std::unordered_map<std::string, HandlerFunc> action_map_;

    void do_read();

    void handle_request(http::request<http::string_body> req);

    void init_action_map();

    http::response<http::string_body> on_home(const json& body, int version);
    http::response<http::string_body> on_profile(const json& body, int version);
    http::response<http::string_body> on_create(const json& body, int version);
    http::response<http::string_body> on_search(const json& body, int version);
    http::response<http::string_body> on_search_user(const json& body, int version);
    http::response<http::string_body> on_user_action(const json& body, int version);
    http::response<http::string_body> handle_unknown_action(const std::string& action, int version);

    void do_write(http::response<http::string_body> res);
    void do_write(http::response<http::vector_body<unsigned char>> res);
};

// -------------------- CatalogService --------------------
// Слушает порт и создаёт CatalogSession для каждого нового подключения
class CatalogService {
public:
    CatalogService(net::io_context& ioc, DB& db, unsigned short port);

    void run();

private:
    tcp::acceptor acceptor_;
    DB&           db_;

    void do_accept();
    void on_accept(beast::error_code ec, tcp::socket socket);
};
