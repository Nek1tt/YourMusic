#include <QScreen> //for Qscreen *screen = QApplication::primaryScreen();
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
// #include <QWebSocket>
#include <QMessageBox>
#include <QDir>
#include <QPainter>
#include <QPainterPath>
#include "userProfileWidget.h"
#include "setstyle.h"


UserProfileWidget::UserProfileWidget(QWidget *parent) // класс для информации о профиле
    : QWidget(parent)      //пользователя. хранится ава, имя, тег, информация о подписчиках и тдд
{
    QHBoxLayout *ProfileLayout = new QHBoxLayout(this);// слой, заполняющий весь виджет, в нем хранятся все объекты
    avatarLabel = new QLabel(this);//label, слой хранящий аватарку
    avatarLabel->setFixedSize(200,200);//размеры слоя с авой
    ProfileLayout->addWidget(avatarLabel);// добавляем слой с авой в слой профиля

    QVBoxLayout *ProfileInfoLayout = new QVBoxLayout(); // все то же самое что и с авой но с блоком информации
    usertagLabel = new QLabel(this);
    usertagLabel->setFixedHeight(30);
    usertagLabel->setStyleSheet("padding-top: 5px; color: #615D5D; font-size: 15px; font-family: 'Tahoma'; font-weight: bold;");
    ProfileInfoLayout->addWidget(usertagLabel);
    usernameLabel = new QLabel(this);
    usernameLabel->setFixedHeight(80);
    usernameLabel->setStyleSheet("font-size: 32px; font-family: 'Tahoma'; font-weight: bold; padding-bottom: 30px");
    ProfileInfoLayout->addWidget(usernameLabel);

    // QLabel* test = new QLabel(this);
    followersLabel= new QPushButton(this);
    followingLabel = new QPushButton(this);
    tracksLoadedLabel =new QPushButton(this);
    tracksAddedLabel = new QPushButton(this);
    connect(followersLabel, &QPushButton::clicked, [this]() {
        emit followersButtonClicked();
    });
    connect(followingLabel, &QPushButton::clicked, [this]() {
        emit followingButtonClicked();
    });
    connect(tracksLoadedLabel, &QPushButton::clicked, [this]() {
        emit tracksLoadedButtonClicked();
    });
    connect(tracksAddedLabel, &QPushButton::clicked, [this]() {
        emit tracksAddedButtonClicked();
    });


    set_button_style(followersLabel, 15);
    set_button_style(followingLabel, 15);
    set_button_style(tracksLoadedLabel, 15);
    set_button_style(tracksAddedLabel, 15);
    followersNumLabel = new QLabel(this);
    followingNumLabel = new QLabel(this);
    tracksLoadedNumLabel = new QLabel(this);
    tracksAddedNumLabel = new QLabel(this);
    QHBoxLayout *miniInfoLayout = new QHBoxLayout();
    QWidget *followersWidget = new QWidget();
    QVBoxLayout *followersLayout = new QVBoxLayout(followersWidget);
    followersLayout->setAlignment(Qt::AlignHCenter);
    followersLabel->setText("Followers");
    //followersLabel->setStyleSheet("font-size: 15px; font-family: 'Tahoma'; font-weight: bold;");
    followersLayout->addWidget(followersLabel);
    followersNumLabel->setAlignment(Qt::AlignHCenter);
    followersNumLabel->setStyleSheet("font-size: 20px; font-family: 'Tahoma'; font-weight: bold;");
    followersLayout->addWidget(followersNumLabel);
    miniInfoLayout->addWidget(followersWidget);
    QWidget *followingWidget = new QWidget();
    QVBoxLayout *followingLayout = new QVBoxLayout(followingWidget);
    followingLayout->setAlignment(Qt::AlignHCenter);
    followingLabel->setText("Following");
    //followingLabel->setStyleSheet("font-size: 15px; font-family: 'Tahoma'; font-weight: bold;");
    followingLayout->addWidget(followingLabel);
    followingNumLabel->setAlignment(Qt::AlignHCenter);
    followingNumLabel->setStyleSheet("font-size: 20px; font-family: 'Tahoma'; font-weight: bold;");
    followingLayout->addWidget(followingNumLabel);
    miniInfoLayout->addWidget(followingWidget);
    QWidget *tracksLoadedWidget = new QWidget();
    QVBoxLayout *tracksLoadedLayout = new QVBoxLayout(tracksLoadedWidget);
    tracksLoadedLayout->setAlignment(Qt::AlignHCenter);
    tracksLoadedLabel->setText("Tracks loaded");
    //tracksLoadedLabel->setStyleSheet("font-size: 15px; font-family: 'Tahoma'; font-weight: bold;");
    tracksLoadedLayout->addWidget(tracksLoadedLabel);
    tracksLoadedNumLabel->setAlignment(Qt::AlignHCenter);
    tracksLoadedNumLabel->setStyleSheet("font-size: 20px; font-family: 'Tahoma'; font-weight: bold;");
    tracksLoadedLayout->addWidget(tracksLoadedNumLabel);
    miniInfoLayout->addWidget(tracksLoadedWidget);
    QWidget *tracksAddedWidget = new QWidget();
    QVBoxLayout *tracksAddedLayout = new QVBoxLayout(tracksAddedWidget);
    tracksAddedLayout->setAlignment(Qt::AlignHCenter);
    tracksAddedLabel->setText("Tracks added");
    //tracksAddedLabel->setStyleSheet("font-size: 15px; font-family: 'Tahoma'; font-weight: bold;");
    tracksAddedLayout->addWidget(tracksAddedLabel);
    tracksAddedNumLabel->setAlignment(Qt::AlignHCenter);
    tracksAddedNumLabel->setStyleSheet("font-size: 20px; font-family: 'Tahoma'; font-weight: bold;");
    tracksAddedLayout->addWidget(tracksAddedNumLabel);
    miniInfoLayout->addWidget(tracksAddedWidget);

    ProfileInfoLayout->addLayout(miniInfoLayout);

    ProfileLayout->addLayout(ProfileInfoLayout);
    this->setMinimumWidth(670);
    this->setMaximumWidth(1000);

    // followersLabel = new QLabel(this);
    // layout->addWidget(followersLabel);
}

