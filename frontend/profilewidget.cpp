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


#include "profilewidget.h"
#include "userProfileWidget.h"
#include "myalbumswidget.h"
#include "albumwidget.h"
#include "mytrackswidget.h"
#include "albomlistwidget.h"
#include "userslistwidget.h"

ProfileWidget::ProfileWidget(QVector <album> albumList, album likedTracks, album loadedTracks, QWidget *parent, QWidget *tab)
    : QWidget(parent),
    likedTracks(likedTracks),
    loadedTracks(loadedTracks),
    albumList(albumList),
    userProfile(new UserProfileWidget(this)),
    albumwidget(new MyAlbumsWidget(this)),
    likedtrackwidget(new MyTracksWidget(this)),
    loadedtrackwidget(new MyTracksWidget(this))
{
    // Основной стек, управляющий отображением
    innerStacked = new QStackedWidget();
    qDebug()<<"ssad";
    qDebug()<<albumList[1].author;
    qDebug()<<"3";
    // --- Основной скроллируемый виджет (только для главного экрана) ---
    mainWidget = new QWidget();
    QVBoxLayout *mainWidgetLayout = new QVBoxLayout(mainWidget);
    mainWidgetLayout->setContentsMargins(10, 10, 10, 10);

    // Заполнение профиля
    UserInfo usernurshat{"../resources/imgs/ava.png", "User Name", "@usertag", 100, 100, 100, 100};
    userProfile->setUserProfile(usernurshat);
    mainWidgetLayout->addWidget(userProfile, 0, Qt::AlignTop);

    // Соединения
    connect(albumwidget, &MyAlbumsWidget::albumButtonClicked, this, &ProfileWidget::onAlbumClicked);
    connect(likedtrackwidget, &MyTracksWidget::trackButtonClicked, this, &ProfileWidget::onTrackdoubleClicked);
    connect(loadedtrackwidget, &MyTracksWidget::trackButtonClicked, this, &ProfileWidget::onTrackdoubleClicked);
    connect(likedtrackwidget, &MyTracksWidget::mytracksButtonClicked, this, &ProfileWidget::onAlbumClicked);
    connect(loadedtrackwidget, &MyTracksWidget::mytracksButtonClicked, this, &ProfileWidget::onAlbumClicked);
    connect(albumwidget, &MyAlbumsWidget::myAlbumsButtonClicked, this, &ProfileWidget::onAlbumListClicked);

    connect(userProfile, &UserProfileWidget::followersButtonClicked, this, &ProfileWidget::onFollowersButtonClicked);
    connect(userProfile, &UserProfileWidget::followingButtonClicked, this, &ProfileWidget::onFollowingButtonClicked);
    connect(userProfile, &UserProfileWidget::tracksLoadedButtonClicked, this, &ProfileWidget::onTracksLoadedButtonClicked);
    connect(userProfile, &UserProfileWidget::tracksAddedButtonClicked, this, &ProfileWidget::onTracksAddedButtonClicked);

    // Добавление треков и альбомов
    likedtrackwidget->add_liked_tracks(likedTracks, "My tracks", 1);
    loadedtrackwidget->add_liked_tracks(loadedTracks, "My loaded tracks", 0);
    albumwidget->add_albums(albumList);

    mainWidgetLayout->addWidget(albumwidget);
    mainWidgetLayout->addWidget(likedtrackwidget);
    mainWidgetLayout->addWidget(loadedtrackwidget);

    // --- Обёртка для скролла главного виджета ---
    scrollWidget = new QWidget();
    QVBoxLayout *profileLayout = new QVBoxLayout(scrollWidget);
    profileLayout->setContentsMargins(0, 0, 0, 0);
    profileLayout->addWidget(mainWidget);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Добавляем scrollArea как главный экран в стек
    innerStacked->addWidget(scrollArea);
    //mainWidget->setStyleSheet("QWidget { border: 1px solid red; }");

    // Размещаем стек в основной вкладке
    QVBoxLayout *tabLayout = new QVBoxLayout(tab);
    tabLayout->setContentsMargins(0, 0, 0, 0);
    tabLayout->addWidget(innerStacked);
}


QStackedWidget *ProfileWidget::getInnerStacked(){
    return innerStacked;
}

