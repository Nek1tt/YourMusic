#include <QScreen> //for Qscreen *screen = QApplication::primaryScreen();
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
// #include <QWebSocket>
#include <QMessageBox>
#include <QDir>


#include "musicmain.h"
#include "./ui_musicmain.h"



MusicMain::MusicMain(QWidget *parent)//класс для окна
    : QMainWindow(parent), ui(new Ui::MusicMain)
    //profilewidget(new ProfileWidget(this, ui->Profile))
    //userProfile(new UserProfileWidget(this)),
    //playlistwidget(new MyPlaylistsWidget(this))
    //createwidget(new CreateWidget(this,ui->Create))
{
    ui->setupUi(this);
    // Создаем вкладку create
    createwidget = new CreateWidget(this, ui->Create);
    profilewidget = new ProfileWidget(this, ui->Profile);
    QScreen *screen = QApplication::primaryScreen();// сохраняем экран
    QRect screenGeometry = screen->availableGeometry();//извлекаем параметры экрана
    // Получаем размер окна
    int windowWidth = this->width();
    //int windowHeight = this->height();

    // Вычисляем позицию для окна, чтобы правый верхний угол сопоставлялся с правым верхним углом экрана
    int xPos = screenGeometry.right() - windowWidth;
    int yPos = screenGeometry.top();

    // Перемещаем окно в вычисленную позицию
    this->move(xPos, yPos);
    //создаем профиль пользователя с помощью соответствующего класса
    //UserInfo usernurshat{"imgs/ava.png", "User Name", "@usertag", 100, 100, 100, 100};
    //userProfile->setUserProfile(usernurshat);//заполняем профиль
    //QVBoxLayout *profileLayout = new QVBoxLayout(ui->Profile); //создаем слой для вкладки профиль. в нем будет храниться профильинфо, плейлисты, треки все
    //profileLayout->setContentsMargins(50, 50, 50, 50); //ставим отступы внутри слоя
    //profileLayout->addWidget(userProfile, 0, Qt::AlignTop); //добавляем в этот слой профиля userProfile - "шапка профиля с инфой"
    //userProfile->setStyleSheet("QWidget { border: 1px solid red; }");
    //profileLayout->addWidget(playlistwidget);

    // Создаем виджет для прокрутки
    // scrollWidget = new QWidget();  // Виджет для прокручиваемого содержимого
    /*scrollWidget->setLayout(profileLayout);*/ // Устанавливаем layout в scrollWidget

    // Создаем QScrollArea
    //QScrollArea *scrollArea = new QScrollArea(this);  // Прокручиваемая область
    // scrollArea->setWidget(scrollWidget);  // Устанавливаем scrollWidget в scrollArea

    // Настроим прокрутку
    /*scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);*/  // Отключаем горизонтальную прокрутку
    /*scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);*/ // Прокрутка будет по вертикали только если нужно

    // Устанавливаем scrollArea в основной layout профиля
    /*ui->Profile->setLayout(new QVBoxLayout());*/  // Очистить текущий layout
    // ui->Profile->layout()->addWidget(scrollArea);


    // ui->Profile->setStyleSheet("QWidget { border: 1px solid blue; }");
    // playlistwidget->setStyleSheet("QWidget { border: 1px solid red; }");




    //создаем профиль пользователя с помощью соответствующего клас
    tabwidget = ui->tabWidget; // это у нас объект с главными вкладками
    tabwidget->setTabVisible(0,0);// делаем видимым вкладку
    tabwidget->setCurrentIndex(0);// ставим текущим вкладку 0
    ui->homeTab->setStyleSheet("color : black");
    ui->createTab->setStyleSheet("color : white");
    ui->profileTab->setStyleSheet("color : white");
    currentTab = ui->homeTab;
    currentTab->setEnabled(false);


    mainTabButtons = { ui->homeTab, ui->createTab, ui->profileTab}; //main tub buttons on screen


    tabwidget->resize(screenGeometry.width(), screenGeometry.height()); //seting the window size


    setInitialSize(this->width());

    // webSocket = new QWebSocket();
    // connect(webSocket, &QWebSocket::connected, this, &MusicMain::onConnected);
    // connect(webSocket, &QWebSocket::disconnected, this, &MusicMain::onDisconnected);
    // connect(webSocket, &QWebSocket::textMessageReceived, this, &MusicMain::onTextMessageReceived);
    // webSocket->open(QUrl("ws://84.237.53.143:881")); // Укажите правильный IP и порт
}



MusicMain::~MusicMain()
{
    delete ui;
}

void MusicMain::toggle_buttons(QPushButton* pushedButton){ // changes the button from which we switched to a new tab to white. and
    for (int i=0; i<3; i++){
        if(mainTabButtons[i]==pushedButton){
            tabwidget->setCurrentIndex(i);
        }
    }
    currentTab->setStyleSheet("color : white");
    pushedButton->setEnabled(false);
    pushedButton->setStyleSheet("color : black");
    pushedButton->setFlat(true);
    qDebug()<<pushedButton->isFlat();
    currentTab->setEnabled(true);
    currentTab = pushedButton;
}


void MusicMain::on_homeTab_clicked()
{
    qDebug()<<this->width();
    toggle_buttons(ui->homeTab);
}


void MusicMain::on_createTab_clicked()
{
    qDebug()<<"click";
    toggle_buttons(ui->createTab);
}

void MusicMain::on_profileTab_clicked()
{
    //playlistwidget->add_playlists();
    profilewidget->button_profile_clicked();
    qDebug()<<"click";
    toggle_buttons(ui->profileTab);
}


void MusicMain::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event); // Вызов базового метода
    profilewidget->resizeProfile(this->width());
    qDebug()<<this->width();
}

void MusicMain::setInitialSize(int width){
    profilewidget->resizeProfile(width);
}



// void MusicMain::on_toserver_clicked()
// {
//     if (webSocket->state() == QAbstractSocket::ConnectedState) {
//         // Отправляем сообщение на сервер
//         webSocket->sendTextMessage(R"({"command": "play_track", "track_id": 1})");
//     } else {
//         QMessageBox::warning(this, "Ошибка", "Нет подключения к серверу");
//     }
// }

// void MusicMain::onConnected() {
//     QMessageBox::information(this, "Успех", "Подключение к серверу установлено");
// }

// void MusicMain::onDisconnected() {
//     QMessageBox::warning(this, "Ошибка", "Соединение с сервером разорвано");
// }

// void MusicMain::onTextMessageReceived(const QString &message) {
//     qDebug()<<message;
//     QMessageBox::information(this, "Ответ от сервера", message);

// }
