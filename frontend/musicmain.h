#ifndef MUSICMAIN_H
#define MUSICMAIN_H

#include <QMainWindow>
#include <QLabel>
#include <QSplitter>
// #include <QWebSocket>

//#include "./ui_musicmain.h"
//#include "profiletab.h"
#include "hometab.h"
#include "userProfileWidget.h"  // Подключаем header для UserProfileWidget
#include "myalbumswidget.h"
#include "rightbarwidget.h"
#include "mytrackswidget.h"
#include "websocketclient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MusicMain;
}
QT_END_NAMESPACE

class MusicMain : public QMainWindow
{
    Q_OBJECT

public:
    MusicMain(QString *usertag, WebSocketClient *webSocket, WebSocketClient *webSocketStas, QWidget *parent = nullptr);
    void on_homeTab_clicked();
    void onTextMessageReceived(const QString &type, const QJsonObject &data);
    void getCurrentTab();
    ~MusicMain();

private slots:
    void toggle_buttons(QPushButton*);
    void on_createTab_clicked();
    void on_profileTab_clicked();

    void resizeEvent(QResizeEvent *event);
    void setInitialSize(int width);

    void on_backButton_clicked();
    void on_forwardButton_clicked();
    void on_albumButton_clicked();
    void on_TrackButton_clicked(track *trackData);
    void onSplitterMoved(int pos, int index);

private:
    WebSocketClient *webSocket;
    WebSocketClient *webSocketStas;
    QString *usertag;
    QSplitter *mainSplitter;
    QWidget *mainWidget;
    QHBoxLayout *mainLayout;
    QTabWidget *tabwidget;
    QWidget *leftBarWidget;
    QWidget *rifghtBarWidget;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;

    QString inactiveStyle;
    QString activeStyle;

    QWidget *Create;

    QPushButton *backButton;
    QPushButton *forwardButton;
    QPushButton *homeTabButton;
    QPushButton *createTabButton;
    QPushButton *profileTabButton;

    track *currentTrack;
    AbstractTab *currentTabWidget;
    std::vector<QPushButton*> mainTabButtons;
    QPushButton* currentTab;
    //QWebSocket* webSocket;
    ProfileTab *profileTab;
    HomeTab *homeTab;
    CreateTab *createTab;
    RightBarWidget *rightbarwidget;

    QVector<album> *randomAlbums;
    QVector<album> *newAlbums;
    album *newTracks;
    album *recTracks;
    QVector<album> *recAlbums;

    QString typeOfQuery;

    QVector<album> *likedAlbums;
    QVector<album> *loadedAlbums;
    album *likedTracks;
    album *loadedTracks;
    UserInfo *mainUserInfo;
    UserInfo *viewUserInfo;
    //UserProfileWidget *userProfile;
    //MyPlaylistsWidget *playlistwidget;
    //QWidget *scrollWidget;

};

#endif // MUSICMAIN_H
