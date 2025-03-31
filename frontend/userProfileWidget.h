#ifndef USERPROFILEWIDGET_H
#define USERPROFILEWIDGET_H

#include <QWidget>
#include <QLabel>

struct UserInfo
{
    QString avatarPath;
    QString username;
    QString usertag;
    int followersnum;
    int followingnum;
    int tracksLoadednum;
    int tracksAddednum;
};

class UserProfileWidget : public QWidget {
    Q_OBJECT

public:
    explicit UserProfileWidget(QWidget *parent = nullptr);
    void setUserProfile(const UserInfo &user);

private:
    QLabel *avatarLabel;
    QLabel *usernameLabel;
    QLabel *usertagLabel;
    QLabel *followersLabel;
    QLabel *followingLabel;
    QLabel *tracksLoadedLabel;
    QLabel *tracksAddedLabel;
    QLabel *followersNumLabel;
    QLabel *followingNumLabel;
    QLabel *tracksLoadedNumLabel;
    QLabel *tracksAddedNumLabel;
};
#endif // USERPROFILEWIDGET_H
