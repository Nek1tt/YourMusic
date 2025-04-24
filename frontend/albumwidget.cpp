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


AlbumTrackButton::AlbumTrackButton(const track &trackData, QString number, QWidget *parent)
    : QPushButton(parent), trackData(trackData), number(number)
{
    QHBoxLayout *trackLayout = new QHBoxLayout(this);
    trackLayout->setAlignment(Qt::AlignLeft);
    trackLayout->setContentsMargins(0, 0, 0, 0);  // ← Вот это убирает смещение
    QLabel *numberOfTrackLabel = new QLabel(this);
    numberOfTrackLabel->setMargin(0);
    numberOfTrackLabel->setText(number);
    numberOfTrackLabel->setFixedSize(50, 50);
    numberOfTrackLabel->setAlignment(Qt::AlignCenter);
    numberOfTrackLabel->setStyleSheet("font-weight: bold; font-size: 14px; font-family: 'Tahoma';");
    trackLayout->addWidget(numberOfTrackLabel);

    QVBoxLayout *name_and_author = new QVBoxLayout();
    QLabel *nameLabel = new QLabel(trackData.name);
    nameLabel->setFixedHeight(16);
    nameLabel->setStyleSheet("font-weight: bold; font-size: 14px; font-family: 'Tahoma';");
    nameLabel->setAlignment(Qt::AlignLeft);

    QLabel *authorLabel = new QLabel(trackData.author);
    authorLabel->setFixedHeight(14);
    authorLabel->setStyleSheet("color: #828282; font-weight: bold; font-size: 12px; font-family: 'Tahoma';");
    authorLabel->setAlignment(Qt::AlignLeft);

    name_and_author->addWidget(nameLabel);
    name_and_author->addWidget(authorLabel);
    trackLayout->addLayout(name_and_author);

    QString duration = QString::number(trackData.min) + ":" + QString::number(trackData.sec);
    QLabel *durationLabel = new QLabel(duration);

    durationLabel->setFixedHeight(14);
    durationLabel->setFixedWidth(200);
    durationLabel->setStyleSheet("color: #828282; font-weight: bold; font-size: 12px; font-family: 'Tahoma';");
    durationLabel->setAlignment(Qt::AlignRight);
    trackLayout->addWidget(durationLabel);

    //this->setStyleSheet("border: 1px solid red");
    setLayout(trackLayout);
    setFixedHeight(50);
};

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
    //this->setStyleSheet("border: 1px solid red");
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

    //tracklist widget
    QWidget *tracklistWidget = new QWidget();
    QVBoxLayout *trackListLayout = new QVBoxLayout(tracklistWidget);
    scrollLayout->addWidget(tracklistWidget);

    QPushButton *playAlbumButton = new QPushButton();
    playAlbumButton->setCursor(Qt::PointingHandCursor);
    playAlbumButton->setFixedSize(50, 50);
    QLabel *playAlbumButtonLabel = new QLabel(playAlbumButton);
    QPixmap playAlbumButtonPath("../resources/icons/playbutton.png");
    playAlbumButtonLabel->setPixmap(playAlbumButtonPath.scaled(50, 50, Qt::KeepAspectRatio));
    albumNameLayout->addWidget(playAlbumButton);




    // 5. Пример: добавим несколько кнопок
    for (int i = 0; i < 20; ++i) {
        track sometrack {"dedejo", "N-Jo", "../resources/imgs/covers/cover1", 3, 12};
        QString trackNumber = QString::number(i + 1); // превращаем число в строку
        AlbumTrackButton *button = new AlbumTrackButton(sometrack, trackNumber);
        scrollLayout->addWidget(button);
    }




    // 6. Вложим scrollWidget в scrollArea
    scrollArea->setWidget(scrollWidget);
    // 7. Добавим scrollArea в основной layout
    mainLayout->addWidget(scrollArea);
}
