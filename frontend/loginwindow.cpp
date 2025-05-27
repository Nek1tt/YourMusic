#include "loginwindow.h"
#include <QApplication>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QFrame>
#include <QTabBar>


LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
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

    QPixmap pixmap("../resources/photo/photo.jpg");
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
    formFrame->setFixedSize(300, 350);
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

    loginLayout->addWidget(new QLabel("Email"));
    logUsernameEdit = new QLineEdit();
    logUsernameEdit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");
    logUsernameEdit->setPlaceholderText("Enter your email");
    loginLayout->addWidget(logUsernameEdit);

    loginLayout->addWidget(new QLabel("Password"));
    logPasswordEdit = new QLineEdit();
    logPasswordEdit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");
    logPasswordEdit->setPlaceholderText("Enter your password");
    logPasswordEdit->setEchoMode(QLineEdit::Password);
    loginLayout->addWidget(logPasswordEdit);

    loginButton = new QPushButton("Login");
    loginButton->setStyleSheet(
        "QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; }"
        "QPushButton { background-color: #c00; border: none; padding: 8px 16px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #e00; }");
    loginLayout->addStretch();
    loginLayout->addWidget(loginButton, 0, Qt::AlignCenter);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::tryLogin);

    QWidget *registerTab = new QWidget();
    registerTab->setStyleSheet("background-color: #222;");

//     registerTab->setStyleSheet(R"(
//     QTabWidget {background-color: #222;
//     border: none;
//     color: white;
// }
//     QLineEdit {
//         background-color: #111;
//         color: white;
//         border: 1px solid #555;
//         border-radius: 4px;
//         padding: 4px 8px;
//     }

//     QPushButton {
//         background-color: #c00;
//         color: white;
//         border: none;
//         border-radius: 4px;
//         padding: 6px 12px;
//     }

//     QPushButton:hover {
//         background-color: #a00;
//     }

//     QPushButton:pressed {
//         background-color: #800;
//     }
// )");


    tabs->addTab(loginTab, "Login");
    tabs->addTab(registerTab, "Register");

    // Register tab (как на скрине)
    QVBoxLayout *registerLayout = new QVBoxLayout(registerTab);

    registerLayout->addWidget(new QLabel("Email"));
    regUsernameEdit = new QLineEdit();
    regUsernameEdit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");
    regUsernameEdit->setPlaceholderText("Enter your email");
    registerLayout->addWidget(regUsernameEdit);

    registerLayout->addWidget(new QLabel("Password"));
    regPasswordEdit = new QLineEdit();
    regPasswordEdit->setStyleSheet("QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; } QLineEdit { background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");
    regPasswordEdit->setPlaceholderText("Enter your password");
    regPasswordEdit->setEchoMode(QLineEdit::Password);
    registerLayout->addWidget(regPasswordEdit);

    registerButton = new QPushButton("Register");
    registerButton->setStyleSheet(
                   "QLabel, QLineEdit, QPushButton { color: white; font-size: 14px; }"
                   "QPushButton { background-color: #c00; border: none; padding: 8px 16px; border-radius: 5px; }"
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

}

void LoginWindow::tryRegister()
{
    // Простой пример: разрешаем вход при любом логине
    // Здесь ты можешь добавить свою проверку логина/пароля
    if (!regUsernameEdit->text().isEmpty()) {
        accept(); // закрыть окно и вернуть QDialog::Accepted
    } else {
        // можно показать ошибку
    }
}

void LoginWindow::tryLogin()
{
    // Простой пример: разрешаем вход при любом логине
    // Здесь ты можешь добавить свою проверку логина/пароля
    if (!logUsernameEdit->text().isEmpty()) {
        accept(); // закрыть окно и вернуть QDialog::Accepted
    } else {
        // можно показать ошибку
    }
}

void LoginWindow::on_Login_clicked(){
    isLogin = false;
    toggle_buttons();
}

void LoginWindow::on_Register_clicked(){
    isLogin = true;
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

    if (isLogin) {
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
