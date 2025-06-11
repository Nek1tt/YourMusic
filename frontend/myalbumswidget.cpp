// Стандартные заголовочные файлы Qt
#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QString>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QBuffer>
#include <QMessageBox>
#include <QDir>
#include <QDebug>    // Для отладки

// Стандартные библиотеки
#include <fstream>
#include <vector>

// Ваши заголовочные файлы
#include "myalbumswidget.h"
#include "setstyle.h"

// =============== AlbumButton ====================

AlbumButton::AlbumButton(const struct album &albumData, QWidget *parent)
    : QPushButton(parent), albumData(albumData)
{
    // Макет кнопки
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Обложка альбома
    QLabel *coverLabel = new QLabel(this);
    loadCover(albumData.coverpath, coverLabel);
    layout->addWidget(coverLabel);

    // Кнопка с названием альбома
    QPushButton *nameButton = new QPushButton(albumData.name, this);
    connect(nameButton, &QPushButton::clicked, [this]() {
        emit albumNameButtonClicked();
    });
    nameButton->setFixedHeight(16);
    set_button_style(nameButton, 11);
    layout->addWidget(nameButton);

    // Кнопка с именем автора
    QPushButton *authorButton = new QPushButton(albumData.authorUsername, this);
    connect(authorButton, &QPushButton::clicked, [this]() {
        emit authorButtonClicked();
    });
    authorButton->setFixedHeight(15);
    set_button_style(authorButton, 10, "#828282");
    layout->addWidget(authorButton);

    // Оформление всей кнопки
    setLayout(layout);
    setFixedSize(170, 210);
    this->setStyleSheet(
        "QPushButton {"
        "    background: none;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 30);"
        "    border: none;"
        "    outline: none;"
        "}"
        );
}

