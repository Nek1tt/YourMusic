#include "homewidget.h"


HomeWidget::HomeWidget()
    :newAlbumsWidget(new MyAlbumsWidget( "New relises", this)),
    recommendedAlbumsWidget(new MyAlbumsWidget("Recommended relises", this))
{
    QWidget *contentWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Создаём поле для ввода
    QLineEdit *searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Что послушаем?");
    searchEdit->setFixedWidth(400); // ширина формы
    searchEdit->setStyleSheet(
        "QLineEdit {"
        "  padding: 10px;"
        "  border: 2px solid #888;"
        "  border-radius: 10px;"
        "  font-size: 18px;"
        "  color: white;"
        "  background-color: #333;"
        "}"
        "QLineEdit:focus {"
        "  border: 2px solid red;"
        "}"
        );

    // Центрируем по горизонтали
    QWidget *centerWidget = new QWidget();
    centerWidget->setFixedHeight(100);
    QHBoxLayout *centerLayout = new QHBoxLayout(centerWidget);
    centerLayout->addWidget(searchEdit);

    mainLayout->addWidget(centerWidget, 0, Qt::AlignTop);

    mainLayout->addWidget(newAlbumsWidget, 1, Qt::AlignTop);
    mainLayout->addWidget(recommendedAlbumsWidget, 2, Qt::AlignTop);



    // Оборачиваем всё в scrollArea
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addWidget(scrollArea);

}

void HomeWidget::homeButtonClicked(QVector<album> album_list){
    recommendedAlbumsWidget->add_albums(album_list);
    newAlbumsWidget->add_albums(album_list);
}
