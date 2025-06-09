// api-gateway.cpp
#include "api-gateway.h"

// ===================== AuthHandler =====================
class ApiSession::AuthHandler {
public:
    explicit AuthHandler(ApiSession& session) : session_(session) {
        
    }

    void handle(const nlohmann::json& req) {
        if (!req.contains("action")) {
            sendErrorResponse("Missing 'action' field");
            return;
        }
        
        std::string action = req["action"];

        std::string endpoint;
        if (action == "register") {
            endpoint = "/register";
        } else if (action == "login") {
            endpoint = "/login";
        } else {
            sendErrorResponse("Unknown action: " + action);
            return;
        }
        
        sendHttpRequest(endpoint, req);
    }

private:
    ApiSession& session_;
    
    void sendHttpRequest(const std::string& endpoint, const nlohmann::json& requestData) {
        try {
            net::io_context ioc;
            tcp::resolver resolver(ioc);
            auto const results = resolver.resolve("127.0.0.1", "8082");
            beast::tcp_stream stream(ioc);
            stream.connect(results);
            nlohmann::json requestBody;
            for (auto& [key, value] : requestData.items()) {
                if (key != "endpoint" && key != "action") {
                    requestBody[key] = value;
                }
            }
            
            std::string body = requestBody.dump();
  
            http::request<http::string_body> req{http::verb::post, endpoint, 11};
            req.set(http::field::host, "127.0.0.1:8082");
            req.set(http::field::user_agent, "API-Gateway/1.0");
            req.set(http::field::content_type, "application/json");
            req.body() = body;
            req.prepare_payload();

            //для отладки
            std::cout << "[ApiGateway→Auth] OUTGOING HTTP:\n" << req << "\n";
            
            http::write(stream, req);

            beast::flat_buffer buffer;
            http::response<http::string_body> res;
            http::read(stream, buffer, res);

            beast::error_code ec;
            stream.socket().shutdown(tcp::socket::shutdown_both, ec);

            sendResponse(res.body());
            
        } catch (const std::exception& e) {
            std::cerr << "HTTP request error: " << e.what() << "\n";
            sendErrorResponse("Auth service unavailable");
        }
    }
    
    void sendResponse(const std::string& response) {
        try {
            nlohmann::json responseJson;
            responseJson["type"] = "auth_response";
            responseJson["data"] = nlohmann::json::parse(response);
            
            session_.sendMessage(responseJson.dump());
        } catch (const std::exception& e) {
            std::cerr << "Response parsing error: " << e.what() << "\n";
            sendErrorResponse("Invalid response from auth service");
        }
    }
    
    void sendErrorResponse(const std::string& error) {
        nlohmann::json errorResponse;
        errorResponse["type"] = "auth_response";
        errorResponse["data"]["status"] = "error";
        errorResponse["data"]["message"] = error;
        
        session_.sendMessage(errorResponse.dump());
    }
};

// ===================== CatalogHandler =====================
class ApiSession::CatalogHandler {
public:
    explicit CatalogHandler(ApiSession& session) : session_(session) {}

    void handle(const nlohmann::json& req) {
        if (!req.contains("action")) {
            sendErrorResponse("Missing 'action' field");
            return;
        }

        std::string action = req["action"];
        std::string endpoint;
        if (action == "home") {
            endpoint = "/home";
        } else if (action == "profile") {
            endpoint = "/profile";
        } else if (action == "create") {
            endpoint = "/create";
        }
          else if (action == "search") {
            endpoint = "/search";
        }
          else if (action == "search_user") {
            endpoint = "/search_user";
        }
        else if (action == "user_action") {
            endpoint = "/user_action";
        }
        else {
            sendErrorResponse("Unknown action: " + action);
            return;
        }
        sendHttpRequest(endpoint, req);
    }

private:
    ApiSession& session_;

    void sendHttpRequest(const std::string& endpoint, const nlohmann::json& requestData) {

        try {
            net::io_context ioc;
            tcp::resolver resolver(ioc);
            auto const results = resolver.resolve("127.0.0.1", "8083");

            beast::tcp_stream stream(ioc);
            stream.connect(results);
            stream.expires_after(std::chrono::seconds(120));

            nlohmann::json requestBody;
            for (auto& [key, value] : requestData.items()) {
                if (key != "endpoint") {
                    requestBody[key] = value;
                }
            }  
            std::string body = requestBody.dump();

            http::request<http::string_body> httpReq{http::verb::post, endpoint, 11};
            httpReq.set(http::field::host, "127.0.0.1:8083");
            httpReq.set(http::field::user_agent, "API-Gateway/1.0");
            httpReq.set(http::field::content_type, "application/json");
            httpReq.body() = body;
            httpReq.prepare_payload();

            //для отладки
            std::cout << "[ApiGateway→Catalog] OUTGOING HTTP:\n" << httpReq << "\n";
            http::write(stream, httpReq);
            stream.expires_never();

            stream.expires_after(std::chrono::seconds(120));

            beast::flat_buffer buffer;
            http::response<http::string_body> httpRes;
            http::read(stream, buffer, httpRes);
            stream.expires_never();

            beast::error_code ec;
            stream.socket().shutdown(tcp::socket::shutdown_both, ec);

            sendResponse(httpRes.body());
        } catch (const std::exception& e) {
            std::cerr << "[CatalogHandler] Exception: " << e.what() << "\n";
            sendErrorResponse("Catalog service unavailable");
        }
    }

