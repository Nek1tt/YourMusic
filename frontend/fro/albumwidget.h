#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>

#include "mytrackswidget.h"
#include "myalbumswidget.h"
#ifndef ALBUMWIDGET_H
#define ALBUMWIDGET_H

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
