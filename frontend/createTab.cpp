#include <QScreen> //for Qscreen *screen = QApplication::primaryScreen();
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
// #include <QWebSocket>
#include <QMessageBox>
#include <QDir>


#include "hometab.h"
#include "myalbumswidget.h"
#include "profilewidget.h"

CreateTab::CreateTab(QString *usertag, WebSocketClient *webSocketStas, QWidget *parent)
    : AbstractTab(parent)
{
    // Initialize buttons
    innerStacked = new QStackedWidget();
    mainWidget = new CreateWidget(usertag, webSocketStas);

    scrollWidget = new QWidget();
    mainLayout = new QVBoxLayout(scrollWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(mainWidget);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Добавляем scrollArea как главный экран в стек
    innerStacked->addWidget(scrollArea);
    //mainWidget->setStyleSheet("QWidget { border: 1px solid red; }");

    // Размещаем стек в основной вкладке
    QVBoxLayout *tabLayout = new QVBoxLayout(this);
    tabLayout->setContentsMargins(0, 0, 0, 0);
    tabLayout->addWidget(innerStacked);
    setStyleSheet("border: 1px, solid, red");

    // Set the layout of the Create tab to this layout
   // createtab->setLayout(createLayout);
}

