#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "hometab.h"
#include "myalbumswidget.h"
#include "profilewidget.h"

CreateTab::CreateTab(QString *usertag, WebSocketClient *webSocket, QWidget *parent)
    : AbstractTab(parent)
{
    // создаём стек виджетов для переключения между экранами
    innerStacked = new QStackedWidget();

    // основной виджет вкладки создания с передачей параметров
    mainWidget = new CreateWidget(usertag, webSocket);

    // виджет для скролла и его layout
    scrollWidget = new QWidget();
    mainLayout = new QVBoxLayout(scrollWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0); // убираем внешние отступы
    mainLayout->addWidget(mainWidget);

    // настраиваем scroll area для прокрутки содержимого
    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true); // чтобы содержимое масштабировалось по размеру окна
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // горизонтальная прокрутка отключена
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);    // вертикальная прокрутка по необходимости

    // добавляем scrollArea в стек виджетов
    innerStacked->addWidget(scrollArea);

    // создаём основной layout вкладки и добавляем в него стек
    QVBoxLayout *tabLayout = new QVBoxLayout(this);
    tabLayout->setContentsMargins(0, 0, 0, 0);
    tabLayout->addWidget(innerStacked);
}
