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
#include "setstyle.h"


AlbumTrackButton::AlbumTrackButton(const track &trackData, QString number, QWidget *parent)
    : QPushButton(parent), trackData(trackData), number(number)
{
    QHBoxLayout *trackLayout = new QHBoxLayout(this);
    trackLayout->setAlignment(Qt::AlignLeft);
    trackLayout->setContentsMargins(0, 0, 0, 0);  // ← Вот это убирает смещение
    QLabel *numberOfTrackLabel = new QLabel(this);
    numberOfTrackLabel->setMargin(0);
    numberOfTrackLabel->setText(number);
    numberOfTrackLabel->setFixedSize(100,50);
    numberOfTrackLabel->setStyleSheet("font-weight: bold; font-size: 14px; font-family: 'Tahoma'; padding-left: 20px");
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

    int minutes = trackData.duration_ms / 1000 / 60;
    int seconds = trackData.duration_ms / 1000 % 60;
    QString duration = QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
    QLabel *durationLabel = new QLabel(duration);

    durationLabel->setFixedHeight(14);
    durationLabel->setFixedWidth(50);
    durationLabel->setStyleSheet("color: #828282; font-weight: bold; font-size: 12px; font-family: 'Tahoma';");
    trackLayout->addStretch(); // добавляет растяжку перед durationLabel
    trackLayout->addWidget(durationLabel);

    //this->setStyleSheet("border: 1px solid red");
    setLayout(trackLayout);
    setFixedHeight(50);
    setFlat(true); // убирает стандартный фон
    setStyleSheet(R"(
    QPushButton {
        background-color: transparent;
        border: none;
        border-bottom: 1px solid rgba(255, 255, 255, 70);
    }
    QPushButton:hover {
        background-color: rgba(255, 255, 255, 30);
        outline: none;
    }
)");

};

QString AlbumTrackButton::getTrackName(){
    return trackData.name;
}

track * AlbumTrackButton::getTrack(){
    return &trackData;
}

void AlbumTrackButton::mouseDoubleClickEvent(QMouseEvent *event){
    qDebug()<<getTrackName();
    emit trackButtonClicked(getTrack());
}




AlbumTrackButton::AlbumTrackButton(QWidget *parent)
    : QPushButton(parent)
{
    QHBoxLayout *trackLayout = new QHBoxLayout(this);
    trackLayout->setAlignment(Qt::AlignLeft);
    trackLayout->setContentsMargins(0, 0, 0, 0);  // ← Вот это убирает смещение
    QLabel *numberOfTrackLabel = new QLabel(this);
    numberOfTrackLabel->setMargin(0);
    numberOfTrackLabel->setText("#");
    numberOfTrackLabel->setFixedWidth(50);
    numberOfTrackLabel->setStyleSheet("font-weight: bold; font-size: 14px; font-family: 'Tahoma'; padding-left: 20px");
    trackLayout->addWidget(numberOfTrackLabel);

    QVBoxLayout *name_and_author = new QVBoxLayout();
    QLabel *nameLabel = new QLabel("Name");
    nameLabel->setFixedWidth(250);
    nameLabel->setStyleSheet("font-weight: bold; font-size: 14px; font-family: 'Tahoma';");
    nameLabel->setAlignment(Qt::AlignCenter);

    name_and_author->addWidget(nameLabel);
    trackLayout->addLayout(name_and_author);

    int minutes = 60;
    int seconds = 60;
    QString duration = QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
    QLabel *durationLabel = new QLabel(duration);

    durationLabel->setFixedHeight(14);
    durationLabel->setFixedWidth(50);
    durationLabel->setStyleSheet("color: #828282; font-weight: bold; font-size: 12px; font-family: 'Tahoma';");
    trackLayout->addStretch(); // добавляет растяжку перед durationLabel
    trackLayout->addWidget(durationLabel);

    //this->setStyleSheet("border: 1px solid red");
    setLayout(trackLayout);
    setFixedHeight(30);
    setStyleSheet(R"(
    QPushButton {
        background-color: transparent;
        border: none;
        border-bottom: 1px solid rgba(255, 255, 255, 70);
    }
    QPushButton:hover { background: none;}
)");
};

