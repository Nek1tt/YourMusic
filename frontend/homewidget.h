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
    void homeButtonClicked(QVector<album> album_list);

private:
    MyAlbumsWidget *newAlbumsWidget;
    MyAlbumsWidget *recommendedAlbumsWidget;
};

#endif // HOMEWIDGET_H
