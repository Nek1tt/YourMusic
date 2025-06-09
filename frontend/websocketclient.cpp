#include "WebSocketClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QDebug>

//класс для вебсокета, с помощью него мы можем раз создать и подключиться к серверу и отправлять и получать данные
WebSocketClient::WebSocketClient(QString port, QObject *parent)
    : QObject(parent)
{
    //прописываем функции на реакции сокета
    connect(&m_socket, &QWebSocket::textMessageReceived,
            this, &WebSocketClient::onTextMessageReceived);
    connect(&m_socket, &QWebSocket::connected,
            this, &WebSocketClient::onConnected);
    connect(&m_socket, &QWebSocket::disconnected,
            this, &WebSocketClient::onDisconnected);

    m_socket.open(QUrl(port));
}


//функция отправки сообщения через сокет, принимает уже подготовленный json файл
void WebSocketClient::sendMessage(const QString &message)
{
    if (m_socket.state() == QAbstractSocket::ConnectedState) {
        m_socket.sendTextMessage(message);
        qDebug() << "sendMessage:" << message;
    } else {
        // QMessageBox::warning(nullptr, "Ошибка", "Нет подключения к серверу");
        qDebug() << "Попытка отправки сообщения при отсутствии подключения";
    }
}

QWebSocket* WebSocketClient::socket()
{
    return &m_socket;
}
    //функция получения ответа от сервера, посылает сигнал. разные обработчики ответа на разных уровнях виджетов получают сигнал и решают что с ним делать
void WebSocketClient::onTextMessageReceived(const QString &message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());

    if (!doc.isNull() && doc.isObject()) {
        QJsonObject obj = doc.object();
        QString type = obj.value("type").toString();
        QJsonObject dataObj = obj.value("data").toObject();

        qDebug() << "Тип запроса:" << type;
        // qDebug() << "Данные:" << dataObj;

        emit messageReceived(type, dataObj);
    } else {
        //QMessageBox::warning(nullptr, "Ошибка", "Ответ сервера не является корректным JSON");
    }
}
//функция срабатывающая при подключенни
void WebSocketClient::onConnected()
{
    qDebug() << "Успешное подключение к серверу";
    //QMessageBox::information(nullptr, "Успех", "Подключение к серверу установлено");
}
//функция срабатывающая при отключении от сервера
void WebSocketClient::onDisconnected()
{
    qDebug() << "Соединение с сервером разорвано";
    // QMessageBox::warning(nullptr, "Ошибка", "Соединение с сервером разорвано");

    // Попытка переподключения
    m_socket.open(QUrl("ws://84.237.53.143:8080"));
}
