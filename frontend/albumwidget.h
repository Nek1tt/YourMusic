#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>

#include "mytrackswidget.h"

#ifndef ALBUMWIDGET_H
#define ALBUMWIDGET_H

class AlbumWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlbumWidget(const track &trackWidget, QWidget *parent = nullptr);

private:
    track trackWidget;
};

#endif // ALBUMWIDGET_H
