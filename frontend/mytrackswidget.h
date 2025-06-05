#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>

#include "myalbumswidget.h"

#ifndef MYTRACKSWIDGET_H
#define MYTRACKSWIDGET_H


class TrackButton : public QPushButton {
    Q_OBJECT
public:
    explicit TrackButton(const track &trackData,QWidget *parent = nullptr);
    QString getTrackName();
    track* getTrack();
    int getTrackId();
    void resize_trackbutton(int width);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    track trackData;

signals:
    void trackButtonClicked(track *trackData);
    void trackNameButtonClicked();
    void trackAuthorButtonClicked();
};


class MyTracksWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyTracksWidget(QString widgetName, QWidget *parent = nullptr);
    void resize_tracks(int width);
    void clearLayout(QLayout *layout);
    void add_liked_tracks(album newTracks);
    //void add_loaded_tracks(album loadedTracks);
    QString getAlbumName();
    album getAlbum();

    void onTrackdoubleClicked(track *trackData);

private:
    QVBoxLayout *MyTracksLayout;
    QPushButton *myTracksButton;
    QWidget *tracks;
    QHBoxLayout *tracks_layout_of_verticals;
    album likedTracks;
    //album loadedTracks;
    //QVector<track> tracks_vector;
    std::vector<TrackButton*> trackButtons;

signals:
    void trackButtonClicked(track *trackData);
    void trackNameButtonClicked(int trackId);
    void authorButtonClicked(int authorId);
    void mytracksButtonClicked(const album albumData);
};


// void write_track(track &track);
// void read_tracks(std::vector<track> &tracks, std::string track_path);
QVector<track> readFromJson(const QString &filepath);

#endif // MYTRACKSWIDGET_H
