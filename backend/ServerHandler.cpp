#include "ServerHandler.h"
#include <nlohmann/json.hpp>

ServerHandler::ServerHandler() : player(nullptr), running(true) {}

nlohmann::json ServerHandler::handle_json(const nlohmann::json& request) {
    nlohmann::json response;
    if (!request.contains("command") || !request["command"].is_string()) {
        response["error"] = "Missing or invalid 'command' field";
        return response;
    }

    std::string command = request["command"];
    try {
        if (command == "load") return handle_load(request);
        if (command == "resume") return handle_resume();
        if (command == "pause") return handle_pause();
        if (command == "quit") return handle_quit();
        if (command == "seeking") return handle_seek(request);
        if (command == "volume") return handle_volume(request);
        response["error"] = "Unknown command: " + command;
    } catch (const std::exception& e) {
        response["error"] = e.what();
    }
    return response;
}

bool ServerHandler::is_running() const { return running; }

nlohmann::json ServerHandler::handle_load(const nlohmann::json& j) 
{
    nlohmann::json response;
    if (!j.contains("path") || !j["path"].is_string()) 
    {
        response["error"] = "Missing or invalid 'path' field for load";
        return response;
    }

    std::string path = j["path"].get<std::string>();

    if (player) 
    {
        player->stopAudio();
        player.reset();
    }

    player = std::make_unique<AudioPlayer>(path.c_str());
    if (!player->init()) 
    {
        player.reset();
        response["error"] = "Failed to load audio file: " + path;
        return response;
    }

    player->playAudio();
    response["status"] = "Playing";
    return response;
}

nlohmann::json ServerHandler::handle_resume() 
{
    nlohmann::json response;
    if (!player) 
    {
        response["error"] = "No track loaded";
        return response;
    }
    player->playAudio();
    response["status"] = "Resumed";
    return response;
}

nlohmann::json ServerHandler::handle_pause() 
{
    nlohmann::json response;
    if (!player) 
    {
        response["error"] = "No track loaded";
        return response;
    }
    player->pauseAudio();
    response["status"] = "Paused";
    return response;
}

nlohmann::json ServerHandler::handle_quit() 
{
    nlohmann::json response;
    if (player) 
    {
        player->stopAudio();
        player.reset();
    }
    response["status"] = "Stopped";
    running = false;
    return response;
}

nlohmann::json ServerHandler::handle_seek(const nlohmann::json& j) 
{
    nlohmann::json response;
    if (!player) 
    {
        response["error"] = "No track loaded";
        return response;
    }
    if (!j.contains("position") || !j["position"].is_number()) 
    {
        response["error"] = "Missing or invalid 'position' field";
        return response;
    }
    double pos = j["position"].get<double>();
    if (pos < 0.0 || pos > 1.0) 
    {
        response["error"] = "Position must be between 0.0 and 1.0";
        return response;
    }
    if (!player->seekTo(pos)) 
    {
        response["error"] = "Failed to seek to position " + std::to_string(pos);
        return response;
    }
    response["status"] = "Seeked";
    return response;
}

nlohmann::json ServerHandler::handle_volume(const nlohmann::json& j) 
{
    nlohmann::json response;
    if (!player) 
    {
        response["error"] = "No track loaded";
        return response;
    }
    if (!j.contains("level") || !j["level"].is_number()) 
    {
        response["error"] = "Missing or invalid 'level' field";
        return response;
    }
    double level = j["level"].get<double>();
    if (level < 0.0 || level > 1.0) 
    {
        response["error"] = "Volume level must be between 0.0 and 1.0";
        return response;
    }
    player->setVolume(static_cast<float>(level));
    response["status"] = "Volume set to " + std::to_string(level);
    return response;
}
