#include "musicmain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MusicMain w;
    w.setMinimumSize(1190, 500); // можешь оставить или убрать
    w.showMaximized(); // открывается сразу на весь экран
    return a.exec();
}
