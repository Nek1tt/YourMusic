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
    likedtrackwidget(new MyTracksWidget(this)),
    loadedtrackwidget(new MyTracksWidget(this))
{
    QVBoxLayout *profileLayout = new QVBoxLayout(tab);
    innerStacked = new QStackedWidget();
    mainWidget = new QWidget();
    //mainWidget->setStyleSheet("border: 1px solid red");
    QVBoxLayout *mainWidgetLayout = new QVBoxLayout(mainWidget);
    innerStacked->addWidget(mainWidget); // main stacked widget
    profileLayout->addWidget(innerStacked);

    UserInfo usernurshat{"../resources/imgs/ava.png", "User Name", "@usertag", 100, 100, 100, 100};
    userProfile->setUserProfile(usernurshat);//заполняем профиль
    mainWidgetLayout->setContentsMargins(10, 10, 10, 10);
    mainWidgetLayout->addWidget(userProfile, 0, Qt::AlignTop);

    connect(albumwidget, &MyAlbumsWidget::albumButtonClicked, this, &ProfileWidget::onAlbumClicked);
    connect(likedtrackwidget, &MyTracksWidget::trackButtonClicked, this, &ProfileWidget::onTrackdoubleClicked);
    connect(loadedtrackwidget, &MyTracksWidget::trackButtonClicked, this, &ProfileWidget::onTrackdoubleClicked);

    likedtrackwidget->add_liked_tracks();
    loadedtrackwidget->add_loaded_tracks();

    mainWidgetLayout->addWidget(albumwidget);
    mainWidgetLayout->addWidget(likedtrackwidget);
    mainWidgetLayout->addWidget(loadedtrackwidget);
    //likedtrackwidget->setStyleSheet("QWidget { border: 1px solid red; }");
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


void ProfileWidget::button_profile_clicked(){
    albumwidget->add_albums();
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

    AlbumWidget *albumwidget = new AlbumWidget(albumData);
    innerStacked->addWidget(albumwidget);
    innerStacked->setCurrentWidget(albumwidget);
    //albumwidget->setStyleSheet("border: 1px solid red");

    emit onAlbomClickedSignal(albumData);
}

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


