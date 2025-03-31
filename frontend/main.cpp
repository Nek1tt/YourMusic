#include "musicmain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MusicMain w;
    w.setMinimumSize(1000, 1000);
    w.show();
    return a.exec();
}
