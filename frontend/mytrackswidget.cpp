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
#include <string>
#include <qDebug>



#include "mytrackswidget.h"

TrackButton::TrackButton(const struct track &track, QWidget *parent)
    :QPushButton(parent), track(track)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *coverLabel = new QLabel(this);
    coverLabel->setFixedWidth(80);
    QPixmap coverPixmap(track.coverpath);
    coverLabel->setPixmap(coverPixmap.scaled(70, 70, Qt::KeepAspectRatio));
    layout->addWidget(coverLabel);  // Добавляем обложку в макет

    QVBoxLayout *name_and_author = new QVBoxLayout();
    QLabel *nameLabel = new QLabel(track.name);
    nameLabel->setFixedHeight(16);
    nameLabel->setStyleSheet("font-weight: bold; font-size: 14px; font-family: 'Tahoma';");
    nameLabel->setAlignment(Qt::AlignLeft);

    QLabel *authorLabel = new QLabel(track.author);
    authorLabel->setFixedHeight(14);
    authorLabel->setStyleSheet("color: #828282; font-weight: bold; font-size: 12px; font-family: 'Tahoma';");
    authorLabel->setAlignment(Qt::AlignLeft);

    name_and_author->addWidget(nameLabel);
    name_and_author->addWidget(authorLabel);
    layout->addLayout(name_and_author);

    QString duration = QString::number(track.min) + ":" + QString::number(track.sec);
    QLabel *durationLabel = new QLabel(duration);
    durationLabel->setFixedHeight(14);
    durationLabel->setStyleSheet("color: #828282; font-weight: bold; font-size: 12px; font-family: 'Tahoma';");
    durationLabel->setAlignment(Qt::AlignRight);
    layout->addWidget(durationLabel);

    setLayout(layout);
    setFixedSize(170, 90);

}

QString TrackButton::getTrackName(){
    return track.name;
}

void TrackButton::resize_trackbutton(int width){
    this->setFixedWidth(width*0.48);
}


MyTracksWidget::MyTracksWidget(QWidget *parent)  :QWidget(parent)
{
    MyTracksLayout = new QVBoxLayout(this);
    QPushButton *myTracksButton = new QPushButton("My tracks");
    myTracksButton->setStyleSheet(
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
    myTracksButton->setFixedSize(160, 40);
    MyTracksLayout->addWidget(myTracksButton);

    tracks = new QWidget();
    tracks_layout_of_verticals = new QHBoxLayout(tracks);
    tracks_layout_of_verticals->setContentsMargins(10,10,10,10); //отступ со всех сторон


    add_tracks();

}

void MyTracksWidget::add_tracks() {
    clearLayout(tracks_layout_of_verticals);
    tracks_vector.clear();
    read_tracks(tracks_vector, "text/tracks.txt");

    QVBoxLayout *leftColumn = new QVBoxLayout();
    QVBoxLayout *rightColumn = new QVBoxLayout();

    for (int i=0;i<tracks_vector.size(); i++){
        if (i==3){
            break;
        }
        track track = tracks_vector[i];
        TrackButton *trackButton = new TrackButton(track);
        trackButtons.push_back(trackButton);
        leftColumn->addWidget(trackButton);

        connect(trackButton, &QPushButton::clicked, [trackButton]() {
            qDebug() << "Открыт трек: " << trackButton->getTrackName();
        });

    }

    for (int i=3;i<tracks_vector.size(); i++){
        if (i==6){
            break;
        }
        track track = tracks_vector[i];
        TrackButton *trackButton = new TrackButton(track);
        trackButtons.push_back(trackButton);
        rightColumn->addWidget(trackButton);

        connect(trackButton, &QPushButton::clicked, [trackButton]() {
            qDebug() << "Открыт трек: " << trackButton->getTrackName();
        });

    }

    tracks_layout_of_verticals->addLayout(leftColumn);
    tracks_layout_of_verticals->addSpacing(20); // Отступ в 20 пикселей
    tracks_layout_of_verticals->addLayout(rightColumn);

    tracks->setLayout(tracks_layout_of_verticals);

    if (MyTracksLayout->count() == 1) {
        MyTracksLayout->addWidget(tracks);
    }
}

void MyTracksWidget::clearLayout(QLayout *layout) {
    if (layout == nullptr) return;

    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget(); // Удаляем виджет
        }
        delete item; // Удаляем сам элемент
    }
}

//добавить отступ между рядами в tracks и соответсвенно выровнять все красиво
void MyTracksWidget::resize_tracks(int width) {
    tracks->setFixedWidth(width * 0.6);
    qDebug() << "track width changing";

    // Проходим по всем TrackButton в векторе и изменяем их размер
    for (TrackButton* trackButton : trackButtons) {
        if (trackButton) {
            trackButton->resize_trackbutton(width * 0.6);  // Изменяем размер
        }
    }
}


void write_track(track &track){
    std::ofstream tracksFile("text/tracks.txt", std::ios::app);
    tracksFile << track.name.toStdString() << ' '
               << track.author.toStdString() << ' '
               << track.coverpath.toStdString() << ' '
               << track.min << ' ' << track.sec << std::endl;

    tracksFile.close();
}

void read_tracks(std::vector<track> &tracks, std::string track_path){
    std::ifstream trackFile(track_path);
    std::string line;
    while (std::getline(trackFile, line)){
        QString qline = QString::fromStdString(line);
        QStringList words = qline.split(' ');
        qDebug()<<words;
        track track = {words[0], words[1], words[2], words[3].toInt(), words[4].toInt()};
        tracks.push_back(track);
    }
    trackFile.close();
}
