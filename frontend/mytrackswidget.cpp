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
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFontMetrics>
// #include <QWebSocket>
#include <QMessageBox>
#include <QDir>
#include <fstream>
#include <vector>
#include <string>
//#include <QDebug>  // <--- добавь это



#include "mytrackswidget.h"
#include "setstyle.h"

TrackButton::TrackButton(const struct track &trackData, QWidget *parent)
    :QPushButton(parent), trackData(trackData)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *coverLabel = new QLabel(this);
    setRoundedImage(coverLabel, trackData.coverpath, 70, 15);
    //coverLabel->setFixedWidth(80);
    //QPixmap coverPixmap(trackData.coverpath);
    //coverLabel->setPixmap(coverPixmap.scaled(70, 70, Qt::KeepAspectRatio));
    layout->addWidget(coverLabel);  // Добавляем обложку в макет

    QVBoxLayout *name_and_author = new QVBoxLayout();
    QPushButton *nameButton = new QPushButton(trackData.name);
    connect(nameButton, &QPushButton::clicked, [this]() {
        emit trackNameButtonClicked();
    });
    //nameButton->setFixedHeight(16);
    QFontMetrics fm(nameButton->font());
    int textWidth = fm.horizontalAdvance(trackData.name);
    nameButton->setFixedWidth(textWidth + 25); // +10 — небольшой отступ по краям
    //nameButton->setStyleSheet("font-weight: bold; font-size: 14px; font-family: 'Tahoma'; text-align: left;");
    set_button_style(nameButton, 14, "white");
    //nameButton->setAlignment(Qt::AlignLeft);

    QPushButton *authorButton = new QPushButton(trackData.author);
    connect(authorButton, &QPushButton::clicked, [this]() {
        emit trackAuthorButtonClicked();
    });
    authorButton->setFixedHeight(14);
    //int widthOfAuthor = trackData.author.size()*7;
    QFontMetrics fm_au(authorButton->font());
    int textWidth_au = fm_au.horizontalAdvance(trackData.author);
    authorButton->setFixedWidth(textWidth_au + 20); // +10 — небольшой отступ по краям

    set_button_style(authorButton, 12, "#828282");
    //authorButton->setStyleSheet("color: #828282; font-weight: bold; font-size: 12px; font-family: 'Tahoma';");
    //authorButton->setAlignment(Qt::AlignLeft);

    name_and_author->addWidget(nameButton);
    name_and_author->addWidget(authorButton);
    layout->addLayout(name_and_author);

    int minutes = trackData.duration_ms / 1000 / 60;
    int seconds = trackData.duration_ms / 1000 % 60;
    QString duration = QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));

    QLabel *durationLabel = new QLabel(duration);
    durationLabel->setFixedHeight(14);
    durationLabel->setStyleSheet("color: #828282; font-weight: bold; font-size: 12px; font-family: 'Tahoma';");
    durationLabel->setAlignment(Qt::AlignRight);
    layout->addWidget(durationLabel);

    setLayout(layout);
    setFixedSize(170, 90);

    this->setStyleSheet(
        "QPushButton {"
        "    background: none;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 30);"
        "    border: none;"
        "    outline: none;"
        "}"
        );

}

QString TrackButton::getTrackName(){
    return trackData.name;
}
int TrackButton::getTrackId(){
    return trackData.id;
}

track * TrackButton::getTrack(){
    return &trackData;
}


void TrackButton::resize_trackbutton(int width){
    this->setFixedWidth(width*0.48);
}

void TrackButton::mouseDoubleClickEvent(QMouseEvent *event){
    emit trackButtonClicked(getTrack());
}



