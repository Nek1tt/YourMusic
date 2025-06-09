# Auth Service (Сервис аутентификации)

## Зависимости
- **Boost.Beast** - для HTTP-функциональности
- **Boost.Asio** - для асинхронного ввода-вывода
- **nlohmann/json** - для работы с JSON
- **DB** (локальный заголовок) - для работы с базой данных

## Ключевые классы

### HttpSession
Обрабатывает одно HTTP-соединение: читает запрос и отправляет ответ.

\```cpp
class HttpSession : public std::enable_shared_from_this<HttpSession>
\```

#### Публичные методы:
- `HttpSession(tcp::socket socket, DB& db)` - конструктор, принимает сокет и ссылку на БД
- `void run()` - запускает обработку сессии

#### Приватные методы:
- `void do_read()` - асинхронное чтение запроса
- `void handle_request(http::request<http::string_body> req)` - обработка HTTP-запроса
- `http::response<http::string_body> handle_register(const json& body)` - обработка регистрации
- `http::response<http::string_body> handle_login(const json& body)` - обработка входа
- `void do_write(http::response<http::string_body> res)` - отправка ответа клиенту

### AuthService
HTTP-сервер, который слушает порт и создает HttpSession для каждого подключения.

\```cpp
class AuthService
\```

#### Публичные методы:
- `AuthService(net::io_context& ioc, DB& db, unsigned short port)` - конструктор
- `void run()` - запускает сервер

#### Приватные методы:
- `void do_accept()` - асинхронное принятие подключений
- `void on_accept(beast::error_code ec, tcp::socket socket)` - обработка нового подключения

## Функциональность сервиса
1. **Регистрация пользователей**
   - Обработка POST-запросов на `/register`
   - Валидация входных данных
   - Создание учетной записи в БД

2. **Аутентификация пользователей**
   - Обработка POST-запросов на `/login`
   - Проверка учетных данных
   - Генерация токена доступа

## Протокол взаимодействия
- Работает по HTTP (не WebSocket)
- Использует JSON-формат для тела запросов/ответов
- Поддерживаемые методы:
  - `POST /register` - регистрация нового пользователя
  - `POST /login` - аутентификация существующего пользователя


## Запуск

По умолчанию запускается на порту 8082