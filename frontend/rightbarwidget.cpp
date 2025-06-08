#include "rightbarwidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QTabWidget>
#include <QTextStream>
#include <QPainter>
#include <QPainterPath>
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
    // Настройка кнопок
    auto *shuffleButton = makeIconButton("resources/icons/playbutton.png");
    auto *prevButton    = makeIconButton("resources/icons/prev.png");
    pauseButton   = makeIconButton("resources/icons/pause.png");
    auto *nextButton    = makeIconButton("resources/icons/next.png");
    auto *repeatButton  = makeIconButton("resources/icons/playbutton.png");

    auto *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(shuffleButton);
    buttonsLayout->addWidget(prevButton);
    buttonsLayout->addWidget(pauseButton);
    buttonsLayout->addWidget(nextButton);
    buttonsLayout->addWidget(repeatButton);
    buttonsLayout->addStretch();

    connect(shuffleButton, &QPushButton::clicked, this, &RightBarWidget::on_PlayAndLoad_clicked);

    connect(repeatButton, &QPushButton::clicked, this, &RightBarWidget::on_Play_clicked);

    connect(pauseButton, &QPushButton::clicked, this, &RightBarWidget::on_Pause_clicked);

    slider = new QSlider(Qt::Horizontal);
    connect(slider, &QSlider::valueChanged, this, &RightBarWidget::onSliderValueChanged);

    currentTimeLabel = new QLabel("0:00");
    totalTimeLabel = new QLabel("4:35");
    // totalTimeLabel->setStyleSheet("background: transparent;");
    // currentTimeLabel->setStyleSheet("background: transparent;");

    slider->setRange(0, 275);  // например, totalSeconds = 120
    slider->setValue(0);
    connect(slider, &QSlider::sliderReleased, this, [this]() {
        int value = slider->value();
        on_seeking_clicked(value, slider);
    });

    auto *sliderLayout = new QHBoxLayout();
    sliderLayout->addWidget(currentTimeLabel);
    sliderLayout->addWidget(slider);
    sliderLayout->addWidget(totalTimeLabel);

    timer = new QTimer(this);
    //connect(timer, &QTimer::timeout, this, &RightBarWidget::onTimerTick);

    connect(timer, &QTimer::timeout, this, [this]() {
        int currentValue = slider->value();
        if (currentValue < slider->maximum()) {
            slider->setValue(currentValue + 1);
        } else {
            // дошли до конца
            // timer->stop();
        }
    });
    //timer->start(1000);  // срабатывание раз в секунду
    //timer->stop();

    // Основной layout
    playLayout->addLayout(sliderLayout);
    playLayout->addLayout(buttonsLayout);

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
    avaLabel = new QLabel();

    infoLayout->addWidget(avaLabel,0, Qt::AlignHCenter);


    authorNameButton = new QPushButton();
    set_button_style(authorNameButton, 18, "white", "left", "10px");
    infoLayout->addWidget(authorNameButton, 0, Qt::AlignHCenter);
    followButton = new QPushButton();

    connect(followButton, &QPushButton::clicked, this, &RightBarWidget::on_followButton_clicked);
    followButton->setFixedWidth(100);
    set_border_button_style(followButton, 16, "white");
    infoLayout->addWidget(followButton, 0, Qt::AlignHCenter);
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

QPushButton* RightBarWidget::makeIconButton(const QString& iconPath)
{
    // Загружаем картинку
    QPixmap originalPixmap(iconPath);

    // Делаем crop по центру
    int squareSize = 50;
    QSize srcSize = originalPixmap.size();
    int centerX = srcSize.width() / 2;
    int centerY = srcSize.height() / 2;
    int halfSize = squareSize / 2;

    QRect cropRect(centerX - halfSize, centerY - halfSize, squareSize, squareSize);
    QPixmap croppedPixmap = originalPixmap.copy(cropRect);

    // Создаём кнопку
    QPushButton *btn = new QPushButton();
    btn->setIcon(QIcon(croppedPixmap));
    btn->setIconSize(QSize(squareSize, squareSize));
    btn->setFlat(true);
    btn->setText("");  // без текста
    btn->setFixedSize(squareSize + 20, squareSize + 20);  // например, чуть больше чтобы красиво смотрелось
    btn->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
        }
    )");

    return btn;
}



void RightBarWidget::on_seeking_clicked(int value,QSlider *slider){
    onSliderValueChanged(value);
    float normalized = (float)value / slider->maximum();

    QJsonObject payload;
    payload["command"] = "seeking";
    payload["position"] = normalized;
    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    webSocket->sendMessage(message);
}

void RightBarWidget::onSliderValueChanged(int value)
{
    currentSeconds = value;
    currentTimeLabel->setText(QString::number(value / 60) + ":" + QString("%1").arg(value % 60, 2, 10, QChar('0')));
}

void RightBarWidget::onTimerTick()
{
    qDebug()<<"Tick";
    if (currentSeconds < totalSeconds)
    {
        ++currentSeconds;
        slider->setValue(currentSeconds);
    }
    else
    {
        timer->stop();
    }
}

void RightBarWidget::on_Play_clicked(){

}

void RightBarWidget::on_Pause_clicked(){
    if (!timer->isActive())
    {
        QJsonObject payload;
        payload["command"] = "resume";

        QJsonDocument doc(payload);
        QString message = QString::fromUtf8(doc.toJson());
        webSocket->sendMessage(message);
        timer->start(1000);

        pauseButton->setIcon(QIcon("resources/icons/pause.png"));
    }
    else
    {
        QJsonObject payload;
        payload["command"] = "pause";
        QJsonDocument doc(payload);
        QString message = QString::fromUtf8(doc.toJson());
        webSocket->sendMessage(message);
        timer->stop();
        pauseButton->setIcon(QIcon("resources/icons/play.png"));
    }
}



void RightBarWidget::on_PlayAndLoad_clicked(){
    if (!timer->isActive())
    {
        QJsonObject payload;
        payload["command"] = "load";
        payload["path"] = "resources/music/test.mp3";

        QJsonDocument doc(payload);
        QString message = QString::fromUtf8(doc.toJson());
        webSocket->sendMessage(message);
        timer->start(1000);
    }
}

void RightBarWidget::setNewCurrentTrack(const track &trackData) {
    currentTrack = new track(trackData);

    currentAlbumNameButton->setText(currentTrack->name);

    setRoundedImage(currentAlbumCoverLabel, currentTrack->coverpath, coverSize, 15);

    currentTrackNameButton->setText(currentTrack->name);

    currentTrackAuthorButton->setText(currentTrack->authors[0]);

    setRoundedImage(avaLabel, "resources/imgs/ava.png", 200, 15);

    authorNameButton->setText(currentTrack->authors[0]);

    if (isFollowed){
        followButton->setText("Unfollow");
    }else{
        followButton->setText("Follow");
    }

    QFile file("resources/text/song_texts/noize.txt");
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
