// api-gateway.cpp
#include "api-gateway.h"

// ===================== AuthHandler =====================
class ApiSession::AuthHandler {
public:
    explicit AuthHandler(ApiSession& session) : session_(session) {
        
    }

    void handle(const nlohmann::json& req) {
        //отправлять http запрос на auth service и принимать ответ здесь
    }

private:
    ApiSession& session_; // Ссылка на сессию API.
};
    
// ===================== CatalogHandler =====================
class ApiSession::CatalogHandler {
public:
    explicit CatalogHandler(ApiSession& session) : session_(session) {
        
    }

    void handle(const nlohmann::json& req) {
        //отправлять http запрос на catalog service и принимать ответ здесь
    }

private:
    ApiSession& session_; // Ссылка на сессию API.
};

// ===================== StreamingHandler =====================
class ApiSession::StreamingHandler {
public:
    explicit StreamingHandler(ApiSession& session) : session_(session) {
        
    }

    void handle(const nlohmann::json& req) {
        //отправлять http запрос на streaming service и принимать ответ здесь.
        // ATTENTION - проигрывание трека минует этот код и сразу в streaming
    }
private:
    ApiSession& session_; // Ссылка на сессию API.
};
    
// ===================== ApiSession =====================

ApiSession::ApiSession(tcp::socket&& socket)
    : ws_(std::move(socket)),
      authHandler_(nullptr),
      catalogHandler_(nullptr),
      streamingHandler_(nullptr) {
    // Конструктор инициализирует WebSocket-сессию с переданным сокетом.
    // Обработчики для различных эндпоинтов (auth, catalog, streaming) инициализируются как nullptr.
}

void ApiSession::run() {
    // Асинхронное принятие WebSocket-соединения.
    // Если соединение успешно установлено, вызывается метод doRead для чтения данных.
    ws_.async_accept([self = shared_from_this()](beast::error_code ec) {
        if (!ec) self->doRead(); // Если нет ошибок, начинаем чтение данных.
    });
}

void ApiSession::doRead() {
    // Асинхронное чтение данных из WebSocket.
    ws_.async_read(buffer_, [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred); // Игнорируем количество переданных байт (не используется).
        if (ec == websocket::error::closed) return; // Если соединение закрыто, завершаем чтение.
        if (ec) return self->logError(ec, "read"); // Логируем ошибку чтения, если она возникла.

        try {
            // Преобразуем данные из буфера в строку.
            auto message = beast::buffers_to_string(self->buffer_.data());
            // Парсим JSON-сообщение.
            auto request = nlohmann::json::parse(message);
            self->handleRequest(request); // Передаем запрос на обработку.
        } catch (const std::exception& e) {
            // Логируем ошибку парсинга JSON.
            std::cerr << "JSON error: " << e.what() << "\n";
        }

        self->buffer_.consume(self->buffer_.size()); // Очищаем буфер после обработки данных.
        self->doRead(); // Рекурсивно вызываем doRead для продолжения чтения данных.
    });
}

void ApiSession::handleRequest(const nlohmann::json& req) {
    // Проверяем, содержит ли запрос поле "endpoint".
    if (!req.contains("endpoint")) {
        std::cerr << "Missing 'endpoint' field\n"; // Логируем ошибку, если поле отсутствует.
        return;
    }

    std::string endpoint = req["endpoint"]; // Извлекаем значение поля "endpoint".
    routeToHandler(endpoint, req); // Передаем запрос на соответствующий обработчик.
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
    else if (path == "/streaming") {
        if (!streamingHandler_) streamingHandler_ = std::make_unique<StreamingHandler>(*this); // Создаем обработчик стриминга, если он еще не создан.
        streamingHandler_->handle(req); // Передаем запрос обработчику стриминга.
    }
    else {
        std::cerr << "Unknown endpoint: " << path << "\n"; // Логируем неизвестный эндпоинт.
    }
}

void ApiSession::logError(beast::error_code ec, const std::string& where) {
    // Логирование ошибок с указанием контекста (where) и сообщения об ошибке.
    std::cerr << where << ": " << ec.message() << "\n";
}

// ===================== ApiGatewayServer =====================
ApiGatewayServer::ApiGatewayServer(net::io_context& ioc, unsigned short port)
    : acceptor_(ioc, { tcp::v4(), port }) {
    // Инициализация сервера: создание акцептора для прослушивания входящих соединений на указанном порту.
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
    std::setlocale(LC_ALL, "C"); 
    net::io_context ioc; // Создаем контекст асинхронных операций.
    ApiGatewayServer server(ioc, 8080); // Инициализируем сервер на порту 8080.
    ioc.run(); // Запускаем цикл обработки событий.
}