#include "AudioPlayer.h"
#include <iostream>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <unordered_set>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <nlohmann/json.hpp>

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
using json = nlohmann::json;

class AudioServer {
public:
    AudioServer(boost::asio::io_context& ioc) : player(nullptr), ioc_(ioc) {}

    void handle_request(http::request<http::string_body> req, http::response<http::string_body>& res) {
        try {
            auto json_body = json::parse(req.body());

            if (!json_body.contains("command")) {
                return send_error(res, http::status::bad_request, "Missing 'command' field");
            }

            std::string command = json_body["command"];

            if (command == "load") handle_load(json_body, res);
            else if (command == "resume") handle_resume(res);
            else if (command == "pause") handle_pause(res);
            else if (command == "quit") handle_quit(res);
            else if (command == "seeking") handle_seek(json_body, res);
            else if (command == "volume") handle_volume(json_body, res);
            else send_error(res, http::status::bad_request, "Unknown command");
        }
        catch (const std::exception& e) {
            send_error(res, http::status::internal_server_error, e.what());
        }
    }

    bool is_running() const { return running; }
    void stop() { running = false; }

private:
    std::unique_ptr<AudioPlayer> player;
    bool running = true;
    boost::asio::io_context& ioc_;

    void send_error(http::response<http::string_body>& res, http::status status, const std::string& message) {
        res.result(status);
        res.set(http::field::content_type, "application/json");
        res.body() = json({{"error", message}}).dump();
        res.prepare_payload();
    }

    void send_status(http::response<http::string_body>& res, const std::string& message) {
        res.result(http::status::ok);
        res.set(http::field::content_type, "application/json");
        res.body() = json({{"status", message}}).dump();
        res.prepare_payload();
    }

    void handle_load(const json& j, http::response<http::string_body>& res) {
        if (!j.contains("path")) {
            return send_error(res, http::status::bad_request, "Missing 'path' for load command");
        }

        std::string path = j["path"];
        if (player) {
            player->stopAudio();
            player.reset();
        }

        player = std::make_unique<AudioPlayer>(path.c_str());
        if (!player->init()) {
            player.reset();
            return send_error(res, http::status::internal_server_error, "Failed to load audio file");
        }

        player->playAudio();
        send_status(res, "Playing");
    }

    void handle_resume(http::response<http::string_body>& res) {
        if (!player) return send_error(res, http::status::bad_request, "No track loaded");
        player->playAudio();
        send_status(res, "Resumed");
    }

    void handle_pause(http::response<http::string_body>& res) {
        if (!player) return send_error(res, http::status::bad_request, "No track loaded");
        player->pauseAudio();
        send_status(res, "Paused");
    }

    void handle_quit(http::response<http::string_body>& res) {
        if (player) {
            player->stopAudio();
            player.reset();
        }
        send_status(res, "Stopped");
        stop();
    }

    void handle_seek(const json& j, http::response<http::string_body>& res) {
        if (!player) return send_error(res, http::status::bad_request, "No track loaded");
        if (!j.contains("position") || !j["position"].is_number()) {
            return send_error(res, http::status::bad_request, "Missing or invalid 'position' field");
        }
        double pos = j["position"];
        if (pos < 0.01 || pos > 1.0) {
            return send_error(res, http::status::bad_request, "Position must be between 0.01 and 1.00");
        }
        if (!player->seekTo(pos)) {
            return send_error(res, http::status::internal_server_error, "Failed to seek");
        }
        send_status(res, "Seeked");
    }

    void handle_volume(const json& j, http::response<http::string_body>& res) {
        if (!player) return send_error(res, http::status::bad_request, "No track loaded");
        if (!j.contains("level") || !j["level"].is_number()) {
            return send_error(res, http::status::bad_request, "Missing or invalid 'level' field");
        }
        float vol = j["level"];
        if (vol < 0.01f || vol > 1.0f) {
            return send_error(res, http::status::bad_request, "Volume level must be between 0.01 and 1.00");
        }
        player->setVolume(vol);
        send_status(res, "Volume set");
    }
};

int main(int argc, char* argv[]) {
    try {
        boost::asio::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {tcp::v4(), 8080}};
        AudioServer server(ioc);

        std::cout << "Server listening on port 8080" << std::endl;

        for (;;) {
            tcp::socket socket{ioc};
            acceptor.accept(socket);

            boost::beast::flat_buffer buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req);

            http::response<http::string_body> res;
            server.handle_request(std::move(req), res);

            http::write(socket, res);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