void UserProfileWidget::setUserProfile(const UserInfo &user) {
    QPixmap avatarPixmap(user.avatarPath);
    QPixmap scaledPixmap = avatarPixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Создаем круглую маску
    QPixmap roundedPixmap(scaledPixmap.size());
    roundedPixmap.fill(Qt::transparent); // Прозрачный фон

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(roundedPixmap.rect(), 20, 20); // Закругление по радиусу (100 – круг)
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, scaledPixmap);

    avatarLabel->setPixmap(roundedPixmap); //добавляем в слой авы саму аву
    usernameLabel->setText(user.username);
    usertagLabel->setText(user.usertag);
    followersNumLabel->setText(QString::number(user.followersnum));
    followingNumLabel->setText(QString::number(user.followingnum));
    tracksLoadedNumLabel->setText(QString::number(user.tracksLoadednum));
    tracksAddedNumLabel->setText(QString::number(user.tracksAddednum));
}// функция, что заполняет информацией и авой профиль.



QVector<UserInfo> loadUsersFromJson(const QString &filePath) {
    QFile file(filePath);
    QVector<UserInfo> users;

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file:" << filePath;
        return users;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return users;
    }

    QJsonArray userArray = doc.array();
    for (const QJsonValue &userValue : userArray) {
        QJsonObject root = userValue.toObject();
        UserInfo user;

        user.avatarPath = root["avatarPath"].toString();
        user.username = root["username"].toString();
        user.usertag = root["usertag"].toString();
        user.authorId = root["authorId"].toInt();
        user.followersnum = root["followersnum"].toInt();
        user.followingnum = root["followingnum"].toInt();
        user.tracksLoadednum = root["tracksLoadednum"].toInt();
        user.tracksAddednum = root["tracksAddednum"].toInt();

        users.append(user);
    }

    return users;
}