AlbumWidget::AlbumWidget(const struct album &albumData, QWidget *parent)
    : QWidget(parent), albumData(albumData)
{
    // 1. Основной layout для AlbumWidget
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    // 2. Scroll area — контейнер, который умеет прокручивать
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    // 3. scrollWidget — внутренняя "коробка" в scrollArea
    QWidget *scrollWidget = new QWidget();
    // 4. Layout для scrollWidget
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);

    QWidget *albumInfoWidget = new QWidget();
    scrollLayout->addWidget(albumInfoWidget);
    scrollLayout->setContentsMargins(0,20,0,0); //left, top, right, bottom
    QHBoxLayout *albumInfoLayout = new QHBoxLayout(albumInfoWidget);
    albumInfoLayout->setContentsMargins(0,0,0,0);

    QLabel *coverLabel = new QLabel(this);
    setRoundedImage(coverLabel, albumData.coverpath, 200, 15);
    albumInfoLayout->addWidget(coverLabel);

    QWidget *albumNameWidget = new QWidget();
    //this->setStyleSheet("border: 1px solid red");
    albumInfoLayout->addWidget(albumNameWidget);
    albumNameWidget->setFixedHeight(200);
    albumInfoWidget->setFixedHeight(200);
    QVBoxLayout *albumNameLayout = new QVBoxLayout(albumNameWidget);
    albumInfoLayout->setContentsMargins(20,0,0,0); //left, top, right, bottom
    albumNameLayout->setContentsMargins(20,0,0,0); //left, top, right, bottom
    //albumInfoLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *typeOfObject = new QLabel();
    typeOfObject->setText("ALBUM");
    typeOfObject->setFixedHeight(35);
    typeOfObject->setStyleSheet("color: #615D5D; font-size: 20px; font-family: 'K2D'; font-weight: bold; padding-bottom: 5px");

    QLabel *albumNameLabel = new QLabel();
    albumNameLabel->setText(albumData.name);
    albumNameLabel->setFixedHeight(50);
    albumNameLabel->setStyleSheet("font-size: 40px; font-family: 'Tahoma'; font-weight: bold;");


    QPushButton *albumAuthorButton= new QPushButton(albumData.author);
    QFontMetrics fm(albumData.author);
    int textWidth = fm.horizontalAdvance(albumData.author);
    albumAuthorButton->setFixedWidth(textWidth + 35); // +10 — небольшой отступ по краям
    //albumAuthorButton->setFixedSize(200, 20);
    set_button_style(albumAuthorButton, 20, "white");

    QLabel *albumInfoLabel = new QLabel();
    albumInfoLabel->setText("December 14, 2016 - 8 songs, 24 minutes");
    albumInfoLabel->setFixedHeight(20);
    albumInfoLabel->setStyleSheet("font-size: 10px; font-family: 'Tahoma'; font-weight: bold;");

    albumNameLayout->addWidget(typeOfObject);
    albumNameLayout->addWidget(albumNameLabel);
    albumNameLayout->addWidget(albumAuthorButton);
    albumNameLayout->addWidget(albumInfoLabel);

    //tracklist widget
    QWidget *tracklistWidget = new QWidget();
    QVBoxLayout *trackListLayout = new QVBoxLayout(tracklistWidget);
    trackListLayout->setAlignment(Qt::AlignTop);
    trackListLayout->setContentsMargins(0, 0, 0, 0); // убирает внешние отступы (слева, сверху, справа, снизу)
    trackListLayout->setSpacing(0);                 // убирает промежутки между виджетами

    scrollLayout->addWidget(tracklistWidget);

    QPushButton *playAlbumButton = new QPushButton();
    playAlbumButton->setCursor(Qt::PointingHandCursor);
    playAlbumButton->setFixedSize(50, 50);
    QLabel *playAlbumButtonLabel = new QLabel(playAlbumButton);
    QPixmap playAlbumButtonPath("../resources/icons/playbutton.png");
    playAlbumButtonLabel->setPixmap(playAlbumButtonPath.scaled(50, 50, Qt::KeepAspectRatio));
    albumNameLayout->addWidget(playAlbumButton);



    AlbumTrackButton *trackInfo = new AlbumTrackButton(this);
    trackInfo->setEnabled(false);                // отключает клики
    trackInfo->setFlat(true);                    // убирает фон и границы
    trackInfo->setFocusPolicy(Qt::NoFocus);      // убирает фокусировку (без рамки при Tab/клике)
    trackInfo->setCursor(Qt::ArrowCursor);       // убирает "указатель-руку"

    trackListLayout->addWidget(trackInfo);
    // 5. Пример: добавим несколько кнопок
    for (int i = 0; i < albumData.tracks.size() ; ++i) {
        track sometrack = albumData.tracks[i];
        QString trackNumber = QString::number(i + 1); // превращаем число в строку
        AlbumTrackButton *button = new AlbumTrackButton(sometrack, trackNumber, this);
        connect(button, &AlbumTrackButton::trackButtonClicked, this, &AlbumWidget::onTrackdoubleClicked);

        trackListLayout->addWidget(button);
    }

    //this->setStyleSheet("border: 1px solid red");


    // 6. Вложим scrollWidget в scrollArea
    scrollArea->setWidget(scrollWidget);
    // 7. Добавим scrollArea в основной layout
    mainLayout->addWidget(scrollArea);
}



void AlbumWidget::onTrackdoubleClicked(track *trackData){
    emit trackButtonClicked(trackData);
}




