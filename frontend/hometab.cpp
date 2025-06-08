#include "profilewidget.h"
#include "hometab.h"

HomeTab::HomeTab(QWidget *tab) {
    innerStacked = new QStackedWidget();
    mainWidget = new HomeWidget();

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
}

void HomeTab::homeButtonClicked(QVector<album> *newAlbums, QVector<album> *recAlbums, QVector<album> *randomAlbums){
    QVector <album> albums_vector = loadAlbumsFromJson("resources/jsons/myalbums.json");
    mainWidget->homeButtonClicked(newAlbums, recAlbums, randomAlbums);
}

int AbstractTab::getCurrentIndex()  {
    return innerStacked->currentIndex(); // или что у тебя там
}

int AbstractTab::getTotalIndex()  {
    return innerStacked->count(); // или другой аналог
}

void AbstractTab::setCurrentIndex(int currentIndex){

    innerStacked->setCurrentIndex(currentIndex);
}
