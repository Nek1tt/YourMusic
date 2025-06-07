#include <QScreen> //for Qscreen *screen = QApplication::primaryScreen();
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
// #include <QWebSocket>
#include <QMessageBox>
#include <QDir>

#include "websocketclient.h"
#ifndef CREATEWIDGET_H
#define CREATEWIDGET_H

class CreateTab : public QWidget
{
    Q_OBJECT
public:
    explicit CreateTab(, QWidget *parent = nullptr, QWidget *tab = nullptr);


private slots:
    void onAddNoizeClicked();     // Slot for Add Noize button click
    void onAddAnacondazClicked(); // Slot for Add Anacondaz button click
    void onAddOxxyClicked();

private:
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    //QWidget *tab;
    QVBoxLayout *createLayout;
};

#endif // CREATEWIDGET_H
