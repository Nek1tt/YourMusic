#include "auth-service.h"

// -------------------- HttpSession implementation --------------------

HttpSession::HttpSession(tcp::socket socket, DB& db)
    : socket_(std::move(socket)), db_(db)
{}

void HttpSession::run() {
    do_read();
}

void HttpSession::do_read() {
    http::async_read(socket_, buffer_, req_,
        [self = shared_from_this()](beast::error_code ec, std::size_t) {
            if (ec) {
                std::cerr << "[HttpSession] Read error: " << ec.message() << "\n";
                return;
            }
            self->handle_request(std::move(self->req_));
        }
    );
}

void HttpSession::handle_request(http::request<http::string_body> req) {
    std::ostringstream oss;
    oss << req;
    std::string rawRequest = oss.str();
    std::cerr << "[HttpSession] RAW HTTP received:\n" << rawRequest << "\n";

    http::response<http::string_body> res;
    try {
        json body_json = json::parse(req.body());

        if (req.method() == http::verb::post) {
            if (req.target() == "/register") {
                res = handle_register(body_json);
            }
            else if (req.target() == "/login") {
                res = handle_login(body_json);
            }
            else {
                res = http::response<http::string_body>(http::status::not_found, req.version());
                res.set(http::field::content_type, "application/json");
                res.body() = R"({"status":"error","message":"Unknown endpoint"})";
                res.prepare_payload();
            }
        }
        else {
            res = http::response<http::string_body>(http::status::bad_request, req.version());
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"status":"error","message":"Only POST allowed"})";
            res.prepare_payload();
        }
    }
    catch (const std::exception& e) {
        res = http::response<http::string_body>(http::status::bad_request, req.version());
        res.set(http::field::content_type, "application/json");
        res.body() = std::string(R"({"status":"error","message":"Invalid JSON: )") + e.what() + "\"}";
        res.prepare_payload();
    }

    do_write(std::move(res));
}

http::response<http::string_body> HttpSession::handle_register(const json& body) {
    try {
        std::string username = body.at("username").get<std::string>();
        std::string email    = body.at("email").get<std::string>();
        std::string password = body.at("password").get<std::string>();
        std::string usertag  = body.at("usertag").get<std::string>();

        std::cerr << "[HttpSession] Register: username=" << username
                  << ", email=" << email << ", usertag=" << usertag << "\n";

        if (db_.user_exists_by_usertag(usertag)) {
            std::cerr << "[HttpSession] Register failed: usertag '" << usertag
                      << "' already exists\n";
            json resp = {
                {"status", "error"},
                {"message", "Usertag already exists"}
            };
            http::response<http::string_body> r{http::status::conflict, req_.version()};
            r.set(http::field::content_type, "application/json");
            r.body() = resp.dump();
            r.prepare_payload();
            return r;
        }

        if (db_.user_exists_by_email(email)) {
            std::cerr << "[HttpSession] Register failed: email '" << email
                      << "' already exists\n";
            json resp = {
                {"status", "error"},
                {"message", "Email already exists"}
            };
            http::response<http::string_body> r{http::status::conflict, req_.version()};
            r.set(http::field::content_type, "application/json");
            r.body() = resp.dump();
            r.prepare_payload();
            return r;
        }

        std::cerr << "[HttpSession] Attempting to add user to DB\n";
        if (!db_.add_user(username, email, password, usertag)) {
            std::cerr << "[HttpSession] add_user() returned false for usertag='" 
                      << usertag << "'\n";
            json resp = {
                {"status", "error"},
                {"message", "DB insert failed"}
            };
            http::response<http::string_body> r{http::status::internal_server_error, req_.version()};
            r.set(http::field::content_type, "application/json");
            r.body() = resp.dump();
            r.prepare_payload();
            return r;
        }
        std::cerr << "[HttpSession] add_user() succeeded for usertag='" << usertag << "'\n";

        std::cerr << "[HttpSession] Attempting to set default profile cover\n";
        if (!db_.set_default_profile_cover(usertag)) {
            std::cerr << "[HttpSession] set_default_profile_cover() failed for usertag='"
                      << usertag << "'\n";
        } else {
            std::cerr << "[HttpSession] Default profile cover set for usertag='"
                      << usertag << "'\n";
        }

        json resp = {
            {"status", "ok"},
            {"message", "User registered successfully"}
        };
        http::response<http::string_body> r{http::status::ok, req_.version()};
        r.set(http::field::content_type, "application/json");
        r.body() = resp.dump();
        r.prepare_payload();
        std::cerr << "[HttpSession] Registration flow completed for usertag='"
                  << usertag << "'\n";
        return r;
    }
    catch (const std::exception& e) {
        std::cerr << "[HttpSession] Exception in handle_register: " << e.what() << "\n";
        json resp = {
            {"status", "error"},
            {"message", "Missing required fields or invalid JSON"}
        };
        http::response<http::string_body> r{http::status::bad_request, req_.version()};
        r.set(http::field::content_type, "application/json");
        r.body() = resp.dump();
        r.prepare_payload();
        return r;
    }
}


