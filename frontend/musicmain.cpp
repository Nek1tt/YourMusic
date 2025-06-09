#include <QMainWindow>
#include <QPushButton>
#include <QDebug>  // <--- добавь это
#include <QScreen> //for Qscreen *screen = QApplication::primaryScreen();
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QTabBar>
#include <QWebSocket>
#include <QDir>
#include <QApplication>
#include <QStackedWidget>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


#include "musicmain.h"
//#include "./ui_musicmain.h"


MusicMain::MusicMain(QString *usertag, WebSocketClient *webSocket, WebSocketClient *webSocketStas, QWidget *parent)//класс для окна
    : QMainWindow(parent),
    webSocket(webSocket),
    mainUsertag(*usertag),
    webSocketStas(webSocketStas)
{
    mainUserInfo = new UserInfo;
    QScreen *screen = QApplication::primaryScreen();// сохраняем экран
    QRect screenGeometry = screen->availableGeometry();//извлекаем параметры экрана
    // // Получаем размер окна
    int windowWidth = this->width();
    int windowHeight = this->height();
    mainWidget = new QWidget();
    mainSplitter = new QSplitter(Qt::Horizontal, mainWidget);
    connect(mainSplitter, &QSplitter::splitterMoved, this, &MusicMain::onSplitterMoved);

    mainLayout = new QHBoxLayout(mainWidget);

    leftBarWidget = new QWidget();
    //leftBarWidget->setFixedWidth(1);
    leftLayout = new QVBoxLayout(leftBarWidget);

    QWidget *navButtons = new QWidget();
    navButtons->setFixedHeight(100);
    QHBoxLayout *navLayout = new QHBoxLayout(navButtons);
    backButton = new QPushButton(" < ");
    forwardButton = new QPushButton(" > ");

    connect(backButton, &QPushButton::clicked, this, &MusicMain::on_backButton_clicked);
    connect(forwardButton, &QPushButton::clicked, this, &MusicMain::on_forwardButton_clicked);

    activeStyle = R"(
    QPushButton {
        background-color: #A0A0A0;
        border: none;
        border-radius: 15px;
        min-width: 30px;
        min-height: 30px;
        font-weight: bold;
        color: white;
    }
    QPushButton:hover {
        border: 1px solid white;
    }
)";

    inactiveStyle = R"(
    QPushButton {
        background-color: #555555; /* Более тёмный серый для неактивной */
        border: none;
        border-radius: 15px;
        min-width: 30px;
        min-height: 30px;
        font-weight: bold;
        color: #AAAAAA;
    }
)";
    backButton->setStyleSheet(inactiveStyle);
    forwardButton->setStyleSheet(inactiveStyle);
    navLayout->addWidget(backButton);
    navLayout->addWidget(forwardButton);


    homeTabButton = new QPushButton("Home");
    createTabButton = new QPushButton("Create");
    profileTabButton = new QPushButton("Profile");

    connect(homeTabButton, &QPushButton::clicked, this, &MusicMain::on_homeTab_clicked);
    connect(createTabButton, &QPushButton::clicked, this, &MusicMain::on_createTab_clicked);
    connect(profileTabButton, &QPushButton::clicked, this, &MusicMain::on_profileTab_clicked);

    leftLayout->addWidget(navButtons, 0, Qt::AlignTop);
    leftLayout->addWidget(homeTabButton);
    leftLayout->addWidget(createTabButton);
    leftLayout->addWidget(profileTabButton);

    mainSplitter->addWidget(leftBarWidget);

    tabwidget = new QTabWidget(this);
    //tabwidget->setFixedWidth(1);
    tabwidget->setStyleSheet("QTabWidget::pane { margin: 0px; padding: 0px; }");
    //Create = new QWidget();
    //Home = new QWidget();
    //Profile = new QWidget();

    tabwidget->tabBar()->hide();
    mainSplitter->addWidget(tabwidget);

    this->setCentralWidget(mainWidget);
    homeTab = new HomeTab(&mainUsertag, *mainUserInfo, webSocket, this);
    // // Создаем вкладку create
    createTab = new CreateTab(&mainUsertag, webSocket, this);
    profileTab = new ProfileTab(&mainUsertag, *mainUserInfo, webSocket, this);

    connect(profileTab, &ProfileTab::onAlbomClickedSignal, this, &MusicMain::on_albumButton_clicked);
    connect(profileTab, &ProfileTab::onTrackDoubleClickedignal, this, &MusicMain::on_TrackButton_clicked); 
    connect(homeTab, &HomeTab::onAlbomClickedSignal, this, &MusicMain::on_albumButton_clicked);
    connect(homeTab, &HomeTab::onTrackDoubleClickedignal, this, &MusicMain::on_TrackButton_clicked);
    profileTab->setContentsMargins(0, 0, 0, 0);

    tabwidget->addTab(homeTab, "Home");
    tabwidget->addTab(createTab, "Create");
    tabwidget->addTab(profileTab, "Profile");
    // Пример загрузки треков
    QVector<track> tracks;
    track *currentTrack;
    tracks = readFromJson("resources/jsons/mytracks.json");

    if (!tracks.empty()) {
        currentTrack = &tracks[0];

    }

    rightbarwidget = new RightBarWidget(webSocketStas, windowWidth, windowHeight, this, currentTrack);
    rightbarwidget->setNewCurrentTrack(*currentTrack);
    rightbarwidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    //rightbarwidget->resize(400, this->height());
    mainSplitter->addWidget(rightbarwidget);
    mainLayout->addWidget(mainSplitter);
    mainWidget->setLayout(mainLayout);

    // // Вычисляем позицию для окна, чтобы правый верхний угол сопоставлялся с правым верхним углом экрана
    int xPos = screenGeometry.right() - windowWidth;
    int yPos = screenGeometry.top();
    tabwidget->setTabVisible(0,0);// делаем видимым вкладку
    tabwidget->setCurrentIndex(0);// ставим текущим вкладку 0
    homeTabButton->setStyleSheet("color : black");
    createTabButton->setStyleSheet("color : white");
    profileTabButton->setStyleSheet("color : white");
    currentTab = homeTabButton;
    currentTab->setEnabled(false);


    mainTabButtons = { homeTabButton, createTabButton, profileTabButton}; //main tub buttons on screen


