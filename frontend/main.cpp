#include "musicmain.h"
#include "loginwindow.h"
#include <QApplication>
#include "websocketclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WebSocketClient *wsclient = new WebSocketClient("ws://84.237.53.143:8080");
    WebSocketClient *stasWeb = new WebSocketClient("ws://127.0.0.1:8080");

    LoginWindow login(wsclient);
    if (login.exec() == QDialog::Accepted) {
        QString usertag = login.getUsertag();
        MusicMain w(usertag, wsclient, stasWeb);
        w.on_homeTab_clicked();
        w.show();
        w.setMinimumSize(1190, 500); // можешь оставить или убрать
        w.showMaximized(); // открывается сразу на весь экран
        return a.exec();
    }
    return 0;
}



