#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QTabWidget>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include "websocketclient.h"

class QPushButton;
class QLineEdit;

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    LoginWindow(WebSocketClient *webSocket, QWidget *parent = nullptr);
    QString *getUsertag();

private slots:
    void tryLogin();
    void tryRegister();
    void toggle_buttons();
    void on_Login_clicked();
    void on_Register_clicked();
    void onTextMessageReceived(const QString &type, const QJsonObject &data);
    void resetLineEditStyle(QLineEdit *lineEdit);

private:
    bool isRegister = false;
    // В LoginWindow.h (например, в private)
    QString current_query;
    QString serverStatus;
    QString serverMessage;
    QString userTag;
    QJsonObject serverPayload;
    QLabel *logErrorLabel;
    QLabel *regErrorLabel;

    WebSocketClient *webSocket;
    QTabWidget *tabs;
    QPushButton *loginTabBtn;
    QPushButton *registerTabBtn;
    QLineEdit *logUserEmaildit;
    QLineEdit *logPasswordEdit;
    QLineEdit *regUsernameEdit;
    QLineEdit *regUserEmailEdit;
    QLineEdit *regUsertagEdit;
    QLineEdit *regPasswordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
};

#endif // LOGINWINDOW_H
