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


#include "profilewidget.h"
#include "userProfileWidget.h"
#include "myplaylistswidget.h"


ProfileWidget::ProfileWidget(QWidget *parent, QWidget *tab)
    :QWidget(parent),
    userProfile(new UserProfileWidget(this)),
    playlistwidget(new MyPlaylistsWidget(this)),
    trackwidget(new MyTracksWidget(this))
{
    UserInfo usernurshat{"imgs/ava.png", "User Name", "@usertag", 100, 100, 100, 100};
    userProfile->setUserProfile(usernurshat);//заполняем профиль
    QVBoxLayout *profileLayout = new QVBoxLayout(tab);
    profileLayout->setContentsMargins(50, 50, 50, 50);
    profileLayout->addWidget(userProfile, 0, Qt::AlignTop);
    profileLayout->addWidget(playlistwidget);
    profileLayout->addWidget(trackwidget);
    //trackwidget->setStyleSheet("QWidget { border: 1px solid red; }");
    scrollWidget = new QWidget();
    scrollWidget->setLayout(profileLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    tab->setLayout(new QVBoxLayout());
    tab->layout()->addWidget(scrollArea);
}

void ProfileWidget::resizeProfile(int width){
    playlistwidget->resizePlaylists(width);
    scrollWidget->setFixedWidth(width * 0.8);
    trackwidget->resize_tracks(width);
    //Обновляем размеры виджета при изменении размеров окна
    if (userProfile && width>1340 && width<1500) {
        userProfile->setFixedSize(width * 0.4, 220);
    } else if (userProfile && width<1340){
        userProfile->setFixedSize(1340 * 0.4, 220);
    } else {
        userProfile->setFixedSize(1500 * 0.4, 220);
    }
    qDebug()<<width;
}


void ProfileWidget::button_profile_clicked(){
    playlistwidget->add_playlists();
}


