#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>
#include <QResizeEvent>
#include <QVector>
#include "userProfileWidget.h"

#ifndef USERSLISTWIDGET_H
#define USERSLISTWIDGET_H


class UserButton : public QPushButton {
    Q_OBJECT

public:
    explicit UserButton(UserInfo userData);
    QString *getUsertag();
private:
    UserInfo userData;
};


class UsersListWidget : public QWidget
{ Q_OBJECT
public:
    explicit UsersListWidget(QVector<UserInfo> users);

private:
    QScrollArea *scrollArea;
    QWidget *containerWidget;
    QWidget *centerWrapper;
    QGridLayout *trackListLayout;
    QVector<UserInfo> users;
    //QVector<AlbumButton> buttons;

    void buildGrid();
signals:
    void userButtonClicked(QString *usertag);
};

#endif // USERSLISTWIDGET_H
