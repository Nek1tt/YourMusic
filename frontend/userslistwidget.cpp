#include "setstyle.h"

#include "userslistwidget.h"


//кнопка в виде полльзователя в списке пользователей, при нажатии на нее открывается профиль этого пользователя за счет его usertag
UserButton::UserButton(UserInfo userData)
    :userData(userData)
{
    QHBoxLayout *trackLayout = new QHBoxLayout(this);
    trackLayout->setAlignment(Qt::AlignLeft);
    trackLayout->setContentsMargins(0, 0, 0, 0);
    QLabel *coverLabel = new QLabel(this);
    setRoundedImage(coverLabel, userData.avatarPath, 45, 15);
    trackLayout->addWidget(coverLabel);

    QVBoxLayout *name_and_author = new QVBoxLayout();
    QLabel *nameLabel = new QLabel(userData.username);
    nameLabel->setFixedHeight(16);
    nameLabel->setStyleSheet("font-weight: bold; font-size: 14px; font-family: 'Tahoma';");
    nameLabel->setAlignment(Qt::AlignLeft);

    QLabel *authorLabel = new QLabel(userData.usertag);
    authorLabel->setFixedHeight(14);
    authorLabel->setStyleSheet("color: #828282; font-weight: bold; font-size: 12px; font-family: 'Tahoma';");
    authorLabel->setAlignment(Qt::AlignLeft);

    name_and_author->addWidget(nameLabel);
    name_and_author->addWidget(authorLabel);
    trackLayout->addLayout(name_and_author);

    int minutes = 60;
    int seconds = 60;
    QString duration = QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
    QLabel *durationLabel = new QLabel(duration);

    durationLabel->setFixedHeight(14);
    durationLabel->setFixedWidth(50);
    durationLabel->setStyleSheet("color: #828282; font-weight: bold; font-size: 12px; font-family: 'Tahoma';");
    trackLayout->addStretch();
    trackLayout->addWidget(durationLabel);

    //this->setStyleSheet("border: 1px solid red");
    setLayout(trackLayout);
    setFixedHeight(50);
    setFlat(true); // убирает стандартный фон
    setStyleSheet(R"(
    QPushButton {
        background-color: transparent;
        border: none;
        border-bottom: 1px solid rgba(255, 255, 255, 70);

    }
    QPushButton:hover {
        background-color: rgba(255, 255, 255, 30);
        outline: none;
    }
)");
    setFixedWidth(700);

}

//функция для получения usertag для отправки сигнала нажатия на юзера
QString *UserButton::getUsertag(){
    return &userData.usertag;
};


//конструктор получает список юзертегов и делает из него виджет со списком
UsersListWidget::UsersListWidget(QVector<UserInfo> users)
    : users(users)
{
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    //setStyleSheet("border: 1px solid red");
    centerWrapper = new QWidget();
    QVBoxLayout *centerLayout = new QVBoxLayout(centerWrapper);
    centerLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    QWidget *albumsWidget = new QWidget();
    albumsWidget->setFixedWidth(700);
    albumsWidget->setFixedHeight(40);
    QHBoxLayout *albumsLayout = new QHBoxLayout(albumsWidget);
    QLabel *albumsLabel = new QLabel("Users");
    albumsLabel->setStyleSheet(
        "padding-bottom: 0px;"
        "font-weight: bold;"
        "font-size: 23px;"
        "font-family: 'Tahoma';"
        "background: none;"
        "border: none;"
        "text-decoration: none;"
        "}"
        "QPushButton:hover {"
        "text-decoration: underline;");
    albumsLayout->addWidget(albumsLabel);
    containerWidget = new QWidget();

    trackListLayout = new QGridLayout(containerWidget);
    trackListLayout->addWidget(albumsWidget, 0, 0);
    trackListLayout->setContentsMargins(0,0,0,0);

    containerWidget->setLayout(trackListLayout);
    centerLayout->addWidget(containerWidget);
    scrollArea->setWidget(centerWrapper);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(scrollArea);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    buildGrid();
}
//функция строит сетку из юзеров
void UsersListWidget::buildGrid()
{
    int columns = 1;

    for (int i = 0; i < users.size(); ++i) {
        auto *userButton = new UserButton(users[i]);
        connect(userButton, &QPushButton::clicked, [this, userButton]() {
            emit userButtonClicked(userButton->getUsertag());
        });
        int row = i / columns+1;
        int col = i % columns;
        trackListLayout->addWidget(userButton, row, col);
    }

    containerWidget->adjustSize();
    centerWrapper->adjustSize();
}
