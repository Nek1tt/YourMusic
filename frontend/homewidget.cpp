#include "homewidget.h"

// Конструктор HomeWidget
HomeWidget::HomeWidget()
    : newAlbumsWidget(new MyAlbumsWidget("New releases", this)),
    recommendedAlbumsWidget(new MyAlbumsWidget("Recommended releases", this)),
    randomAlbumsWidget(new MyAlbumsWidget("Recommended tracks", this))
{
    // Основной виджет-контейнер
    QWidget *contentWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // === Поле поиска ===
    QLineEdit *searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Что послушаем?");
    searchEdit->setFixedWidth(400);
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

    // Центрирование поля поиска по горизонтали
    QWidget *centerWidget = new QWidget();
    centerWidget->setFixedHeight(100);
    QHBoxLayout *centerLayout = new QHBoxLayout(centerWidget);
    centerLayout->addWidget(searchEdit);

    // Добавляем элементы на главный layout
    mainLayout->addWidget(centerWidget, 0, Qt::AlignTop);
    mainLayout->addWidget(newAlbumsWidget, 1, Qt::AlignTop);
    mainLayout->addWidget(recommendedAlbumsWidget, 2, Qt::AlignTop);
    mainLayout->addWidget(randomAlbumsWidget, 3, Qt::AlignTop);

    // === ScrollArea для прокрутки содержимого ===
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Внешний layout для всего виджета HomeWidget
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addWidget(scrollArea);

    // === Подключение сигналов от newAlbumsWidget ===
    connect(newAlbumsWidget, &MyAlbumsWidget::albumButtonClicked, this, &HomeWidget::onAlbumClicked);
    connect(newAlbumsWidget, &MyAlbumsWidget::myAlbumsButtonClicked, this, &HomeWidget::onAlbumListClicked);
    connect(newAlbumsWidget, &MyAlbumsWidget::authorButtonClicked, this, &HomeWidget::onAuthorButtonClicked);

    // === Подключение сигналов от recommendedAlbumsWidget ===
    connect(recommendedAlbumsWidget, &MyAlbumsWidget::albumButtonClicked, this, &HomeWidget::onAlbumClicked);
    connect(recommendedAlbumsWidget, &MyAlbumsWidget::myAlbumsButtonClicked, this, &HomeWidget::onAlbumListClicked);
    connect(recommendedAlbumsWidget, &MyAlbumsWidget::authorButtonClicked, this, &HomeWidget::onAuthorButtonClicked);

    // === Подключение сигналов от randomAlbumsWidget ===
    connect(randomAlbumsWidget, &MyAlbumsWidget::albumButtonClicked, this, &HomeWidget::onAlbumClicked);
    connect(randomAlbumsWidget, &MyAlbumsWidget::myAlbumsButtonClicked, this, &HomeWidget::onAlbumListClicked);
    connect(randomAlbumsWidget, &MyAlbumsWidget::authorButtonClicked, this, &HomeWidget::onAuthorButtonClicked);
}

// Метод обновления содержимого домашней страницы
void HomeWidget::homeButtonClicked(QVector<album> *newAlbums, QVector<album> *recAlbums, QVector<album> *randomAlbums)
{
    recommendedAlbumsWidget->add_albums(*recAlbums);
    newAlbumsWidget->add_albums(*newAlbums);
    randomAlbumsWidget->add_albums(*randomAlbums);
}

// === Обработчики сигналов ===

// Обработчик сигнала нажатия на кнопку автора
void HomeWidget::onAuthorButtonClicked(QString *authorUsertag)
{
    emit authorButtonClicked(authorUsertag);
}

// Обработчик сигнала нажатия на альбом
void HomeWidget::onAlbumClicked(album albumData)
{
    emit albumClicked(albumData);
}

// Обработчик сигнала нажатия на список альбомов
void HomeWidget::onAlbumListClicked(QVector<album> albumList)
{
    emit albumListClicked(albumList);
}
