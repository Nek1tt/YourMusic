#include "streaming.h"

// ===================== StreamingServiceSession Implementation =====================
StreamingServiceSession::StreamingServiceSession(tcp::socket&& socket)
    : ws_(std::move(socket)) {
    // Инициализация WebSocket-сессии с переданным сокетом.
    // Регистрация команды "play" в реестре команд.
    commandRegistry_["play"] = std::make_unique<PlayCommand>();
}

void StreamingServiceSession::run() {
    // Асинхронное принятие WebSocket-соединения.
    // Если соединение успешно установлено, вызывается метод doRead для чтения данных.
    ws_.async_accept([self = shared_from_this()](beast::error_code ec) {
        if (!ec) self->doRead(); // Если нет ошибок, начинаем чтение данных.
    });
}

void StreamingServiceSession::doRead() {
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
            if (request.contains("command")) { // Проверяем, содержит ли запрос поле "command".
                std::string command = request["command"]; // Извлекаем название команды.
                if (self->commandRegistry_.count(command)) { // Проверяем, зарегистрирована ли команда.
                    self->commandRegistry_[command]->execute(*self, request); // Выполняем соответствующую команду.
                } else {
                    std::cerr << "Unknown command: " << command << "\n"; // Логируем неизвестную команду.
                }
            }
        } catch (const std::exception& e) {
            // Логируем ошибку парсинга JSON.
            std::cerr << "JSON parse error: " << e.what() << "\n";
        }

        self->buffer_.consume(self->buffer_.size()); // Очищаем буфер после обработки данных.
        self->doRead(); // Рекурсивно вызываем doRead для продолжения чтения данных.
    });
}

void StreamingServiceSession::sendTrackChunk() {
    if (!trackFile_.is_open()) return; // Если файл трека не открыт, завершаем выполнение.

    std::vector<char> data(4096); // Создаем буфер для чтения данных из файла.
    trackFile_.read(data.data(), data.size()); // Читаем данные из файла в буфер.
    std::size_t readSize = trackFile_.gcount(); // Получаем количество прочитанных байт.

    if (readSize > 0) {
        // Асинхронная отправка данных клиенту через WebSocket.
        ws_.async_write(net::buffer(data.data(), readSize), 
            [self = shared_from_this()](beast::error_code ec, std::size_t) {
                if (!ec) self->sendTrackChunk(); // Если нет ошибок, продолжаем отправку следующего блока данных.
            });
    } else {
        trackFile_.close(); // Если данные закончились, закрываем файл.
    }
}

void StreamingServiceSession::logError(beast::error_code ec, const std::string& what) {
    // Логирование ошибок с указанием контекста (what) и сообщения об ошибке.
    std::cerr << what << ": " << (ec ? ec.message() : "Unknown error") << "\n";
}

// ===================== PlayCommand Implementation =====================
void PlayCommand::execute(StreamingServiceSession& session, const nlohmann::json& request) {
    if (request.contains("track_id")) { // Проверяем, содержит ли запрос идентификатор трека.
        // Открываем файл трека для чтения в двоичном режиме.
        session.trackFile_.open("tracks/" + std::to_string(static_cast<int>(request["track_id"])) + ".mp3", std::ios::binary); // TODO: Находить файл в хранилище треков. 
        if (!session.trackFile_) { // Если файл не удалось открыть, логируем ошибку.
            session.logError({}, "File not found");
            return;
        }
        session.sendTrackChunk(); // Начинаем отправку данных трека клиенту.
    }
}

// ===================== StreamingServiceServer Implementation =====================
StreamingServiceServer::StreamingServiceServer(net::io_context& ioc, unsigned short port)
    : acceptor_(ioc, { tcp::v4(), port }) {
    // Инициализация сервера: создание акцептора для прослушивания входящих соединений на указанном порту.
    doAccept(); // Начинаем принимать входящие соединения.
}

void StreamingServiceServer::doAccept() {
    // Асинхронное принятие входящего TCP-соединения.
    acceptor_.async_accept([this](beast::error_code ec, tcp::socket socket) {
        if (!ec) {
            // Если соединение успешно принято, создаем новую сессию и запускаем её.
            std::make_shared<StreamingServiceSession>(std::move(socket))->run();
        }
        doAccept(); // Рекурсивно вызываем doAccept для продолжения приема новых соединений.
    });
}

// ===================== main =====================
int main() {
    std::setlocale(LC_ALL, "C"); 
    net::io_context ioc; // Создаем контекст асинхронных операций.
    StreamingServiceServer server(ioc, 8081); // Инициализируем сервер на порту 8081.
    ioc.run(); // Запускаем цикл обработки событий.
}