#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QTabWidget>

class QPushButton;
class QLineEdit;

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);

private slots:
    void tryLogin();
    void tryRegister();
    void toggle_buttons();
    void on_Login_clicked();
    void on_Register_clicked();

private:
    bool isLogin = true;
    QTabWidget *tabs;
    QPushButton *loginTabBtn;
    QPushButton *registerTabBtn;
    QLineEdit *logUsernameEdit;
    QLineEdit *logPasswordEdit;
    QLineEdit *regUsernameEdit;
    QLineEdit *regPasswordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
};

#endif // LOGINWINDOW_H
