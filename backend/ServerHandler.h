#pragma once
#include "AudioPlayer.h"
#include <nlohmann/json.hpp>
#include <memory>


class AudioPlayer;

class ServerHandler {
public:
    ServerHandler();

    nlohmann::json handle_json(const nlohmann::json& request);
    nlohmann::json handle_load(const nlohmann::json& j);
    nlohmann::json handle_load_only(const nlohmann::json& j);
    nlohmann::json handle_resume();
    nlohmann::json handle_pause();
    nlohmann::json handle_quit();
    nlohmann::json handle_seek(const nlohmann::json& j);
    nlohmann::json handle_volume(const nlohmann::json& j);
    nlohmann::json handle_auth(const nlohmann::json& request);
    nlohmann::json handle_duration(const nlohmann::json& j);
    bool is_running() const;

private:
    std::unique_ptr<AudioPlayer> player;
    bool running;
};
