#include <QScreen> //for Qscreen *screen = QApplication::primaryScreen();
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDir>
#include <QStackedWidget>
#include <QScrollArea>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "homewidget.h"
#include "userProfileWidget.h"
#include "profilewidget.h"
#include "createwidget.h"

#ifndef ABSTRACTTAB_H
#define ABSTRACTTAB_H

#include <QWidget>
#include <QString>

class AbstractTab : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractTab(QWidget *parent = nullptr) : QWidget(parent) {}

    virtual int getCurrentIndex();
    virtual int getTotalIndex();
    virtual void setCurrentIndex(int index);
    virtual ~AbstractTab() {}

protected:
    QStackedWidget *innerStacked;
    QWidget *scrollWidget;
    QVBoxLayout *mainLayout;
    QScrollArea *scrollArea;
    int cuurentViewerIndex = 0;
    UserInfo currentAuthorOfObjects;
    QVector<QString> currentViewers;
    QString typeOfQuery="None";


signals:
    void onAlbomClickedSignal();
    void onTrackDoubleClickedignal(track *trackData);
};

#endif // ABSTRACTTAB_H

#ifndef HOMETAB_H
#define HOMETAB_H

class HomeTab : public AbstractTab
{
    Q_OBJECT
public:
    explicit HomeTab(QString *usertag, UserInfo currentAuthor, WebSocketClient *webSocket, QWidget *tab = nullptr);
    void homeButtonClicked(UserInfo userData, QVector<album> *newAlbums, QVector<album> *recAlbums, QVector<album> *randomAlbums);

    void onAlbumClicked(album albumData, QVector<int> likedTracks);
    void onAlbumListClicked(QVector<album> albumList);
    void onUsersListClicked(QVector<UserInfo> users);
    void onUserButtonClicked(UserInfo *userData, QVector <album> *albums_vector, album *loadedTracks, album *likedTracks);
    void onAuthorButtonClicked(QString *authorUsertag);
    void onAlbumByTrack(track *TrackData);
    void onTrackdoubleClicked(track *trackData);
    void onTextMessageReceived(const QString &type, const QJsonObject &dataObj);
    void onFollowersButtonClicked();
    void onFollowingButtonClicked();
    void onTracksLoadedButtonClicked();
    void onTracksAddedButtonClicked();
    void onAlbumById(album albumData);

private:    
    HomeWidget *mainWidget;
    QString *mainUsertag;
    WebSocketClient *webSocket;

};

#endif // HOMETAB_H



#ifndef CREATETAB_H
#define CREATETAB_H

class CreateTab : public AbstractTab
{
    Q_OBJECT
public:
    explicit CreateTab(QString *usertag, WebSocketClient *webSocket, QWidget *parent = nullptr);
    void homeButtonClicked();
    void chooseImage();
private:
    CreateWidget *mainWidget;
    QLabel *imageLabel;
    QLineEdit *nameEdit;
    QLineEdit *descriptionEdit;
    QPushButton *addAuthorButton;

};

#endif // CREATETAB_H



#ifndef PROFILETAB_H
#define PROFILETAB_H

class ProfileTab : public AbstractTab
{
    Q_OBJECT
public:
    explicit ProfileTab(QString *usertag, UserInfo currentAuthorOfObjects, WebSocketClient *webSocket, QWidget  *parent = nullptr, QWidget *tab = nullptr);
    void resizeProfile(int width);
    void profileButtonClicked(UserInfo userData, QVector<album> albumList, album likedTracks, album loadedTracks);
    void onAlbumClicked(album albumData, QVector<int> likedTracks);
    void onAlbumListClicked(QVector<album> albumList);
    void onUsersListClicked(QVector<UserInfo> users);
    void onUserButtonClicked(UserInfo *userData, QVector <album> *albums_vector, album *loadedTracks, album *likedTracks);
    void onAuthorButtonClicked(QString *authorUsertag);
    void onAlbumByTrack(track *TrackData);
    void onAlbumById(album albumData);

    int get_widgetWidth();
    void onTrackdoubleClicked(track *trackData);
    void onFollowersButtonClicked();
    void onFollowingButtonClicked();
    void onTracksLoadedButtonClicked();
    void onTracksAddedButtonClicked();
    QStackedWidget *getInnerStacked();
    void onTextMessageReceived(const QString &type, const QJsonObject &dataObj);

private:
    int widthForResize;
    ProfileWidget *mainWidget;
    int widgetWidth;
    QString *mainUsertag;
    WebSocketClient *webSocket;
    //int cuurentViewerIndex=0;
};

#endif // PROFILEWIDGET_H
