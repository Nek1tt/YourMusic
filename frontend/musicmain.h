#ifndef MUSICMAIN_H
#define MUSICMAIN_H

#include <QMainWindow>
#include <QLabel>
#include <QSplitter>
// #include <QWebSocket>

//#include "./ui_musicmain.h"
#include "profilewidget.h"
#include "userProfileWidget.h"  // Подключаем header для UserProfileWidget
#include "myalbumswidget.h"
#include "createwidget.h"
#include "rightbarwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MusicMain;
}
QT_END_NAMESPACE

class MusicMain : public QMainWindow
{
    Q_OBJECT

public:
    MusicMain(QWidget *parent = nullptr);
    ~MusicMain();

private slots:
    void toggle_buttons(QPushButton*);

    void on_homeTab_clicked();
    void on_createTab_clicked();
    void on_profileTab_clicked();

    void resizeEvent(QResizeEvent *event);
    void setInitialSize(int width);

    void on_backButton_clicked();
    void on_forwardButton_clicked();
    void on_albumButton_clicked();
    void onSplitterMoved(int pos, int index);



private:
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

    QWidget *Home;
    QWidget *Create;
    QWidget *Profile;

    QPushButton *backButton;
    QPushButton *forwardButton;
    QPushButton *homeTab;
    QPushButton *createTab;
    QPushButton *profileTab;

    track *currentTrack;

    std::vector<QPushButton*> mainTabButtons;
    QPushButton* currentTab;
    //QWebSocket* webSocket;
    ProfileWidget *profilewidget;
    CreateWidget *createwidget;
    RightBarWidget *rightbarwidget;


    //UserProfileWidget *userProfile;
    //MyPlaylistsWidget *playlistwidget;
    //QWidget *scrollWidget;

};

#endif // MUSICMAIN_H
