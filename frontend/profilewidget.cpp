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

#include "profilewidget.h"
#include "userProfileWidget.h"
#include "myalbumswidget.h"
#include "albumwidget.h"
#include "mytrackswidget.h"

ProfileWidget::ProfileWidget(QWidget *parent, QWidget *tab)
    :QWidget(parent),
    userProfile(new UserProfileWidget(this)),
    albumwidget(new MyAlbumsWidget(this)),
    trackwidget(new MyTracksWidget(this))
{
    QVBoxLayout *profileLayout = new QVBoxLayout(tab);
    innerStacked = new QStackedWidget();
    mainWidget = new QWidget();
    QVBoxLayout *mainWidgetLayout = new QVBoxLayout(mainWidget);
    innerStacked->addWidget(mainWidget); // main stacked widget
    profileLayout->addWidget(innerStacked);

    UserInfo usernurshat{"imgs/ava.png", "User Name", "@usertag", 100, 100, 100, 100};
    userProfile->setUserProfile(usernurshat);//заполняем профиль
    mainWidgetLayout->setContentsMargins(50, 50, 50, 50);
    mainWidgetLayout->addWidget(userProfile, 0, Qt::AlignTop);

    connect(albumwidget, &MyAlbumsWidget::albumButtonClicked, this, &ProfileWidget::onAlbumClicked);

    mainWidgetLayout->addWidget(albumwidget);
    mainWidgetLayout->addWidget(trackwidget);
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
    albumwidget->resizeAlbums(width);
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
    albumwidget->add_albums();
}

void ProfileWidget::onAlbumClicked(QString albumName){
    qDebug() << "albumButton clicked: " << albumName;

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

    // Добавляем новый виджет
    track currentTrack;
    AlbumWidget *alwidget = new AlbumWidget(currentTrack);
    innerStacked->addWidget(alwidget);
    innerStacked->setCurrentWidget(alwidget);

    emit onAlbomClickedSignal(albumName);
}



int ProfileWidget::getCurrentIndex(){
    return innerStacked->currentIndex();
}

int ProfileWidget::getTotalIndex(){
    return innerStacked->count();
}

void ProfileWidget::setCurrentIndex(int currentIndex){
    qDebug()<<"setCurrent";
    qDebug()<<currentIndex;
    qDebug()<<"total "<<getTotalIndex();
    innerStacked->setCurrentIndex(currentIndex);
}




