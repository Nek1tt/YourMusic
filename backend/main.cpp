#include "AudioPlayer.h"
#include "httplib.h"
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class AudioServer {
public:
    AudioServer() : player(nullptr) {}

    void handle_request(const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_body = json::parse(req.body);

            if (!json_body.contains("command")) {
                return send_error(res, 400, "Missing 'command' field");
            }

            std::string command = json_body["command"];

            if (command == "load") {
                handle_load(json_body, res);
            }
            else if (command == "resume") {
                handle_resume(res);
            }
            else if (command == "pause") {
                handle_pause(res);
            }
            else if (command == "quit") {
                handle_quit(res);
            }
            else if (command == "seeking") {
                handle_seek(json_body, res);
            }
            else if (command == "volume") {
                handle_volume(json_body, res);
            }
            else {
                send_error(res, 400, "Unknown command");
            }
        }
        catch (const std::exception& e) {
            send_error(res, 500, e.what());
        }
    }

    bool is_running() const { return running; }
    void stop() { running = false; }

private:
    std::unique_ptr<AudioPlayer> player;
    bool running = true;

    void send_error(httplib::Response& res, int status, const std::string& message) {
        res.status = status;
        res.set_content(json({{"error", message}}).dump(), "application/json");
    }

    void send_status(httplib::Response& res, const std::string& message) {
        res.set_content(json({{"status", message}}).dump(), "application/json");
    }

    void handle_load(const json& j, httplib::Response& res) {
        if (!j.contains("path")) {
            return send_error(res, 400, "Missing 'path' for load command");
        }

        std::string path = j["path"];
        if (player) {
            player->stopAudio();
            player.reset();
        }

        player = std::make_unique<AudioPlayer>(path.c_str());

        if (!player->init()) {
            player.reset();
            return send_error(res, 500, "Failed to load audio file");
        }

        player->playAudio();
        send_status(res, "Playing");
    }

    void handle_resume(httplib::Response& res) {
        if (!player) {
            return send_error(res, 400, "No track loaded");
        }
        player->playAudio();
        send_status(res, "Resumed");
    }

    void handle_pause(httplib::Response& res) {
        if (!player) {
            return send_error(res, 400, "No track loaded");
        }
        player->pauseAudio();
        send_status(res, "Paused");
    }

    void handle_quit(httplib::Response& res) {
        if (player) {
            player->stopAudio();
            player.reset();
        }
        send_status(res, "Stopped");
        stop();
    }

    void handle_seek(const json& j, httplib::Response& res) {
        if (!player) {
            return send_error(res, 400, "No track loaded");
        }

        if (!j.contains("position") || !j["position"].is_number()) {
            return send_error(res, 400, "Missing or invalid 'position' field");
        }

        double pos = j["position"];
        if (pos < 0.01 || pos > 1.0) {
            return send_error(res, 400, "Position must be between 0.01 and 1.00");
        }

        if (!player->seekTo(pos)) {
            return send_error(res, 500, "Failed to seek");
        }

        send_status(res, "Seeked");
    }

    void handle_volume(const json& j, httplib::Response& res) {
        if (!player) {
            return send_error(res, 400, "No track loaded");
        }

        if (!j.contains("level") || !j["level"].is_number()) {
            return send_error(res, 400, "Missing or invalid 'level' field");
        }

        float vol = j["level"];
        if (vol < 0.01f || vol > 1.0f) {
            return send_error(res, 400, "Volume level must be between 0.01 and 1.00");
        }

        player->setVolume(vol);
        send_status(res, "Volume set");
    }
};

int main() {
    AudioServer server;

    httplib::Server svr;

    svr.Post("/", [&](const httplib::Request& req, httplib::Response& res) {
        server.handle_request(req, res);
        if (!server.is_running()) {
            svr.stop();
        }
    });

    std::cout << "Server listening on port 8080\n";
    svr.listen("0.0.0.0", 8080);

    return 0;
}
