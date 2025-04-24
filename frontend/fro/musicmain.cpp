#include <QMainWindow>
#include <QPushButton>
//#include <QDebug>  // <--- добавь это
#include <QScreen> //for Qscreen *screen = QApplication::primaryScreen();
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QTabBar>
// #include <QWebSocket>
#include <QMessageBox>
#include <QDir>
#include <QSplitter>


#include "musicmain.h"
#include "./ui_musicmain.h"


MusicMain::MusicMain(QWidget *parent)//класс для окна
    : QMainWindow(parent)
{

    mainWidget = new QWidget();
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, mainWidget);
    mainLayout = new QHBoxLayout(mainWidget);

    leftBarWidget = new QWidget();
    //leftBarWidget->setFixedWidth(1);
    leftLayout = new QVBoxLayout(leftBarWidget);

    QWidget *navButtons = new QWidget();
    navButtons->setFixedHeight(100);
    QHBoxLayout *navLayout = new QHBoxLayout(navButtons);
    backButton = new QPushButton(" < ");
    forwardButton = new QPushButton(">");

    connect(backButton, &QPushButton::clicked, this, &MusicMain::on_backButton_clicked);
    connect(forwardButton, &QPushButton::clicked, this, &MusicMain::on_forwardButton_clicked);

    activeStyle = R"(
    QPushButton {
        background-color: #A0A0A0;
        border: none;
        border-radius: 15px;
        min-width: 30px;
        min-height: 30px;
        font-weight: bold;
        color: white;
    }
    QPushButton:hover {
        border: 1px solid white;
    }
)";

    inactiveStyle = R"(
    QPushButton {
        background-color: #555555; /* Более тёмный серый для неактивной */
        border: none;
        border-radius: 15px;
        min-width: 30px;
        min-height: 30px;
        font-weight: bold;
        color: #AAAAAA;
    }
)";
    backButton->setStyleSheet(inactiveStyle);
    forwardButton->setStyleSheet(inactiveStyle);
    navLayout->addWidget(backButton);
    navLayout->addWidget(forwardButton);

    homeTab = new QPushButton("Home");
    createTab = new QPushButton("Create");
    profileTab = new QPushButton("Profile");


    connect(homeTab, &QPushButton::clicked, this, &MusicMain::on_homeTab_clicked);
    connect(createTab, &QPushButton::clicked, this, &MusicMain::on_createTab_clicked);
    connect(profileTab, &QPushButton::clicked, this, &MusicMain::on_profileTab_clicked);

    leftLayout->addWidget(navButtons, 0, Qt::AlignTop);
    leftLayout->addWidget(homeTab);
    leftLayout->addWidget(createTab);
    leftLayout->addWidget(profileTab);

    mainSplitter->addWidget(leftBarWidget);

    tabwidget = new QTabWidget(this);
    //tabwidget->setFixedWidth(1);
    tabwidget->setStyleSheet("QTabWidget::pane { margin: 0px; padding: 0px; }");
    Create = new QWidget();
    Home = new QWidget();
    Profile = new QWidget();

    tabwidget->tabBar()->hide();
    tabwidget->addTab(Home, "Home");
    tabwidget->addTab(Create, "Create");
    tabwidget->addTab(Profile, "Profile");
    mainSplitter->addWidget(tabwidget);

    this->setCentralWidget(mainWidget);



    // // Создаем вкладку create
    createwidget = new CreateWidget(this, Create);
    profilewidget = new ProfileWidget(this, Profile);
    connect(profilewidget, &ProfileWidget::onAlbomClickedSignal, this, &MusicMain::on_albumButton_clicked);

    profilewidget->setContentsMargins(0, 0, 0, 0);

    rightbarwidget = new RightBarWidget(this, currentTrack);
    rightbarwidget->resize(400, this->height());
    mainSplitter->addWidget(rightbarwidget);
    mainLayout->addWidget(mainSplitter);
    mainWidget->setLayout(mainLayout);

    QScreen *screen = QApplication::primaryScreen();// сохраняем экран
    QRect screenGeometry = screen->availableGeometry();//извлекаем параметры экрана
    // // Получаем размер окна
    int windowWidth = this->width();
    int windowHeight = this->height();

    // // Вычисляем позицию для окна, чтобы правый верхний угол сопоставлялся с правым верхним углом экрана
    int xPos = screenGeometry.right() - windowWidth;
    int yPos = screenGeometry.top();

    // // Перемещаем окно в вычисленную позицию
    //this->move(xPos, yPos);
    // this->setStyleSheet("QWidget {border: 1px solid black}");

    // QHBoxLayout *mainLayout = new QHBoxLayout(this);
    // mainLayout->addWidget(tabwidget);
    // mainLayout->addWidget(rightbarwidget);
    // //создаем профиль пользователя с помощью соответствующего клас
    // tabwidget = ui->tabWidget; // это у нас объект с главными вкладками
    tabwidget->setTabVisible(0,0);// делаем видимым вкладку
    tabwidget->setCurrentIndex(0);// ставим текущим вкладку 0
    homeTab->setStyleSheet("color : black");
    createTab->setStyleSheet("color : white");
    profileTab->setStyleSheet("color : white");
    currentTab = homeTab;
    currentTab->setEnabled(false);


    mainTabButtons = { homeTab, createTab, profileTab}; //main tub buttons on screen


    this->resize(screenGeometry.width()/2, screenGeometry.height()/2); //seting the window size
    tabwidget->resize(this->width()-400,this->height());
    setInitialSize(this->width());

    // webSocket = new QWebSocket();
    // connect(webSocket, &QWebSocket::connected, this, &MusicMain::onConnected);
    // connect(webSocket, &QWebSocket::disconnected, this, &MusicMain::onDisconnected);
    // connect(webSocket, &QWebSocket::textMessageReceived, this, &MusicMain::onTextMessageReceived);
    // webSocket->open(QUrl("ws://84.237.53.143:881")); // Укажите правильный IP и порт
}