//this->resize(screenGeometry.width(), screenGeometry.height()); //seting the window size
    tabwidget->resize(50,this->height());
    setInitialSize(this->width());
    mainSplitter->setSizes({200, this->width()-200-360, 360});

    connect(webSocket, &WebSocketClient::messageReceived,
            this, &MusicMain::onTextMessageReceived);
}

void MusicMain::onTextMessageReceived(const QString &type, const QJsonObject &data){
    if(type == "catalog_response"){
        if(typeOfQuery == "homeButton"){
            qDebug()<<data;
            randomAlbums = new QVector<album>;
            newAlbums = new QVector<album>;
            recAlbums = new QVector<album>;
                // Получаем массив random_tracks

            QJsonArray randomTracksArray = data["random_tracks"].toArray();
            QJsonArray newAlbumsArray = data["new_albums"].toArray();
            QJsonArray newTracksArray = data["new_tracks"].toArray();
            QJsonArray recAlbumsArray = data["recomendations_albums"].toArray();
            QJsonArray recTracksArray = data["recomendations_tracks"].toArray();

            for (const QJsonValue& val : randomTracksArray) {
                QJsonObject trackObj = val.toObject();

                track t;
                t.id = trackObj["id"].toInt();
                t.album_id = trackObj["album_id"].toInt();
                t.name = trackObj["title"].toString();
                t.duration_s = trackObj["duration_seconds"].toInt();
                QString coverUrl = QString("http://84.237.53.143:8083/track/%1").arg(t.id);
                t.coverpath = coverUrl;
                t.description = trackObj["description"].toString();
                QString authorsStr = trackObj["authors"].toString();
                QStringList authorList = authorsStr.split(",", Qt::SkipEmptyParts);
                for (const QString &author : authorList) {
                    t.authors.append(author.trimmed());  // убираем пробелы вокруг
                }
                QString usertagsStr = trackObj["author_usertag"].toString();
                QStringList userList = usertagsStr.split(",", Qt::SkipEmptyParts);
                for (const QString &author : userList) {
                    t.authorUsertags.append(author.trimmed());
                }

                album a;
                a.id = 0;                      // можно брать album_id == 0, либо генерировать уникальный id
                a.name = t.name;                        // в данном случае можно просто взять название трека
                a.coverpath = t.coverpath;
                a.description = t.description;
                a.tracks.push_back(t);
                if (!t.authors.isEmpty()) {
                    a.authorUsername = t.authors[0];
                } else {
                    a.authorUsername = "Unknown";  // или "" — на твой вкус
                }
                if (!t.authorUsertags.isEmpty()) {
                    a.authorUsertag = t.authorUsertags[0];
                } else {
                    a.authorUsertag = "stas";  // или "" — на твой вкус
                }

                a.total_duration = t.duration_s;
                a.authorUsertag = 0;                       // если author_id нет в JSON — задаём -1 или 0
                a.track_count = 1;

                randomAlbums->append(a);
            }

            for (const QJsonValue& val : newAlbumsArray) {
                QJsonObject albumObj = val.toObject();
                album a;
                a.authorUsername = albumObj["authors"].toString();
                a.authorUsertag = albumObj["author_usertag"].toString();
                a.createDate = albumObj["created_date"].toString();
                a.description = albumObj["description"].toString();
                a.id = albumObj["id"].toInt();
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
                newAlbums->append(a);
            }

            // Если в альбоме есть массив tracks — парсим его
            for (const QJsonValue& val : newTracksArray) {
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
                    t.authors.append(author.trimmed());  // убираем пробелы вокруг
                }
                QString usertagsStr = trackObj["author_usertag"].toString();
                QStringList userList = usertagsStr.split(",", Qt::SkipEmptyParts);
                for (const QString &author : userList) {
                    qDebug()<<"author";
                    qDebug()<<author;

                    t.authorUsertags.append(author.trimmed());
                }

                album a;
                a.id = 0;                      // можно брать album_id == 0, либо генерировать уникальный id
                a.name = t.name;                        // в данном случае можно просто взять название трека
                a.coverpath = t.coverpath;
                a.authorUsername = t.authors[0];
                a.authorUsertag = 0;                       // если author_id нет в JSON — задаём -1 или 0
                a.track_count = 1;

                newAlbums->append(a);
            }

            for (const QJsonValue& val : recAlbumsArray) {
                QJsonObject albumObj = val.toObject();
                album a;
                a.authorUsername = albumObj["authors"].toString();
                a.authorUsertag = albumObj["author_usertag"].toString();
                a.createDate = albumObj["created_date"].toString();
                a.description = albumObj["description"].toString();
                a.id = albumObj["id"].toInt();
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
                recAlbums->append(a);
            }

            // Если в альбоме есть массив tracks — парсим его
            for (const QJsonValue& val : recTracksArray) {
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
                    t.authors.append(author.trimmed());  // убираем пробелы вокруг
                }
                QString usertagsStr = trackObj["author_usertag"].toString();
                QStringList userList = usertagsStr.split(",", Qt::SkipEmptyParts);
                for (const QString &author : userList) {
                    qDebug()<<"author";
                    qDebug()<<author;

                    t.authorUsertags.append(author.trimmed());
                }

                album a;
                a.id = 0;                      // можно брать album_id == 0, либо генерировать уникальный id
                a.name = t.name;                        // в данном случае можно просто взять название трека
                a.coverpath = t.coverpath;
                a.authorUsername = t.authors[0];
                a.authorUsertag = 0;                       // если author_id нет в JSON — задаём -1 или 0
                a.track_count = 1;

                recAlbums->append(a);
            }
            qDebug()<<'end';
            mainUserInfo->usertag = mainUsertag;
            homeTab->homeButtonClicked(*mainUserInfo, newAlbums, recAlbums, randomAlbums);
            typeOfQuery="None";

        }
        if(typeOfQuery=="profileButton"){
            qDebug()<<data;
            likedAlbums = new QVector<album>;
            loadedAlbums = new QVector<album>;
            mainUserInfo = new UserInfo;
            viewUserInfo = new UserInfo;


            QJsonArray likedAlbumsArray = data["liked_albums"].toArray();
            QJsonArray likedTracksArray = data["liked_tracks"].toArray();
            QJsonArray loadedAlbumsArray = data["loaded_albums"].toArray();
            QJsonArray loadedTracksArray = data["loaded_tracks"].toArray();
            QJsonObject user = data.value("user_info").toObject();
            mainUserInfo->username = user["username"].toString();
            mainUserInfo->usertag = user["usertag"].toString();
            QString coverUrl = QString("http://84.237.53.143:8083/profile/%1").arg(mainUserInfo->usertag);
            mainUserInfo->avatarPath = coverUrl;
            mainUserInfo->followersnum = user["followersnum"].toInt();
            mainUserInfo->followingnum = user["followingnum"].toInt();
            mainUserInfo->tracksAddednum = user["tracksAddedNum"].toInt();
            mainUserInfo->tracksLoadednum = user["tracksLoadedNum"].toInt();

            likedTracks = new album;
            likedTracks->authorUsername = mainUserInfo->username;
            likedTracks->authorUsertag = mainUserInfo->usertag;
            likedTracks->name = "Liked Tracks";
            likedTracks->total_duration = 0;
            likedTracks->track_count = 0;
            coverUrl = QString("http://84.237.53.143:8083/track/%1").arg(4007);
            likedTracks->coverpath = coverUrl;

            loadedTracks = new album;
            loadedTracks = new album;
            loadedTracks->authorUsername = mainUserInfo->username;
            loadedTracks->authorUsertag = mainUserInfo->usertag;
            loadedTracks->name = "Loaded Tracks";
            loadedTracks->total_duration = 0;
            loadedTracks->track_count = 0;
            coverUrl = QString("http://84.237.53.143:8083/track/%1").arg(4006);
            loadedTracks->coverpath = coverUrl;



            for (const QJsonValue& val : likedAlbumsArray) {
                QJsonObject albumObj = val.toObject();
                album a;
                a.authorUsername = albumObj["authors"].toString();
                a.authorUsertag = albumObj["author_usertag"].toString();
                qDebug()<<a.authorUsername;
                qDebug()<<a.authorUsertag;
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
                likedTracks->total_duration += t.duration_s;
                likedTracks->track_count++;
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
                loadedTracks->total_duration += t.duration_s;
                loadedTracks->track_count++;
            }


            profileTab->profileButtonClicked(*mainUserInfo, *likedAlbums, *likedTracks, *loadedTracks);
            QList<int> sizes = mainSplitter->sizes();
            //tabwidget->resize(20,this->height());
            profileTab->resizeProfile(sizes[1]);
            typeOfQuery="None";

        }
    }
}


