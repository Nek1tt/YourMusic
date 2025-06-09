# API Сервис музыкального сервиса

## Зависимости
- **Boost.Beast** - для HTTP/WebSocket функциональности
- **Boost.Asio** - для асинхронного ввода-вывода
- **nlohmann/json** - для работы с JSON

## Ключевые классы

### ApiSession
Класс для управления клиентскими сессиями через WebSocket.

\```cpp
class ApiSession : public std::enable_shared_from_this<ApiSession>
\```

#### Публичные методы:
- `ApiSession(tcp::socket&& socket)` - конструктор, принимающий сокет
- `void run()` - запускает сессию
- `void sendMessage(const std::string& message)` - отправляет сообщение клиенту

#### Приватные методы:
- `void doRead()` - асинхронное чтение сообщения
- `void handleRequest(const nlohmann::json& req)` - обработка входящего запроса
- `void routeToHandler(const std::string& path, const nlohmann::json& req)` - маршрутизация запроса
- `void logError(beast::error_code ec, const std::string& where)` - логирование ошибок

#### Внутренние обработчики:
- `AuthHandler` - для аутентификации
- `CatalogHandler` - для работы с каталогом

### ApiGatewayServer
Класс WebSocket сервера для обработки API-запросов.

\```cpp
class ApiGatewayServer
\```

#### Публичные методы:
- `ApiGatewayServer(net::io_context& ioc, unsigned short port)` - конструктор, принимает контекст ввода-вывода и порт

#### Приватные методы:
- `void doAccept()` - асинхронное принятие новых подключений


## Запуск

По умолчанию запускается на порту 8080