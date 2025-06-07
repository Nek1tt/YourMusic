#include "WebSocketClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>


WebSocketClient::WebSocketClient(QString port, QObject *parent)
    : QObject(parent)
{
    connect(&m_socket, &QWebSocket::textMessageReceived,
            this, &WebSocketClient::onTextMessageReceived);
    connect(&m_socket, &QWebSocket::connected,
            this, &WebSocketClient::onConnected);
    connect(&m_socket, &QWebSocket::disconnected,
            this, &WebSocketClient::onDisconnected);

    m_socket.open(QUrl(port));
}

void WebSocketClient::sendMessage(const QString &message) {
    if (m_socket.state() == QAbstractSocket::ConnectedState) {
        m_socket.sendTextMessage(message);
        qDebug()<<"sendMeassage";
    } else {
        //QMessageBox::warning(nullptr, "Ошибка", "Нет подключения к серверу");
    }

}

QWebSocket *WebSocketClient::socket() {
    return &m_socket;
}

void WebSocketClient::onTextMessageReceived(const QString &message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());

    if (!doc.isNull() && doc.isObject()) {
        QJsonObject obj = doc.object();
        QString type = obj.value("type").toString(); // <-- правильно: из корня
        QJsonObject dataObj = obj.value("data").toObject();

        qDebug() << "Тип запроса:" << type;
        //qDebug() << "Данные:" << dataObj;

        emit messageReceived(type, dataObj);
    } else {
        QMessageBox::warning(nullptr, "Ошибка", "Ответ сервера не является корректным JSON");
    }
}


void WebSocketClient::onConnected() {
    qDebug()<<"success";
    QMessageBox::information(nullptr, "Успех", "Подключение к серверу установлено");
}

void WebSocketClient::onDisconnected() {
    qDebug()<<"disconnected";
   // QMessageBox::warning(nullptr, "Ошибка", "Соединение с сервером разорвано");
     m_socket.open(QUrl("ws://84.237.53.143:8080"));
}