// Загрузка обложки альбома по URL
void AlbumButton::loadCover(const QString& url, QLabel *label)
{
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

// Геттеры для albumData
QString AlbumButton::getAlbumName()
{
    return albumData.name;
}

album AlbumButton::getAlbum()
{
    return albumData;
}

QString AlbumButton::getAuthorUsertag()
{
    return albumData.authorUsertag;
}

// =============== MyAlbumsWidget ====================

MyAlbumsWidget::MyAlbumsWidget(QString widgetName, QWidget *parent)
    : QWidget(parent)
{
    // Основной вертикальный макет
    MyAlbumsLayout = new QVBoxLayout(this);

    // Верхняя кнопка (заголовок виджета)
    QWidget *myAlbumButtonWidget = new QWidget();
    QVBoxLayout *myAlbumButtonLayout = new QVBoxLayout(myAlbumButtonWidget);

    QPushButton *myAlbumButton = new QPushButton(widgetName);
    myAlbumButton->setStyleSheet(
        "QPushButton {"
        "font-weight: bold;"
        "font-size: 32px;"
        "text-align: left;"
        "font-family: 'Tahoma';"
        "background: none;"
        "border: none;"
        "text-decoration: none;"
        "}"
        "QPushButton:hover {"
        "text-decoration: underline;"
        "}"
        );

    // Сигнал при нажатии на кнопку заголовка
    connect(myAlbumButton, &QPushButton::clicked, [this]() {
        emit myAlbumsButtonClicked(getAlbum());
    });

    myAlbumButtonLayout->addWidget(myAlbumButton);
    MyAlbumsLayout->addWidget(myAlbumButtonWidget);

    // Прокручиваемая область с альбомами
    albums = new QWidget();
    scrollAreaAlbums = new QScrollArea(this);
    setFixedHeight(330);

    albumsLayout = new QHBoxLayout(albums);
}

// Геттер для текущего списка альбомов
QVector<album> MyAlbumsWidget::getAlbum()
{
    return albums_vector;
}

// Добавление альбомов в виджет
void MyAlbumsWidget::add_albums(QVector<album> newAlbumList)
{
    // Очищаем предыдущие элементы
    clearLayout(albumsLayout);

    albums_vector = newAlbumList;

    // Прячем или показываем блок в зависимости от наличия альбомов
    if (newAlbumList.isEmpty()) {
        // this->hide();
    } else {
        this->show();
    }

    // Добавляем кнопки для каждого альбома
    for (auto& album : albums_vector) {
        AlbumButton *albumButton = new AlbumButton(album);
        albumsLayout->addWidget(albumButton);

        connect(albumButton, &QPushButton::clicked, [this, albumButton]() {
            emit albumButtonClicked(albumButton->getAlbum());
        });

        connect(albumButton, &AlbumButton::albumNameButtonClicked, [this, albumButton]() {
            emit albumButtonClicked(albumButton->getAlbum());
        });

        connect(albumButton, &AlbumButton::authorButtonClicked, [this, albumButton]() {
            QString tempUsertag = albumButton->getAuthorUsertag();
            emit authorButtonClicked(&tempUsertag);
        });
    }

    // Добавляем stretch в конец
    albumsLayout->addStretch();

    // Обновляем виджет
    albums->setLayout(albumsLayout);
    albums->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    albums->setMinimumWidth(albums_vector.size() * 180);
    albums->setMinimumHeight(230);

    scrollAreaAlbums->setWidget(albums);
    scrollAreaAlbums->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollAreaAlbums->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Добавляем scrollAreaAlbums в основной layout только 1 раз
    if (MyAlbumsLayout->count() == 1) {
        MyAlbumsLayout->addWidget(scrollAreaAlbums);
    }

    scrollAreaAlbums->update();
}

// Очистка layout (удаление всех виджетов)
void MyAlbumsWidget::clearLayout(QLayout *layout)
{
    if (layout == nullptr) return;

    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

// Изменение ширины виджета альбомов
void MyAlbumsWidget::resizeAlbums(int width)
{
    this->setFixedWidth(width * 0.9);
}

// =============== Загрузка альбомов из JSON ====================

// Загрузка списка альбомов
QVector<album> loadAlbumsFromJson(const QString &filePath)
{
    QFile file(filePath);
    QVector<album> albums;

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file:" << filePath;
        return albums;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return albums;
    }

    QJsonArray albumArray = doc.array();
    for (const QJsonValue &albumValue : albumArray) {
        QJsonObject root = albumValue.toObject();
        album albumdata;

        albumdata.id = root["id"].toInt();
        albumdata.name = root["title"].toString();
        albumdata.coverpath = root["cover_path"].toString();
        albumdata.authorUsername = "nurshat";  // Пример
        albumdata.authorUsertag = "1";         // Пример
        albumdata.track_count = root["track_count"].toInt();

        QJsonArray trackArray = root["tracks"].toArray();
        for (const QJsonValue &trackValue : trackArray) {
            QJsonObject trackObj = trackValue.toObject();
            track trackdata;
            trackdata.id = trackObj["id"].toInt();
            trackdata.album_id = trackObj["album_id"].toInt();
            trackdata.name = trackObj["title"].toString();
            trackdata.duration_s = trackObj["durationMs"].toInt();
            trackdata.coverpath = trackObj["coverPath"].toString();
            trackdata.authors.push_back(trackObj["author"].toString());
            albumdata.tracks.append(trackdata);
        }

        albums.append(albumdata);
    }

    return albums;
}

// Загрузка одного альбома (single album)
album loadSingleAlbumFromJson(const QString &filePath)
{
    QFile file(filePath);
    album albumdata;

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file:" << filePath;
        return albumdata;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return albumdata;
    }

    QJsonObject root = doc.object();
    QJsonArray trackArray = root["tracks"].toArray();

    // Заполняем альбом (примерные данные)
    albumdata.id = 0;
    albumdata.name = "Mixed Tracks Album";
    albumdata.coverpath = "resources/imgs/ava.png";
    albumdata.authorUsername = "nurshat";
    albumdata.authorUsertag = "1";
    albumdata.track_count = trackArray.size();

    for (const QJsonValue &trackValue : trackArray) {
        QJsonObject trackObj = trackValue.toObject();
        track trackdata;
        trackdata.id = trackObj["id"].toInt();
        trackdata.album_id = trackObj["albumId"].toInt();
        trackdata.name = trackObj["title"].toString();
        trackdata.duration_s = trackObj["durationMs"].toInt();
        trackdata.coverpath = trackObj["coverPath"].toString();
        trackdata.authors.push_back(trackObj["author"].toString());
        albumdata.tracks.append(trackdata);
    }
    return albumdata;
}
