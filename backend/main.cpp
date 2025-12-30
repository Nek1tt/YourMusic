#include "ServerHandler.h"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <thread>

using tcp = boost::asio::ip::tcp;
namespace beast     = boost::beast;
namespace websocket = beast::websocket;
using json = nlohmann::json;

int main(int argc, char* argv[]) 
{
    std::cout << "Server started" << std::endl;
    try 
    {
        boost::asio::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {boost::asio::ip::make_address("127.0.0.1"), 8085}};

        std::cout << "WebSocket ServerHandler listening on ws://127.0.0.1:8085\n";

        while (true) 
        {
            std::cout << "Waiting for a new connection..." << std::endl;

            tcp::socket socket{ioc};
            acceptor.accept(socket);

            std::thread{[s = std::move(socket)]() mutable {
                try 
                {
                    websocket::stream<tcp::socket> ws{std::move(s)};
                    ws.accept();

                    ServerHandler handler;

                    beast::flat_buffer buffer;
                    while (handler.is_running()) 
                    {
                        ws.read(buffer);
                        std::string raw_request = beast::buffers_to_string(buffer.data());
                        buffer.consume(buffer.size());

                        json request_json;
                        json response_json;
                        try 
                        {
                            request_json = json::parse(raw_request);
                            response_json = handler.handle_json(request_json);
                        }
                        catch (const std::exception& e) 
                        {
                            response_json = { {"error", std::string("Invalid JSON: ") + e.what()} };
                        }

                        ws.write(boost::asio::buffer(response_json.dump()));
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
