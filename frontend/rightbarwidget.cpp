#include "rightbarwidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QTabWidget>
#include <QTextStream>
#include <QPainter>
#include <QPainterPath>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "setstyle.h"


RightBarWidget::RightBarWidget(WebSocketClient *webSocket, int initialScreenWidth, int initialScreenHeight, QWidget *parent, track *currentTrack)
    : QWidget(parent), currentTrack(currentTrack), initialScreenWidth(initialScreenWidth), initialScreenHeight(initialScreenHeight), webSocket(webSocket)
{

    //sizes initalization
    barSize = 320;
    coverSize = barSize/1.2;


    // Создаем прокручиваемую область
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true); // растягивает содержимое по размеру
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Внутренний виджет, который будет прокручиваться
    scrollWidget = new QWidget();
    scrollWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    scrollWidget->setFixedWidth(barSize);

    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);

    // кнопка с названием альбома трека
    currentAlbumNameButton = new QPushButton();
    currentAlbumNameButton->setCursor(Qt::PointingHandCursor);
    set_button_style(currentAlbumNameButton, 25, "white", "center", "10px");
    // currentAlbumNameButton->setStyleSheet(
    //     "QPushButton {"
    //     "text-align: center;"
    //     "padding-bottom: 10px;"
    //     "font-weight: bold;"
    //     "font-size: 25px;"
    //     "font-family: 'Tahoma';"
    //     "    background: none;"                  // Убираем фон
    //     "    border: none;"                      // Убираем рамку
    //     //"    border: 2px solid blue;"  // рамка 2px
    //     "    text-decoration: none;"             // Убираем подчеркивание по умолчанию
    //     "}"
    //     "QPushButton:hover {"
    //     "    text-decoration: underline;"        // Подчеркиваем текст при наведении
    //     "}"
    //     );

    scrollLayout->addWidget(currentAlbumNameButton);

    currentAlbumCoverButton = new QPushButton();

    currentAlbumCoverButton->setCursor(Qt::PointingHandCursor);
    currentAlbumCoverButton->setFixedSize(coverSize, coverSize);

    currentAlbumCoverLabel = new QLabel(currentAlbumCoverButton);

    // Загружаем и масштабируем изображение
    //QPixmap albumCoverPath(currentTrack->coverpath);
    setRoundedImage(currentAlbumCoverLabel, currentTrack->coverpath, coverSize, 15);
    scrollLayout->addWidget(currentAlbumCoverButton, 0, Qt::AlignHCenter);



    currentTrackNameButton = new QPushButton();
    currentTrackNameButton->setCursor(Qt::PointingHandCursor);
    set_button_style(currentTrackNameButton, 20, "white", "center", "5px");
    // currentTrackNameButton->setStyleSheet(
    //     "QPushButton {"
    //     "text-align: center;"
    //     "padding-top: 5px;"
    //     "font-weight: bold;"
    //     "font-size: 20px;"
    //     "font-family: 'Tahoma';"
    //     "    background: none;"                  // Убираем фон
    //     "    border: none;"                      // Убираем рамку
    //     //"    border: 2px solid blue;"  // рамка 2px
    //     "    text-decoration: none;"             // Убираем подчеркивание по умолчанию
    //     "}"
    //     "QPushButton:hover {"
    //     "    text-decoration: underline;"        // Подчеркиваем текст при наведении
    //     "}"
    //     );

    scrollLayout->addWidget(currentTrackNameButton);



    currentTrackAuthorButton = new QPushButton();
    currentTrackAuthorButton->setCursor(Qt::PointingHandCursor);
    set_button_style(currentTrackAuthorButton, 15, "white", "center", "10px");

    // currentTrackAuthorButton->setStyleSheet(
    //     "QPushButton {"
    //     "text-align: center;"
    //     "padding-bottom: 10px;"
    //     "font-weight: bold; color: #828282;"
    //     "font-size: 15px;"
    //     "font-family: 'Tahoma';"
    //     "    background: none;"                  // Убираем фон
    //     "    border: none;"                      // Убираем рамку
    //     //"    border: 2px solid blue;"  // рамка 2px
    //     "    text-decoration: none;"             // Убираем подчеркивание по умолчанию
    //     "}"
    //     "QPushButton:hover {"
    //     "    text-decoration: underline;"        // Подчеркиваем текст при наведении
    //     "}"
    //     );

    scrollLayout->addWidget(currentTrackAuthorButton);


    QWidget *playWidget = new QWidget();
    QVBoxLayout *playLayout = new QVBoxLayout(playWidget);
    QPushButton *playButton = new QPushButton("Play");

    QPushButton *pauseButton = new QPushButton("Pause");
    connect(playButton, &QPushButton::clicked, this, &RightBarWidget::on_Play_clicked);

    connect(pauseButton, &QPushButton::clicked, this, &RightBarWidget::on_Pause_clicked);

    playLayout->addWidget(playButton);
    playLayout->addWidget(pauseButton);


    scrollLayout->addWidget(playWidget);

    QWidget *choice_author_text_widget = new QWidget();
    QHBoxLayout *choice_author_text_layout = new QHBoxLayout(choice_author_text_widget);
    toAuthor = new QPushButton("To author");
    toLyrics = new QPushButton("To text");
    choice_author_text_layout->addWidget(toAuthor);
    choice_author_text_layout->addWidget(toLyrics);
    connect(toAuthor, &QPushButton::clicked, this, &RightBarWidget::on_toAuthor_clicked);
    connect(toLyrics, &QPushButton::clicked, this, &RightBarWidget::on_toLyrics_clicked);

    scrollLayout->addWidget(choice_author_text_widget);

    author_lyrics_tab = new QTabWidget(scrollWidget);
    //tabwidget->setFixedWidth(1);
    author_lyrics_tab->setStyleSheet("QTabWidget::pane { margin: 0px; padding: 0px; }");
    lyricsWidget = new QWidget();
    authorWidget = new QWidget();
    //authorWidget->setStyleSheet("border: 1px solid red;");

    author_lyrics_tab->tabBar()->hide();
    author_lyrics_tab->addTab(authorWidget, "authorWidget");
    scrollLayout->addWidget(author_lyrics_tab);


    QVBoxLayout *authorInfoLayout = new QVBoxLayout();
    QWidget *infoWidget = new QWidget();
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setAlignment(Qt::AlignTop & Qt::AlignCenter);  // Устанавливаем выравнивание по верхнему краю
    infoLayout->setContentsMargins(0, 0, 0, 0);
    authorInfoLayout->addWidget(infoWidget);

    // QPixmap authorAva("../resources/imgs/ava.png");
    avaLabel = new QLabel();
    // avaLabel->setFixedHeight(200);
    // avaLabel->setAlignment(Qt::AlignHCenter);  // Сохранение верхней части картинки
    // //avaLabel->setStyleSheet("padding-bottom: 0px");
    infoLayout->addWidget(avaLabel,0, Qt::AlignHCenter);


    authorNameButton = new QPushButton();
    set_button_style(authorNameButton, 18, "white", "left", "10px");
    // authorNameButton->setStyleSheet(
    //     "QPushButton {"
    //     "text-align: left; padding-left: 10px;
    //     "padding-bottom: 10px;"
    //     "font-weight: bold;"
    //     "font-size: 18px;"
    //     "font-family: 'Tahoma';"
    //     "    background: none;"                  // Убираем фон
    //     "    border: none;"                      // Убираем рамку
    //     //"    border: 2px solid blue;"  // рамка 2px
    //     "    text-decoration: none;"             // Убираем подчеркивание по умолчанию
    //     "}"
    //     "QPushButton:hover {"
    //     "    text-decoration: underline;"        // Подчеркиваем текст при наведении
    //     "}"
    //     );
    infoLayout->addWidget(authorNameButton, 0, Qt::AlignHCenter);
    followButton = new QPushButton();

    connect(followButton, &QPushButton::clicked, this, &RightBarWidget::on_followButton_clicked);
    followButton->setFixedWidth(100);
    set_border_button_style(followButton, 16, "white");
    // followButton->setStyleSheet(
    //     "QPushButton {"
    //     "    text-align: center;"  // Центрируем текст
    //     "    font-weight: bold;"
    //     "    font-size: 16px;"
    //     "    font-family: 'Tahoma';"
    //     "    background-color: #333333;"  // Темно-серый фон
    //     "    border: 1px solid white;"     // Белая рамка 1px
    //     "    border-radius: 4px;"          // Скругленные углы радиусом 4px
    //     "    text-decoration: none;"       // Убираем подчеркивание по умолчанию
    //     "}"
    //     "QPushButton:hover {"
    //     "    border: 2px solid white;"      // Увеличенная рамка до 2px при наведении
    //     "}"
    //     );
    infoLayout->addWidget(followButton, 0, Qt::AlignHCenter);


    // QLabel *authorInfoLabel = new QLabel(currentTrack->artist);
    // authorInfoLayout->addWidget(authorInfoLabel);
    authorWidget->setLayout(authorInfoLayout); // ← добавлено


    lyricsLabel = new QLabel();
    lyricsLabel->setWordWrap(true);
    lyricsLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    lyricsLabel->setStyleSheet(
        "font-size: 16px;"
        "font-family: 'Tahoma';"
        "color: white;"
        );
    lyricsLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    lyricsLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // Контейнер с layout
    QWidget *lyricsContainer = new QWidget();
    QVBoxLayout *lyricsLayout = new QVBoxLayout(lyricsContainer);
    lyricsLayout->setContentsMargins(0, 0, 0, 0);
    lyricsLayout->addWidget(lyricsLabel);

    // Область прокрутки
    QScrollArea *lirycsScrollArea = new QScrollArea();
    lirycsScrollArea->setWidgetResizable(true);
    lirycsScrollArea->setWidget(lyricsContainer);
    lirycsScrollArea->setStyleSheet("background: transparent;");
    lirycsScrollArea->setFixedHeight(500);
    // Добавляем во вкладку
    author_lyrics_tab->addTab(lirycsScrollArea, "textWidget");
    toggle_buttons();



    // Установим внутренний виджет в область прокрутки
    scrollArea->setWidget(scrollWidget);

    scrollLayout->addStretch();
    // Layout для RightBarWidget
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(scrollArea);
    mainLayout->setContentsMargins(0, 0, 0, 0); // убираем отступы
    this->setMaximumWidth(barSize);
    this->setMinimumWidth(235);
    //this->resize(barSize, initialScreenHeight);
    //this->setFixedWidth(barSize);

}

