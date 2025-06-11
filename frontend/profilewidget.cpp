#include "profilewidget.h"
#include <QVBoxLayout>

ProfileWidget::ProfileWidget(QWidget *parent)
    : QWidget(parent),
    userProfile(new UserProfileWidget(this)),
    albumwidget(new MyAlbumsWidget("My albums", this)),
    likedtrackwidget(new MyTracksWidget("My tracks", this)),
    loadedtrackwidget(new MyTracksWidget("My loaded tracks", this))
{
    QWidget *contentWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->addWidget(userProfile, 0, Qt::AlignTop);

    mainLayout->addWidget(albumwidget);
    mainLayout->addWidget(likedtrackwidget);
    mainLayout->addWidget(loadedtrackwidget);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addWidget(scrollArea);

    connect(albumwidget, &MyAlbumsWidget::albumButtonClicked, this, &ProfileWidget::onAlbumClicked);
    connect(albumwidget, &MyAlbumsWidget::myAlbumsButtonClicked, this, &ProfileWidget::onAlbumListClicked);
    connect(albumwidget, &MyAlbumsWidget::authorButtonClicked, this, &ProfileWidget::onAuthorButtonClicked);

    connect(likedtrackwidget, &MyTracksWidget::trackButtonClicked, this, &ProfileWidget::onTrackDoubleClicked);
    connect(loadedtrackwidget, &MyTracksWidget::trackButtonClicked, this, &ProfileWidget::onTrackDoubleClicked);

    connect(likedtrackwidget, &MyTracksWidget::mytracksButtonClicked, this, &ProfileWidget::onAlbumClicked);
    connect(loadedtrackwidget, &MyTracksWidget::mytracksButtonClicked, this, &ProfileWidget::onAlbumClicked);

    connect(likedtrackwidget, &MyTracksWidget::trackNameButtonClicked, this, &ProfileWidget::onTrackNameButtonClicked);
    connect(loadedtrackwidget, &MyTracksWidget::trackNameButtonClicked, this, &ProfileWidget::onTrackNameButtonClicked);

    connect(likedtrackwidget, &MyTracksWidget::authorButtonClicked, this, &ProfileWidget::onAuthorButtonClicked);
    connect(loadedtrackwidget, &MyTracksWidget::authorButtonClicked, this, &ProfileWidget::onAuthorButtonClicked);

    connect(userProfile, &UserProfileWidget::followersButtonClicked, this, &ProfileWidget::followersButtonClicked);
    connect(userProfile, &UserProfileWidget::followingButtonClicked, this, &ProfileWidget::followingButtonClicked);
    connect(userProfile, &UserProfileWidget::tracksLoadedButtonClicked, this, &ProfileWidget::tracksLoadedButtonClicked);
    connect(userProfile, &UserProfileWidget::tracksAddedButtonClicked, this, &ProfileWidget::tracksAddedButtonClicked);
}


void ProfileWidget::resizeProfile(int width){
    albumwidget->resizeAlbums(width);
    likedtrackwidget->resize_tracks(width);
    loadedtrackwidget->resize_tracks(width);
    userProfile->resize(width * 0.6, 220);
}

void ProfileWidget::updateAlbums(UserInfo userData, QVector<album> albumList, album likedTracks, album loadedTracks){
    userProfile->setUserProfile(userData);
    likedtrackwidget->add_liked_tracks(likedTracks);
    loadedtrackwidget->add_liked_tracks(loadedTracks);
    albumwidget->add_albums(albumList);
}

void ProfileWidget::onFollowersButtonClicked(){
    emit followersButtonClicked();
};
void ProfileWidget::onFollowingButtonClicked(){
    emit followingButtonClicked();
};
void ProfileWidget::onTracksLoadedButtonClicked(){
    emit tracksLoadedButtonClicked();
};
void ProfileWidget::onTracksAddedButtonClicked(){
    emit tracksAddedButtonClicked();
};

void ProfileWidget::onAuthorButtonClicked(QString *authorUsertag){
    emit authorButtonClicked(authorUsertag);
}

void ProfileWidget::onAlbumClicked(album albumData){
    emit albumClicked(albumData);
};
void ProfileWidget::onAlbumListClicked(QVector<album> albumList){
    emit albumListClicked(albumList);
};
void ProfileWidget::onTrackDoubleClicked(track *trackData){
    emit trackDoubleClicked(trackData);
};

void ProfileWidget::onTrackNameButtonClicked(track *TrackData){
    emit trackNameButtonClicked(TrackData);
}


