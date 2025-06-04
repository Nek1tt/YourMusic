#pragma once

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include "db.h"                    // server/common/db.h
#include "home-handler.h"          // наш новый хендлер для “HOME”

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
using json = nlohmann::json;

// Тип для функции-обработчика, принимающей только HTTP-версию.
// (в нашем случае “body” нам не нужен, но оставим json-параметр,
// чтобы интерфейс на будущее был единообразным)
using HandlerFunc = std::function<http::response<http::string_body>(const json&)>;

// -------------------- CatalogSession --------------------
// Отвечает за один HTTP-запрос: читает, парсит JSON, диспатчит по action, отвечает
class CatalogSession : public std::enable_shared_from_this<CatalogSession> {
public:
    CatalogSession(tcp::socket socket, DB& db);

    // Запустить цикл чтения
    void run();

private:
    tcp::socket socket_;
    DB& db_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;

    // Словарь action → функцию-обработчик
    std::unordered_map<std::string, HandlerFunc> action_map_;

    void do_read();
    void handle_request(http::request<http::string_body> req);

    // Регистрируем action_map_
    void init_action_map();

    // Обработчик “home” (вызовем HomeHandler)
    http::response<http::string_body> on_home(const json& body);

    http::response<http::string_body> on_profile(const json& body);

    // Если action не найден
    http::response<http::string_body> handle_unknown_action(const std::string& action);

    // Отправляем ответ
    void do_write(http::response<http::string_body> res);
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
