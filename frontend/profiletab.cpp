#include <QScreen> //for Qscreen *screen = QApplication::primaryScreen();
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
// #include <QWebSocket>
#include <QMessageBox>
#include <QDir>
#include <QStackedWidget>
#include <QScrollBar>


#include "hometab.h"
#include "userProfileWidget.h"
#include "myalbumswidget.h"
#include "albumwidget.h"
#include "mytrackswidget.h"
#include "albomlistwidget.h"
#include "userslistwidget.h"

ProfileTab::ProfileTab(QString *usertag, UserInfo currentAuthor, WebSocketClient *webSocket, QWidget  *parent, QWidget *tab)
    : AbstractTab(parent),
    mainUsertag(usertag),
    webSocket(webSocket)
{
    innerStacked = new QStackedWidget();
    mainWidget = new ProfileWidget();
    currentAuthorOfObjects = currentAuthor;

    connect(mainWidget, &ProfileWidget::followersButtonClicked, this, &ProfileTab::onFollowersButtonClicked);
    connect(mainWidget, &ProfileWidget::followingButtonClicked, this, &ProfileTab::onFollowingButtonClicked);
    connect(mainWidget, &ProfileWidget::tracksLoadedButtonClicked, this, &ProfileTab::onTracksLoadedButtonClicked);
    connect(mainWidget, &ProfileWidget::tracksAddedButtonClicked, this, &ProfileTab::onTracksAddedButtonClicked);

    connect(mainWidget, &ProfileWidget::albumClicked, this, &ProfileTab::onAlbumById);
    connect(mainWidget, &ProfileWidget::albumListClicked, this, &ProfileTab::onAlbumListClicked);
    connect(mainWidget, &ProfileWidget::trackDoubleClicked, this, &ProfileTab::onTrackdoubleClicked);


    connect(mainWidget, &ProfileWidget::trackNameButtonClicked, this, &ProfileTab::onAlbumByTrack);

    connect(mainWidget, &ProfileWidget::authorButtonClicked, this, &ProfileTab::onAuthorButtonClicked);

    scrollWidget = new QWidget();
    mainLayout = new QVBoxLayout(scrollWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(mainWidget);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Добавляем scrollArea как главный экран в стек
    innerStacked->addWidget(scrollArea);
    //mainWidget->setStyleSheet("QWidget { border: 1px solid red; }");

    // Размещаем стек в основной вкладке
    QVBoxLayout *tabLayout = new QVBoxLayout(this);
    tabLayout->setContentsMargins(0, 0, 0, 0);
    tabLayout->addWidget(innerStacked);
    setStyleSheet("border: 1px, solid, red");
    connect(webSocket, &WebSocketClient::messageReceived,
            this, &ProfileTab::onTextMessageReceived);
}


QStackedWidget *ProfileTab::getInnerStacked(){
    return innerStacked;
}

void ProfileTab::resizeProfile(int width){
    widthForResize = width;
    //scrollWidget->setFixedWidth(width-25);
    mainWidget->resizeProfile(width);
}

void ProfileTab::profileButtonClicked(UserInfo userData, QVector<album> albumList, album likedTracks, album loadedTracks){
    mainWidget->updateAlbums(userData, albumList, likedTracks, loadedTracks);
    currentAuthorOfObjects = userData;
    currentViewers.push_back(currentAuthorOfObjects.usertag);
}

void ProfileTab::onTextMessageReceived(const QString &type, const QJsonObject &data) {
    qDebug()<<type;
    if(type=="catalog_response"){
        if(typeOfQuery=="likedTracksButton"){
            qDebug()<<data;
            QJsonArray tracksArray = data["tracks"].toArray();

            album a;
            a.id = 0;
            a.name = "Liked Tracks";
            a.coverpath = "http://84.237.53.143:8083/track/3021";
            a.authorUsername = currentAuthorOfObjects.username;
            a.authorUsertag = currentAuthorOfObjects.usertag;
            a.track_count = tracksArray.size();
            a.total_duration = 0;
            a.createDate = "";
            a.description = "";
            QVector<int> likedTracks;
            for (const QJsonValue &val : tracksArray) {
                QJsonObject trackObj = val.toObject();
                track t;
                t.id = trackObj["id"].toInt();
                t.album_id = a.id;  // можно присвоить album.id
                t.name = trackObj["title"].toString();
                t.duration_s = trackObj["duration_seconds"].toInt();
                QString coverUrl = QString("http://84.237.53.143:8083/track/%1").arg(t.id);
                t.coverpath = coverUrl;
                QString authorsStr = trackObj["authors"].toString();
                QStringList authorList = authorsStr.split(",", Qt::SkipEmptyParts);
                for (const QString &author : authorList) {
                    t.authors.append(author.trimmed());
                }
                QString usertagsStr = trackObj["author_usertag"].toString();
                QStringList userList = usertagsStr.split(",", Qt::SkipEmptyParts);
                for (const QString &author : userList) {
                    qDebug()<<"author";
                    qDebug()<<author;

                    t.authorUsertags.append(author.trimmed());
                }
                t.upload_date = trackObj["upload_date"].toString();
                a.total_duration += t.duration_s;
                a.tracks.append(t);
                likedTracks.push_back(t.id);
            }

            onAlbumClicked(a, likedTracks);
            typeOfQuery="None";
        }else if(typeOfQuery=="loadedTracksButton"){
            qDebug()<<data;
            QJsonArray tracksArray = data["tracks"].toArray();

            album a;
            a.id = 0;
            a.name = "Loaded Tracks";
            a.coverpath = "http://84.237.53.143:8083/track/3021";
            a.authorUsername = currentAuthorOfObjects.username;
            a.authorUsertag = currentAuthorOfObjects.usertag;
            a.track_count = tracksArray.size();
            a.total_duration = 0;
            a.createDate = "";
            a.description = "";
            QVector<int> likedTracks;
            for (const QJsonValue &val : tracksArray) {
                QJsonObject trackObj = val.toObject();
                track t;
                t.id = trackObj["id"].toInt();
                t.album_id = a.id;  // можно присвоить album.id
                t.name = trackObj["title"].toString();
                t.duration_s = trackObj["duration_seconds"].toInt();
                QString coverUrl = QString("http://84.237.53.143:8083/track/%1").arg(t.id);
                t.coverpath = coverUrl;
                QString authorsStr = trackObj["authors"].toString();
                QStringList authorList = authorsStr.split(",", Qt::SkipEmptyParts);
                for (const QString &author : authorList) {
                    t.authors.append(author.trimmed());
                }
                QString usertagsStr = trackObj["author_usertag"].toString();
                QStringList userList = usertagsStr.split(",", Qt::SkipEmptyParts);
                for (const QString &author : userList) {
                    t.authorUsertags.append(author.trimmed());
                }
                t.upload_date = trackObj["upload_date"].toString();
                a.total_duration += t.duration_s;
                a.tracks.append(t);
                likedTracks.push_back(t.id);
            }

            onAlbumClicked(a, likedTracks);
            typeOfQuery="None";
        } else if(typeOfQuery=="followingsTracksButton"){
            qDebug()<<data;
            QVector<UserInfo> usersVector;

            QJsonArray usersArray = data["users"].toArray();
            for (const QJsonValue &val : usersArray) {
                QJsonObject userObj = val.toObject();
                UserInfo u;

                u.avatarPath = "";  // В этом JSON нет поля avatarPath — оставляем пустым или можно задать дефолтный аватар.
                u.username = userObj["username"].toString();
                u.usertag = userObj["usertag"].toString();
                u.followersnum = userObj["followersNum"].toInt();
                u.followingnum = userObj["followingNum"].toInt();
                u.tracksLoadednum = userObj["tracksLoadedNum"].toInt();
                u.tracksAddednum = 0;  // В этом JSON нет tracksAddedNum — задаём 0.

                usersVector.append(u);
            }
            onUsersListClicked(usersVector);
            typeOfQuery="None";
        }else if(typeOfQuery=="followersTracksButton"){
            qDebug()<<data;
            QVector<UserInfo> usersVector;

            QJsonArray usersArray = data["users"].toArray();
            for (const QJsonValue &val : usersArray) {
                QJsonObject userObj = val.toObject();
                UserInfo u;

                u.avatarPath = "";  // В этом JSON нет поля avatarPath — оставляем пустым или можно задать дефолтный аватар.
                u.username = userObj["username"].toString();
                u.usertag = userObj["usertag"].toString();
                u.followersnum = userObj["followersNum"].toInt();
                u.followingnum = userObj["followingNum"].toInt();
                u.tracksLoadednum = userObj["tracksLoadedNum"].toInt();
                u.tracksAddednum = 0;  // В этом JSON нет tracksAddedNum — задаём 0.

                usersVector.append(u);
            }
            onUsersListClicked(usersVector);

            typeOfQuery="None";
        }else if(typeOfQuery=="albumButton"){
            qDebug()<<data;
            album a;

            QJsonObject albumObj = data["album"].toObject();

            a.id = albumObj["id"].toInt();
            a.name = albumObj["title"].toString();
            a.coverpath = QString("http://84.237.53.143:8083/album/%1").arg(a.id);  // пример, берем первую liked_track как обложку
            a.authorUsername = albumObj["authors"].toString();
            a.authorUsertag = albumObj["author_usertag"].toString();
            qDebug()<<"            qDebug()<<a.authorUsertag;";
            qDebug()<<a.authorUsertag;
            a.createDate = albumObj["created_date"].toString();
            a.description = albumObj["description"].toString();
            a.track_count = albumObj["track_count"].toInt();
            a.total_duration = albumObj["total_duration"].toInt();

            QVector<int> likedTracks;
            QJsonArray likedTracksArray = albumObj["liked_tracks"].toArray();
            for (const QJsonValue &val : likedTracksArray) {
                likedTracks.append(val.toInt());
            }

            QJsonArray tracksArray = albumObj["tracks"].toArray();
            for (const QJsonValue &val : tracksArray) {
                QJsonObject trackObj = val.toObject();
                track t;
                t.id = trackObj["id"].toInt();
                t.album_id = trackObj["album_id"].toInt();
                t.name = trackObj["title"].toString();
                t.duration_s = trackObj["duration_seconds"].toInt();
                QString coverUrl = QString("http://84.237.53.143:8083/track/%1").arg(t.id);
                t.coverpath = coverUrl;
                QString authorsStr = trackObj["authors"].toString();
                QStringList authorList = authorsStr.split(",", Qt::SkipEmptyParts);
                for (const QString &author : authorList) {
                    t.authors.append(author.trimmed());
                }
                t.authorUsertags.push_back(a.authorUsertag);
                t.upload_date = trackObj["upload_date"].toString();
                a.tracks.append(t);
            }
            onAlbumClicked(a, likedTracks);
            typeOfQuery="None";

        }else if(type == "catalog_response" && typeOfQuery=="userButton"){
            qDebug()<<data;
            qDebug()<<data;
            QVector<album> *likedAlbums = new QVector<album>;
            QVector<album> *loadedAlbums = new QVector<album>;
            album *likedTracks = new album;
            album *loadedTracks = new album;
            UserInfo *viewUserInfo = new UserInfo;


            QJsonArray likedAlbumsArray = data["liked_albums"].toArray();
            QJsonArray likedTracksArray = data["liked_tracks"].toArray();
            QJsonArray loadedAlbumsArray = data["loaded_albums"].toArray();
            QJsonArray loadedTracksArray = data["loaded_tracks"].toArray();
            QJsonObject user = data.value("user_info").toObject();
            viewUserInfo->username = user["username"].toString();
            viewUserInfo->usertag = user["usertag"].toString();
            QString coverUrl = QString("http://84.237.53.143:8083/profile/%1").arg(viewUserInfo->usertag);
            viewUserInfo->avatarPath = coverUrl;
            viewUserInfo->followersnum = user["followersnum"].toInt();
            viewUserInfo->followingnum = user["followingnum"].toInt();
            viewUserInfo->tracksAddednum = user["tracksAddedNum"].toInt();
            viewUserInfo->tracksLoadednum = user["tracksLoadedNum"].toInt();

            qDebug() << "  mainUserInfo->username:" << viewUserInfo->username;
            qDebug() << "  mainUserInfo->usertag:" << viewUserInfo->usertag;
            qDebug() << "  mainUserInfo->followersnum:" << viewUserInfo->followersnum;


            for (const QJsonValue& val : likedAlbumsArray) {
                QJsonObject albumObj = val.toObject();
                album a;
                a.authorUsername = albumObj["author_username"].toString();
                a.authorUsertag = albumObj["author_usertag"].toString();
                a.createDate = albumObj["created_date"].toString();
                a.description = albumObj["description"].toString();
                a.id = albumObj["album_id"].toInt();
                a.name = albumObj["title"].toString();
                a.total_duration = albumObj["total_duration"].toInt();
                a.track_count = albumObj["track_count"].toInt();
                QString coverUrl = QString("http://84.237.53.143:8083/album/%1").arg(a.id);
                a.coverpath = coverUrl;

                // Если в альбоме есть массив tracks — парсим его
                QJsonArray tracksArray = albumObj["tracks"].toArray();
                for (const QJsonValue& trackVal : tracksArray) {
                    QJsonObject trackObj = trackVal.toObject();
                    track t;
                    t.album_id = trackObj["album_id"].toInt();
                    QString authorsStr = trackObj["authors"].toString();
                    QStringList authorList = authorsStr.split(",", Qt::SkipEmptyParts);
                    for (const QString &author : authorList) {
                        t.authors.append(author.trimmed());  // убираем пробелы вокруг
                    }
                    QString usertagsStr = trackObj["author_usertag"].toString();
                    QStringList userList = usertagsStr.split(",", Qt::SkipEmptyParts);
                    for (const QString &author : userList) {
                        qDebug()<<"author";
                        qDebug()<<author;

                        t.authorUsertags.append(author.trimmed());
                    }
                    t.duration_s = trackObj["duration_seconds"].toInt();
                    t.id = trackObj["id"].toInt();
                    t.name = trackObj["title"].toString();
                    t.upload_date = trackObj["upload_date"].toString();
                    QString coverUrl = QString("http://84.237.53.143:8083/track/%1").arg(t.id);
                    t.coverpath = coverUrl;
                    a.tracks.append(t);
                }
                likedAlbums->append(a);
            }

            for (const QJsonValue& trackVal : likedTracksArray) {
                QJsonObject trackObj = trackVal.toObject();
                track t;
                t.album_id = trackObj["album_id"].toInt();
                QString authorsStr = trackObj["authors"].toString();
                QStringList authorList = authorsStr.split(",", Qt::SkipEmptyParts);
                for (const QString &author : authorList) {
                    t.authors.append(author.trimmed());  // убираем пробелы вокруг
                }
                QString usertagsStr = trackObj["author_usertag"].toString();
                QStringList userList = usertagsStr.split(",", Qt::SkipEmptyParts);
                for (const QString &author : userList) {
                    qDebug()<<"author";
                    qDebug()<<author;

                    t.authorUsertags.append(author.trimmed());
                }
                t.duration_s = trackObj["duration_seconds"].toInt();
                t.id = trackObj["id"].toInt();
                t.name = trackObj["title"].toString();
                t.upload_date = trackObj["upload_date"].toString();
                QString coverUrl = QString("http://84.237.53.143:8083/track/%1").arg(t.id);
                t.coverpath = coverUrl;
                likedTracks->tracks.append(t);
            }

            for (const QJsonValue& trackVal : loadedTracksArray) {
                QJsonObject trackObj = trackVal.toObject();
                track t;
                t.album_id = trackObj["album_id"].toInt();
                QString authorsStr = trackObj["authors"].toString();
                QStringList authorList = authorsStr.split(",", Qt::SkipEmptyParts);
                for (const QString &author : authorList) {
                    t.authors.append(author.trimmed());  // убираем пробелы вокруг
                }
                QString usertagsStr = trackObj["author_usertag"].toString();
                QStringList userList = usertagsStr.split(",", Qt::SkipEmptyParts);
                for (const QString &author : userList) {
                    qDebug()<<"author";
                    qDebug()<<author;

                    t.authorUsertags.append(author.trimmed());
                }
                t.duration_s = trackObj["duration_seconds"].toInt();
                t.id = trackObj["id"].toInt();
                t.name = trackObj["title"].toString();
                t.upload_date = trackObj["upload_date"].toString();
                QString coverUrl = QString("http://84.237.53.143:8083/track/%1").arg(t.id);
                t.coverpath = coverUrl;
                loadedTracks->tracks.append(t);
            }

            onUserButtonClicked(viewUserInfo, likedAlbums, loadedTracks, likedTracks);

            typeOfQuery="None";
        }

    }
}



void ProfileTab::onFollowersButtonClicked(){
    QJsonObject payload;
    payload["endpoint"] = "/catalog";
    payload["action"] = "user_action";
    QJsonObject subObj;

    subObj.insert("usertag", currentAuthorOfObjects.usertag);

    payload["params"] = subObj;
    payload["subaction"] = "followers";

    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    qDebug()<<message;
    //current_query = "register";
    typeOfQuery="followersTracksButton";
    webSocket->sendMessage(message);
    int currentViewerIdx = currentViewerIndex;
    int totalViewerIdx = currentViewers.size();
    qDebug()<<currentViewerIdx;
    qDebug()<<totalViewerIdx;
    for (int i = totalViewerIdx - 1; i > currentViewerIdx; --i) {
        qDebug()<<i;
        qDebug()<<currentViewers[i];

        currentViewers.removeAt(i);
    }
    qDebug()<<currentViewerIndex;
    qDebug()<<currentViewers.size();
    currentViewers.push_back(currentAuthorOfObjects.usertag);
    currentViewerIndex++;
};

void ProfileTab::onFollowingButtonClicked(){
    QJsonObject payload;
    payload["endpoint"] = "/catalog";
    payload["action"] = "user_action";
    QJsonObject subObj;

    subObj.insert("usertag", currentAuthorOfObjects.usertag);

    payload["params"] = subObj;
    payload["subaction"] = "following";

    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    qDebug()<<message;
    //current_query = "register";
    typeOfQuery="followingsTracksButton";
    webSocket->sendMessage(message);
    int currentViewerIdx = currentViewerIndex;
    int totalViewerIdx = currentViewers.size();
    qDebug()<<currentViewerIdx;
    qDebug()<<totalViewerIdx;
    for (int i = totalViewerIdx - 1; i > currentViewerIdx; --i) {
        currentViewers.removeAt(i);

    }
    qDebug()<<currentViewerIndex;
    qDebug()<<currentViewers.size();
    currentViewers.push_back(currentAuthorOfObjects.usertag);
    currentViewerIndex++;

};

void ProfileTab::onTracksLoadedButtonClicked(){
    QJsonObject payload;
    payload["endpoint"] = "/catalog";
    payload["action"] = "user_action";
    QJsonObject subObj;

    subObj.insert("usertag", currentAuthorOfObjects.usertag);

    payload["params"] = subObj;
    payload["subaction"] = "my_loaded_tracks";

    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    qDebug()<<message;
    //current_query = "register";
    typeOfQuery = "loadedTracksButton";
    webSocket->sendMessage(message);
    int currentViewerIdx = currentViewerIndex;
    int totalViewerIdx = currentViewers.size();

    for (int i = totalViewerIdx - 1; i > currentViewerIdx; --i) {
        currentViewers.removeAt(i);
    }
    currentViewers.push_back(currentAuthorOfObjects.usertag);
    currentViewerIndex++;

};
void ProfileTab::onTracksAddedButtonClicked(){
    QJsonObject payload;
    payload["endpoint"] = "/catalog";
    payload["action"] = "user_action";
    QJsonObject subObj;
    subObj.insert("usertag", currentAuthorOfObjects.usertag);

    payload["params"] = subObj;
    payload["subaction"] = "my_tracks";

    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    qDebug()<<message;
    //current_query = "register";
    qDebug()<<"clicked";
    typeOfQuery = "likedTracksButton";
    webSocket->sendMessage(message);
    int currentViewerIdx = currentViewerIndex;
    int totalViewerIdx = currentViewers.size();

    for (int i = totalViewerIdx - 1; i > currentViewerIdx; --i) {
        currentViewers.removeAt(i);
    }
    currentViewers.push_back(currentAuthorOfObjects.usertag);
    currentViewerIndex++;

};

void ProfileTab::onAuthorButtonClicked(QString *authorUsertag){
    QJsonObject payload;
    payload["endpoint"] = "/catalog";
    payload["action"] = "profile";
    payload["usertag1"] = currentAuthorOfObjects.usertag;
    payload["usertag2"] = *authorUsertag;
    payload["flag"] = 0;

    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    qDebug()<<message;
    typeOfQuery="userButton";
    webSocket->sendMessage(message);

    int currentViewerIdx = currentViewerIndex;
    int totalViewerIdx = currentViewers.size();

    for (int i = totalViewerIdx - 1; i > currentViewerIdx; --i) {
        currentViewers.removeAt(i);

    }
    currentAuthorOfObjects.usertag = *authorUsertag;
    currentViewers.push_back(currentAuthorOfObjects.usertag);
    currentViewerIndex++;
}

void ProfileTab::onAlbumByTrack(track *trackData){
    if(trackData->album_id==0){
        album a;
        a.id = 0;
        a.name = trackData->name;
        a.coverpath = QString("http://84.237.53.143:8083/track/%1").arg(trackData->id);  // пример, берем первую liked_track как обложку
        qDebug()<<"ssd";
        a.authorUsername = trackData->authors[0];
        qDebug()<<"ssd";

        a.authorUsertag = trackData->authorUsertags[0];
        qDebug()<<"1ssd";

        a.createDate = trackData->upload_date;
        a.description = trackData->description;
        a.track_count = 1;
        a.total_duration = trackData->duration_s;
        a.tracks.push_back(*trackData);
        QVector<int> likedTracks;
        likedTracks.push_back(trackData->id);
        onAlbumClicked(a, likedTracks);
        int currentViewerIdx = currentViewerIndex;
        int totalViewerIdx = currentViewers.size();

        for (int i = totalViewerIdx - 1; i > currentViewerIdx; --i) {
            currentViewers.removeAt(i);

        }

        currentViewers.push_back(currentAuthorOfObjects.usertag);
        currentViewerIndex++;

    }else{
        QJsonObject payload;
        payload["endpoint"] = "/catalog";
        payload["action"] = "user_action";
        QJsonObject subObj;
        subObj.insert("usertag", *mainUsertag);
        subObj.insert("album_id", trackData->album_id);

        payload["params"] = subObj;
        payload["subaction"] = "get_album";

        QJsonDocument doc(payload);
        QString message = QString::fromUtf8(doc.toJson());
        qDebug()<<message;
        //current_query = "register";
        qDebug()<<"clicked";
        typeOfQuery = "albumButton";

        webSocket->sendMessage(message);
        int currentViewerIdx = currentViewerIndex;
        int totalViewerIdx = currentViewers.size();

        for (int i = totalViewerIdx - 1; i > currentViewerIdx; --i) {
            currentViewers.removeAt(i);

        }

        currentViewers.push_back(currentAuthorOfObjects.usertag);
        currentViewerIndex++;

    }
}

void ProfileTab::onAlbumById(album albumData){
    if(albumData.id==0){
        QVector<int> likedTracks;
        onAlbumClicked(albumData, likedTracks);
        int currentViewerIdx = currentViewerIndex;
        int totalViewerIdx = currentViewers.size();

        for (int i = totalViewerIdx - 1; i > currentViewerIdx; --i) {
            currentViewers.removeAt(i);

        }

        currentViewers.push_back(currentAuthorOfObjects.usertag);
        currentViewerIndex++;

    }else{
        QJsonObject payload;
        payload["endpoint"] = "/catalog";
        payload["action"] = "user_action";
        QJsonObject subObj;
        subObj.insert("usertag", *mainUsertag);
        subObj.insert("album_id", albumData.id);

        payload["params"] = subObj;
        payload["subaction"] = "get_album";

        QJsonDocument doc(payload);
        QString message = QString::fromUtf8(doc.toJson());
        qDebug()<<message;
        qDebug()<<"clicked";
        typeOfQuery = "albumButton";

        webSocket->sendMessage(message);
        int currentViewerIdx = currentViewerIndex;
        int totalViewerIdx = currentViewers.size();

        for (int i = totalViewerIdx - 1; i > currentViewerIdx; --i) {
            currentViewers.removeAt(i);

        }

        currentViewers.push_back(currentAuthorOfObjects.usertag);
        currentViewerIndex++;

    }
}

void ProfileTab::onUsersListClicked(QVector<UserInfo> users){
    int currentWidgetIndex = getCurrentIndex();
    int totalIndex = getTotalIndex();

    // Удаляем все виджеты после текущего
    for (int i = totalIndex - 1; i > currentWidgetIndex; --i) {
        QWidget* widgetToRemove = innerStacked->widget(i);
        if (widgetToRemove) {
            innerStacked->removeWidget(widgetToRemove);
            delete widgetToRemove;
        }
    }

    auto *userListWidget = new UsersListWidget(users);
    connect(userListWidget, &UsersListWidget::userButtonClicked, this, &ProfileTab::onAuthorButtonClicked);

    innerStacked->addWidget(userListWidget);
    innerStacked->setCurrentWidget(userListWidget);
    scrollArea->verticalScrollBar()->setValue(0);

    //albumwidgetTab->setStyleSheet("border: 1px solid red");

    emit onAlbomClickedSignal();
}

void ProfileTab::onUserButtonClicked(UserInfo *userData, QVector <album> *albums_vector, album *loadedTracks, album *likedTracks){
    int currentWidgetIndex = getCurrentIndex();
    int totalIndex = getTotalIndex();
    // Удаляем все виджеты после текущего
    for (int i = totalIndex - 1; i > currentWidgetIndex; --i) {
        QWidget* widgetToRemove = innerStacked->widget(i);
        if (widgetToRemove) {
            innerStacked->removeWidget(widgetToRemove);
            delete widgetToRemove;
        }
    }

    qDebug()<<currentViewers;


    ProfileWidget *userProfileWidget = new ProfileWidget();
    //profileLayout->addWidget(userProfileWidget);
    userProfileWidget->updateAlbums(*userData, *albums_vector, *likedTracks, *loadedTracks);
    userProfileWidget->resizeProfile(widthForResize);
    connect(userProfileWidget, &ProfileWidget::followersButtonClicked, this, &ProfileTab::onFollowersButtonClicked);
    connect(userProfileWidget, &ProfileWidget::followingButtonClicked, this, &ProfileTab::onFollowingButtonClicked);
    connect(userProfileWidget, &ProfileWidget::tracksLoadedButtonClicked, this, &ProfileTab::onTracksLoadedButtonClicked);
    connect(userProfileWidget, &ProfileWidget::tracksAddedButtonClicked, this, &ProfileTab::onTracksAddedButtonClicked);

    connect(userProfileWidget, &ProfileWidget::albumClicked, this, &ProfileTab::onAlbumById);
    connect(userProfileWidget, &ProfileWidget::albumListClicked, this, &ProfileTab::onAlbumListClicked);
    connect(userProfileWidget, &ProfileWidget::trackDoubleClicked, this, &ProfileTab::onTrackdoubleClicked);

    connect(userProfileWidget, &ProfileWidget::trackNameButtonClicked, this, &ProfileTab::onAlbumByTrack);

    connect(userProfileWidget, &ProfileWidget::authorButtonClicked, this, &ProfileTab::onAuthorButtonClicked);
    innerStacked->addWidget(userProfileWidget);
    innerStacked->setCurrentWidget(userProfileWidget);
    scrollArea->verticalScrollBar()->setValue(0);

    //albumwidgetTab->setStyleSheet("border: 1px solid red");

    emit onAlbomClickedSignal();

}

void ProfileTab::onAlbumClicked(album albumData, QVector<int> likedTracks){

    int currentWidgetIndex = getCurrentIndex();
    int totalIndex = getTotalIndex();

    // Удаляем все виджеты после текущего
    for (int i = totalIndex - 1; i > currentWidgetIndex; --i) {
        QWidget* widgetToRemove = innerStacked->widget(i);
        if (widgetToRemove) {
            innerStacked->removeWidget(widgetToRemove);
            delete widgetToRemove;
        }
    }


    AlbumWidget *albumwidgetTab = new AlbumWidget(albumData, likedTracks);

    connect(albumwidgetTab, &AlbumWidget::trackButtonClicked, this, &ProfileTab::onTrackdoubleClicked);
    connect(albumwidgetTab, &AlbumWidget::trackNameButtonClicked, this, &ProfileTab::onAlbumByTrack);
    connect(albumwidgetTab, &AlbumWidget::authorButtonClickedByTrackId, this, &ProfileTab::onAuthorButtonClicked);
    connect(albumwidgetTab, &AlbumWidget::authorButtonClicked, this, &ProfileTab::onAuthorButtonClicked);
    connect(albumwidgetTab, &AlbumWidget::albumTrackLiked, this, &ProfileTab::on_TrackLikeButton);

    innerStacked->addWidget(albumwidgetTab);
    innerStacked->setCurrentWidget(albumwidgetTab);
    scrollArea->verticalScrollBar()->setValue(0);

    //albumwidgetTab->setStyleSheet("border: 1px solid red");

    emit onAlbomClickedSignal();
}

void ProfileTab::on_TrackLikeButton(track *trackData){
    QJsonObject payload;
    payload["endpoint"] = "/catalog";
    payload["action"] = "user_action";
    QJsonObject subObj;
    subObj.insert("usertag", *mainUsertag);
    subObj.insert("album_id", trackData->id);

    payload["params"] = subObj;
    payload["subaction"] = "like_track";

    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    qDebug()<<message;
    qDebug()<<"clicked";
    typeOfQuery = "albumButton";

    webSocket->sendMessage(message);
}

void ProfileTab::onAlbumListClicked(QVector<album> albumList){

    int currentWidgetIndex = getCurrentIndex();
    int totalIndex = getTotalIndex();

    // Удаляем все виджеты после текущего
    for (int i = totalIndex - 1; i > currentWidgetIndex; --i) {
        QWidget* widgetToRemove = innerStacked->widget(i);
        if (widgetToRemove) {
            innerStacked->removeWidget(widgetToRemove);
            delete widgetToRemove;
        }
    }

    int currentViewerIdx = currentViewerIndex;
    int totalViewerIdx = currentViewers.size();

    for (int i = totalViewerIdx - 1; i > currentViewerIdx; --i) {
        currentViewers.removeAt(i);

    }

    currentViewers.push_back(currentAuthorOfObjects.usertag);
    currentViewerIndex++;

    //AlbumWidget *albumwidget = new AlbumWidget(albumData);
    //connect(albumwidget, &AlbumWidget::trackButtonClicked, this, &ProfileWidget::onTrackdoubleClicked);
    AlbomListWidget *albumListWidget = new AlbomListWidget(albumList);
    connect(albumListWidget, &AlbomListWidget::albumButtonClicked, this, &ProfileTab::onAlbumById);
    connect(albumListWidget, &AlbomListWidget::authorButtonClicked, this, &ProfileTab::onAuthorButtonClicked);


    // // Добавляем тестовые кнопки
    // for (int i = 0; i < 20; ++i) {
    //     albumListWidget->addButton(QString("Button %1я").arg(1 + 1));
    // }
    innerStacked->addWidget(albumListWidget);
    innerStacked->setCurrentWidget(albumListWidget);
    scrollArea->verticalScrollBar()->setValue(0);

    //albumListWidget->setStyleSheet("border: 1px solid red");

    emit onAlbomClickedSignal();
}
// остановились на том что addbutton не работает класса albomlist нужно продебажить лишь а так все заебись

void ProfileTab::onTrackdoubleClicked(track *trackData){
    emit onTrackDoubleClickedignal(trackData);
}


int ProfileTab::get_widgetWidth(){
    return widgetWidth;
}


