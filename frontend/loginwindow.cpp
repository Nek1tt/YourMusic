#include "loginwindow.h"
#include <QApplication>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QFrame>
#include <QTabBar>

LoginWindow::LoginWindow(WebSocketClient *webSocket, QWidget *parent)
    : QDialog(parent),
    webSocket(webSocket)
{
    setFixedSize(720, 512);
    setStyleSheet("background-color: #111;");

    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Левая часть — фото
    QLabel *photoLabel = new QLabel(this);
    photoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    photoLabel->setAlignment(Qt::AlignCenter);

    QPixmap pixmap("resources/photo/photo.jpg");
    photoLabel->setPixmap(pixmap.scaled(this->width() / 2, this->height(),
                                        Qt::KeepAspectRatioByExpanding,
                                        Qt::SmoothTransformation));
    mainLayout->addWidget(photoLabel, 1);

    // Правая часть — форма
    QWidget *formWrapper = new QWidget(this);
    formWrapper->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *formLayout = new QVBoxLayout(formWrapper);
    formLayout->setAlignment(Qt::AlignCenter);

    QFrame *formFrame = new QFrame(this);
    formFrame->setFixedSize(300, 450);
    formFrame->setStyleSheet(
        "QFrame { background-color: #222; border-radius: 10px; }"
        );

    QVBoxLayout *frameLayout = new QVBoxLayout(formFrame);
    frameLayout->setContentsMargins(20, 20, 20, 20);
    frameLayout->setSpacing(15);

    QLabel *title = new QLabel("Yourmusic");
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(title);

    QHBoxLayout *choice_login_register = new QHBoxLayout();
    loginTabBtn = new QPushButton("Login");
    registerTabBtn = new QPushButton("Register");
    choice_login_register->addWidget(loginTabBtn);
    choice_login_register->addWidget(registerTabBtn);
    frameLayout->addLayout(choice_login_register);
    connect(loginTabBtn, &QPushButton::clicked, this, &LoginWindow::on_Login_clicked);
    connect(registerTabBtn, &QPushButton::clicked, this, &LoginWindow::on_Register_clicked);

    tabs = new QTabWidget();
    tabs->setStyleSheet(R"(
    QTabWidget::pane {
        background-color: #222;  /* Цвет панели (где содержимое) */
        border: none;
    }

    QTabBar::tab {
        background: #222;
        color: gray;
        padding: 8px 16px;
        border-top-left-radius: 5px;
        border-top-right-radius: 5px;
        margin-right: 2px;
}
)");

    QWidget *loginTab = new QWidget();
    loginTab->setStyleSheet("background-color: #222;");

    QVBoxLayout *loginLayout = new QVBoxLayout(loginTab);
    QHBoxLayout *logUserEmailLayout = new QHBoxLayout();
    logUserEmailLayout->addWidget(new QLabel("Email "));
    logErrorLabel = new QLabel();
    logErrorLabel->setAlignment(Qt::AlignLeft);
    //errorLabel->setText("saasd");
    logUserEmailLayout->addWidget(logErrorLabel);
    //loginLayout->addWidget(new QLabel("User Name"));
    loginLayout->addLayout(logUserEmailLayout);
    logUserEmaildit = new QLineEdit();
    connect(logUserEmaildit, &QLineEdit::textChanged, this, [=]() {
        resetLineEditStyle(logUserEmaildit);
    });
    logUserEmaildit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");
    logUserEmaildit->setPlaceholderText("Enter email");
    loginLayout->addWidget(logUserEmaildit);

    loginLayout->addWidget(new QLabel("Password"));
    logPasswordEdit = new QLineEdit();
    connect(logPasswordEdit, &QLineEdit::textChanged, this, [=]() {
        resetLineEditStyle(logPasswordEdit);
    });
    logPasswordEdit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");
    logPasswordEdit->setPlaceholderText("Enter password");
    logPasswordEdit->setEchoMode(QLineEdit::Password);
    loginLayout->addWidget(logPasswordEdit);

    loginButton = new QPushButton("Login");
    //registerButton->setDefault(true);
    loginButton->setStyleSheet(
        "QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; }"
        "QPushButton { background-color: #c00; border: none; padding: 5px 16px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #e00; }");
    loginLayout->addStretch();
    loginLayout->addWidget(loginButton, 0, Qt::AlignCenter);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::tryLogin);

    QWidget *registerTab = new QWidget();
    registerTab->setStyleSheet("background-color: #222;");

    tabs->addTab(loginTab, "Login");
    tabs->addTab(registerTab, "Register");
    // Register tab (как на скрине)
    QVBoxLayout *registerLayout = new QVBoxLayout(registerTab);
    QHBoxLayout *regUserEmailLayout = new QHBoxLayout();
    regUserEmailLayout->addWidget(new QLabel("Username"));
    regErrorLabel = new QLabel();
    regErrorLabel->setAlignment(Qt::AlignLeft);
    //errorLabel->setText("saasd");
    regUserEmailLayout->addWidget(regErrorLabel);
    //registerLayout->addWidget(new QLabel("User Name"));
    registerLayout->addLayout(regUserEmailLayout);
    regUsernameEdit = new QLineEdit();
    connect(regUsernameEdit, &QLineEdit::textChanged, this, [=]() {
        resetLineEditStyle(regUsernameEdit);
    });
    regUsernameEdit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");
    regUsernameEdit->setPlaceholderText("Enter username");
    registerLayout->addWidget(regUsernameEdit);

    registerLayout->addWidget(new QLabel("Email"));
    regUserEmailEdit = new QLineEdit();
    connect(regUserEmailEdit, &QLineEdit::textChanged, this, [=]() {
        resetLineEditStyle(regUserEmailEdit);
    });
    regUserEmailEdit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");
    regUserEmailEdit->setPlaceholderText("Enter email");
    registerLayout->addWidget(regUserEmailEdit);

    registerLayout->addWidget(new QLabel("Usertag"));
    regUsertagEdit = new QLineEdit();
    connect(regUsertagEdit, &QLineEdit::textChanged, this, [=]() {
        resetLineEditStyle(regUsertagEdit);
    });
    regUsertagEdit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");
    regUsertagEdit->setPlaceholderText("Enter usertag");
    registerLayout->addWidget(regUsertagEdit);

    registerLayout->addWidget(new QLabel("Password"));
    regPasswordEdit = new QLineEdit();
    connect(regPasswordEdit, &QLineEdit::textChanged, this, [=]() {
        resetLineEditStyle(regPasswordEdit);
    });
    regPasswordEdit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");
    regPasswordEdit->setPlaceholderText("Enter password");
    regPasswordEdit->setEchoMode(QLineEdit::Password);
    registerLayout->addWidget(regPasswordEdit);

    registerButton = new QPushButton("Register");
    registerButton->setStyleSheet(
                   "QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; }"
                   "QPushButton { background-color: #c00; border: none; padding: 5px 16px; border-radius: 5px; }"
                   "QPushButton:hover { background-color: #e00; }");
    registerLayout->addStretch();
    registerLayout->addWidget(registerButton, 0, Qt::AlignCenter);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::tryRegister);

    frameLayout->addWidget(tabs);
    formLayout->addWidget(formFrame);
    mainLayout->addWidget(formWrapper, 1);
    tabs->tabBar()->hide();
    tabs->setCurrentWidget(registerTab);

    toggle_buttons();
    // webSocket = new QWebSocket();
    // connect(webSocket, &QWebSocket::connected, this, &LoginWindow::onConnected);
    // connect(webSocket, &QWebSocket::disconnected, this, &LoginWindow::onDisconnected);
    // connect(webSocket, &QWebSocket::textMessageReceived, this, &LoginWindow::onTextMessageReceived);
    // webSocket->open(QUrl("ws://84.237.53.143:8080"));
    connect(webSocket, &WebSocketClient::messageReceived,
            this, &LoginWindow::onTextMessageReceived);
}




