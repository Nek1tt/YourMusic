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
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPixmap>
#include <QLabel>
#include <QBuffer>
// #include <QWebSocket>
#include <QMessageBox>
#include <QDir>
#include <fstream>
#include <vector>
#include <QDebug>


#include "albumwidget.h"
#include "setstyle.h"


AlbumTrackButton::AlbumTrackButton(const track &trackData, bool isLiked, QString number, QWidget *parent)
    : QPushButton(parent), trackData(trackData), number(number), liked(isLiked)
{
    // создаём горизонтальный лэйаут для трека
    QHBoxLayout *trackLayout = new QHBoxLayout(this);
    trackLayout->setAlignment(Qt::AlignLeft);
    trackLayout->setContentsMargins(0, 0, 0, 0);  // убираем отступы вокруг лэйаута

    // label с номером трека
    QLabel *numberOfTrackLabel = new QLabel(this);
    numberOfTrackLabel->setMargin(0);
    numberOfTrackLabel->setText(number);
    numberOfTrackLabel->setFixedSize(100, 50);
    numberOfTrackLabel->setStyleSheet("font-weight: bold; font-size: 14px; font-family: 'Tahoma'; padding-left: 20px");
    trackLayout->addWidget(numberOfTrackLabel);

    // вертикальный лэйаут для названия и автора
    QVBoxLayout *name_and_author = new QVBoxLayout();

    // кнопка с названием трека
    QPushButton *nameButton = new QPushButton(trackData.name);
    connect(nameButton, &QPushButton::clicked, [this]() {
        emit trackNameButtonClicked();
    });
    QFontMetrics fm(nameButton->font());
    int textWidth = fm.horizontalAdvance(trackData.name);
    nameButton->setFixedWidth(textWidth + 25);  // немного добавляем ширины для отступов
    set_button_style(nameButton, 14, "white");

    // кнопка с автором трека
    QPushButton *authorButton = new QPushButton(trackData.authors[0]);
    connect(authorButton, &QPushButton::clicked, [this]() {
        emit trackAuthorButtonClicked();
    });
    authorButton->setFixedHeight(16);
    QFontMetrics fm_au(authorButton->font());
    int textWidth_au = fm_au.horizontalAdvance(trackData.authors[0]);
    authorButton->setFixedWidth(textWidth_au + 20);  // добавляем ширину для отступов
    set_button_style(authorButton, 12, "#828282");

    // добавляем кнопки в вертикальный лэйаут
    name_and_author->addWidget(nameButton);
    name_and_author->addWidget(authorButton);
    trackLayout->addLayout(name_and_author);

    // кнопка лайка с иконкой
    QPushButton *likeTrackButton = new QPushButton();
    likeTrackButton->setFlat(true);  // убираем стандартный фон кнопки
    likeTrackButton->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            border: none;
            padding: 0px;
        }
    )");
    likeTrackButton->setCursor(Qt::PointingHandCursor);
    likeTrackButton->setFixedSize(50, 50);

    likeTrackButtonLabel = new QLabel(likeTrackButton);

    // обработчик клика по лайку - переключает состояние
    connect(likeTrackButton, &QPushButton::clicked, [this]() {
        liked = !liked;
        QPixmap pixmap;
        if (liked) {
            pixmap.load("resources/icons/added.png");
            liked = false;
        } else {
            pixmap.load("resources/icons/adding.png");
            liked = true;
        }
        likeTrackButtonLabel->setPixmap(pixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        emit albumLiked();
    });

    // инициализация иконки лайка в зависимости от текущего состояния
    if (isLiked) {
        QPixmap likeTrackButtonPath("resources/icons/added.png");
        likeTrackButtonLabel->setPixmap(likeTrackButtonPath.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        QPixmap likeTrackButtonPath("resources/icons/adding.png");
        likeTrackButtonLabel->setPixmap(likeTrackButtonPath.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    trackLayout->addStretch();  // добавляем растяжку перед лайком и длительностью
    trackLayout->addWidget(likeTrackButton);

    // форматируем длительность трека в мм:сс
    int minutes = trackData.duration_s / 60;
    int seconds = trackData.duration_s % 60;
    QString duration = QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));

    QLabel *durationLabel = new QLabel(duration);
    durationLabel->setFixedHeight(14);
    durationLabel->setFixedWidth(50);
    durationLabel->setStyleSheet("color: #828282; font-weight: bold; font-size: 12px; font-family: 'Tahoma';");
    trackLayout->addWidget(durationLabel);

    // задаём лэйаут кнопке
    setLayout(trackLayout);
    setFixedHeight(55);
    setFlat(true);  // убираем стандартный фон QPushButton

    // стили для кнопки
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
}


QString AlbumTrackButton::getTrackName(){
    return trackData.name;
}

track * AlbumTrackButton::getTrack(){
    return &trackData;
}

QString *AlbumTrackButton::getTrackAuthorUsertag(){
    return &trackData.authorUsertags[0];
}

void AlbumTrackButton::mouseDoubleClickEvent(QMouseEvent *event){
    emit trackButtonClicked(getTrack());
}


AlbumTrackButton::AlbumTrackButton(QWidget *parent)
    : QPushButton(parent)
{
    // создаём горизонтальный layout для кнопки трека
    QHBoxLayout *trackLayout = new QHBoxLayout(this);
    trackLayout->setAlignment(Qt::AlignLeft);
    trackLayout->setContentsMargins(0, 0, 0, 0);  // убираем отступы вокруг layout

    // метка для номера трека с фиксированной шириной и стилями
    QLabel *numberOfTrackLabel = new QLabel(this);
    numberOfTrackLabel->setMargin(0);
    numberOfTrackLabel->setText("#");
    numberOfTrackLabel->setFixedWidth(50);
    numberOfTrackLabel->setStyleSheet("background: transparent; font-weight: bold; font-size: 14px; font-family: 'Tahoma'; padding-left: 20px");
    trackLayout->addWidget(numberOfTrackLabel);

    // вертикальный layout для названия трека и автора
    QVBoxLayout *name_and_author = new QVBoxLayout();

    QLabel *nameLabel = new QLabel("Name");
    nameLabel->setFixedWidth(250);
    nameLabel->setStyleSheet("background: transparent; font-weight: bold; font-size: 14px; font-family: 'Tahoma';");
    nameLabel->setAlignment(Qt::AlignCenter);
    name_and_author->addWidget(nameLabel);

    trackLayout->addLayout(name_and_author);

    // пример длительности трека в формате mm:ss
    int minutes = 60;
    int seconds = 60;
    QString duration = QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));

    QLabel *durationLabel = new QLabel(duration);
    durationLabel->setFixedHeight(14);
    durationLabel->setFixedWidth(50);
    durationLabel->setStyleSheet("background: transparent; color: #828282; font-weight: bold; font-size: 12px; font-family: 'Tahoma';");

    trackLayout->addStretch();  // добавляем растяжку, чтобы durationLabel прижалась к правому краю
    trackLayout->addWidget(durationLabel);

    // задаём layout и размеры кнопки
    setLayout(trackLayout);
    setFixedHeight(30);
    setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            border: none;
            border-bottom: 1px solid rgba(255, 255, 255, 70);
        }
        QPushButton:hover { background: none; }
    )");
}

