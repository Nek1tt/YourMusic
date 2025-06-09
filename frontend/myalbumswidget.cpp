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
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
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
//#include <QDebug>  // <--- добавь это


#include "myalbumswidget.h"
#include "setstyle.h"


AlbumButton::AlbumButton(const struct album &albumData, QWidget *parent)
    : QPushButton(parent), albumData(albumData) {

    // Настроим макет кнопки
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Создаем метку для обложки
    QLabel *coverLabel = new QLabel(this);
    loadCover(albumData.coverpath, coverLabel);
    //setRoundedImage(coverLabel, albumData.coverpath, 150, 15);
    layout->addWidget(coverLabel);  // Добавляем обложку в макет

    // Создаем метки для названия и автора
    QPushButton *nameButton = new QPushButton(albumData.name, this);
    connect(nameButton, &QPushButton::clicked, [this]() {
        emit albumNameButtonClicked();
    });

    nameButton->setFixedHeight(16);
    set_button_style(nameButton, 11);
    //nameLabel->setStyleSheet("font-weight: bold; font-size: 11px; font-family: 'Tahoma';");
    //nameLabel->setAlignment(Qt::AlignLeft);
    layout->addWidget(nameButton);

    QPushButton *authorButton = new QPushButton(albumData.authorUsername, this);
    connect(authorButton, &QPushButton::clicked, [this]() {
        emit authorButtonClicked();
    });
    authorButton->setFixedHeight(15);
    set_button_style(authorButton, 10, "#828282");
    //authorLabel->setStyleSheet("color: #828282; font-weight: bold; font-size: 10px; font-family: 'Tahoma';");
    //authorLabel->setAlignment(Qt::AlignLeft);
    layout->addWidget(authorButton);

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

    // Устанавливаем размер кнопки
}

void AlbumButton::loadCover(const QString& url, QLabel *label) {
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


QString AlbumButton::getAlbumName(){
    return albumData.name;
}

album AlbumButton::getAlbum(){
    return albumData;
}

QString AlbumButton::getAuthorUsertag(){
    return albumData.authorUsertag;
}


MyAlbumsWidget::MyAlbumsWidget(QString widgetName, QWidget *parent) : QWidget(parent)
{
    MyAlbumsLayout = new QVBoxLayout(this);
    QWidget *myAlbumButtonWidget = new QWidget();
    QVBoxLayout *myAlbumButtonLayout = new QVBoxLayout(myAlbumButtonWidget);
    QPushButton *myAlbumButton = new QPushButton(widgetName);
    myAlbumButton->setStyleSheet(
        "QPushButton {"
        "font-weight: bold;"
        "font-size: 32px;"
        "text-align: left;"
        "font-family: 'Tahoma';"
        "    background: none;"                  // Убираем фон
        "    border: none;"                      // Убираем рамку
        //"    border: 2px solid blue;"  // рамка 2px
        "    text-decoration: none;"             // Убираем подчеркивание по умолчанию
        "}"
        "QPushButton:hover {"
        "    text-decoration: underline;"        // Подчеркиваем текст при наведении
        "}"
        );
    connect(myAlbumButton, &QPushButton::clicked, [this]() {
        emit myAlbumsButtonClicked(getAlbum());
    });
    //myAlbumButton->setFixedSize(200, 40);
    myAlbumButtonLayout->addWidget(myAlbumButton);
    MyAlbumsLayout->addWidget(myAlbumButtonWidget);


    albums = new QWidget();  // виджет для прокрутки
    scrollAreaAlbums = new QScrollArea(this);  // прокручиваемая область
    setFixedHeight(330);

    // Создаем горизонтальный layout
    albumsLayout = new QHBoxLayout(albums);
}

QVector<album> MyAlbumsWidget::getAlbum(){
    return albums_vector;
}

void MyAlbumsWidget::add_albums(QVector<album> newAlbumList) {
    clearLayout(albumsLayout);
    albums_vector = newAlbumList;
    if(newAlbumList.isEmpty()){
        //this->hide();
    }else{
         this->show();
    }

    // connect(albumButton, &QPushButton::clicked, this, [this, albumButton]() {
    //     emit albumClicked(albumButton->getAlbumName());
    // });
    // if(!newAlbumList.isEmpty()){
    //     qDebug()<<newAlbumList[0].name;
    //     qDebug()<<newAlbumList[0].author;

    //     qDebug()<<newAlbumList[0].author_id;
    //     qDebug()<<newAlbumList[0].coverpath;

    //     qDebug()<<newAlbumList[0].id;
    //     qDebug()<<newAlbumList[0].track_count;
    // }


    // Добавляем элементы в layout
    for (auto& album : albums_vector) {
        // Создаем кнопки для каждого альбома
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

    // Добавляем отступ в конец, чтобы при нехватке элементов они не сжимались
    albumsLayout->addStretch();

    // Обновляем размер родительского виджета
    albums->setLayout(albumsLayout);
    albums->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    albums->setMinimumWidth(albums_vector.size() * 180);  // Примерная ширина всех плейлистов
    albums->setMinimumHeight(230);

    scrollAreaAlbums->setWidget(albums);
    scrollAreaAlbums->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollAreaAlbums->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Удостоверимся, что `scrollAreaPLaylists` добавлен в layout только один раз
    if (MyAlbumsLayout->count() == 1) {
        MyAlbumsLayout->addWidget(scrollAreaAlbums);
    }

    scrollAreaAlbums->update();  // Принудительное обновление прокрутки
}


void MyAlbumsWidget::clearLayout(QLayout *layout) {
    if (layout == nullptr) return;

    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget(); // Удаляем виджет
        }
        delete item; // Удаляем сам элемент
    }
}

void MyAlbumsWidget::resizeAlbums(int width) {
    this->setFixedWidth(width*0.9);
}


QVector<album> loadAlbumsFromJson(const QString &filePath) {
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
        albumdata.authorUsername = "nurshat";
        albumdata.authorUsertag = "1";
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

album loadSingleAlbumFromJson(const QString &filePath) {
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

    // Пример: создаём альбом вручную (можешь заменить на реальные данные)
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
        trackdata.album_id = trackObj["albumId"].toInt(); // по сути всегда 0
        trackdata.name = trackObj["title"].toString();
        trackdata.duration_s = trackObj["durationMs"].toInt();
        trackdata.coverpath = trackObj["coverPath"].toString();
        trackdata.authors.push_back(trackObj["author"].toString());
        albumdata.tracks.append(trackdata);
    }

    return albumdata;
}