    void sendResponse(const std::string& response) {
        try {
            nlohmann::json responseJson;
            responseJson["type"] = "catalog_response";
            responseJson["data"] = nlohmann::json::parse(response);

            session_.sendMessage(responseJson.dump());
        } catch (const std::exception& e) {
            std::cerr << "[CatalogHandler] Response parsing error: " << e.what() << "\n";
            sendErrorResponse("Invalid response from catalog service");
        }
    }

    void sendErrorResponse(const std::string& error) {
        nlohmann::json errorResponse;
        errorResponse["type"] = "catalog_response";
        errorResponse["data"]["status"] = "error";
        errorResponse["data"]["message"] = error;

        session_.sendMessage(errorResponse.dump());
    }
};
    
// ===================== ApiSession =====================

ApiSession::ApiSession(tcp::socket&& socket)
    : ws_(std::move(socket)),
      authHandler_(nullptr),
      catalogHandler_(nullptr) {}

void ApiSession::run() {
    ws_.async_accept([self = shared_from_this()](beast::error_code ec) {
        if (!ec) self->doRead();
    });
}

void ApiSession::doRead() {
    ws_.async_read(buffer_, [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred); 
        if (ec == websocket::error::closed) return;
        if (ec) return self->logError(ec, "read");

        try {
            auto message = beast::buffers_to_string(self->buffer_.data());
            auto request = nlohmann::json::parse(message);
            self->handleRequest(request);
        } catch (const std::exception& e) {
            std::cerr << "JSON error: " << e.what() << "\n";
        }

        self->buffer_.consume(self->buffer_.size()); 
        self->doRead();
    });
}

void ApiSession::handleRequest(const nlohmann::json& req) {
    if (!req.contains("endpoint")) {
        std::cerr << "Missing 'endpoint' field\n"; 
        return;
    }

    std::string endpoint = req["endpoint"];
    routeToHandler(endpoint, req);
}

void ApiSession::routeToHandler(const std::string& path, const nlohmann::json& req) {
    // Маршрутизация запроса на основе значения "path".
    if (path == "/auth") {
        if (!authHandler_) authHandler_ = std::make_unique<AuthHandler>(*this); // Создаем обработчик авторизации, если он еще не создан.
        authHandler_->handle(req); // Передаем запрос обработчику авторизации.
    }
    else if (path == "/catalog") {
        if (!catalogHandler_) catalogHandler_ = std::make_unique<CatalogHandler>(*this); // Создаем обработчик каталога, если он еще не создан.
        catalogHandler_->handle(req); // Передаем запрос обработчику каталога.
    }
    else {
        std::cerr << "Unknown endpoint: " << path << "\n"; // Логируем неизвестный эндпоинт.
    }
}

void ApiSession::sendMessage(const std::string& message) {
    auto msg = std::make_shared<std::string>(message);
    ws_.async_write(
        net::buffer(*msg),
        [self = shared_from_this(), msg](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (ec) {
                self->logError(ec, "write");
            }
        }
    );
}

void ApiSession::logError(beast::error_code ec, const std::string& where) {
    std::cerr << where << ": " << ec.message() << "\n";
}

// ===================== ApiGatewayServer =====================
ApiGatewayServer::ApiGatewayServer(net::io_context& ioc, unsigned short port)
    : acceptor_(ioc, { tcp::v4(), port }) {
    doAccept(); // Начинаем принимать входящие соединения.
}

void ApiGatewayServer::doAccept() {
    // Асинхронное принятие входящего TCP-соединения.
    acceptor_.async_accept([this](beast::error_code ec, tcp::socket socket) {
        if (!ec) {
            // Если соединение успешно принято, создаем новую сессию и запускаем её.
            std::make_shared<ApiSession>(std::move(socket))->run();
        }
        doAccept(); // Рекурсивно вызываем doAccept для продолжения приема новых соединений.
    });
}

// ===================== main =====================
int main() {
    net::io_context ioc;
    ApiGatewayServer server(ioc, 8080);
    std::cout << "[ApiGateway] Server is running on port 8080...\n";
    ioc.run();
}