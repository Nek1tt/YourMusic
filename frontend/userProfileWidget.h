#ifndef USERPROFILEWIDGET_H
#define USERPROFILEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

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
    void loadCover(const QString& url, QLabel *label);

private:
    QLabel *avatarLabel;
    QLabel *usernameLabel;
    QLabel *usertagLabel;
    QPushButton *followersLabel;
    QPushButton *followingLabel;
    QPushButton *tracksLoadedLabel;
    QPushButton *tracksAddedLabel;
    QLabel *followersNumLabel;
    QLabel *followingNumLabel;
    QLabel *tracksLoadedNumLabel;
    QLabel *tracksAddedNumLabel;

signals:
    void tracksAddedButtonClicked();
    void tracksLoadedButtonClicked();
    void followersButtonClicked();
    void followingButtonClicked();
};


QVector<UserInfo> loadUsersFromJson(const QString &filePath);

#endif // USERPROFILEWIDGET_H
