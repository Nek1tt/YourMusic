#pragma once

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <optional>
#include "db.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

// -------------------- StreamingSession --------------------
// Обрабатывает один HTTP-запрос: читает GET /stream/{track_id}, 
// получает MP3‑BLOB из БД и отдает его клиенту.
class StreamingSession : public std::enable_shared_from_this<StreamingSession> {
public:
    StreamingSession(beast::tcp_stream&& stream, DB& db);

    // Запускает асинхронное чтение HTTP-запроса
    void run();

private:
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    DB& db_;

    // Читает HTTP-запрос
    void do_read();

    // Обрабатывает распарсенный запрос
    void handle_request(http::request<http::string_body> req);

    // Записывает ответ с текстовым телом
    void do_write(http::response<http::string_body> res);

    // Записывает ответ с бинарным телом (MP3)
    void do_write(http::response<http::vector_body<uint8_t>> res);

    // Закрывает соединение
    void do_close();
};

// -------------------- StreamingService --------------------
// Слушает порт и создаёт StreamingSession для каждого нового соединения
class StreamingService {
public:
    StreamingService(net::io_context& ioc, DB& db, unsigned short port);

    // Начинает приём входящих соединений
    void run();

private:
    tcp::acceptor acceptor_;
    DB& db_;

    // Асинхронно ждёт новое соединение
    void do_accept();

    // Обрабатывает результат accept
    void on_accept(beast::error_code ec, tcp::socket socket);
};