MusicMain::~MusicMain()
{
    // delete ui;
}

void MusicMain::toggle_buttons(QPushButton* pushedButton){ // changes the button from which we switched to a new tab to white. and
    for (int i=0; i<3; i++){
        if(mainTabButtons[i]==pushedButton){
            tabwidget->setCurrentIndex(i);
        }
    }
    currentTab->setStyleSheet("color : white");
    pushedButton->setEnabled(false);
    pushedButton->setStyleSheet("color : black");
    pushedButton->setFlat(true);
    //
    currentTab->setEnabled(true);
    currentTab = pushedButton;
}


void MusicMain::on_homeTab_clicked()
{

    QJsonObject payload;
    payload["endpoint"] = "/catalog";
    payload["action"] = "home";
    payload["usertag"] = mainUsertag;

    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    qDebug()<<message;
    webSocket->sendMessage(message);
    typeOfQuery = "homeButton";

    currentTabWidget = homeTab;
    toggle_buttons(homeTabButton);

    int current = currentTabWidget->getCurrentIndex();

    if (current == 0) {
        backButton->setStyleSheet(inactiveStyle);
    }else{
        backButton->setStyleSheet(activeStyle);
    }

    int total = currentTabWidget->getTotalIndex();

    if (current == total-1) {
        forwardButton->setStyleSheet(inactiveStyle);
    }else{
        forwardButton->setStyleSheet(activeStyle);
    }
}


