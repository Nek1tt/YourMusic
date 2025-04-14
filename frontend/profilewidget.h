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

#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

class ProfileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProfileWidget(QWidget  *parent = nullptr, QWidget *tab = nullptr);
    void resizeProfile(int width);
    void button_profile_clicked();
    void onAlbumClicked(QString albumName);

    int getCurrentIndex();
    int getTotalIndex();
    void setCurrentIndex(int index);

private:
    UserProfileWidget *userProfile;
    MyAlbumsWidget *albumwidget;
    MyTracksWidget *trackwidget;
    QStackedWidget *innerStacked;
    QWidget *scrollWidget;
    QWidget *mainWidget;

signals:
    void onAlbomClickedSignal(QString albumName);
};

#endif // PROFILEWIDGET_H
