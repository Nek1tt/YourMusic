#include "catalog-service.h"
#include "home-handler.h"
#include "profile-handler.h"
#include "create-handler.h"
#include "search-handler.h"
#include "search-user-handler.h"
#include "user-action-handler.h"


// -------------------- CatalogSession implementation --------------------

CatalogSession::CatalogSession(boost::beast::tcp_stream&& stream, DB& db)
    : stream_(std::move(stream)), db_(db)
{
    init_action_map();
}

void CatalogSession::run() {
    do_read();
}

void CatalogSession::do_read() {

    stream_.expires_after(std::chrono::seconds(120));
    auto parser = std::make_shared<http::request_parser<http::string_body>>();
    parser->body_limit(750ULL * 1024 * 1024);
    buffer_.reserve(750ULL * 1024 * 1024);
    http::async_read(stream_, buffer_, *parser,
        [self = shared_from_this(), parser](beast::error_code ec, std::size_t bytes_transferred) {
            std::cerr << "[CatalogSession] async_read completed, ec = " << ec.message() << ", bytes_transferred = " << bytes_transferred << "\n";

            if (ec) {
                std::cerr << "[CatalogSession] Read error: " << ec.message() << "\n";
                return;
            }

            http::request<http::string_body> req = parser->get();
            self->handle_request(std::move(req));
        }
    );
}

#include <boost/beast/http/vector_body.hpp>

void CatalogSession::handle_request(http::request<http::string_body> req) {
    // Log raw request
    {
        std::ostringstream oss;
        oss << req;
        std::cerr << "[CatalogSession] RAW HTTP received:\n" << oss.str() << "\n";
    }

    int version = req.version();

    // --- NEW: Image GET routes ---
    if (req.method() == http::verb::get) {
        // Convert target to std::string
        boost::beast::string_view sv = req.target();
        std::string target(sv.data(), sv.size());

        // Helper: try route, return true if handled
        auto handle_blob_route = [&](const std::string& prefix, auto getBlob) {
            if (target.rfind(prefix, 0) != 0)
                return false;

            std::string id_str = target.substr(prefix.size());
            try {
                using BlobOpt = std::optional<std::vector<uint8_t>>;
                BlobOpt blob = getBlob(id_str);

                if (!blob || blob->empty()) {
                    http::response<http::string_body> res{http::status::not_found, version};
                    res.set(http::field::content_type, "application/json");
                    res.body() = R"({"status":"error","message":"Not found"})";
                    res.prepare_payload();
                    do_write(res);
                    return true;
                }

                auto const& data = *blob;
                std::string mime;
                // PNG signature
                if (data.size() >= 4 && data[0]==0x89 && data[1]==0x50 && data[2]==0x4E && data[3]==0x47)
                    mime = "image/png";
                // JPEG signature
                else if (data.size() >= 3 && data[0]==0xFF && data[1]==0xD8 && data[2]==0xFF)
                    mime = "image/jpeg";
                else {
                    http::response<http::string_body> res{http::status::unsupported_media_type, version};
                    res.set(http::field::content_type, "application/json");
                    res.body() = R"({"status":"error","message":"Unsupported image format"})";
                    res.prepare_payload();
                    do_write(res);
                    return true;
                }

                http::response<http::vector_body<unsigned char>> res{http::status::ok, version};
                res.set(http::field::content_type, mime);
                res.content_length(data.size());
                res.body() = data;
                do_write(std::move(res));
                return true;
            }
            catch (...) {
                http::response<http::string_body> res{http::status::bad_request, version};
                res.set(http::field::content_type, "application/json");
                res.body() = R"({"status":"error","message":"Invalid identifier"})";
                res.prepare_payload();
                do_write(res);
                return true;
            }
        };

        // Try each image route
        if (handle_blob_route("/album/", [&](const std::string& id) { return db_.get_album_cover(std::stoi(id)); }))
            return;
        if (handle_blob_route("/track/", [&](const std::string& id) { return db_.get_track_cover(std::stoi(id)); }))
            return;
        if (handle_blob_route("/profile/", [&](const std::string& tag) { return db_.get_user_cover(tag); }))
            return;
    }

    // --- Existing POST JSON logic ---
    http::response<http::string_body> res;
    try {
        auto body_json = json::parse(req.body());
        if (req.method() == http::verb::post) {
            if (!body_json.contains("action") || !body_json["action"].is_string()) {
                res = http::response<http::string_body>(http::status::bad_request, version);
                res.set(http::field::content_type, "application/json");
                res.body() = R"({"status":"error","message":"Missing or invalid 'action'"})";
                res.prepare_payload();
            } else {
                std::string action = body_json["action"].get<std::string>();
                auto it = action_map_.find(action);
                if (it != action_map_.end()) {
                    if (action == "home")         res = on_home(body_json, version);
                    else if (action == "profile")  res = on_profile(body_json, version);
                    else if (action == "create")   res = on_create(body_json, version);
                    else if (action == "search")   res = on_search(body_json, version);
                    else if (action == "search_user") res = on_search_user(body_json, version);
                    else if (action == "user_action") res = on_user_action(body_json, version);
                } else {
                    res = handle_unknown_action(action, version);
                }
            }
        } else {
            res = http::response<http::string_body>(http::status::bad_request, version);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"status":"error","message":"Only POST is allowed"})";
            res.prepare_payload();
        }
    } catch (const std::exception& e) {
        res = http::response<http::string_body>(http::status::bad_request, version);
        res.set(http::field::content_type, "application/json");
        res.body() = std::string("{\"status\":\"error\",\"message\":\"Invalid JSON: ") + e.what() + "\"}";
        res.prepare_payload();
    }

    do_write(std::move(res));
}


