#include <QScreen> //for Qscreen *screen = QApplication::primaryScreen();
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
// #include <QWebSocket>
#include <QMessageBox>
#include <QDir>
#include <QStackedWidget>

#include "userProfileWidget.h"  // Подключаем header для UserProfileWidget
#include "myalbumswidget.h"
#include "mytrackswidget.h"
#include "profilewidget.h"

#ifndef PROFILETAB_H
#define PROFILETAB_H_H

class ProfileTab : public QWidget
{
    Q_OBJECT
public:
    explicit ProfileTab(UserInfo userData, QVector <album> albumList, album likedTracks, album loadedTracks, QWidget  *parent = nullptr, QWidget *tab = nullptr);
    void resizeProfile(int width);
    void button_profile_clicked(QVector <album> albums_vector);
    void onAlbumClicked(album albumData);
    void onAlbumListClicked(QVector<album> albumList);
    void onUsersListClicked(QVector<UserInfo> users);
    void onUserButtonClicked(UserInfo userData);
    void onAuthorButtonClicked(int authorId);
    void onAlbumByTrackId(int trackId);
    void onAuthorByTrackId(int trackId);

    int getCurrentIndex();
    int getTotalIndex();
    void setCurrentIndex(int index);
    int get_widgetWidth();
    void onTrackdoubleClicked(track *trackData);
    void onFollowersButtonClicked();
    void onFollowingButtonClicked();
    void onTracksLoadedButtonClicked();
    void onTracksAddedButtonClicked();
    QStackedWidget *getInnerStacked();


private:
    UserInfo userData;
    int widthForResize;
    QVBoxLayout *profileLayout;
    UserProfileWidget *userProfile;
    MyAlbumsWidget *albumwidget;
    MyTracksWidget *likedtrackwidget;
    MyTracksWidget *loadedtrackwidget;
    QStackedWidget *innerStacked;
    QWidget *scrollWidget;
    ProfileWidget *mainWidget;
    QScrollArea *scrollArea;
    int widgetWidth;
    album likedTracks;
    album loadedTracks;
    QVector <album> albumList;

signals:
    void onAlbomClickedSignal();
    void onTrackDoubleClickedignal(track *trackData);
};

#endif // PROFILEWIDGET_H
