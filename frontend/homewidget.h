#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "myalbumswidget.h"

#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

class HomeWidget : public QWidget
{
    Q_OBJECT
public:
    HomeWidget();
    void homeButtonClicked(QVector<album> *newAlbums, QVector<album> *recAlbums, QVector<album> *randomAlbums);

private:
    MyAlbumsWidget *newAlbumsWidget;
    MyAlbumsWidget *recommendedAlbumsWidget;
    MyAlbumsWidget *randomAlbumsWidget;
};

#endif // HOMEWIDGET_H
