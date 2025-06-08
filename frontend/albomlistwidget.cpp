#include "albomlistwidget.h"
#include "setstyle.h"
#include <QDebug>

AlbomListWidget::AlbomListWidget(QVector<album> albumList, QWidget *parent)
    : QWidget(parent)
{
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    //setStyleSheet("border: 1px solid red");
    centerWrapper = new QWidget();
    QVBoxLayout *centerLayout = new QVBoxLayout(centerWrapper);
    centerLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    QWidget *albumsWidget = new QWidget();
    albumsWidget->setFixedHeight(40);
    QHBoxLayout *albumsLayout = new QHBoxLayout(albumsWidget);
    QLabel *albumsLabel = new QLabel("Albums");
    albumsLabel->setStyleSheet(
          "padding-left: 5px;"
        "padding-bottom: 0px;"
     "font-weight: bold;"
     "font-size: 23px;"
      "font-family: 'Tahoma';"
      "background: none;"
      "border: none;"
      "text-decoration: none;"
      "}"
      "QPushButton:hover {"
      "text-decoration: underline;");
    albumsLayout->addWidget(albumsLabel);
    centerLayout->addWidget(albumsWidget);
    containerWidget = new QWidget();
    gridLayout = new QGridLayout(containerWidget);
    gridLayout->setSpacing(10);
    gridLayout->setContentsMargins(10, 10, 10, 10);

    containerWidget->setLayout(gridLayout);
    centerLayout->addWidget(containerWidget);
    scrollArea->setWidget(centerWrapper);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(scrollArea);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    buildGrid(albumList);
}

void AlbomListWidget::buildGrid(QVector<album> albumList)
{
    int columns = 4;

    for (int i = 0; i < albumList.size(); ++i) {
        auto *albumButton = new AlbumButton(albumList[i]);
        connect(albumButton, &QPushButton::clicked, [this, albumButton]() {
            emit albumButtonClicked(albumButton->getAlbum());
        });

        connect(albumButton, &AlbumButton::albumNameButtonClicked, [this, albumButton]() {
            emit albumButtonClicked(albumButton->getAlbum());
        });
        connect(albumButton, &AlbumButton::authorButtonClicked, [this, albumButton]() {
            emit authorButtonClicked(albumButton->getAuthorUsertag());
        });
        //button->setMinimumSize(100, 100);
        //buttons.push_back(button);

        int row = i / columns;
        int col = i % columns;
        gridLayout->addWidget(albumButton, row, col);
    }

    containerWidget->adjustSize();
    centerWrapper->adjustSize();
}
