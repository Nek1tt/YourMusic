#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>

#ifndef MYPLAYLISTSWIDGET_H
#define MYPLAYLISTSWIDGET_H

struct track {
    int id;
    int album_id;
    QString name;
    int duration_ms;
    QString coverpath;
    QString author;
};


struct album {
    int id;
    QString name;
    QString coverpath;
    QString author;
    int track_count;
    QVector<track> tracks;
};

class AlbumButton : public QPushButton {
    Q_OBJECT
public:
    explicit AlbumButton(const album &albumData, QWidget *parent = nullptr);

    QString getAlbumName();
    album getAlbum();

private:
    album albumData;
};

class MyAlbumsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAlbumsWidget(QWidget *parent = nullptr);
    void clearLayout(QLayout *layout);
    void add_albums(QVector<album> newAlbumList);
    void resizeAlbums(int width);
    QVector<album> getAlbum();

private:
    QVector<album> albums_vector = {};
    QRect screenGeometry;
    QVBoxLayout *MyAlbumsLayout;
    QWidget *albums;
    QHBoxLayout *albumsLayout;
    QScrollArea *scrollAreaAlbums;

signals:
    void albumButtonClicked(const album albumData);
    void myAlbumsButtonClicked(const QVector<album> albumList);
};


// void write_albums(std::vector<album> &albums);
// void write_album(album &album);
// void read_albums(std::vector<album> &albums);
QVector<album> loadAlbumsFromJson(const QString &filePath);
album loadSingleAlbumFromJson(const QString &filePath);

#endif // MYPLAYLISTSWIDGET_H
