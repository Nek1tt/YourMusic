#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include <QWidget>
#include "userProfileWidget.h"
#include "myalbumswidget.h"
#include "mytrackswidget.h"

class ProfileWidget : public QWidget {
    Q_OBJECT

public:
    ProfileWidget(QWidget *parent = nullptr);
    void resizeProfile(int width);
    void updateAlbums(UserInfo userData, QVector<album> albumList, album likedTracks, album loadedTracks);

    void onAlbumClicked(album albumData);
    void onAlbumListClicked(QVector<album> albumList);
    void onTrackDoubleClicked(track *trackData);
    void onFollowersButtonClicked();
    void onFollowingButtonClicked();
    void onTracksLoadedButtonClicked();
    void onTracksAddedButtonClicked();
    void onAuthorButtonClicked(int authorId);
    void onTrackNameButtonClicked(int trackId);


signals:
    void albumClicked(album albumData);
    void albumListClicked(QVector<album> albumList);
    void trackDoubleClicked(track *trackData);
    void followersButtonClicked();
    void followingButtonClicked();
    void tracksLoadedButtonClicked();
    void tracksAddedButtonClicked();
    void authorButtonClicked(int authorId);
    void trackNameButtonClicked(int trackId);

private:
    UserInfo userData;
    UserProfileWidget *userProfile;
    MyAlbumsWidget *albumwidget;
    MyTracksWidget *likedtrackwidget;
    MyTracksWidget *loadedtrackwidget;
};

#endif // PROFILEWIDGET_H
