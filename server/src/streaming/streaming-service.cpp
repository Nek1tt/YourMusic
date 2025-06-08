#include "streaming-service.h"
#include <iostream>
#include <cstring>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

// -------------------- StreamingSession implementation --------------------

StreamingSession::StreamingSession(beast::tcp_stream&& stream, DB& db)
    : stream_(std::move(stream)), db_(db)
{}

void StreamingSession::run() {
    do_read();
}

void StreamingSession::do_read() {
    stream_.expires_after(std::chrono::seconds(120));
    auto parser = std::make_shared<http::request_parser<http::string_body>>();
    parser->body_limit(0);

    http::async_read(stream_, buffer_, *parser,
        [self = shared_from_this(), parser](beast::error_code ec, std::size_t) {
            if (ec) {
                std::cerr << "[StreamingSession] Read error: " << ec.message() << "\n";
                return;
            }
            http::request<http::string_body> req = parser->get();
            self->handle_request(std::move(req));
        }
    );
}

void StreamingSession::handle_request(http::request<http::string_body> req) {
    {
        std::ostringstream oss;
        oss << req;
        std::cerr << "[StreamingSession] RAW HTTP received:\n" << oss.str() << "\n";
    }

    int version = req.version();

    if (req.method() != http::verb::get) {
        http::response<http::string_body> res{http::status::bad_request, version};
        res.set(http::field::content_type, "text/plain");
        res.body() = "Only GET is supported";
        res.prepare_payload();
        return do_write(std::move(res));
    }

    std::string target = std::string(req.target());
    const std::string prefix = "/stream/";
    if (target.rfind(prefix, 0) != 0) {
        http::response<http::string_body> res{http::status::not_found, version};
        res.set(http::field::content_type, "text/plain");
        res.body() = "Not found";
        res.prepare_payload();
        return do_write(std::move(res));
    }

    int track_id = 0;
    std::string subscriber, author;
    try {
        track_id = std::stoi(target.substr(prefix.size()));
        std::string path = target.substr(prefix.size());
        std::string qs;
        auto qpos = path.find('?');
        if (qpos != std::string::npos) {
            qs   = path.substr(qpos + 1);
            path = path.substr(0, qpos);
        }
        track_id = std::stoi(path);
        std::istringstream qss(qs);
        std::string kv;
        while (std::getline(qss, kv, '&')) {
            auto eq = kv.find('=');
            if (eq == std::string::npos) continue;
            auto key = kv.substr(0, eq);
            auto val = kv.substr(eq + 1);
            if (key == "user") subscriber = val;
            if (key == "author") author = val;
        }
    } catch (...) {
        http::response<http::string_body> res{http::status::bad_request, version};
        res.set(http::field::content_type, "text/plain");
        res.body() = "Invalid track_id";
        res.prepare_payload();
        return do_write(std::move(res));
    }

    auto maybe_blob = db_.get_track_blob(track_id);
    if (!maybe_blob.has_value()) {
        http::response<http::string_body> res{http::status::not_found, version};
        res.set(http::field::content_type, "text/plain");
        res.body() = "Track not found";
        res.prepare_payload();
        return do_write(std::move(res));
    }
    auto& blob = *maybe_blob; // std::vector<uint8_t>

    http::response<http::vector_body<uint8_t>> res{http::status::ok, version};
    res.set(http::field::server, "StreamingService/1.0");
    res.set(http::field::content_type, "audio/mpeg");
    if (!subscriber.empty() && !author.empty()) {
        bool isSub = db_.is_following(subscriber, author);
        res.set("X-Subscribed", isSub ? "true" : "false");
    }
    res.content_length(blob.size());
    res.body() = blob;
    res.keep_alive(req.keep_alive());

    return do_write(std::move(res));
}

void StreamingSession::do_write(http::response<http::string_body> res) {
    auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
    stream_.expires_after(std::chrono::seconds(120));
    http::async_write(stream_, *sp,
        [self = shared_from_this(), sp](beast::error_code ec, std::size_t) {
            if (ec) {
                std::cerr << "[StreamingSession] Write error: " << ec.message() << "\n";
            }
            self->do_close();
        }
    );
}

void StreamingSession::do_write(http::response<http::vector_body<uint8_t>> res) {
    auto sp = std::make_shared<http::response<http::vector_body<uint8_t>>>(std::move(res));
    stream_.expires_after(std::chrono::seconds(120));
    http::async_write(stream_, *sp,
        [self = shared_from_this(), sp](beast::error_code ec, std::size_t) {
            if (ec) {
                std::cerr << "[StreamingSession] Write error: " << ec.message() << "\n";
            }
            self->do_close();
        }
    );
}

void StreamingSession::do_close() {
    beast::error_code ec;
    stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
}

// -------------------- StreamingService implementation --------------------

StreamingService::StreamingService(net::io_context& ioc, DB& db, unsigned short port)
    : acceptor_(ioc, tcp::endpoint(tcp::v4(), port)), db_(db)
{}

void StreamingService::run() {
    do_accept();
}

void StreamingService::do_accept() {
    acceptor_.async_accept(
        [this](beast::error_code ec, tcp::socket socket) {
            on_accept(ec, std::move(socket));
        }
    );
}

void StreamingService::on_accept(beast::error_code ec, tcp::socket socket) {
    if (!ec) {
        beast::tcp_stream stream(std::move(socket));
        stream.expires_never();
        std::make_shared<StreamingSession>(std::move(stream), db_)->run();
    } else {
        std::cerr << "[StreamingService] Accept error: " << ec.message() << "\n";
    }
    do_accept();
}

// -------------------- main --------------------

int main() {
    try {
        net::io_context ioc;
        DB db("127.0.0.1", "root", "YourMusic", "yourmusic");
        if (!db.is_connected()) {
            std::cerr << "[main] Failed to connect to database.\n";
            return 1;
        }
        StreamingService service(ioc, db, 8084);
        service.run();
        std::cout << "[main] StreamingService is running on port 8084...\n";
        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "[main] Exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}