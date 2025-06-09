#include "musicmain.h"
#include "loginwindow.h"
#include "websocketclient.h"

#include <QApplication>
#include <QProcess>      // для запуска внешнего процесса (плеера)
#include <QFile>
#include <QDebug>        // для qWarning

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Устанавливаем тёмную тему из файла стилей
    QFile file("styles/darkstyle.qss");
    if (file.open(QFile::ReadOnly))
    {
        QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
    }

    // Создаем процесс для запуска внешнего плеера
    QProcess *externalProcess = new QProcess();

    // Путь к исполняемому файлу плеера
    QString playerPath = "resources/backend/player.exe";

    // Запускаем плеер
    externalProcess->start(playerPath);

    // Проверяем, успешно ли стартовал процесс
    if (!externalProcess->waitForStarted()) {
        qWarning() << "Failed to start external process:" << playerPath;
    }

    // Создаем WebSocket клиент для сервера
    WebSocketClient *wsclient = new WebSocketClient("ws://84.237.53.143:8080");

    // Второй WebSocket клиент (видимо для локального взаимодействия)
    WebSocketClient *stasWeb = new WebSocketClient("ws://127.0.0.1:8080");

    // Создаем окно логина и показываем его модально
    LoginWindow login(wsclient);

    int result = 0; // сюда сохраним код возврата приложения

    if (login.exec() == QDialog::Accepted) {
        // После успешного логина получаем usertag
        QString *usertag = login.getUsertag();

        // Запускаем главное окно плеера
        MusicMain w(usertag, wsclient, stasWeb);

        // Программно эмулируем клик по вкладке Home (запускаем обработчик on_homeTab_clicked)
        w.on_homeTab_clicked();

        // Показываем главное окно
        w.show();
        w.setMinimumSize(1190, 500);
        w.showMaximized();

        // Запускаем главный event loop приложения
        result = a.exec();
    }

    // После завершения главного event loop — завершаем плеер (если он ещё работает)
    if (externalProcess->state() == QProcess::Running) {
        externalProcess->kill();
        externalProcess->waitForFinished();
    }

    // Освобождаем память
    delete externalProcess;

    return result;
}
