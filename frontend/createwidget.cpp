#include "createwidget.h"
#include "setstyle.h"
#include <QScrollArea>
#include <QPainter>
#include <QPainterPath>


CreateWidget::CreateWidget(QWidget *parent)
    : QWidget(parent)
{
    // --- Контейнерный виджет, в котором будет лежать содержимое ---
    QWidget *contentWidget = new QWidget;
    QWidget *formWidget = new QWidget();
    formWidget->setFixedWidth(1000);

    QHBoxLayout *formLayout = new QHBoxLayout(formWidget);

    // --- Левая часть: картинка ---
    QPushButton *addCoverButton = new QPushButton();

    addCoverButton->setCursor(Qt::PointingHandCursor);
    addCoverButton->setFixedSize(310, 310);

    imageLabel = new QLabel(addCoverButton);

    // Загружаем и масштабируем изображение
    //QPixmap albumCoverPath(currentTrack->coverpath);
    setRoundedImage(imageLabel, "../resources/photo/addCover.png", 300, 15);


    connect(addCoverButton, &QPushButton::clicked, this, &CreateWidget::chooseImage);

    // --- Правая часть: поля ---
    QWidget *nameWidget = new QWidget();
    QVBoxLayout *nameLayout = new QVBoxLayout(nameWidget);
    nameLayout->setAlignment(Qt::AlignTop);


    typeButton = new QPushButton("SINGLE");
    connect(typeButton, &QPushButton::clicked, this, &CreateWidget::on_typeButton_clicked);
    typeButton->setCursor(Qt::PointingHandCursor);
    typeButton->setFixedHeight(16);
    typeButton->setStyleSheet("color: #615D5D; font-size: 15px; font-family: 'K2D'; font-weight: bold; text-align:left;");

    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Name");
    nameEdit->setStyleSheet("QLineEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");

    descriptionEdit = new QTextEdit();
    descriptionEdit->setPlaceholderText("Description");
    descriptionEdit->setStyleSheet("QTextEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; }");
    descriptionEdit->setFixedHeight(32);
    descriptionEdit->setWordWrapMode(QTextOption::WordWrap);
    descriptionEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    descriptionEdit->setContentsMargins(0, 0, 0, 0);
    connect(descriptionEdit, &QTextEdit::textChanged, this, [=]() {
        QTextDocument *doc = descriptionEdit->document();
        doc->setTextWidth(descriptionEdit->viewport()->width());
        QSize docSize = doc->size().toSize();

        int newHeight = docSize.height(); // добавим чуть-чуть на отступ
        if (newHeight < 30)
            newHeight = 30;
        else if (newHeight > 100) {
            newHeight = 80;
            descriptionEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else {
            descriptionEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }

        descriptionEdit->setFixedHeight(newHeight);
    });


    lyricsEdit = new QTextEdit();
    lyricsEdit->setPlaceholderText("Lyrics");
    lyricsEdit->setStyleSheet("QTextEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; }");
    lyricsEdit->setFixedHeight(32);
    lyricsEdit->setWordWrapMode(QTextOption::WordWrap);
    lyricsEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lyricsEdit->setContentsMargins(0, 0, 0, 0);
    connect(lyricsEdit, &QTextEdit::textChanged, this, [=]() {
        QTextDocument *doc = lyricsEdit->document();
        doc->setTextWidth(lyricsEdit->viewport()->width());
        QSize docSize = doc->size().toSize();

        int newHeight = docSize.height(); // добавим чуть-чуть на отступ
        if (newHeight < 30)
            newHeight = 30;
        else if (newHeight > 100) {
            newHeight = 80;
            lyricsEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else {
            lyricsEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }

        lyricsEdit->setFixedHeight(newHeight);
    });

    QPushButton *uploadButton = new QPushButton("UPLOAD");
    uploadButton->setStyleSheet(
        "QLabel, QLineEdit, QPushButton { color: white; font-size: 20px; }"
        "QPushButton { background-color: #c00; border: none; padding: 5px 16px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #e00; }");


    //nameLayout->addWidget(authorWidget);  // ← добавляем в основное поле

    nameLayout->addWidget(typeButton);
    nameLayout->addWidget(nameEdit);
    nameLayout->addWidget(descriptionEdit);
    nameLayout->addWidget(lyricsEdit);
    nameLayout->addWidget(uploadButton, 0, Qt::AlignBottom);
    //nameLayout->addWidget(authorScrollArea, 4, Qt::AlignTop);

    // QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->addWidget(addCoverButton);
    formLayout->addWidget(nameWidget);

    // --- Основной горизонтальный layout ---
    QVBoxLayout *mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    mainLayout->addWidget(formWidget, 0, Qt::AlignTop);

    trackContainer = new QWidget(this);
    trackLayout = new QVBoxLayout(trackContainer);
    trackLayout->setAlignment(Qt::AlignTop);
    trackLayout->setSpacing(10);
    trackLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(trackContainer);


    // Добавляем первую кнопку
    addNewTrackButton();




    // --- ScrollArea ---
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setWidget(contentWidget);

    // --- Внешний layout, включающий скролл ---
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->addWidget(scrollArea);
    setLayout(outerLayout);
}

void CreateWidget::addNewTrackButton() {
    QPushButton *addTrackBtn = new QPushButton("➕ Добавить трек");
    addTrackBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    addTrackBtn->setStyleSheet("padding: 10px; text-align: left; font-size: 14px;");

    trackLayout->addWidget(addTrackBtn);

    connect(addTrackBtn, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Выберите MP3-файл", "", "MP3 Files (*.mp3)");
        if (!fileName.isEmpty()) {
            // Создание нового виджета трека
            QWidget *trackWidget = new QWidget();
            trackWidget->setFixedHeight(150);
            trackWidget->setStyleSheet("background-color: #2a2a2a; border-radius: 10px;");
            QVBoxLayout *outerLayout = new QVBoxLayout(trackWidget);
            outerLayout->setContentsMargins(10, 10, 10, 10);
            outerLayout->setSpacing(5);

            // Крестик (удалить виджет)
            QPushButton *removeBtn = new QPushButton("✖");
            removeBtn->setFixedSize(24, 24);
            removeBtn->setStyleSheet(
                "QPushButton { background: transparent; color: white; font-weight: bold; border: none; }"
                "QPushButton:hover { color: red; }"
                );
            QHBoxLayout *headerLayout = new QHBoxLayout();
            headerLayout->addStretch();
            headerLayout->addWidget(removeBtn);

            // Поле ввода названия трека
            QLineEdit *titleEdit = new QLineEdit();
            titleEdit->setPlaceholderText("Title");
            titleEdit->setStyleSheet("QLineEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");


            // Название файла
            QLabel *fileLabel = new QLabel(QFileInfo(fileName).fileName());
            fileLabel->setStyleSheet("color: gray; font-style: italic;");

            // Кнопка "Добавить автора"
            QWidget *authorWidget = new QWidget();
            QScrollArea *authorScrollArea = new QScrollArea();
            authorScrollArea->setWidgetResizable(true);
            authorScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            authorScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            authorScrollArea->setFrameStyle(QFrame::NoFrame); // Убрать рамку если нужно
            authorScrollArea->setFixedHeight(40); // Установи нужную высоту

            // Контейнер для layout-а внутри scroll-а
            QWidget *authorContent = new QWidget();
            authorLayout = new QHBoxLayout(authorContent);
            authorLayout->setContentsMargins(0, 0, 0, 0);
            authorLayout->setSpacing(5);

            // Кнопка добавления автора
            QPushButton *addAuthorButton = new QPushButton("+ Add author");
            addAuthorButton->setFlat(true);
            addAuthorButton->setStyleSheet("color: white;");
            authorLayout->addWidget(addAuthorButton);

            // Обязательно задать layout и widget в scroll
            authorContent->setLayout(authorLayout);
            authorScrollArea->setWidget(authorContent);

            // Всплывающее окно
            QWidget *popupWidget = new QWidget(nullptr, Qt::Popup);
            popupWidget->setStyleSheet("background-color: #222; color: white; border: 1px solid #444;");
            popupWidget->setFixedWidth(200);

            QVBoxLayout *popupLayout = new QVBoxLayout(popupWidget);
            QLineEdit *searchEdit = new QLineEdit();
            searchEdit->setPlaceholderText("Search author...");
            popupLayout->addWidget(searchEdit);

            // Скроллируемая зона для результатов
            QScrollArea *popupscrollArea = new QScrollArea();
            popupscrollArea->setWidgetResizable(true);
            popupscrollArea->setFixedHeight(150);
            QWidget *resultsWidget = new QWidget();
            QVBoxLayout *resultsLayout = new QVBoxLayout(resultsWidget);

            // Примерные кнопки результатов
            for (int i = 1; i <= 3; ++i) {
                QPushButton *resultButton = new QPushButton(QString("Author %1").arg(i));
                resultButton->setStyleSheet("text-align: left; color: white;");
                resultsLayout->addWidget(resultButton);

                // Добавление автора по нажати

                connect(resultButton, &QPushButton::clicked, this, [=]() {
                    addAuthor("New Author");
                });

            }
            resultsWidget->setLayout(resultsLayout);
            popupscrollArea->setWidget(resultsWidget);
            popupLayout->addWidget(popupscrollArea);

            // Показ popup при нажатии
            connect(addAuthorButton, &QPushButton::clicked, this, [=]() {
                QPoint buttonPos = addAuthorButton->mapToGlobal(QPoint(0, addAuthorButton->height()));
                popupWidget->move(buttonPos);
                popupWidget->show();
            });

            authorLayout->addWidget(addAuthorButton);
            QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
            authorLayout->addItem(spacer);

            // Компоновка
            outerLayout->addLayout(headerLayout);
            outerLayout->addWidget(titleEdit);
            outerLayout->addWidget(fileLabel);
            outerLayout->addWidget(authorScrollArea);

            // Вставляем вместо кнопки
            int index = trackLayout->indexOf(addTrackBtn);
            trackLayout->removeWidget(addTrackBtn);
            addTrackBtn->deleteLater();
            trackLayout->insertWidget(index, trackWidget);

            // Кнопка удаления трека
            connect(removeBtn, &QPushButton::clicked, this, [=]() {
                trackLayout->removeWidget(trackWidget);
                trackWidget->deleteLater();
            });

            // Добавить новую кнопку ниже
            addNewTrackButton();
        }
    });
}



void CreateWidget::on_typeButton_clicked(){
    if(isSingle){
        typeButton->setText("ALBUM");
        isSingle = 0;
    }else{
        typeButton->setText("SINGLE");
        isSingle=1;
    }
}

void CreateWidget::addAuthor(const QString &name) {
    // Контейнер для автора с именем и кнопкой удаления
    QWidget *authorItem = new QWidget();
    QHBoxLayout *itemLayout = new QHBoxLayout(authorItem);
    itemLayout->setContentsMargins(5, 0, 5, 0);
    itemLayout->setSpacing(5);

    QLabel *nameLabel = new QLabel(name);
    nameLabel->setStyleSheet("color: white;");

    QPushButton *removeButton = new QPushButton("✖");
    removeButton->setFixedSize(20, 20);
    removeButton->setStyleSheet("QPushButton {"
                                "color: white;"
                                "background-color: transparent;"
                                "border: none;"
                                "font-weight: bold;"
                                "}"
                                "QPushButton:hover { color: red; }");

    itemLayout->addWidget(nameLabel);
    itemLayout->addWidget(removeButton);

    // Добавить в layout перед кнопкой "Add author"
    int insertIndex = 0; // Добавлять сразу после кнопки
    authorLayout->insertWidget(insertIndex, authorItem); // +1 потому что кнопка первая


    // Удаление по нажатию на крестик
    connect(removeButton, &QPushButton::clicked, this, [=]() {
        authorLayout->removeWidget(authorItem);
        authorItem->deleteLater();
    });
}


void CreateWidget::chooseImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select Cover Image", "", "Images (*.png *.jpg *.jpeg)");
    if (!fileName.isEmpty()) {
        QPixmap originalPixmap(fileName);
        QPixmap scaledPixmap = originalPixmap.scaled(imageLabel->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

        QPixmap roundedPixmap(scaledPixmap.size());
        roundedPixmap.fill(Qt::transparent); // Прозрачный фон

        QPainter painter(&roundedPixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addRoundedRect(QRectF(0, 0, 300, 300), 15, 15); // <-- Радиус скругления
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, scaledPixmap);

        imageLabel->setPixmap(roundedPixmap);
    }

}
