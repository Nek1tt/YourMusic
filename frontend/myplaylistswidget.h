#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>


#ifndef MYPLAYLISTSWIDGET_H
#define MYPLAYLISTSWIDGET_H

struct playlist
{
    QString name;
    QString artist = "";
    QString coverpath;
};

class PlaylistButton : public QPushButton {
    Q_OBJECT
public:
    explicit PlaylistButton(const playlist &playlist, QWidget *parent = nullptr);

    QString getPlaylistName();

private:
    playlist playlist;
};

class MyPlaylistsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyPlaylistsWidget(QWidget *parent = nullptr);
    void clearLayout(QLayout *layout);
    void add_playlists();
    void resizePlaylists(int width);

private:
    std::vector<playlist> playlists_vector = {};
    QRect screenGeometry;
    QVBoxLayout *MyPlaylistsLayout;
    QWidget *playlists;
    QHBoxLayout *playlistsLayout;
    QScrollArea *scrollAreaPLaylists;

};


void write_playlists(std::vector<playlist> &playlists);
void write_playlist(playlist &playlist);
void read_playlists(std::vector<playlist> &playlists);

#endif // MYPLAYLISTSWIDGET_H
