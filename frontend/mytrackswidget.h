#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>


#ifndef MYTRACKSWIDGET_H
#define MYTRACKSWIDGET_H


struct track
{
    QString name;
    QString author;
    QString coverpath;
    int min;
    int sec;
};

class TrackButton : public QPushButton {
    Q_OBJECT
public:
    explicit TrackButton(const track &track,QWidget *parent = nullptr);
    QString getTrackName();
    void resize_trackbutton(int width);

private:
    track track;
};


class MyTracksWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyTracksWidget(QWidget *parent = nullptr);
    void resize_tracks(int width);
    void clearLayout(QLayout *layout);
    void add_tracks();

private:
    QVBoxLayout *MyTracksLayout;
    QWidget *tracks;
    QHBoxLayout *tracks_layout_of_verticals;
    std::vector<track> tracks_vector;
    std::vector<TrackButton*> trackButtons;

};


void write_track(track &track);
void read_tracks(std::vector<track> &tracks, std::string track_path);


#endif // MYTRACKSWIDGET_H