void CatalogSession::init_action_map() {
    action_map_.emplace("home", [this](const json& b) {
        return http::response<http::string_body>();
    });
    action_map_.emplace("profile", [this](const json& b) {
        return http::response<http::string_body>();
    });
    action_map_.emplace("create", [this](const json& b) {
        return http::response<http::string_body>();
    });
    action_map_.emplace("search", [this](const json& b) {
        return http::response<http::string_body>();
    });
    action_map_.emplace("search_user", [this](const json& b) {
        return http::response<http::string_body>();
    });
    action_map_.emplace("user_action", [this](const json& b) {
        return http::response<http::string_body>();
    });
}


http::response<http::string_body> CatalogSession::on_home(const json& body, int version) {
    if (!body.contains("usertag") || !body["usertag"].is_string()) {
        http::response<http::string_body> res{http::status::bad_request, version};
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"status":"error","message":"Missing or invalid 'usertag'"})";
        res.prepare_payload();
        return res;
    }
    std::string usertag = body["usertag"].get<std::string>();
    HomeHandler handler(db_, usertag);
    return handler(version);
}

http::response<http::string_body> CatalogSession::on_profile(const json& body, int version) {
    if (!body.contains("usertag1") || !body["usertag1"].is_string()) {
        http::response<http::string_body> res{http::status::bad_request, version};
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"status":"error","message":"Missing or invalid 'usertag1'"})";
        res.prepare_payload();
        return res;
    }
    std::string usertag1 = body["usertag1"].get<std::string>();
    std::optional<std::string> usertag2;
    if (body.contains("usertag2") && body["usertag2"].is_string()) {
        usertag2 = body["usertag2"].get<std::string>();
    }
    ProfileHandler handler(db_, usertag1, usertag2);
    return handler(version);
}

http::response<http::string_body> CatalogSession::on_create(const json& body, int version) {
    if (!body.contains("entity")   || !body["entity"].is_string()   ||
        !body.contains("usertags") || !body["usertags"].is_array()) 
    {
        http::response<http::string_body> res{http::status::bad_request, version};
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"status":"error","message":"Missing or invalid 'entity' or 'usertags'"})";
        res.prepare_payload();
        return res;
    }

    CreateHandler handler(db_);
    return handler(version, body);
}

http::response<http::string_body> CatalogSession::on_search(const json& body, int version) {
    if (!body.contains("query") || !body["query"].is_string()) {
        http::response<http::string_body> res{http::status::bad_request, version};
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"status":"error","message":"Missing or invalid 'query'"})";
        res.prepare_payload();
        return res;
    }
    SearchHandler handler(db_);
    return handler(version, body);
}

http::response<http::string_body> CatalogSession::on_search_user(const json& body, int version) {
    if (!body.contains("query") || !body["query"].is_string()) {
        http::response<http::string_body> res{http::status::bad_request, version};
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"status":"error","message":"Missing or invalid 'query'"})";
        res.prepare_payload();
        return res;
    }
    SearchUserHandler handler(db_);
    return handler(version, body);
}

http::response<http::string_body> CatalogSession::on_user_action(const json& body, int version) {
     if (!body.contains("subaction") || !body["subaction"].is_string()) {
        http::response<http::string_body> res{http::status::bad_request, version};
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"status":"error","message":"Missing or invalid 'subaction'"})";
        res.prepare_payload();
        return res;
    }
    UserActionHandler handler(db_);
    return handler(version, body);
}
http::response<http::string_body> CatalogSession::handle_unknown_action(const std::string& action, int version) {
    json resp_body = {
        {"status", "error"},
        {"message", "Unknown action: " + action}
    };
    http::response<http::string_body> res{http::status::bad_request, version};
    res.set(http::field::content_type, "application/json");
    res.body() = resp_body.dump();
    res.prepare_payload();
    return res;
}

void CatalogSession::do_write(http::response<http::string_body> res) {
    stream_.expires_after(std::chrono::seconds(120));
    auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
    http::async_write(stream_, *sp,
        [self = shared_from_this(), sp](beast::error_code ec, std::size_t) {
            if (ec) {
                std::cerr << "[CatalogSession] Write error: " << ec.message() << "\n";
            }
            beast::error_code ec_shutdown;
            self->stream_.socket().shutdown(tcp::socket::shutdown_send, ec_shutdown);
        }
    );
}

void CatalogSession::do_write(http::response<http::vector_body<unsigned char>> res) {
    stream_.expires_after(std::chrono::seconds(120));
    auto sp = std::make_shared<http::response<http::vector_body<unsigned char>>>(std::move(res));
    http::async_write(stream_, *sp,
        [self = shared_from_this(), sp](beast::error_code ec, std::size_t) {
            beast::error_code ec_shutdown;
            self->stream_.socket().shutdown(tcp::socket::shutdown_send, ec_shutdown);
        });
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
        beast::tcp_stream stream(std::move(socket));
        stream.expires_never();
        std::make_shared<CatalogSession>(std::move(stream), db_)->run();
    } else {
        int err_code = ec.value();
        std::string msg_cp = ec.message();
        std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> to_wide;
        std::wstring u16 = to_wide.from_bytes(msg_cp);
        std::string  u8  = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(u16);
        std::cerr << "[CatalogService] Accept error (" << err_code << "): " << u8 << "\n";
    }
    do_accept();
}

int main() {
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
