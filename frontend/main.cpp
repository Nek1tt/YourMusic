#include "musicmain.h"
#include "loginwindow.h"
#include <QApplication>
#include "websocketclient.h"
#include <QProcess>  // добавляем для работы с процессом

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file("styles/darkstyle.qss");
    if (file.open(QFile::ReadOnly))
    {
        QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
    }
    // a.setStyle("fusion");

    // // Создаём тёмную палитру

    // QPalette darkPalette;

    // QColor backgroundColor("#111111");  // основной фон
    // QColor frameColor("#222222");       // цвета форм, панелей
    // QColor textColor(Qt::white);        // текст
    // QColor buttonColor("#c00c00");      // кнопки
    // QColor buttonHoverColor("#e00e00"); // hover кнопок
    // QColor borderColor("#333333");      // рамки

    // darkPalette.setColor(QPalette::Window, backgroundColor);
    // darkPalette.setColor(QPalette::WindowText, textColor);
    // darkPalette.setColor(QPalette::Base, QColor("#111111"));
    // darkPalette.setColor(QPalette::AlternateBase, frameColor);
    // darkPalette.setColor(QPalette::ToolTipBase, textColor);
    // darkPalette.setColor(QPalette::ToolTipText, textColor);
    // darkPalette.setColor(QPalette::Text, textColor);
    // darkPalette.setColor(QPalette::Button, buttonColor);
    // darkPalette.setColor(QPalette::ButtonText, textColor);
    // darkPalette.setColor(QPalette::BrightText, Qt::red);
    // darkPalette.setColor(QPalette::Highlight, buttonHoverColor);
    // darkPalette.setColor(QPalette::HighlightedText, Qt::white);

    // a.setPalette(darkPalette);
    // В основном перед запуском приложения или сразу после установки палитры:
    // a.setStyleSheet(R"(
    // QScrollBar:vertical, QScrollBar:horizontal {
    //     background: transparent;
    //     width: 12px; /* Можно подкорректировать */
    //     margin: 0px;
    // }
    // QScrollBar::handle:vertical, QScrollBar::handle:horizontal {
    //     background: rgba(255, 255, 255, 80); /* белый с прозрачностью */
    //     border-radius: 6px;
    //     min-height: 20px;
    //     min-width: 20px;
    // }
    // QScrollBar::handle:vertical:hover, QScrollBar::handle:horizontal:hover {
    //     background: rgba(255, 255, 255, 140); /* ярче при наведении */
    // }
    // QScrollBar::add-line, QScrollBar::sub-line,
    // QScrollBar::add-page, QScrollBar::sub-page {
    //     background: none;
    //     border: none;
    // }
// )");


    // создаём QProcess
    QProcess *externalProcess = new QProcess();

    // запускаем exe
    //externalProcess->start("recources/backend/player.exe");
    externalProcess->start("resources/backend/player.exe");


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
