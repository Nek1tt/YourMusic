// main_ws.cpp

#include "AudioPlayer.h"
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <nlohmann/json.hpp>

using tcp = boost::asio::ip::tcp;
namespace beast     = boost::beast;
namespace websocket = beast::websocket;
using json = nlohmann::json;


class AudioServer 
{
public:
    AudioServer() : player(nullptr), running(true) {}


    json handle_json(const json& request)
    {
        json response;


        if (!request.contains("command") || !request["command"].is_string())
        {
            response["error"] = "Missing or invalid 'command' field";
            return response;
        }

        std::string command = request["command"].get<std::string>();

        try {
            if (command == "load") 
            {
                return handle_load(request);
            }
            else if (command == "resume") 
            {
                return handle_resume();
            }
            else if (command == "pause") 
            {
                return handle_pause();
            }
            else if (command == "quit") 
            {
                return handle_quit();
            }
            else if (command == "seeking") 
            {
                return handle_seek(request);
            }
            else if (command == "volume") 
            {
                return handle_volume(request);
            }
            else 
            {
                response["error"] = "Unknown command: " + command;
                return response;
            }
        }
        catch (const std::exception& e) 
        {

            response["error"] = e.what();
            return response;
        }
    }

    bool is_running() const { return running; }

private:
    std::unique_ptr<AudioPlayer> player;
    bool running;


    json handle_load(const json& j) 
    {
        json response;
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

    json handle_resume() 
    {
        json response;
        if (!player) 
        {
            response["error"] = "No track loaded";
            return response;
        }
        player->playAudio();
        response["status"] = "Resumed";
        return response;
    }

    json handle_pause() 
    {
        json response;
        if (!player) 
        {
            response["error"] = "No track loaded";
            return response;
        }
        player->pauseAudio();
        response["status"] = "Paused";
        return response;
    }

    json handle_quit() 
    {
        json response;
        if (player) 
        {
            player->stopAudio();
            player.reset();
        }
        response["status"] = "Stopped";

        running = false;
        return response;
    }

    json handle_seek(const json& j) 
    {
        json response;
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

    json handle_volume(const json& j) 
    {
        json response;
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
};


int main(int argc, char* argv[]) 
{
    try 
    {

        boost::asio::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {boost::asio::ip::make_address("127.0.0.1"), 8080}};

        std::cout << "WebSocket AudioServer listening on ws://127.0.0.1:8080\n";

        while (true) 
        {
            tcp::socket socket{ioc};
            acceptor.accept(socket);

            std::thread{[s = std::move(socket)]() mutable {
                try 
                {

                    websocket::stream<tcp::socket> ws{std::move(s)};

                    ws.accept();

                    AudioServer server_logic;

                    beast::flat_buffer buffer;
                    while (server_logic.is_running()) 
                    {

                        ws.read(buffer);
                        std::string raw_request = beast::buffers_to_string(buffer.data());
                        buffer.consume(buffer.size());

                        json request_json;
                        json response_json;
                        try 
                        {
                            request_json = json::parse(raw_request);
                            response_json = server_logic.handle_json(request_json);
                        }
                        catch (const std::exception& e) 
                        {

                            response_json = { {"error", "Invalid JSON: "} };
                            response_json["error"] = std::string("Invalid JSON: ") + e.what();
                        }

                        std::string resp_str = response_json.dump();
                        ws.write(boost::asio::buffer(resp_str));
                    }

                    ws.close(websocket::close_code::normal);
                }
                catch (const std::exception& e) 
                {
                    std::cerr << "[Session] Exception: " << e.what() << "\n";
                }
            }}.detach();
        }


    }
    catch (const std::exception& e) 
    {
        std::cerr << "Fatal error in main: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