void MusicMain::on_createTab_clicked()
{
    //currentTabWidget = Create;
    int current = currentTabWidget->getCurrentIndex();

    if (current == 0) {
        backButton->setStyleSheet(inactiveStyle);
    }else{
        backButton->setStyleSheet(activeStyle);
    }

    int total = currentTabWidget->getTotalIndex();

    qDebug()<<total;
    if (current == total-1) {
        forwardButton->setStyleSheet(inactiveStyle);
    }else{
        forwardButton->setStyleSheet(activeStyle);
    }
    toggle_buttons(createTabButton);
}

void MusicMain::on_profileTab_clicked()
{
    QJsonObject payload;
    payload["endpoint"] = "/catalog";
    payload["action"] = "profile";
    payload["usertag1"] = mainUsertag;
    payload["flag"] = 0;

    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    qDebug()<<message;
    webSocket->sendMessage(message);
    typeOfQuery = "profileButton";

    toggle_buttons(profileTabButton);
    currentTabWidget = profileTab;

    int current = currentTabWidget->getCurrentIndex();
    if (current == 0) {
        backButton->setStyleSheet(inactiveStyle);
    }else{
        backButton->setStyleSheet(activeStyle);
    }

    int total = currentTabWidget->getTotalIndex();

    if (current == total-1) {
        forwardButton->setStyleSheet(inactiveStyle);
    }else{
        forwardButton->setStyleSheet(activeStyle);
    }

}