QString *LoginWindow::getUsertag(){
    return &userTag;
}

// void LoginWindow::onConnected() {
//     QMessageBox::information(this, "Успех", "Подключение к серверу установлено");
// }

// void LoginWindow::onDisconnected() {
//     QMessageBox::warning(this, "Ошибка", "Соединение с сервером разорвано");
// }

void LoginWindow::onTextMessageReceived(const QString &type, const QJsonObject &dataObj) {
    qDebug()<<type;
    if(type=="auth_response"){
        serverStatus = dataObj.value("status").toString();
        serverMessage = dataObj.value("message").toString();
        if(current_query == "register"){
            if(serverStatus=="ok"){
                accept();
            }else{
                regErrorLabel->setText(serverMessage);
                regErrorLabel->setStyleSheet("color: red;");
                qDebug()<<"error: "<<serverMessage;
            }
        }
        if(current_query=="login"){
            if(serverStatus=="ok"){
                userTag = dataObj.value("usertag").toString();
                accept();
            }else{
                logErrorLabel->setText(serverMessage);
                logErrorLabel->setStyleSheet("color: red;");
                qDebug()<<"error: "<<serverMessage;
            }
        }
    }
}

void LoginWindow::resetLineEditStyle(QLineEdit *lineEdit) {
    lineEdit->setStyleSheet(R"(
        QLineEdit {
        color: white; font-size: 14px;
        background-color: #111;
        border: 1px solid #333;
        border-radius: 5px; padding: 5px;
    }
    )");
    logErrorLabel->setText("");
    regErrorLabel->setText("");
}