void RightBarWidget::on_Play_clicked(){
    QJsonObject payload;
    payload["command"] = "load";
    payload["path"] = "C:/Users/Lenovo/upprpo/YourMusic/frontend/resources/music/test.mp3";

    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    qDebug()<<message;
    webSocket->sendMessage(message);
}

void RightBarWidget::on_Pause_clicked(){
    QJsonObject payload;
    payload["command"] = "pause";
    // payload["path"] = "C:/Users/Lenovo/upprpo/YourMusic/frontend/resources/music/test.mp3";

    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    qDebug()<<message;
    webSocket->sendMessage(message);
}

void RightBarWidget::setNewCurrentTrack(const track &trackData) {
    currentTrack = new track(trackData);

    currentAlbumNameButton->setText(currentTrack->name);

    setRoundedImage(currentAlbumCoverLabel, currentTrack->coverpath, coverSize, 15);

    currentTrackNameButton->setText(currentTrack->name);

    currentTrackAuthorButton->setText(currentTrack->author);

    setRoundedImage(avaLabel, "../resources/imgs/ava.png", 200, 15);

    authorNameButton->setText(currentTrack->author);

    if (isFollowed){
        followButton->setText("Unfollow");
    }else{
        followButton->setText("Follow");
    }

    QFile file("../resources/text/song_texts/noize.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString lyrics = in.readAll();
        lyricsLabel->setText(lyrics);
    }
}

