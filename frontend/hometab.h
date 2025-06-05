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
    explicit HomeTab(QWidget *tab = nullptr);
    void homeButtonClicked();
private:    
    HomeWidget *mainWidget;

};

#endif // HOMETAB_H



#ifndef CREATETAB_H
#define CREATETAB_H

class CreateTab : public AbstractTab
{
    Q_OBJECT
public:
    explicit CreateTab(QWidget *parent = nullptr);
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
    explicit ProfileTab(QWidget  *parent = nullptr, QWidget *tab = nullptr);
    void resizeProfile(int width);
    void profileButtonClicked(UserInfo userData, QVector<album> albumList, album likedTracks, album loadedTracks);
    void onAlbumClicked(album albumData);
    void onAlbumListClicked(QVector<album> albumList);
    void onUsersListClicked(QVector<UserInfo> users);
    void onUserButtonClicked(UserInfo userData);
    void onAuthorButtonClicked(int authorId);
    void onAlbumByTrackId(int trackId);
    void onAuthorByTrackId(int trackId);

    int get_widgetWidth();
    void onTrackdoubleClicked(track *trackData);
    void onFollowersButtonClicked();
    void onFollowingButtonClicked();
    void onTracksLoadedButtonClicked();
    void onTracksAddedButtonClicked();
    QStackedWidget *getInnerStacked();


private:
    int widthForResize;
    ProfileWidget *mainWidget;
    int widgetWidth;
};

#endif // PROFILEWIDGET_H
