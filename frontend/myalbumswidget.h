#ifndef MYPLAYLISTSWIDGET_H
#define MYPLAYLISTSWIDGET_H

// Qt includes
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QVector>
#include <QString>
#include <QLayout>
#include <QRect>

// === DATA STRUCTURES ===

struct track {
    int id;
    int album_id;
    QString name;
    int duration_s;
    QString coverpath;
    QVector<QString> authors;
    QVector<QString> authorUsertags;
    QString upload_date;
    QString description;
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

// === AlbumButton ===

class AlbumButton : public QPushButton {
    Q_OBJECT

public:
    explicit AlbumButton(const album &albumData, QWidget *parent = nullptr);

    QString getAlbumName();
    album getAlbum();
    QString getAuthorUsertag();

    void loadCover(const QString &url, QLabel *label);

signals:
    void albumNameButtonClicked();
    void authorButtonClicked();

private:
    album albumData;
};

// === MyAlbumsWidget ===

class MyAlbumsWidget : public QWidget {
    Q_OBJECT

public:
    explicit MyAlbumsWidget(QString widgetName, QWidget *parent = nullptr);

    void clearLayout(QLayout *layout);
    void add_albums(QVector<album> newAlbumList);
    void resizeAlbums(int width);
    QVector<album> getAlbum();

signals:
    void albumButtonClicked(const album albumData);
    void authorButtonClicked(QString *authorUsertag);
    void myAlbumsButtonClicked(const QVector<album> albumList);

private:
    QVector<album> albums_vector {};
    QRect screenGeometry;

    QVBoxLayout *MyAlbumsLayout;
    QWidget *albums;
    QHBoxLayout *albumsLayout;
    QScrollArea *scrollAreaAlbums;
};

// === JSON I/O Functions ===

QVector<album> loadAlbumsFromJson(const QString &filePath);
album loadSingleAlbumFromJson(const QString &filePath);


#endif // MYPLAYLISTSWIDGET_H
