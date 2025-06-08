// api-gateway.h
#ifndef APIGATEWAY_H
#define APIGATEWAY_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <memory>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <locale>
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <boost/asio/connect.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class ApiSession;

// ===================== Сессия клиента =====================
class ApiSession : public std::enable_shared_from_this<ApiSession> {
public:
    explicit ApiSession(tcp::socket&& socket);

    void run();
    void sendMessage(const std::string& message);

private:
    websocket::stream<tcp::socket> ws_;
    beast::flat_buffer buffer_;

    void doRead();
    void handleRequest(const nlohmann::json& req);
    void routeToHandler(const std::string& path, const nlohmann::json& req);

    void logError(beast::error_code ec, const std::string& where);

    // Все обработчики
    class AuthHandler;
    class CatalogHandler;

    std::unique_ptr<AuthHandler> authHandler_;
    std::unique_ptr<CatalogHandler> catalogHandler_;
};

// ===================== WebSocket API-сервер =====================
class ApiGatewayServer {
public:
    ApiGatewayServer(net::io_context& ioc, unsigned short port);

private:
    tcp::acceptor acceptor_;
    void doAccept();
};

#endif // APIGATEWAY_H