MyTracksWidget::MyTracksWidget(QString widgetName, QWidget *parent)  :QWidget(parent)
{
    MyTracksLayout = new QVBoxLayout(this);

    QWidget *myTracksButtonWidget = new QWidget();
    QVBoxLayout *myTracksButtonLayout = new QVBoxLayout(myTracksButtonWidget);
    QPushButton *myTracksButton = new QPushButton(widgetName);
    connect(myTracksButton, &QPushButton::clicked, [this]() {
        emit mytracksButtonClicked(getAlbum());
    });
    set_button_style(myTracksButton, 32, "white", "left");
    //myTracksButton->setFixedSize(300, 40);

    myTracksButtonLayout->addWidget(myTracksButton);
    MyTracksLayout->addWidget(myTracksButtonWidget);

    tracks = new QWidget();
    tracks_layout_of_verticals = new QHBoxLayout(tracks);
    tracks_layout_of_verticals->setContentsMargins(10,10,10,10); //отступ со всех сторон


}

void MyTracksWidget::add_liked_tracks(album newTracks) {
    likedTracks = newTracks;

    clearLayout(tracks_layout_of_verticals);
    QVector<track> tracks_vector = newTracks.tracks;

    QVBoxLayout *leftColumn = new QVBoxLayout();
    QVBoxLayout *rightColumn = new QVBoxLayout();

    for (int i = 0; i < tracks_vector.size(); ++i) {
        if (i >= 6) break;

        track tr = tracks_vector[i];
        TrackButton *trackButton = new TrackButton(tr);
        trackButtons.push_back(trackButton);

        if (i < 3) {
            leftColumn->addWidget(trackButton);
        } else {
            rightColumn->addWidget(trackButton);
        }

        connect(trackButton, &TrackButton::trackButtonClicked, this, &MyTracksWidget::onTrackdoubleClicked);
        connect(trackButton, &TrackButton::trackNameButtonClicked, [this, trackButton]() {
            emit trackNameButtonClicked(trackButton->getTrackId());
        });
        connect(trackButton, &TrackButton::trackAuthorButtonClicked, [this, trackButton]() {
            emit authorButtonClicked(trackButton->getTrackId());
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
    tracks->setFixedWidth(width * 0.8);

    // Проходим по всем TrackButton в векторе и изменяем их размер
    for (TrackButton* trackButton : trackButtons) {
        if (trackButton) {
            trackButton->resize_trackbutton(width * 0.8);  // Изменяем размер
        }
    }
}

void MyTracksWidget::onTrackdoubleClicked(track *trackData){
    emit trackButtonClicked(trackData);
}


// void write_track(track &track){
//     std::ofstream tracksFile("../resources/text/tracks.txt", std::ios::app);
//     tracksFile << track.name.toStdString() << ' '
//                << track.author.toStdString() << ' '
//                << track.coverpath.toStdString() << ' '
//                << track.min << ' ' << track.sec << std::endl;

//     tracksFile.close();
// }

// void read_tracks(std::vector<track> &tracks, std::string track_path){
//     std::ifstream trackFile(track_path);
//     std::string line;
//     while (std::getline(trackFile, line)){
//         QString qline = QString::fromStdString(line);
//         QStringList words = qline.split(' ');
//         track track = {words[0], words[1], words[2], words[3].toInt(), words[4].toInt()};
//         tracks.push_back(track);
//     }
//     trackFile.close();
// }
QVector<track> readFromJson(const QString& filePath) {
    QVector<track> trackList;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << filePath;
        return trackList;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return trackList;
    }

    QJsonArray trackArray = doc.object()["tracks"].toArray();
    for (const QJsonValue& val : trackArray) {
        QJsonObject obj = val.toObject();
        track t;
        t.id = obj["id"].toInt();
        t.name = obj["title"].toString();
        t.duration_ms = obj["durationMs"].toInt();
        t.coverpath = obj["coverPath"].toString();
        t.author = obj["author"].toString();
        t.album_id = obj["albumId"].toInt();
        trackList.append(t);
    }

    return trackList;
}

QString MyTracksWidget::getAlbumName(){
    return likedTracks.name;
}

album MyTracksWidget::getAlbum(){
    return likedTracks;
}