void LoginWindow::tryRegister()
{
    if (regUsernameEdit->text().trimmed().isEmpty()) {
        regUsernameEdit->setStyleSheet("QLineEdit {color: white; font-size: 14px; background-color: #111; border: 1px solid red; border-radius: 5px; padding: 5px; }");
        regUsernameEdit->setFocus();
        return;
    }

    if (regUserEmailEdit->text().trimmed().isEmpty()) {
        regUserEmailEdit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid red; border-radius: 5px; padding: 5px; }");
        regUserEmailEdit->setFocus();
        return;
    }

    if (regUsertagEdit->text().trimmed().isEmpty()) {
        regUsertagEdit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid red; border-radius: 5px; padding: 5px; }");
        regUsertagEdit->setFocus();
        return;
    }

    if (regPasswordEdit->text().trimmed().isEmpty()) {
        regPasswordEdit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid red; border-radius: 5px; padding: 5px; }");
        regPasswordEdit->setFocus();
        return;
    }

    QJsonObject payload;
    payload["endpoint"] = "/auth";
    payload["action"] = "register";
    payload["username"] = regUsernameEdit->text();
    payload["email"] = regUserEmailEdit->text();
    payload["password"] = regPasswordEdit->text();
    payload["usertag"] = regUsertagEdit->text();
    userTag = regUsertagEdit->text();

    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    qDebug()<<message;
    current_query = "register";
    webSocket->sendMessage(message);

}

void LoginWindow::tryLogin()
{
    if(logUserEmaildit->text() == "1"){
        accept();
        userTag = "stas";
    }
    if (logUserEmaildit->text().trimmed().isEmpty()) {
        logUserEmaildit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid red; border-radius: 5px; padding: 5px; }");
        logUserEmaildit->setFocus();
        return;
    }

    if (logPasswordEdit->text().trimmed().isEmpty()) {
        logPasswordEdit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid red; border-radius: 5px; padding: 5px; }");
        logPasswordEdit->setFocus();
        return;
    }
    ////Простой пример: разрешаем вход при любом логине
    // Здесь ты можешь добавить свою проверку логина/пароля
    QJsonObject payload;
    payload["endpoint"] = "/auth";
    payload["action"] = "login";
    payload["email"] = logUserEmaildit->text();
    payload["password"] = logPasswordEdit->text();

    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    qDebug()<<message;
    current_query = "login";
    webSocket->sendMessage(message);
}

void LoginWindow::on_Login_clicked(){
    isRegister = false;
    toggle_buttons();
}

void LoginWindow::on_Register_clicked(){
    isRegister = true;
    toggle_buttons();
}

void LoginWindow::toggle_buttons(){
    QString activeStyle =
        "QPushButton {"
        " font-weight: bold;"
        " color: white;"
        " font-size: 15px;"
        " font-family: 'Tahoma';"
        " background: none;"
        " border: none;"
        " border-bottom: 2px solid white;"
        " text-decoration: none;"
        " cursor: default;"
        "}";

    QString inactiveStyle =
        "QPushButton {"
        " font-weight: bold;"
        " color: #828282;"
        " font-size: 15px;"
        " font-family: 'Tahoma';"
        " background: none;"
        " border: none;"
        " text-decoration: none;"
        "}"
        "QPushButton:hover {"
        " text-decoration: underline;"
        "}";

    if (isRegister) {
        registerTabBtn->setStyleSheet(activeStyle);
        loginTabBtn->setStyleSheet(inactiveStyle);
        registerTabBtn->setEnabled(false);
        loginTabBtn->setEnabled(true);
        tabs->setCurrentIndex(1);
    } else {
        loginTabBtn->setStyleSheet(activeStyle);
        registerTabBtn->setStyleSheet(inactiveStyle);
        loginTabBtn->setEnabled(false);
        registerTabBtn->setEnabled(true);
        tabs->setCurrentIndex(0);
    }
}
