#pragma once

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include "db.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
using json = nlohmann::json;

// ===================== HttpSession =====================
// Обрабатывает одно HTTP-соединение: читает запрос и отвечает
class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    HttpSession(tcp::socket socket, DB& db);

    void run();

private:
    tcp::socket socket_;
    DB& db_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;

    void do_read();
    void handle_request(http::request<http::string_body> req);
    http::response<http::string_body> handle_register(const json& body);
    http::response<http::string_body> handle_login(const json& body);
    void do_write(http::response<http::string_body> res);
};

// ===================== AuthService =====================
// Слушает порт и создаёт HttpSession для каждого входящего соединения
class AuthService {
public:
    AuthService(net::io_context& ioc, DB& db, unsigned short port);

    void run();

private:
    tcp::acceptor acceptor_;
    DB& db_;

    void do_accept();
    void on_accept(beast::error_code ec, tcp::socket socket);
};