MusicMain::~MusicMain()
{
    // delete ui;
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
    //
    //qDebug()<<pushedButton->isFlat();
    currentTab->setEnabled(true);
    currentTab = pushedButton;
}


void MusicMain::on_homeTab_clicked()
{
    //qDebug()<<this->width();
    toggle_buttons(homeTab);
}


void MusicMain::on_createTab_clicked()
{
    //qDebug()<<"click";
    toggle_buttons(createTab);
}

void MusicMain::on_profileTab_clicked()
{
    //playlistwidget->add_playlists();
    profilewidget->button_profile_clicked();
    //qDebug()<<"click";
    toggle_buttons(profileTab);
}


void MusicMain::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event); // Вызов базового метода
    tabwidget->resize(this->width()-400,this->height());
    profilewidget->resizeProfile(this->width());
    //qDebug()<<this->width();
}

void MusicMain::setInitialSize(int width){
    profilewidget->resizeProfile(width);
}

void MusicMain::on_backButton_clicked(){
    int current = profilewidget->getCurrentIndex();
    if (current - 1 >= 0) {
        profilewidget->setCurrentIndex(current - 1);
        backButton->setStyleSheet(activeStyle);
    }
    //qDebug()<<"back";
    current = profilewidget->getCurrentIndex();
    if (current == 0) {
        backButton->setStyleSheet(inactiveStyle);
    }
    int total = profilewidget->getTotalIndex();
    if (current != total-1) {
        forwardButton->setStyleSheet(activeStyle);
    }
}

void MusicMain::on_forwardButton_clicked(){
    int current = profilewidget->getCurrentIndex();
    int total = profilewidget->getTotalIndex();

    if (current + 1 < total) {
        profilewidget->setCurrentIndex(current + 1);
    }
    //qDebug()<<"forward";

    current = profilewidget->getCurrentIndex();
    total = profilewidget->getTotalIndex();
    if (current == total-1) {
        forwardButton->setStyleSheet(inactiveStyle);
    }

    if (current != 0) {
        backButton->setStyleSheet(activeStyle);
    }
}

void MusicMain::on_albumButton_clicked(){
    backButton->setStyleSheet(activeStyle);
    int current = profilewidget->getCurrentIndex();
    int total = profilewidget->getTotalIndex();
    if (current == total-1) {
        forwardButton->setStyleSheet(inactiveStyle);
    }
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
