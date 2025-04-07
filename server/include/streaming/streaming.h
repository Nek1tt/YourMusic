#ifndef STREAMING_H
#define STREAMING_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

// ===================== Forward declarations =====================
class StreamingServiceSession;

// ===================== ICommand interface =====================
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(StreamingServiceSession& session, const nlohmann::json& request) = 0;
};

// ===================== PlayCommand class =====================
class PlayCommand : public ICommand {
public:
    void execute(StreamingServiceSession& session, const nlohmann::json& request) override;
};

// ===================== StreamingServiceSession class =====================
class StreamingServiceSession : public std::enable_shared_from_this<StreamingServiceSession> {
public:
    explicit StreamingServiceSession(tcp::socket&& socket);
    void run();

private:
    websocket::stream<tcp::socket> ws_;
    beast::flat_buffer buffer_;
    std::unordered_map<std::string, std::unique_ptr<ICommand>> commandRegistry_;

    std::ifstream trackFile_;

    void doRead();
    void sendTrackChunk();
    void logError(beast::error_code ec, const std::string& what);

    friend class PlayCommand;
};

// ===================== StreamingServiceServer class =====================
class StreamingServiceServer {
public:
    StreamingServiceServer(net::io_context& ioc, unsigned short port);

private:
    tcp::acceptor acceptor_;

    void doAccept();
};

#endif // STREAMING_H