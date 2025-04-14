#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>


#ifndef MYPLAYLISTSWIDGET_H
#define MYPLAYLISTSWIDGET_H

struct album
{
    QString name;
    QString author = "";
    QString coverpath;
};

class AlbumButton : public QPushButton {
    Q_OBJECT
public:
    explicit AlbumButton(const album &album, QWidget *parent = nullptr);

    QString getAlbumName();

private:
    album album;
};

class MyAlbumsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAlbumsWidget(QWidget *parent = nullptr);
    void clearLayout(QLayout *layout);
    void add_albums();
    void resizeAlbums(int width);

private:
    std::vector<album> albums_vector = {};
    QRect screenGeometry;
    QVBoxLayout *MyAlbumsLayout;
    QWidget *albums;
    QHBoxLayout *albumsLayout;
    QScrollArea *scrollAreaAlbums;

signals:
    void albumButtonClicked(const QString AlbumName);

};


void write_albums(std::vector<album> &albums);
void write_album(album &album);
void read_albums(std::vector<album> &albums);

#endif // MYPLAYLISTSWIDGET_H
