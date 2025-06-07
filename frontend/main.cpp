#include "musicmain.h"
#include "loginwindow.h"
#include <QApplication>
#include "websocketclient.h"
#include <QProcess>  // добавляем для работы с процессом

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // создаём QProcess
    QProcess *externalProcess = new QProcess();

    // запускаем exe
    externalProcess->start("../backend/player.exe");

    // Важно: проверим что процесс реально запустился
    if (!externalProcess->waitForStarted()) {
        qWarning("Failed to start external process!");
    }

    WebSocketClient *wsclient = new WebSocketClient("ws://84.237.53.143:8080");
    WebSocketClient *stasWeb = new WebSocketClient("ws://127.0.0.1:8080");

    LoginWindow login(wsclient);
    int result = 0;

    if (login.exec() == QDialog::Accepted) {
        QString *usertag = login.getUsertag();
        MusicMain w(usertag, wsclient, stasWeb);
        w.on_homeTab_clicked();
        w.show();
        w.setMinimumSize(1190, 500);
        w.showMaximized();

        result = a.exec(); // <-- пока работает главный event loop
    }

    // Когда главный event loop закончился → убиваем процесс если он всё ещё жив
    if (externalProcess->state() == QProcess::Running) {
        externalProcess->kill();
        externalProcess->waitForFinished();
    }

    delete externalProcess;

    return result;
}
