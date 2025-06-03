#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>
#include <QResizeEvent>
#include <QVector>
#include "myalbumswidget.h"


#ifndef ALBOMLISTWIDGET_H
#define ALBOMLISTWIDGET_H

class AlbomListWidget : public QWidget {
    Q_OBJECT

public:
    explicit AlbomListWidget(QVector<album> albumList, QWidget *parent = nullptr);

private:
    QScrollArea *scrollArea;
    QWidget *containerWidget;
    QWidget *centerWrapper;
    QGridLayout *gridLayout;
    QVector<AlbumButton> buttons;

    void buildGrid(QVector<album> albumList);

signals:
    void albumButtonClicked(const album albumData);
    void authorButtonClicked(int authorId);
};
#endif // ALBOMLISTWIDGET_H




