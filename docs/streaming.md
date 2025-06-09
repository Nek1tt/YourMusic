# Streaming Service (Сервис потоковой передачи)

## Зависимости
- **Boost.Beast** - для HTTP-функциональности
- **Boost.Asio** - для асинхронного ввода-вывода  
- **Boost.MySQL** - для работы с MySQL (если используется)
- **nlohmann/json** - для работы с JSON
- **DB** (локальный заголовок) - для работы с базой данных

## Ключевые классы

### StreamingSession
Обрабатывает один HTTP-запрос на потоковую передачу аудио.

\```cpp
class StreamingSession : public std::enable_shared_from_this<StreamingSession>
\```

#### Публичные методы:
- `StreamingSession(beast::tcp_stream&& stream, DB& db)` - конструктор
- `void run()` - запускает обработку сессии

#### Приватные методы:
- `void do_read()` - чтение HTTP-запроса
- `void handle_request(http::request<http::string_body> req)` - обработка запроса
- `void do_write(http::response<http::string_body> res)` - отправка текстового ответа
- `void do_write(http::response<http::vector_body<uint8_t>> res)` - отправка бинарных данных (MP3)
- `void do_close()` - закрытие соединения

### StreamingService
HTTP-сервер для потоковой передачи аудиофайлов.

\```cpp
class StreamingService  
\```

#### Публичные методы:
- `StreamingService(net::io_context& ioc, DB& db, unsigned short port)` - конструктор
- `void run()` - запускает сервер

#### Приватные методы:
- `void do_accept()` - асинхронное принятие подключений
- `void on_accept(beast::error_code ec, tcp::socket socket)` - обработка нового подключения

## Функциональность сервиса
1. **Потоковая передача аудио**
   - Обработка GET-запросов вида `/stream/{track_id}`
   - Получение MP3-BLOB из базы данных
   - Передача аудиопотока клиенту

2. **Особенности реализации**
   - Поддержка бинарной передачи данных
   - Асинхронная обработка запросов
   - Эффективное управление соединениями

## Протокол взаимодействия
- Работает по HTTP (не WebSocket)
- Поддерживаемые методы:
  - `GET /stream/{track_id}` - получение аудиопотока
- Формат ответа:
  - `Content-Type: audio/mpeg` для успешных запросов
  - HTTP 404 если трек не найден
  - HTTP 500 при ошибках сервера

## Запуск

По умолчанию запускается на порту 8084