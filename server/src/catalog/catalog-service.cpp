#include "catalog-service.h"
#include "profile-handler.h" 
#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>

// -------------------- CatalogSession implementation --------------------

CatalogSession::CatalogSession(tcp::socket socket, DB& db)
    : socket_(std::move(socket)), db_(db)
{
    init_action_map();
}

void CatalogSession::run() {
    do_read();
}

void CatalogSession::do_read() {
    http::async_read(socket_, buffer_, req_,
        [self = shared_from_this()](beast::error_code ec, std::size_t) {
            if (ec) {
                std::cerr << "[CatalogSession] Read error: " << ec.message() << "\n";
                return;
            }
            self->handle_request(std::move(self->req_));
        }
    );
}

void CatalogSession::handle_request(http::request<http::string_body> req) {
    // Отобразим полный HTTP-запрос (заголовки + тело) для отладки
    {
        std::ostringstream oss;
        oss << req;
        std::cerr << "[CatalogSession] RAW HTTP received:\n" << oss.str() << "\n";
    }

    http::response<http::string_body> res;
    try {
        json body_json = json::parse(req.body());

        if (req.method() == http::verb::post)
        {
            if (!body_json.contains("action") || !body_json["action"].is_string()) {
                res = http::response<http::string_body>(http::status::bad_request, req.version());
                res.set(http::field::content_type, "application/json");
                res.body() = R"({"status":"error","message":"Missing or invalid 'action'"})";
                res.prepare_payload();
            } 
            else {
                std::string action = body_json["action"].get<std::string>();
                auto it = action_map_.find(action);
                if (it != action_map_.end()) {
                    // Вызываем внешний хендлер (HOME или PROFILE)
                    if (action == "home") {
                        res = on_home(body_json);
                    }
                    else if (action == "profile") {
                        res = on_profile(body_json);
                    }
                } else {
                    res = handle_unknown_action(action);
                }
            }
        }

    }
    catch (const std::exception& e) {
        res = http::response<http::string_body>(http::status::bad_request, req.version());
        res.set(http::field::content_type, "application/json");
        res.body() = std::string(R"({"status":"error","message":"Invalid JSON: )") 
                    + e.what() + "\"}";
        res.prepare_payload();
    }

    do_write(std::move(res));
}

void CatalogSession::init_action_map() {
    // Привязываем “home” к HomeHandler
    action_map_.emplace("home", [this](const json& b) {
        return on_home(b);
    });

    // Привязываем “profile” к ProfileHandler
    action_map_.emplace("profile", [this](const json& b) {
        return on_profile(b);
    });
}

http::response<http::string_body> CatalogSession::on_home(const json& body) {
    if (!body.contains("usertag") || !body["usertag"].is_string()) {
        http::response<http::string_body> res{http::status::bad_request, req_.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"status":"error","message":"Missing or invalid 'usertag'"})";
        res.prepare_payload();
        return res;
    }
    std::string usertag = body["usertag"].get<std::string>();
    HomeHandler handler(db_, usertag);
    return handler(req_.version());
}

http::response<http::string_body> CatalogSession::on_profile(const json& body) {
    if (!body.contains("usertag") || !body["usertag"].is_string() ||
        !body.contains("flag")    || !body["flag"].is_number_integer())
    {
        http::response<http::string_body> res{http::status::bad_request, req_.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"status":"error","message":"Missing or invalid 'usertag' or 'flag'"})";
        res.prepare_payload();
        return res;
    }
    std::string usertag = body["usertag"].get<std::string>();
    int flag = body["flag"].get<int>();  // 0 = свой профиль, 1 = чужой
    ProfileHandler handler(db_, usertag, flag);
    return handler(req_.version());
}

http::response<http::string_body> CatalogSession::handle_unknown_action(const std::string& action) {
    json resp_body = {
        {"status", "error"},
        {"message", "Unknown action: " + action}
    };
    http::response<http::string_body> res{http::status::bad_request, req_.version()};
    res.set(http::field::content_type, "application/json");
    res.body() = resp_body.dump();
    res.prepare_payload();
    return res;
}

void CatalogSession::do_write(http::response<http::string_body> res) {
    auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
    http::async_write(socket_, *sp,
        [self = shared_from_this(), sp](beast::error_code ec, std::size_t) {
            if (ec) {
                std::cerr << "[CatalogSession] Write error: " << ec.message() << "\n";
            }
            // Закрываем соединение после отправки ответа
            beast::error_code ec_shutdown;
            self->socket_.shutdown(tcp::socket::shutdown_send, ec_shutdown);
        }
    );
}

// -------------------- CatalogService implementation --------------------

CatalogService::CatalogService(net::io_context& ioc, DB& db, unsigned short port)
    : acceptor_(ioc, tcp::endpoint(tcp::v4(), port)),
      db_(db)
{}

void CatalogService::run() {
    do_accept();
}

void CatalogService::do_accept() {
    acceptor_.async_accept(
        [this](beast::error_code ec, tcp::socket socket) {
            on_accept(ec, std::move(socket));
        }
    );
}

void CatalogService::on_accept(beast::error_code ec, tcp::socket socket) {
    if (!ec) {
        std::make_shared<CatalogSession>(std::move(socket), db_)->run();
    } else {
        int err_code = ec.value();
        std::string msg_cp = ec.message();
        // Конвертируем ANSI → UTF-8 для корректного вывода
        std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> to_wide;
        std::wstring u16 = to_wide.from_bytes(msg_cp);
        std::string  u8  = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(u16);
        std::cerr << "[CatalogService] Accept error (" << err_code << "): " << u8 << "\n";
    }
    do_accept();
}

int main() {
    std::setlocale(LC_ALL, "C");
    try {
        net::io_context ioc;
        DB db("127.0.0.1", "root", "YourMusic", "yourmusic");
        if (!db.is_connected()) {
            std::cerr << "[main] Failed to connect to database.\n";
            return 1;
        }
        CatalogService service(ioc, db, 8083);
        service.run();
        std::cout << "[main] CatalogService is running on port 8083...\n";
        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "[main] Exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
