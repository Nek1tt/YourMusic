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
#include <QDebug>


#include "albumwidget.h"

AlbumWidget::AlbumWidget(const struct album &albumData, QWidget *parent)
    : QWidget(parent), albumData(albumData)
{
    // 1. Основной layout для AlbumWidget
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    // 2. Scroll area — контейнер, который умеет прокручивать
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);  // 🔥 чтобы scrollWidget подстраивался

    // 3. scrollWidget — внутренняя "коробка" в scrollArea
    QWidget *scrollWidget = new QWidget();
    // 4. Layout для scrollWidget
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);

    QWidget *albumInfoWidget = new QWidget();
    scrollLayout->addWidget(albumInfoWidget);
    QHBoxLayout *albumInfoLayout = new QHBoxLayout(albumInfoWidget);

    QLabel *coverLabel = new QLabel(this);
    coverLabel->setFixedSize(200, 200);
    albumInfoLayout->addWidget(coverLabel);

    QPixmap coverPixmap(albumData.coverpath);
    coverLabel->setPixmap(coverPixmap.scaled(200, 200, Qt::KeepAspectRatio));

    QWidget *albumNameWidget = new QWidget();
    this->setStyleSheet("border: 1px solid red");
    albumInfoLayout->addWidget(albumNameWidget);
    albumNameWidget->setFixedHeight(200);
    albumInfoWidget->setFixedHeight(200);
    QVBoxLayout *albumNameLayout = new QVBoxLayout(albumNameWidget);
    //albumInfoLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *typeOfObject = new QLabel();
    typeOfObject->setText("Album");
    typeOfObject->setFixedHeight(20);
    typeOfObject->setStyleSheet("color: #615D5D; font-size: 15px; font-family: 'Tahoma'; font-weight: bold;");


    QLabel *albumNameLabel = new QLabel();
    albumNameLabel->setText(albumData.name);
    albumNameLabel->setFixedHeight(50);
    albumNameLabel->setStyleSheet("font-size: 40px; font-family: 'Tahoma'; font-weight: bold;");


    QLabel *albumAuthorLabel = new QLabel();
    albumAuthorLabel->setText(albumData.author);

    albumNameLayout->addWidget(typeOfObject);
    albumNameLayout->addWidget(albumNameLabel);
    albumNameLayout->addWidget(albumAuthorLabel);

    // 5. Пример: добавим несколько кнопок
    for (int i = 0; i < 20; ++i) {
        QPushButton *button = new QPushButton(QString("Кнопка %1").arg(i));
        scrollLayout->addWidget(button);
    }



    // 6. Вложим scrollWidget в scrollArea
    scrollArea->setWidget(scrollWidget);
    // 7. Добавим scrollArea в основной layout
    mainLayout->addWidget(scrollArea);
}
