#include "ServerHandler.h"
#include <nlohmann/json.hpp>
#include <regex>

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
        if (command == "auth") return handle_auth(request);
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

    player->setVolume(static_cast<float>(volume_level));

    player->playAudio();
    response["status"] = "Playing";
    return response;
}

nlohmann::json ServerHandler::handle_load_only(const nlohmann::json& j) 
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

    player->setVolume(static_cast<float>(volume_level));

    response["status"] = "Loaded";
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

    volume_level = level; 

    if (player) 
    {
        player->setVolume(static_cast<float>(volume_level));
    }

    response["status"] = "Volume set to " + std::to_string(volume_level);
    return response;
}

bool is_valid_email_or_login(const std::string& input) {
    if (input.find('@') != std::string::npos) {
        const std::regex email_pattern(R"(^[A-Za-z0-9._%-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
        return std::regex_match(input, email_pattern);
    } else {
        const std::regex login_pattern(R"(^[A-Za-z0-9._-]+$)");
        return std::regex_match(input, login_pattern);
    }
}

bool is_valid_password(const std::string& password) {
    if (password.size() < 8 || password.size() > 20) return false;
    const std::regex password_pattern(R"(^[A-Za-z0-9._-]+$)");
    if (!std::regex_match(password, password_pattern)) return false;

    for (char c : password) {
        if (isalnum(static_cast<unsigned char>(c))) return true;
    }
    return false;
}

nlohmann::json ServerHandler::handle_auth(const nlohmann::json& request) {
    nlohmann::json response;
    if (!request.contains("login") || !request["login"].is_string()) {
        response["error"] = "Missing or invalid 'login' field";
        return response;
    }
    if (!request.contains("password") || !request["password"].is_string()) {
        response["error"] = "Missing or invalid 'password' field";
        return response;
    }

    std::string login = request["login"];
    std::string password = request["password"];

    bool login_ok = is_valid_email_or_login(login);
    bool pass_ok = is_valid_password(password);

    if (login_ok && pass_ok) {
        response["status"] = "ACCEPTED";
        response["message"] = "Login and password are valid";
    } else {
        response["status"] = "REJECTED";
        if (!login_ok) response["login_error"] = "Invalid login/email format";
        if (!pass_ok) response["password_error"] = "Invalid password format";
    }
    return response;
}
