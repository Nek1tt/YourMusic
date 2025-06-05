#include <QObject>
#include <QWebSocket>


#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

class WebSocketClient : public QObject {
    Q_OBJECT

public:
    explicit WebSocketClient(QString port, QObject *parent = nullptr);
    void sendMessage(const QString &message);
    void onConnected();
    void onDisconnected();
    QWebSocket *socket();

signals:
    void messageReceived(const QString &type, const QJsonObject &data);

private slots:
    void onTextMessageReceived(const QString &message);

private:
    QWebSocket m_socket;
};

#endif // WEBSOCKETCLIENT_H