http::response<http::string_body> HttpSession::handle_login(const json& body) {
    try {
        std::string email = body.at("email").get<std::string>();
        std::string password = body.at("password").get<std::string>();

        if (db_.check_credentials_by_email(email, password)) {
            auto usertag_opt = db_.get_usertag_by_email(email);
            if (!usertag_opt)
            {
                json resp = {
                    {"status", "error"},
                    {"message", "Usertag not found"}
                };
                http::response<http::string_body> r{http::status::internal_server_error, req_.version()};
                r.set(http::field::content_type, "application/json");
                r.body() = resp.dump();
                r.prepare_payload();
                return r;
            }

            json resp = {
                {"status", "ok"},
                {"message", "Login successful"},
                {"usertag", *usertag_opt}
            };
            http::response<http::string_body> r{http::status::ok, req_.version()};
            r.set(http::field::content_type, "application/json");
            r.body() = resp.dump();
            r.prepare_payload();
            return r;
        }
        else {
            json resp = {
                {"status", "error"},
                {"message", "Invalid email or password"}
            };
            http::response<http::string_body> r{http::status::unauthorized, req_.version()};
            r.set(http::field::content_type, "application/json");
            r.body() = resp.dump();
            r.prepare_payload();
            return r;
        }
    }
    catch (const std::exception&) {
        json resp = {
            {"status", "error"},
            {"message", "Missing required fields: email, password"}
        };
        http::response<http::string_body> r{http::status::bad_request, req_.version()};
        r.set(http::field::content_type, "application/json");
        r.body() = resp.dump();
        r.prepare_payload();
        return r;
    }
}

void HttpSession::do_write(http::response<http::string_body> res) {
    auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
    http::async_write(socket_, *sp,
        [self = shared_from_this(), sp](beast::error_code ec, std::size_t) {
            if (ec) {
                std::cerr << "[HttpSession] Write error: " << ec.message() << "\n";
            }
            beast::error_code ec_shutdown;
            self->socket_.shutdown(tcp::socket::shutdown_send, ec_shutdown);
        }
    );
}


// -------------------- AuthService implementation --------------------

AuthService::AuthService(net::io_context& ioc, DB& db, unsigned short port)
    : acceptor_(ioc, tcp::endpoint(tcp::v4(), port)),
      db_(db)
{}

void AuthService::run() {
    do_accept();
}

void AuthService::do_accept() {
    acceptor_.async_accept(
        [this](beast::error_code ec, tcp::socket socket) {
            on_accept(ec, std::move(socket));
        }
    );
}

void AuthService::on_accept(beast::error_code ec, tcp::socket socket) {
    if (!ec) {
        std::make_shared<HttpSession>(std::move(socket), db_)->run();
    } else {
        int err_code = ec.value();
        std::cerr << "[AuthService] Accept error (" << err_code << "): " << "\n";
    }
    do_accept();
}

// -------------------- main --------------------

int main() {
    try {
        net::io_context ioc;

    const char* host = std::getenv("DB_HOST") ? std::getenv("DB_HOST") : "127.0.0.1";
    const char* user = std::getenv("DB_USER") ? std::getenv("DB_USER") : "root";
    const char* pass = std::getenv("DB_PASS") ? std::getenv("DB_PASS") : "YourMusic";
    const char* name = std::getenv("DB_NAME") ? std::getenv("DB_NAME") : "yourmusic";
    DB db(host, user, pass, name);
        if (!db.is_connected()) {
            std::cerr << "[main] Failed to connect to database.\n";
            return 1;
        }

        AuthService service(ioc, db, 8082);
        service.run();

        std::cout << "[main] AuthService is running on port 8082...\n";
        ioc.run();
    }
    catch (const std::exception& e) {
        std::cerr << "[main] Exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