AlbumWidget::AlbumWidget(const struct album &albumData, QVector<int> likedTracks, QWidget *parent)
    : QWidget(parent), albumData(albumData)
{
    // основной вертикальный layout для виджета альбома
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // создаём scroll area для прокрутки содержимого альбома
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    // виджет-обёртка для содержимого scroll area
    QWidget *scrollWidget = new QWidget();

    // вертикальный layout для scrollWidget
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);

    // виджет для информации об альбоме
    QWidget *albumInfoWidget = new QWidget();
    scrollLayout->addWidget(albumInfoWidget);
    scrollLayout->setContentsMargins(0, 20, 0, 0);  // отступ сверху

    QHBoxLayout *albumInfoLayout = new QHBoxLayout(albumInfoWidget);
    albumInfoLayout->setContentsMargins(0, 0, 0, 0);

    // лейбл для обложки альбома
    QLabel *coverLabel = new QLabel(this);
    loadCover(albumData.coverpath, coverLabel);
    albumInfoLayout->addWidget(coverLabel, 0, Qt::AlignLeft);

    // виджет и layout для названия и автора альбома
    QWidget *albumNameWidget = new QWidget();
    albumInfoLayout->addWidget(albumNameWidget, 1, Qt::AlignLeft);
    albumNameWidget->setFixedHeight(200);
    albumInfoWidget->setFixedHeight(200);

    QVBoxLayout *albumNameLayout = new QVBoxLayout(albumNameWidget);
    albumInfoLayout->setContentsMargins(20, 0, 0, 0);  // отступ слева у albumInfoLayout
    albumNameLayout->setContentsMargins(20, 0, 0, 0);  // отступ слева у albumNameLayout

    // метка для типа объекта: альбом или сингл
    QLabel *typeOfObject = new QLabel();
    typeOfObject->setText(albumData.track_count == 1 ? "SINGLE" : "ALBUM");
    typeOfObject->setFixedHeight(35);
    typeOfObject->setStyleSheet("color: #615D5D; font-size: 20px; font-family: 'K2D'; font-weight: bold;");

    // метка с названием альбома
    QLabel *albumNameLabel = new QLabel();
    albumNameLabel->setText(albumData.name);
    albumNameLabel->setFixedHeight(50);
    albumNameLabel->setStyleSheet("padding-top: 0px; font-size: 40px; font-family: 'Tahoma'; font-weight: bold;");

    // кнопка с именем автора альбома
    QPushButton *albumAuthorButton = new QPushButton(albumData.authorUsername);
    connect(albumAuthorButton, &QPushButton::clicked, [this, albumData]() {
        QString tempUsertag = albumData.authorUsertag;
        emit authorButtonClicked(&tempUsertag);
    });

    // вычисляем ширину кнопки с учётом текста и добавляем отступы
    QFontMetrics fm(albumData.authorUsername);
    int textWidth = fm.horizontalAdvance(albumData.authorUsername);
    albumAuthorButton->setFixedWidth(textWidth + 35);
    set_button_style(albumAuthorButton, 20, "white");  // применяем стиль кнопке

    // метка с информацией об альбоме: дата, количество треков, длительность
    QLabel *albumInfoLabel = new QLabel();
    QString formattedDate = QDate::fromString(albumData.createDate, "yyyy-MM-dd")
                                .toString("MMMM d, yyyy");  // форматируем дату в "July 15, 2025"

    int totalMinutes = albumData.total_duration / 60;
    QString labelText = QString("%1 - %2 %3, %4 %5")
                            .arg(formattedDate)
                            .arg(albumData.track_count)
                            .arg(albumData.track_count == 1 ? "song" : "songs")
                            .arg(totalMinutes)
                            .arg(totalMinutes == 1 ? "minute" : "minutes");

    albumInfoLabel->setText(labelText);
    albumInfoLabel->setFixedHeight(20);
    albumInfoLabel->setStyleSheet("font-size: 10px; font-family: 'Tahoma'; font-weight: bold;");

    // добавляем все элементы в вертикальный layout названия альбома
    albumNameLayout->addWidget(typeOfObject);
    albumNameLayout->addWidget(albumNameLabel);
    albumNameLayout->addWidget(albumAuthorButton);
    albumNameLayout->addWidget(albumInfoLabel);

    // виджет и layout для списка треков
    QWidget *tracklistWidget = new QWidget();
    QVBoxLayout *trackListLayout = new QVBoxLayout(tracklistWidget);
    trackListLayout->setAlignment(Qt::AlignTop);
    trackListLayout->setContentsMargins(0, 0, 0, 0);  // убираем внешние отступы
    trackListLayout->setSpacing(0);                    // убираем промежутки между треками

    scrollLayout->addWidget(tracklistWidget);

    // кнопка "лайк" альбома
    QPushButton *likeAlbumButton = new QPushButton();
    likeAlbumButton->setStyleSheet("background: transparent");
    likeAlbumButton->setCursor(Qt::PointingHandCursor);
    likeAlbumButton->setFixedSize(50, 50);

    QLabel *likeAlbumButtonLabel = new QLabel(likeAlbumButton);
    QPixmap likeAlbumButtonPath("resources/icons/adding.png");
    likeAlbumButtonLabel->setPixmap(likeAlbumButtonPath.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    albumNameLayout->addWidget(likeAlbumButton);

    // неактивная кнопка для заголовка списка треков
    AlbumTrackButton *trackInfo = new AlbumTrackButton(this);
    trackInfo->setEnabled(false);                // отключаем клики
    trackInfo->setFlat(true);                    // убираем фон и границы
    trackInfo->setFocusPolicy(Qt::NoFocus);     // отключаем фокусировку
    trackInfo->setCursor(Qt::ArrowCursor);      // обычный курсор (стрелка)
    trackListLayout->addWidget(trackInfo);

    // создаём кнопки для каждого трека в альбоме
    for (int i = 0; i < albumData.tracks.size(); ++i) {
        track sometrack = albumData.tracks[i];
        QString trackNumber = QString::number(i + 1);  // номер трека как строка
        bool isLiked = likedTracks.contains(sometrack.id);

        AlbumTrackButton *trackButton = new AlbumTrackButton(sometrack, isLiked, trackNumber, this);

        connect(trackButton, &AlbumTrackButton::trackButtonClicked, this, &AlbumWidget::onTrackdoubleClicked);
        connect(trackButton, &AlbumTrackButton::trackNameButtonClicked, [this, trackButton]() {
            emit trackNameButtonClicked(trackButton->getTrack());
        });
        connect(trackButton, &AlbumTrackButton::trackAuthorButtonClicked, [this, trackButton]() {
            emit authorButtonClicked(trackButton->getTrackAuthorUsertag());
        });
        connect(trackButton, &AlbumTrackButton::albumLiked, [this, trackButton]() {
            emit albumLiked(trackButton->getTrack());
        });

        trackListLayout->addWidget(trackButton);
    }

    // виджет с описанием альбома
    QWidget *descriptionWidget = new QWidget;
    if(albumData.description.isEmpty()) {
        descriptionWidget->hide();  // скрываем, если описание отсутствует
    }

    descriptionWidget->setStyleSheet(R"(
        QWidget {
            background-color: #2c2c2c;
            border: 1px solid #444444;
            border-radius: 8px;
            padding: 12px;
        }
        QLabel {
            font-size: 14px;
            color: #dddddd;
        }
    )");

    QVBoxLayout *descLayout = new QVBoxLayout(descriptionWidget);
    descLayout->setContentsMargins(12, 12, 12, 12);

    QLabel *descriptionLabel = new QLabel;
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setText(albumData.description);
    descLayout->addWidget(descriptionLabel);

    // добавляем описание в основной scroll layout
    scrollLayout->addWidget(descriptionWidget);
    scrollLayout->addStretch();  // растягиваем, чтобы описание прижималось вниз

    // помещаем scrollWidget в
    scrollLayout->addWidget(descriptionWidget);
    scrollLayout->addStretch();  // чтобы прижать вниз


    // 6. Вложим scrollWidget в scrollArea
    scrollArea->setWidget(scrollWidget);
    // 7. Добавим scrollArea в основной layout
    mainLayout->addWidget(scrollArea);
}

void AlbumWidget::loadCover(const QString& url, QLabel *label) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [label, manager](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QPixmap pixmap;
            if (pixmap.loadFromData(data)) {
                label->setPixmap(pixmap.scaled(200, 200, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation).copy(0, 0, 200, 200));
            } else {
                qDebug() << "Не удалось загрузить картинку из данных!";
            }
        } else {
            qDebug() << "Ошибка загрузки:" << reply->errorString();
        }
        reply->deleteLater();
        manager->deleteLater();
    });
    QNetworkRequest request(url);
    manager->get(request);
}



void AlbumWidget::onTrackdoubleClicked(track *trackData){
    emit trackButtonClicked(trackData);
}