void MusicMain::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event); // Вызов базового метода
    int rightBarWidgetWidth = rightbarwidget->get_barWidth() + 10;
    QList<int> sizes = mainSplitter->sizes();
    //tabwidget->resize(20,this->height());
    mainSplitter->setSizes({100, this->width()-100-rightBarWidgetWidth, rightBarWidgetWidth}); //mainsplitter может изменяться только так - никаких resize для виджетов mainsplitter
    profileTab->resizeProfile(sizes[1]);

    //rightbarwidget->resizeBarWidget(this->width());
}

void MusicMain::onSplitterMoved(int pos, int index) {
    QList<int> sizes = mainSplitter->sizes();
    rightbarwidget->resizeBarWidget(sizes[2]);
    profileTab->resizeProfile(sizes[1]);
}


void MusicMain::setInitialSize(int width){
    profileTab->resizeProfile(width);
}

void MusicMain::on_backButton_clicked(){

    int current = currentTabWidget->getCurrentIndex();

    if (current - 1 >= 0) {
        currentTabWidget->setCurrentIndex(current - 1);
        backButton->setStyleSheet(activeStyle);
    }
    current = currentTabWidget->getCurrentIndex();
    if (current == 0) {
        backButton->setStyleSheet(inactiveStyle);
    }
    int total = currentTabWidget->getTotalIndex();

    if (current != total-1) {
        forwardButton->setStyleSheet(activeStyle);
    }
}

void MusicMain::on_forwardButton_clicked(){
    int current = currentTabWidget->getCurrentIndex();

    int total = currentTabWidget->getTotalIndex();

    if (current + 1 < total) {
        currentTabWidget->setCurrentIndex(current + 1);
    }

    current = currentTabWidget->getCurrentIndex();

    total = currentTabWidget->getTotalIndex();
    if (current == total-1) {
        forwardButton->setStyleSheet(inactiveStyle);
    }

    if (current != 0) {
        backButton->setStyleSheet(activeStyle);
    }
}

void MusicMain::on_albumButton_clicked(){
    backButton->setStyleSheet(activeStyle);
    int current = profileTab->getCurrentIndex();
    int total = profileTab->getTotalIndex();
    if (current == total-1) {
        forwardButton->setStyleSheet(inactiveStyle);
    }
}


void MusicMain::on_TrackButton_clicked(track *trackData){
    rightbarwidget->setNewCurrentTrack(*trackData);
}


// void MusicMain::on_toserver_clicked()
// {
//     if (webSocket->state() == QAbstractSocket::ConnectedState) {
//         // Отправляем сообщение на сервер
//         webSocket->sendTextMessage(R"({"command": "play_track", "track_id": 1})");
//     } else {
//         QMessageBox::warning(this, "Ошибка", "Нет подключения к серверу");
//     }
// }

// void MusicMain::onConnected() {
//     QMessageBox::information(this, "Успех", "Подключение к серверу установлено");
// }

// void MusicMain::onDisconnected() {
//     QMessageBox::warning(this, "Ошибка", "Соединение с сервером разорвано");
// }

// void MusicMain::onTextMessageReceived(const QString &message) {
//     qDebug()<<message;
//     QMessageBox::information(this, "Ответ от сервера", message);

// }
