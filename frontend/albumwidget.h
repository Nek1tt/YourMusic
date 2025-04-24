#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>

#include "mytrackswidget.h"
#include "myalbumswidget.h"
#ifndef ALBUMWIDGET_H
#define ALBUMWIDGET_H

class AlbumTrackButton : public QPushButton {
    Q_OBJECT
public:
    explicit AlbumTrackButton(const track &trackData, QString number, QWidget *parent = nullptr);
    //QString getTrackName();
    //void resize_trackbutton(int width);

private:
    track trackData;
    QString number;
};

class AlbumWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlbumWidget(const album &albumData, QWidget *parent = nullptr);
    AlbumWidget(const track &trackData, QWidget *parent = nullptr);
private:
    album albumData;
};

#endif // ALBUMWIDGET_H
