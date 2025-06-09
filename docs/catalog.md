# Catalog Service (Сервис каталога)

## Зависимости
- **Boost.Beast** - для HTTP-функциональности
- **Boost.Asio** - для асинхронного ввода-вывода
- **nlohmann/json** - для работы с JSON
- **Boost.MySQL** - для работы с MySQL
- **DB** - для работы с базой данных

## Основные компоненты

### CatalogSession
Обрабатывает HTTP-запросы к каталогу, диспетчеризует по action.

\```cpp
class CatalogSession : public std::enable_shared_from_this<CatalogSession>
\```

#### Публичные методы:
- `CatalogSession(boost::beast::tcp_stream&& stream, DB& db)` - конструктор
- `void run()` - запускает обработку сессии

#### Приватные методы:
- `void do_read()` - чтение HTTP-запроса
- `void handle_request(http::request<http::string_body> req)` - обработка запроса
- `void init_action_map()` - инициализация карты обработчиков
- Методы-обработчики для различных действий (`on_home`, `on_profile` и др.)
- Методы `do_write` для отправки ответов

### CatalogService
HTTP-сервер для обработки запросов к каталогу.

\```cpp
class CatalogService
\```

#### Публичные методы:
- `CatalogService(net::io_context& ioc, DB& db, unsigned short port)` - конструктор
- `void run()` - запускает сервер

#### Приватные методы:
- `void do_accept()` - асинхронное принятие подключений
- `void on_accept(beast::error_code ec, tcp::socket socket)` - обработка нового подключения

## Специализированные обработчики

### CreateHandler
Создание альбомов и треков.

\```cpp
class CreateHandler
\```

#### Особенности:
- Обрабатывает запросы на создание контента
- Поддерживает загрузку бинарных данных (Base64)
- Метод: `operator()(int http_version, const json& body_json)`

### HomeHandler
Главная страница (новинки и рекомендации).

\```cpp
class HomeHandler
\```

#### Особенности:
- Возвращает персонализированные рекомендации
- Учитывает usertag текущего пользователя
- Метод: `operator()(int http_version) const`

### ProfileHandler
Работа с профилями пользователей.

\```cpp
class ProfileHandler
\```

#### Особенности:
- Обрабатывает как собственный, так и чужие профили
- Возвращает комплексную информацию:
  - Основные данные пользователя
  - Альбомы
  - Лайкнутые и загруженные треки
  - Статусы подписок

### SearchHandler
Поиск по трекам и альбомам.

\```cpp
class SearchHandler
\```

#### Особенности:
- Полнотекстовый поиск
- Возвращает структурированные результаты
- Методы: `handle_search()` и `operator()`

### SearchUserHandler
Поиск пользователей.

\```cpp
class SearchUserHandler
\```

#### Особенности:
- Поиск по usertag/username
- Возвращает список совпадений
- Методы: `handle_search_user()` и `operator()`

### UserActionHandler
Обработка действий пользователя.

\```cpp
class UserActionHandler
\```

#### Особенности:
- Универсальный обработчик различных действий
- Использует карту действий (`actions_`) для маршрутизации
- Поддерживает расширяемость через регистрацию новых обработчиков

## API Endpoints

| Endpoint | Method | Handler | Description |
|----------|--------|---------|-------------|
| `/home` | POST | HomeHandler | Главная страница с рекомендациями |
| `/profile` | POST | ProfileHandler | Получение профиля пользователя |
| `/create` | POST | CreateHandler | Создание альбома/трека |
| `/search` | POST | SearchHandler | Поиск по контенту |
| `/search/user` | POST | SearchUserHandler | Поиск пользователей |
| `/user/action` | POST | UserActionHandler | Различные действия пользователя |

## Формат запросов/ответов
- Все запросы и ответы используют JSON
- Для создания контента поддерживается передача бинарных данных (Base64)
- Ответы содержат стандартизированную структуру в зависимости от типа запроса