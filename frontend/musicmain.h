#ifndef MUSICMAIN_H
#define MUSICMAIN_H

#include <QMainWindow>
#include <QLabel>
// #include <QWebSocket>

#include "./ui_musicmain.h"
#include "profilewidget.h"
#include "userProfileWidget.h"  // Подключаем header для UserProfileWidget
#include "myplaylistswidget.h"
#include "createwidget.h"

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


private:
    Ui::MusicMain *ui;
    QTabWidget *tabwidget;
    std::vector<QPushButton*> mainTabButtons;
    QPushButton* currentTab;
    //QWebSocket* webSocket;
    ProfileWidget *profilewidget;
    CreateWidget *createwidget;
    //UserProfileWidget *userProfile;
    //MyPlaylistsWidget *playlistwidget;
    //QWidget *scrollWidget;

};

#endif // MUSICMAIN_H
