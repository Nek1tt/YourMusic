#include <QScreen> //for Qscreen *screen = QApplication::primaryScreen();
#include <QApplication>
#include <QMainWindow>
#include <QString> // Для использования QString
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
// #include <QWebSocket>
#include <QMessageBox>
#include <QDir>
#include <fstream>
#include <vector>
#include <qDebug>

#include "myplaylistswidget.h"

PlaylistButton::PlaylistButton(const struct playlist &playlist, QWidget *parent = nullptr)
    : QPushButton(parent), playlist(playlist) {

    // Настроим макет кнопки
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Создаем метку для обложки
    QLabel *coverLabel = new QLabel(this);
    QPixmap coverPixmap(playlist.coverpath);
    coverLabel->setPixmap(coverPixmap.scaled(150, 150, Qt::KeepAspectRatio));
    layout->addWidget(coverLabel);  // Добавляем обложку в макет

    // Создаем метки для названия и автора
    QLabel *nameLabel = new QLabel(playlist.name, this);
    nameLabel->setFixedHeight(13);
    nameLabel->setStyleSheet("font-weight: bold; font-size: 11px; font-family: 'Tahoma';");
    nameLabel->setAlignment(Qt::AlignLeft);
    layout->addWidget(nameLabel);

    QLabel *authorLabel = new QLabel(playlist.artist, this);
    authorLabel->setFixedHeight(12);
    authorLabel->setStyleSheet("color: #828282; font-weight: bold; font-size: 10px; font-family: 'Tahoma';");
    authorLabel->setAlignment(Qt::AlignLeft);
    layout->addWidget(authorLabel);

    setLayout(layout);
    setFixedSize(170, 200);
    // Устанавливаем размер кнопки
}
QString PlaylistButton::getPlaylistName(){
    return playlist.name;
}




MyPlaylistsWidget::MyPlaylistsWidget(QWidget *parent) : QWidget(parent)
{

    QScreen *screen = QApplication::primaryScreen();// сохраняем экран
    screenGeometry = screen->availableGeometry();//извлекаем параметры экрана


    MyPlaylistsLayout = new QVBoxLayout(this);
    QPushButton *myPlaylistButton = new QPushButton("My playlists");
    myPlaylistButton->setStyleSheet(
        "QPushButton {"
        "font-weight: bold;"
        "font-size: 32px;"
        "font-family: 'Tahoma';"
        "    background: none;"                  // Убираем фон
        "    border: none;"                      // Убираем рамку
        "    text-decoration: none;"             // Убираем подчеркивание по умолчанию
        "}"
        "QPushButton:hover {"
        "    text-decoration: underline;"        // Подчеркиваем текст при наведении
        "}"
        );
    myPlaylistButton->setFixedSize(200, 40);
    MyPlaylistsLayout->addWidget(myPlaylistButton);


    playlists = new QWidget();  // виджет для прокрутки
    scrollAreaPLaylists = new QScrollArea(this);  // прокручиваемая область

    // Создаем горизонтальный layout
    playlistsLayout = new QHBoxLayout(playlists);

    add_playlists();

    // Устанавливаем layout для прокручиваемого виджета

    // Устанавливаем scrollArea в главный layout (или родительский виджет)

}


// проблема - после добавления плейлиста - слой начинает сжиматься сильно не увиличивается слой. так же если в начале нет альбомов, то после добавления альбомов - они не отображаются
void MyPlaylistsWidget::add_playlists() {
    clearLayout(playlistsLayout);

    read_playlists(playlists_vector);

    // Добавляем элементы в layout
    for (auto& playlist : playlists_vector) {
        // Создаем кнопки для каждого альбома
        PlaylistButton *albumButton = new PlaylistButton(playlist);
        playlistsLayout->addWidget(albumButton);

        connect(albumButton, &QPushButton::clicked, [albumButton]() {
            qDebug() << "Открыт альбом: " << albumButton->getPlaylistName();
        });
    }

    // Добавляем отступ в конец, чтобы при нехватке элементов они не сжимались
    playlistsLayout->addStretch();

    // Обновляем размер родительского виджета
    playlists->setLayout(playlistsLayout);
    playlists->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    playlists->setMinimumWidth(playlists_vector.size() * 180);  // Примерная ширина всех плейлистов
    playlists->setMinimumHeight(250);

    scrollAreaPLaylists->setWidget(playlists);
    scrollAreaPLaylists->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollAreaPLaylists->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Удостоверимся, что `scrollAreaPLaylists` добавлен в layout только один раз
    if (MyPlaylistsLayout->count() == 1) {
        MyPlaylistsLayout->addWidget(scrollAreaPLaylists);
    }

    scrollAreaPLaylists->update();  // Принудительное обновление прокрутки
}


void MyPlaylistsWidget::clearLayout(QLayout *layout) {
    if (layout == nullptr) return;

    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget(); // Удаляем виджет
        }
        delete item; // Удаляем сам элемент
    }
}

void MyPlaylistsWidget::resizePlaylists(int width) {
    this->setFixedWidth(width * 0.7);
}


void write_playlist(playlist &playlist){
    std::ofstream playlistFile("text/playlists.txt", std::ios::app);
    playlistFile<<playlist.name.toStdString()<<' '<<playlist.artist.toStdString()<<' '<<playlist.coverpath.toStdString()<<std::endl;

    playlistFile.close();
}

void write_playlists(std::vector<playlist> &playlists){
    std::ofstream playlistFile("text/playlists.txt");
    for (auto& playlist :playlists){
        playlistFile<<playlist.name.toStdString()<<' '<<playlist.artist.toStdString()<<' '
                     <<playlist.coverpath.toStdString()<<std::endl;
    }
    playlistFile.close();
}
void read_playlists(std::vector<playlist> &playlists){
    std::ifstream playlistFile("text/playlists.txt");
    std::string line;
    while (std::getline(playlistFile, line)){
        QString qline = QString::fromStdString(line);
        QStringList words = qline.split(' ');
        qDebug()<<words;
        playlist playlist = {words[0], words[1], words[2]};
        playlists.push_back(playlist);
    }
    playlistFile.close();
}