void ProfileWidget::resizeProfile(int width){
    albumwidget->resizeAlbums(width);
    scrollWidget->setFixedWidth(width-25);
    likedtrackwidget->resize_tracks(width);
    loadedtrackwidget->resize_tracks(width);
    //userProfile->resize(width * 0.6, 220);
    //Обновляем размеры виджета при изменении размеров окна
    // if (userProfile && width>1340 && width<1500) {
    //     userProfile->resize(width * 0.6, 220);
    // } else if (userProfile && width<1340){
    //     userProfile->resize(1340 * 0.4, 220);
    // } else {
    //     userProfile->resize(1500 * 0.4, 220);
    // }
    //qDebug()<<width;
}


void ProfileWidget::button_profile_clicked(QVector <album> albums_vector){
    albumwidget->add_albums(albums_vector);
}

void ProfileWidget::onFollowersButtonClicked(){
    QVector<UserInfo> users = loadUsersFromJson("../resources/jsons/users.json");
    qDebug()<<users[0].usertag;
    onUsersListClicked(users);
};
void ProfileWidget::onFollowingButtonClicked(){
    QVector<UserInfo> users = loadUsersFromJson("../resources/jsons/users.json");
    qDebug()<<users[0].usertag;
    onUsersListClicked(users);
};
void ProfileWidget::onTracksLoadedButtonClicked(){
    onAlbumClicked(loadedTracks);
};
void ProfileWidget::onTracksAddedButtonClicked(){
    onAlbumClicked(likedTracks);
};

void ProfileWidget::onUsersListClicked(QVector<UserInfo> users){
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

    auto *albumwidgetTab = new UsersListWidget(users);
    //connect(albumwidgetTab, &AlbumWidget::trackButtonClicked, this, &ProfileWidget::onTrackdoubleClicked);

    innerStacked->addWidget(albumwidgetTab);
    innerStacked->setCurrentWidget(albumwidgetTab);
    scrollArea->verticalScrollBar()->setValue(0);

    //albumwidgetTab->setStyleSheet("border: 1px solid red");

    emit onAlbomClickedSignal();
}

void ProfileWidget::onAlbumClicked(album albumData){
    //qDebug() << "albumButton clicked: " << albumName;

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

    AlbumWidget *albumwidgetTab = new AlbumWidget(albumData);
    connect(albumwidgetTab, &AlbumWidget::trackButtonClicked, this, &ProfileWidget::onTrackdoubleClicked);

    innerStacked->addWidget(albumwidgetTab);
    innerStacked->setCurrentWidget(albumwidgetTab);
    scrollArea->verticalScrollBar()->setValue(0);

    //albumwidgetTab->setStyleSheet("border: 1px solid red");

    emit onAlbomClickedSignal();
}

void ProfileWidget::onAlbumListClicked(QVector<album> albumList){
    //qDebug() << "albumButton clicked: " << albumName;

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

    //AlbumWidget *albumwidget = new AlbumWidget(albumData);
    //connect(albumwidget, &AlbumWidget::trackButtonClicked, this, &ProfileWidget::onTrackdoubleClicked);
    AlbomListWidget *albumListWidget = new AlbomListWidget(albumList);
    connect(albumListWidget, &AlbomListWidget::albumButtonClicked, this, &ProfileWidget::onAlbumClicked);


    // // Добавляем тестовые кнопки
    // for (int i = 0; i < 20; ++i) {
    //     albumListWidget->addButton(QString("Button %1").arg(1 + 1));
    // }

    innerStacked->addWidget(albumListWidget);
    innerStacked->setCurrentWidget(albumListWidget);
    scrollArea->verticalScrollBar()->setValue(0);

    //albumListWidget->setStyleSheet("border: 1px solid red");

    emit onAlbomClickedSignal();
}
// остановились на том что addbutton не работает класса albomlist нужно продебажить лишь а так все заебись

void ProfileWidget::onTrackdoubleClicked(track *trackData){
    emit onTrackDoubleClickedignal(trackData);
}


int ProfileWidget::getCurrentIndex(){
    return innerStacked->currentIndex();
}

int ProfileWidget::getTotalIndex(){
    return innerStacked->count();
}

void ProfileWidget::setCurrentIndex(int currentIndex){
    //qDebug()<<"setCurrent";
    //qDebug()<<currentIndex;
    //qDebug()<<"total "<<getTotalIndex();
    innerStacked->setCurrentIndex(currentIndex);
}

int ProfileWidget::get_widgetWidth(){
    return widgetWidth;
}


