#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "myalbumswidget.h"

#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

class HomeWidget : public QWidget
{
    Q_OBJECT
public:
    HomeWidget();
    void homeButtonClicked(QVector<album> *newAlbums, QVector<album> *recAlbums, QVector<album> *randomAlbums);
    void onAlbumClicked(album albumData);
    void onAlbumListClicked(QVector<album> albumList);
    void onFollowersButtonClicked();
    void onFollowingButtonClicked();
    void onAuthorButtonClicked(QString *authorUsertag);

signals:
    void albumClicked(album albumData);
    void albumListClicked(QVector<album> albumList);
    void trackDoubleClicked(track *trackData);
    void followersButtonClicked();
    void followingButtonClicked();
    void tracksLoadedButtonClicked();
    void tracksAddedButtonClicked();
    void authorButtonClicked(QString *authorUsertag);
    void trackNameButtonClicked(track *TrackData);
private:
    MyAlbumsWidget *newAlbumsWidget;
    MyAlbumsWidget *recommendedAlbumsWidget;
    MyAlbumsWidget *randomAlbumsWidget;
};

#endif // HOMEWIDGET_H
