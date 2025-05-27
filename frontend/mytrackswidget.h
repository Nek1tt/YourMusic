#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>


#ifndef MYTRACKSWIDGET_H
#define MYTRACKSWIDGET_H


struct track {
    int id;
    int album_id;
    QString name;
    int duration_ms;
    QString coverpath;
    QString author;
};

class TrackButton : public QPushButton {
    Q_OBJECT
public:
    explicit TrackButton(const track &trackData,QWidget *parent = nullptr);
    QString getTrackName();
    track* getTrack();
    void resize_trackbutton(int width);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    track trackData;

signals:
    void trackButtonClicked(track *trackData);
};


class MyTracksWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyTracksWidget(QWidget *parent = nullptr);
    void resize_tracks(int width);
    void clearLayout(QLayout *layout);
    void add_liked_tracks();
    void add_loaded_tracks();

    void onTrackdoubleClicked(track *trackData);

private:
    QVBoxLayout *MyTracksLayout;
    QPushButton *myTracksButton;
    QWidget *tracks;
    QHBoxLayout *tracks_layout_of_verticals;
    QVector<track> tracks_vector;
    std::vector<TrackButton*> trackButtons;

signals:
    void trackButtonClicked(track *trackData);
};


// void write_track(track &track);
// void read_tracks(std::vector<track> &tracks, std::string track_path);
QVector<track> readFromJson(const QString &filepath);

#endif // MYTRACKSWIDGET_H
