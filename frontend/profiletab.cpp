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

ProfileTab::ProfileTab(QWidget  *parent, QWidget *tab)
    : AbstractTab(parent)
{
    innerStacked = new QStackedWidget();
    mainWidget = new ProfileWidget();

    connect(mainWidget, &ProfileWidget::followersButtonClicked, this, &ProfileTab::onFollowersButtonClicked);
    connect(mainWidget, &ProfileWidget::followingButtonClicked, this, &ProfileTab::onFollowingButtonClicked);
    connect(mainWidget, &ProfileWidget::tracksLoadedButtonClicked, this, &ProfileTab::onTracksLoadedButtonClicked);
    connect(mainWidget, &ProfileWidget::tracksAddedButtonClicked, this, &ProfileTab::onTracksAddedButtonClicked);

    connect(mainWidget, &ProfileWidget::albumClicked, this, &ProfileTab::onAlbumClicked);
    connect(mainWidget, &ProfileWidget::albumListClicked, this, &ProfileTab::onAlbumListClicked);
    connect(mainWidget, &ProfileWidget::trackDoubleClicked, this, &ProfileTab::onTrackdoubleClicked);


    connect(mainWidget, &ProfileWidget::trackNameButtonClicked, this, &ProfileTab::onAlbumByTrackId);

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
}

void ProfileTab::onFollowersButtonClicked(){
    QVector<UserInfo> users = loadUsersFromJson("resources/jsons/users.json");
    onUsersListClicked(users);
};
void ProfileTab::onFollowingButtonClicked(){
    QVector<UserInfo> users = loadUsersFromJson("resources/jsons/users.json");
    onUsersListClicked(users);
};

void ProfileTab::onTracksLoadedButtonClicked(){
    //onAlbumClicked(loadedTracks);
};
void ProfileTab::onTracksAddedButtonClicked(){
    //onAlbumClicked(likedTracks);
};

void ProfileTab::onAuthorButtonClicked(QString authorUsername){
    QVector<UserInfo> users = loadUsersFromJson("resources/jsons/users.json");
    onUserButtonClicked(users[1]);
}

void ProfileTab::onAlbumByTrackId(int trackId){
    QVector <album> albums_vector = loadAlbumsFromJson("resources/jsons/myalbums.json");
    onAlbumClicked(albums_vector[0]);
}

void ProfileTab::onAuthorByTrackId(int trackId){
    QVector<UserInfo> users = loadUsersFromJson("resources/jsons/users.json");
    onUserButtonClicked(users[1]);
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
    connect(userListWidget, &UsersListWidget::userButtonClicked, this, &ProfileTab::onUserButtonClicked);

    innerStacked->addWidget(userListWidget);
    innerStacked->setCurrentWidget(userListWidget);
    scrollArea->verticalScrollBar()->setValue(0);

    //albumwidgetTab->setStyleSheet("border: 1px solid red");

    emit onAlbomClickedSignal();
}

void ProfileTab::onUserButtonClicked(UserInfo userData){

    QVector <album> albums_vector = loadAlbumsFromJson("resources/jsons/myalbums.json");
    album loadedTracks = loadSingleAlbumFromJson("resources/jsons/myloadedtracks.json");

    album likedTracks = loadSingleAlbumFromJson("resources/jsons/mytracks.json");

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

    ProfileWidget *userProfileWidget = new ProfileWidget();
    //profileLayout->addWidget(userProfileWidget);
    userProfileWidget->updateAlbums(userData, albums_vector, likedTracks, loadedTracks);
    userProfileWidget->resizeProfile(widthForResize);
    connect(userProfileWidget, &ProfileWidget::followersButtonClicked, this, &ProfileTab::onFollowersButtonClicked);
    connect(userProfileWidget, &ProfileWidget::followingButtonClicked, this, &ProfileTab::onFollowingButtonClicked);
    connect(userProfileWidget, &ProfileWidget::tracksLoadedButtonClicked, this, &ProfileTab::onTracksLoadedButtonClicked);
    connect(userProfileWidget, &ProfileWidget::tracksAddedButtonClicked, this, &ProfileTab::onTracksAddedButtonClicked);

    connect(userProfileWidget, &ProfileWidget::albumClicked, this, &ProfileTab::onAlbumClicked);
    connect(userProfileWidget, &ProfileWidget::albumListClicked, this, &ProfileTab::onAlbumListClicked);
    connect(userProfileWidget, &ProfileWidget::trackDoubleClicked, this, &ProfileTab::onTrackdoubleClicked);

    connect(userProfileWidget, &ProfileWidget::trackNameButtonClicked, this, &ProfileTab::onAlbumByTrackId);

    connect(userProfileWidget, &ProfileWidget::authorButtonClicked, this, &ProfileTab::onAuthorButtonClicked);

    //connect(albumwidgetTab, &AlbumWidget::trackButtonClicked, this, &ProfileTab::onTrackdoubleClicked);

    innerStacked->addWidget(userProfileWidget);
    innerStacked->setCurrentWidget(userProfileWidget);
    scrollArea->verticalScrollBar()->setValue(0);

    //albumwidgetTab->setStyleSheet("border: 1px solid red");

    emit onAlbomClickedSignal();

}

void ProfileTab::onAlbumClicked(album albumData){

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

    connect(albumwidgetTab, &AlbumWidget::trackButtonClicked, this, &ProfileTab::onTrackdoubleClicked);
    connect(albumwidgetTab, &AlbumWidget::trackNameButtonClicked, this, &ProfileTab::onAlbumByTrackId);
    connect(albumwidgetTab, &AlbumWidget::authorButtonClickedByTrackId, this, &ProfileTab::onAuthorByTrackId);
    connect(albumwidgetTab, &AlbumWidget::authorButtonClicked, this, &ProfileTab::onAuthorButtonClicked);

    innerStacked->addWidget(albumwidgetTab);
    innerStacked->setCurrentWidget(albumwidgetTab);
    scrollArea->verticalScrollBar()->setValue(0);

    //albumwidgetTab->setStyleSheet("border: 1px solid red");

    emit onAlbomClickedSignal();
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

    //AlbumWidget *albumwidget = new AlbumWidget(albumData);
    //connect(albumwidget, &AlbumWidget::trackButtonClicked, this, &ProfileWidget::onTrackdoubleClicked);
    AlbomListWidget *albumListWidget = new AlbomListWidget(albumList);
    connect(albumListWidget, &AlbomListWidget::albumButtonClicked, this, &ProfileTab::onAlbumClicked);
    connect(albumListWidget, &AlbomListWidget::authorButtonClicked, this, &ProfileTab::onAuthorButtonClicked);


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

void ProfileTab::onTrackdoubleClicked(track *trackData){
    emit onTrackDoubleClickedignal(trackData);
}

int ProfileTab::get_widgetWidth(){
    return widgetWidth;
}


