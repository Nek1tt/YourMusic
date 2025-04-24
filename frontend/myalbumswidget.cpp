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
//#include <QDebug>  // <--- добавь это


#include "myalbumswidget.h"

AlbumButton::AlbumButton(const struct album &albumData, QWidget *parent)
    : QPushButton(parent), albumData(albumData) {

    // Настроим макет кнопки
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Создаем метку для обложки
    QLabel *coverLabel = new QLabel(this);
    QPixmap coverPixmap(albumData.coverpath);
    coverLabel->setPixmap(coverPixmap.scaled(150, 150, Qt::KeepAspectRatio));
    layout->addWidget(coverLabel);  // Добавляем обложку в макет

    // Создаем метки для названия и автора
    QLabel *nameLabel = new QLabel(albumData.name, this);
    nameLabel->setFixedHeight(13);
    nameLabel->setStyleSheet("font-weight: bold; font-size: 11px; font-family: 'Tahoma';");
    nameLabel->setAlignment(Qt::AlignLeft);
    layout->addWidget(nameLabel);

    QLabel *authorLabel = new QLabel(albumData.author, this);
    authorLabel->setFixedHeight(12);
    authorLabel->setStyleSheet("color: #828282; font-weight: bold; font-size: 10px; font-family: 'Tahoma';");
    authorLabel->setAlignment(Qt::AlignLeft);
    layout->addWidget(authorLabel);

    setLayout(layout);
    setFixedSize(170, 200);
    // Устанавливаем размер кнопки
}
QString AlbumButton::getAlbumName(){
    return albumData.name;
}

album AlbumButton::getAlbum(){
    return albumData;
}


MyAlbumsWidget::MyAlbumsWidget(QWidget *parent) : QWidget(parent)
{

    MyAlbumsLayout = new QVBoxLayout(this);
    QPushButton *myAlbumButton = new QPushButton("My playlists");
    myAlbumButton->setStyleSheet(
        "QPushButton {"
        "font-weight: bold;"
        "font-size: 32px;"
        "font-family: 'Tahoma';"
        "    background: none;"                  // Убираем фон
        "    border: none;"                      // Убираем рамку
        //"    border: 2px solid blue;"  // рамка 2px
        "    text-decoration: none;"             // Убираем подчеркивание по умолчанию
        "}"
        "QPushButton:hover {"
        "    text-decoration: underline;"        // Подчеркиваем текст при наведении
        "}"
        );
    myAlbumButton->setFixedSize(200, 40);
    MyAlbumsLayout->addWidget(myAlbumButton);


    albums = new QWidget();  // виджет для прокрутки
    scrollAreaAlbums = new QScrollArea(this);  // прокручиваемая область

    // Создаем горизонтальный layout
    albumsLayout = new QHBoxLayout(albums);

    add_albums();

}


void MyAlbumsWidget::add_albums() {
    clearLayout(albumsLayout);
    albums_vector.clear();
    read_albums(albums_vector);

    // connect(albumButton, &QPushButton::clicked, this, [this, albumButton]() {
    //     emit albumClicked(albumButton->getAlbumName());
    // });


    // Добавляем элементы в layout
    for (auto& album : albums_vector) {
        //qDebug()<<"added";
        // Создаем кнопки для каждого альбома
        AlbumButton *albumButton = new AlbumButton(album);
        albumsLayout->addWidget(albumButton);

        connect(albumButton, &QPushButton::clicked, [this, albumButton]() {
            //qDebug() << "Открыт альбом: " << albumButton->getAlbumName();
            emit albumButtonClicked(albumButton->getAlbum());
        });
    }

    // Добавляем отступ в конец, чтобы при нехватке элементов они не сжимались
    albumsLayout->addStretch();

    // Обновляем размер родительского виджета
    albums->setLayout(albumsLayout);
    albums->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    albums->setMinimumWidth(albums_vector.size() * 180);  // Примерная ширина всех плейлистов
    albums->setMinimumHeight(200);

    scrollAreaAlbums->setWidget(albums);
    scrollAreaAlbums->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollAreaAlbums->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Удостоверимся, что `scrollAreaPLaylists` добавлен в layout только один раз
    if (MyAlbumsLayout->count() == 1) {
        MyAlbumsLayout->addWidget(scrollAreaAlbums);
    }

    scrollAreaAlbums->update();  // Принудительное обновление прокрутки
}


void MyAlbumsWidget::clearLayout(QLayout *layout) {
    if (layout == nullptr) return;

    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget(); // Удаляем виджет
        }
        delete item; // Удаляем сам элемент
    }
}

void MyAlbumsWidget::resizeAlbums(int width) {
    this->setFixedWidth(width*0.9);
}


void write_album(album &album){
    std::ofstream albumFile("../resources/text/playlists.txt", std::ios::app);
    albumFile<<album.name.toStdString()<<' '<<album.author.toStdString()<<' '<<album.coverpath.toStdString()<<std::endl;

    albumFile.close();
}

void write_albums(std::vector<album> &albums){
    std::ofstream albumFile("../resources/text/playlists.txt");
    for (auto& album :albums){
        albumFile<<album.name.toStdString()<<' '<<album.author.toStdString()<<' '
                     <<album.coverpath.toStdString()<<std::endl;
    }
    albumFile.close();
}
void read_albums(std::vector<album> &albums){
    std::ifstream albumFile("../resources/text/playlists.txt");
    std::string line;
    while (std::getline(albumFile, line)){
        QString qline = QString::fromStdString(line);
        QStringList words = qline.split(' ');
        //qDebug()<<words;
        album album = {words[0], words[1], words[2]};
        albums.push_back(album);
    }
    albumFile.close();
}