void RightBarWidget::toggle_buttons(){
    QString activeStyle =
        "QPushButton {"
        " font-weight: bold;"
        " color: white;"
        " font-size: 15px;"
        " font-family: 'Tahoma';"
        " background: none;"
        " border: none;"
        " border-bottom: 2px solid white;"
        " text-decoration: none;"
        " cursor: default;"
        "}";

    QString inactiveStyle =
        "QPushButton {"
        " font-weight: bold;"
        " color: #828282;"
        " font-size: 15px;"
        " font-family: 'Tahoma';"
        " background: none;"
        " border: none;"
        " text-decoration: none;"
        "}"
        "QPushButton:hover {"
        " text-decoration: underline;"
        "}";

    if (isLyrics) {
        toLyrics->setStyleSheet(activeStyle);
        toAuthor->setStyleSheet(inactiveStyle);
        toLyrics->setEnabled(false);
        toAuthor->setEnabled(true);
        author_lyrics_tab->setCurrentIndex(1);
        author_lyrics_tab->setFixedHeight(500);
    } else {
        toAuthor->setStyleSheet(activeStyle);
        toLyrics->setStyleSheet(inactiveStyle);
        toAuthor->setEnabled(false);
        toLyrics->setEnabled(true);
        author_lyrics_tab->setCurrentIndex(0);
        author_lyrics_tab->setFixedHeight(300);
    }
}


void RightBarWidget::on_toAuthor_clicked(){
    isLyrics = false;
    toggle_buttons();
}

void RightBarWidget::on_toLyrics_clicked(){
    isLyrics = true;
    toggle_buttons();
}

void RightBarWidget::on_followButton_clicked(){
    if (isFollowed){
        isFollowed = false;
        followButton->setText("Follow");
    } else {
        isFollowed = true;
        followButton->setText("Unfollow");
    }
}

void RightBarWidget::resizeBarWidget(int width){
    barSize = width;


    scrollWidget->setFixedWidth(barSize);

    coverSize = barSize/1.2;
    currentAlbumCoverButton->setFixedSize(coverSize, coverSize);
    setRoundedImage(currentAlbumCoverLabel, currentTrack->coverpath, coverSize, 15);
    //scrollLayout->addWidget(currentAlbumCoverButton, 0, Qt::AlignHCenter);
    //this->resize(barSize, initialScreenHeight);
}

int RightBarWidget::get_barWidth(){
    return barSize;
}
