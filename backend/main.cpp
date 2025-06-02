#include "AudioPlayer.h"
#include "httplib.h"  // https://github.com/yhirose/cpp-httplib
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>  // https://github.com/nlohmann/json

int main() {
    std::unique_ptr<AudioPlayer> player;

    httplib::Server svr;

    svr.Post("/", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json = nlohmann::json::parse(req.body);

            if (!json.contains("command")) {
                res.status = 400;
                res.set_content(R"({"error":"Missing 'command' field"})", "application/json");
                return;
            }

            std::string command = json["command"];

            if (command == "load") {
                if (!json.contains("path")) {
                    res.status = 400;
                    res.set_content(R"({"error":"Missing 'path' for load command"})", "application/json");
                    return;
                }

                std::string path = json["path"];

                if (player) {
                    player->stopAudio();
                    player.reset();
                }

                player = std::make_unique<AudioPlayer>(path.c_str());

                if (!player->init()) {
                    player.reset();
                    res.status = 500;
                    res.set_content(R"({"error":"Failed to load audio file"})", "application/json");
                    return;
                }

                player->playAudio();
                res.set_content(R"({"status":"Playing"})", "application/json");

            } 
            else if (command == "resume") {
                if (!player) {
                    res.status = 400;
                    res.set_content(R"({"error":"No track loaded"})", "application/json");
                    return;
                }
                player->playAudio();
                res.set_content(R"({"status":"Resumed"})", "application/json");

            } else if (command == "pause") {
                if (!player) {
                    res.status = 400;
                    res.set_content(R"({"error":"No track loaded"})", "application/json");
                    return;
                }
                player->pauseAudio();
                res.set_content(R"({"status":"Paused"})", "application/json");

            } else if (command == "quit") {
                if (player) {
                    player->stopAudio();
                    player.reset();
                }
                res.set_content(R"({"status":"Stopped"})", "application/json");
                svr.stop();

            } 
            else if (command == "seeking") {
            if (!player) {
                res.status = 400;
                res.set_content(R"({"error":"No track loaded"})", "application/json");
                return;
            }

            if (!json.contains("position") || !json["position"].is_number()) {
                res.status = 400;
                res.set_content(R"({"error":"Missing or invalid 'position' field"})", "application/json");
                return;
            }

            double pos = json["position"];
            if (pos < 0.01 || pos > 1.0) {
                res.status = 400;
                res.set_content(R"({"error":"Position must be between 0.01 and 1.00"})", "application/json");
                return;
            }

            if (!player->seekTo(pos)) {
                res.status = 500;
                res.set_content(R"({"error":"Failed to seek"})", "application/json");
                return;
            }

            res.set_content(R"({"status":"Seeked"})", "application/json");
        }
            else {
                res.status = 400;
                res.set_content(R"({"error":"Unknown command"})", "application/json");
            }

        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(std::string(R"({"error":")") + e.what() + R"("})", "application/json");
        }
    });

    std::cout << "Server listening on port 8080\n";
    svr.listen("0.0.0.0", 8080);

    return 0;
}
