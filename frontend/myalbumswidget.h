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
    int duration_s;
    QString coverpath;
    QVector<QString> authors;
    QString upload_date;
};


struct album {
    int id;
    QString name;
    QString coverpath;
    QString authorUsername;
    QString authorUsertag;
    int track_count;
    QVector<track> tracks;
    QString createDate;
    QString description;
    int total_duration;
};

class AlbumButton : public QPushButton {
    Q_OBJECT
public:
    explicit AlbumButton(const album &albumData, QWidget *parent = nullptr);

    QString getAlbumName();
    album getAlbum();
    QString  getAuthorUsertag();
    void loadCover(const QString& url, QLabel *label);

signals:
    void albumNameButtonClicked();
    void authorButtonClicked();
private:
    album albumData;
};

class MyAlbumsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAlbumsWidget(QString widgetName, QWidget *parent = nullptr);
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
    void authorButtonClicked(QString authorUsertag);
    void myAlbumsButtonClicked(const QVector<album> albumList);
};


// void write_albums(std::vector<album> &albums);
// void write_album(album &album);
// void read_albums(std::vector<album> &albums);
QVector<album> loadAlbumsFromJson(const QString &filePath);
album loadSingleAlbumFromJson(const QString &filePath);

#endif // MYPLAYLISTSWIDGET_H
