#include <QScreen> //for Qscreen *screen = QApplication::primaryScreen();
#include <QApplication>
#include <QMainWindow>
#include <QString> // Для использования QString
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
// #include <QWebSocket>
#include <QMessageBox>
#include <QDir>
#include <fstream>
#include <vector>
#include <qDebug>


#include "albumwidget.h"

AlbumWidget::AlbumWidget(const struct track &trackWidget, QWidget *parent) : QWidget(parent), trackWidget(trackWidget)
{
    QVBoxLayout *mainWidgetLayout = new QVBoxLayout(this);
    QPushButton *someButton = new QPushButton("tap");
    mainWidgetLayout->addWidget(someButton);
}
