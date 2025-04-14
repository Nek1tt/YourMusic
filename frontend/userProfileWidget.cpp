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

#include "userProfileWidget.h"

UserProfileWidget::UserProfileWidget(QWidget *parent) // класс для информации о профиле
    : QWidget(parent)      //пользователя. хранится ава, имя, тег, информация о подписчиках и тдд
{
    QHBoxLayout *ProfileLayout = new QHBoxLayout(this);// слой, заполняющий весь виджет, в нем хранятся все объекты
    avatarLabel = new QLabel(this);//label, слой хранящий аватарку
    avatarLabel->setFixedSize(200,200);//размеры слоя с авой
    ProfileLayout->addWidget(avatarLabel);// добавляем слой с авой в слой профиля

    QVBoxLayout *ProfileInfoLayout = new QVBoxLayout(); // все то же самое что и с авой но с блоком информации
    usertagLabel = new QLabel(this);
    usertagLabel->setFixedHeight(50);
    usertagLabel->setStyleSheet("padding-top: 30px; color: #615D5D; font-size: 15px; font-family: 'Tahoma'; font-weight: bold;");
    ProfileInfoLayout->addWidget(usertagLabel);
    usernameLabel = new QLabel(this);
    usernameLabel->setFixedHeight(40);
    usernameLabel->setStyleSheet("font-size: 32px; font-family: 'Tahoma'; font-weight: bold;");
    ProfileInfoLayout->addWidget(usernameLabel);

    // QLabel* test = new QLabel(this);
    followersLabel= new QLabel(this);
    followingLabel = new QLabel(this);
    tracksLoadedLabel =new QLabel(this);
    tracksAddedLabel = new QLabel(this);
    followersNumLabel = new QLabel(this);
    followingNumLabel = new QLabel(this);
    tracksLoadedNumLabel = new QLabel(this);
    tracksAddedNumLabel = new QLabel(this);
    QHBoxLayout *miniInfoLayout = new QHBoxLayout();
    QVBoxLayout *followersLayout = new QVBoxLayout();
    followersLayout->setAlignment(Qt::AlignHCenter);
    followersLabel->setText("Followers");
    followersLabel->setStyleSheet("font-size: 10px; font-family: 'Tahoma'; font-weight: bold;");
    followersLayout->addWidget(followersLabel);
    followersNumLabel->setAlignment(Qt::AlignHCenter);
    followersNumLabel->setStyleSheet("font-size: 15px; font-family: 'Tahoma'; font-weight: bold;");
    followersLayout->addWidget(followersNumLabel);
    miniInfoLayout->addLayout(followersLayout);
    QVBoxLayout *followingLayout = new QVBoxLayout();
    followingLayout->setAlignment(Qt::AlignHCenter);
    followingLabel->setText("Following");
    followingLabel->setStyleSheet("font-size: 10px; font-family: 'Tahoma'; font-weight: bold;");
    followingLayout->addWidget(followingLabel);
    followingNumLabel->setAlignment(Qt::AlignHCenter);
    followingNumLabel->setStyleSheet("font-size: 15px; font-family: 'Tahoma'; font-weight: bold;");
    followingLayout->addWidget(followingNumLabel);
    miniInfoLayout->addLayout(followingLayout);
    QVBoxLayout *tracksLoadedLayout = new QVBoxLayout();
    tracksLoadedLayout->setAlignment(Qt::AlignHCenter);
    tracksLoadedLabel->setText("Tracks loaded");
    tracksLoadedLabel->setStyleSheet("font-size: 10px; font-family: 'Tahoma'; font-weight: bold;");
    tracksLoadedLayout->addWidget(tracksLoadedLabel);
    tracksLoadedNumLabel->setAlignment(Qt::AlignHCenter);
    tracksLoadedNumLabel->setStyleSheet("font-size: 15px; font-family: 'Tahoma'; font-weight: bold;");
    tracksLoadedLayout->addWidget(tracksLoadedNumLabel);
    miniInfoLayout->addLayout(tracksLoadedLayout);
    QVBoxLayout *tracksAddedLayout = new QVBoxLayout();
    tracksAddedLayout->setAlignment(Qt::AlignHCenter);
    tracksAddedLabel->setText("Tracks added");
    tracksAddedLabel->setStyleSheet("font-size: 10px; font-family: 'Tahoma'; font-weight: bold;");
    tracksAddedLayout->addWidget(tracksAddedLabel);
    tracksAddedNumLabel->setAlignment(Qt::AlignHCenter);
    tracksAddedNumLabel->setStyleSheet("font-size: 15px; font-family: 'Tahoma'; font-weight: bold;");
    tracksAddedLayout->addWidget(tracksAddedNumLabel);
    miniInfoLayout->addLayout(tracksAddedLayout);

    ProfileInfoLayout->addLayout(miniInfoLayout);

    ProfileLayout->addLayout(ProfileInfoLayout);


    // followersLabel = new QLabel(this);
    // layout->addWidget(followersLabel);
}

void UserProfileWidget::setUserProfile(const UserInfo &user) {
    QPixmap avatarPixmap(user.avatarPath);// создаем pixmap с авой. передаем путь где хранится ава.
    avatarLabel->setPixmap(avatarPixmap.scaled(200, 200, Qt::KeepAspectRatio)); //добавляем в слой авы саму аву
    usernameLabel->setText(user.username);
    usertagLabel->setText(user.usertag);
    followersNumLabel->setText(QString::number(user.followersnum));
    followingNumLabel->setText(QString::number(user.followingnum));
    tracksLoadedNumLabel->setText(QString::number(user.tracksLoadednum));
    tracksAddedNumLabel->setText(QString::number(user.tracksAddednum));
}// функция, что заполняет информацией и авой профиль.


