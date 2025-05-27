#include "musicmain.h"
#include "loginwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginWindow login;
    if (login.exec() == QDialog::Accepted) {
        MusicMain w;
        w.show();
        w.setMinimumSize(1190, 500); // можешь оставить или убрать
        w.showMaximized(); // открывается сразу на весь экран
        return a.exec();
    }
    return 0;
}



