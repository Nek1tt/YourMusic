#ifndef ALBOMLISTWIDGET_H
#define ALBOMLISTWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QVector>
#include "myalbumswidget.h" //отсюда берем album и AlbumButton

class AlbomListWidget : public QWidget {
    Q_OBJECT

public:
    explicit AlbomListWidget(QVector<album> albumList, QWidget *parent = nullptr);

signals:
    void albumButtonClicked(const album albumData);
    void authorButtonClicked(QString *trackUsertag);

private:
    QScrollArea *scrollArea;
    QWidget *containerWidget;
    QWidget *centerWrapper;
    QGridLayout *gridLayout;
    QVector<AlbumButton> buttons;

    void buildGrid(QVector<album> albumList);
};

#endif // ALBOMLISTWIDGET_H
