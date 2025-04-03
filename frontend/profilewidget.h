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

#include "userProfileWidget.h"  // Подключаем header для UserProfileWidget
#include "myplaylistswidget.h"
#include "mytrackswidget.h"

#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

class ProfileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProfileWidget(QWidget  *parent = nullptr, QWidget *tab = nullptr);
    void resizeProfile(int width);
    void button_profile_clicked();

private:
    UserProfileWidget *userProfile;
    MyPlaylistsWidget *playlistwidget;
    MyTracksWidget *trackwidget;
    QWidget *scrollWidget;
};

#endif // PROFILEWIDGET_H
