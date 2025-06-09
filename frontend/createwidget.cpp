#include "createwidget.h"
#include "setstyle.h"
#include <QScrollArea>
#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QEvent>


CreateWidget::CreateWidget(QString *usertag, WebSocketClient *webSocketStas, QWidget *parent)
    : QWidget(parent),
    webSocketStas(webSocketStas),
    usertag(usertag)
{
    // --- Контейнер для всего содержимого ---
    QWidget *contentWidget = new QWidget;

    // --- Форма с фиксированной шириной ---
    QWidget *formWidget = new QWidget();
    formWidget->setFixedWidth(1000);

    // горизонтальный layout для формы
    QHBoxLayout *formLayout = new QHBoxLayout(formWidget);

    // --- Левая часть: кнопка с изображением (обложка) ---
    QPushButton *addCoverButton = new QPushButton();
    addCoverButton->setStyleSheet("background: transparent;");
    addCoverButton->setCursor(Qt::PointingHandCursor);
    addCoverButton->setFixedSize(310, 310);

    // метка для изображения на кнопке
    imageLabel = new QLabel(addCoverButton);

    // загружаем и отображаем круглое изображение
    setRoundedImage(imageLabel, "resources/photo/addCover.png", 300, 15);

    // сигнал на выбор изображения при клике
    connect(addCoverButton, &QPushButton::clicked, this, &CreateWidget::chooseImage);

    // --- Правая часть: поля ввода ---
    QWidget *nameWidget = new QWidget();
    QVBoxLayout *nameLayout = new QVBoxLayout(nameWidget);
    nameLayout->setAlignment(Qt::AlignTop);

    // кнопка выбора типа (SINGLE/OTHER)
    typeButton = new QPushButton("SINGLE");
    typeButton->setCursor(Qt::PointingHandCursor);
    typeButton->setFixedHeight(16);
    typeButton->setStyleSheet(
        "background: transparent; color: #615D5D; font-size: 15px; font-family: 'K2D'; font-weight: bold; text-align:left;");
    connect(typeButton, &QPushButton::clicked, this, &CreateWidget::on_typeButton_clicked);

    // поле для названия
    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Name");
    nameEdit->hide();
    nameEdit->setStyleSheet("QLineEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");
    connect(nameEdit, &QLineEdit::textChanged, this, [=]() {
        resetLineEditStyle(nameEdit);
    });

    // поле для описания
    descriptionEdit = new QTextEdit();
    descriptionEdit->setPlaceholderText("Description");
    descriptionEdit->setStyleSheet("QTextEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; }");
    descriptionEdit->setFixedHeight(40);
    descriptionEdit->setWordWrapMode(QTextOption::WordWrap);
    descriptionEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    descriptionEdit->setContentsMargins(0, 0, 0, 0);
    connect(descriptionEdit, &QTextEdit::textChanged, this, [=]() {
        QTextDocument *doc = descriptionEdit->document();
        doc->setTextWidth(descriptionEdit->viewport()->width());
        QSize docSize = doc->size().toSize();

        int newHeight = docSize.height();
        if (newHeight < 40)
            newHeight = 40;
        else if (newHeight > 100) {
            newHeight = 80;
            descriptionEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else {
            descriptionEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
        descriptionEdit->setFixedHeight(newHeight);
    });

    // поле для текста (тексты песен)
    lyricsEdit = new QTextEdit();
    lyricsEdit->hide();
    lyricsEdit->setPlaceholderText("Lyrics");
    lyricsEdit->setStyleSheet("QTextEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; }");
    lyricsEdit->setFixedHeight(32);
    lyricsEdit->setWordWrapMode(QTextOption::WordWrap);
    lyricsEdit->setContentsMargins(0, 0, 0, 0);
    connect(lyricsEdit, &QTextEdit::textChanged, this, [=]() {
        QTextDocument *doc = lyricsEdit->document();
        doc->setTextWidth(lyricsEdit->viewport()->width());
        QSize docSize = doc->size().toSize();

        int newHeight = docSize.height();
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

    // кнопка загрузки
    QPushButton *uploadButton = new QPushButton("UPLOAD");
    uploadButton->setStyleSheet(
        "QLabel, QLineEdit, QPushButton { color: white; font-size: 20px; }"
        "QPushButton { background-color: #c00; border: none; padding: 5px 16px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #e00; }");
    connect(uploadButton, &QPushButton::clicked, this, &CreateWidget::on_upload_clicked);

    // добавляем виджеты в правую часть
    nameLayout->addWidget(typeButton);
    nameLayout->addWidget(nameEdit);
    nameLayout->addWidget(descriptionEdit);
    nameLayout->addWidget(lyricsEdit);
    nameLayout->addWidget(uploadButton, 0, Qt::AlignBottom);

    // добавляем левую и правую часть в форму
    formLayout->addWidget(addCoverButton);
    formLayout->addWidget(nameWidget);

    // --- Основной вертикальный layout контейнера ---
    QVBoxLayout *mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    mainLayout->addWidget(formWidget, 0, Qt::AlignTop);

    // контейнер для треков с фиксированной шириной
    trackContainer = new QWidget(this);
    trackContainer->setFixedWidth(1000);

    trackLayout = new QVBoxLayout(trackContainer);
    trackLayout->setAlignment(Qt::AlignTop);
    trackLayout->setSpacing(10);
    trackLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(trackContainer);

    // добавляем кнопку добавления нового трека
    addNewTrackButton();

    // --- ScrollArea для всего содержимого ---
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setWidget(contentWidget);

    // --- Внешний layout для CreateWidget ---
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->addWidget(scrollArea);
    setLayout(outerLayout);

    // подключаем обработку входящих сообщений вебсокета
    connect(webSocketStas, &WebSocketClient::messageReceived,
            this, &CreateWidget::onTextMessageReceived);
}


void CreateWidget::onTextMessageReceived(const QString &type, const QJsonObject &dataObj) {
    qDebug()<<"type:";
    qDebug()<<type;
}
void CreateWidget::addNewTrackButton() {
    addTrackBtn = new QPushButton();

    addTrackBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    addTrackBtn->setFixedHeight(100);

    // Иконка слева + текст с отступом
    QIcon icon("resources/icons/playbutton.png");
    addTrackBtn->setIcon(icon);
    addTrackBtn->setIconSize(QSize(70, 70));
    addTrackBtn->setText("         ADD AUDIO RECORDING"); // пробелы для отступа

    // Стиль кнопки
    addTrackBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   border-top: 1px solid #555;"
        "   border-bottom: 1px solid #555;"
        "   border-left: none;"
        "   border-right: none;"
        "   color: white;"
        "   font-size: 30px;"
        "   font-weight: bold;"
        "   text-align: left;"
        "   padding-left: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255, 255, 255, 0.05);"
        "}"
        );

    trackLayout->addWidget(addTrackBtn);

    connect(addTrackBtn, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Выберите MP3-файл", "", "MP3 Files (*.mp3)");
        if (fileName.isEmpty())
            return;

        // Создание нового трека
        trackIndex++;
        auto *trackWidget = new TrackWidget(fileName, trackIndex);

        int index = trackLayout->indexOf(addTrackBtn);
        trackLayout->removeWidget(addTrackBtn);
        addTrackBtn->deleteLater();
        trackLayout->insertWidget(index, trackWidget);

        // Подключаем удаление трека
        connect(trackWidget, &TrackWidget::removeButtonClicked, this, [=]() {
            trackLayout->removeWidget(trackWidget);
            trackWidget->deleteLater();

            int indexOfWidget = trackWidgets.indexOf(trackWidget);
            if (indexOfWidget != -1) {
                trackWidgets.removeAt(indexOfWidget);
            }

            // Обновляем индексы треков
            for (int i = 0; i < trackWidgets.size(); i++) {
                trackWidgets[i]->setIndex(i + 1);
            }

            trackIndex--;
            if (trackIndex == 0 && (typeOfObject == "single")) {
                addNewTrackButton();
            }
        });

        if (typeOfObject != "single") {
            addNewTrackButton();
        }

        trackWidgets.push_back(trackWidget);
    });
}

void CreateWidget::on_upload_clicked() {
    // Проверка на заполнение названия для не-синглов
    if (typeOfObject != "single" && nameEdit->text().trimmed().isEmpty()) {
        nameEdit->setStyleSheet(
            "QLineEdit {"
            "color: white; font-size: 18px; background-color: #111;"
            "border: 1px solid red; border-radius: 5px; padding: 5px;"
            "}"
            );
        nameEdit->setFocus();
        qDebug() << "Название не заполнено";
        return;
    }

    // Проверка треков
    if (trackIndex != 0) {
        for (auto *trackwidget : trackWidgets) {
            if (trackwidget->getTitleEdit()->text().trimmed().isEmpty()) {
                trackwidget->getTitleEdit()->setStyleSheet(
                    "QLineEdit {"
                    "color: white; font-size: 18px; background-color: #111;"
                    "border: 1px solid red; border-radius: 5px; padding: 5px;"
                    "}"
                    );
                trackwidget->getTitleEdit()->setFocus();
                qDebug() << "Название трека не заполнено";
                return;
            }
            if (trackwidget->getAuthors()->isEmpty()) {
                trackwidget->setAddButtonStyle("red");
                qDebug() << "Авторы трека не заполнены";
                return;
            }
        }
    }

    // Отладочный вывод названий и авторов
    for (auto *trackwidget : trackWidgets) {
        qDebug() << "Title:" << trackwidget->getTitleEdit()->text();
        for (const auto &author : *trackwidget->getAuthors()) {
            qDebug() << "Author:" << author;
        }
    }

    QJsonObject payload;
    payload["endpoint"] = "/catalog";
    payload["action"] = "create";

    // Читаем cover-файл в base64
    QFile file(fileCoverName);
    QString base64String;
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileData = file.readAll();
        base64String = fileData.toBase64();
    } else {
        qDebug() << "Не удалось открыть обложку";
    }

    if (typeOfObject == "single") {
        payload["title"] = trackWidgets[0]->getTitleEdit()->text();
        payload["entity"] = "track";
        payload["duration_seconds"] = 180; // В идеале - динамическое значение
        QFile musFile(trackWidgets[0]->getfilename());
        QString musBase64;
        if (musFile.open(QIODevice::ReadOnly)) {
            QByteArray musData = musFile.readAll();
            musBase64 = musData.toBase64();
        } else {
            qDebug() << "Не удалось открыть файл трека";
        }

        payload["file_blob"] = musBase64;

        QJsonArray trackTags;
        for (const auto &author : *trackWidgets[0]->getAuthors()) {
            trackTags.append(author);
        }
        payload["lyrics"] = trackWidgets[0]->getTracLyricsEdit()->toPlainText();
        payload["usertags"] = trackTags;
        payload["cover_blob"] = base64String;

    } else {
        payload["entity"] = typeOfObject;
        payload["description"] = descriptionEdit->toPlainText();
        payload["album_title"] = nameEdit->text();

        QJsonArray albumtags;
        albumtags.append(*usertag);
        payload["usertags"] = albumtags;
        payload["cover_blob"] = base64String;

        QJsonArray tracksArray;
        for (auto *widget : trackWidgets) {
            QJsonObject trackObj;
            trackObj["title"] = widget->getTitleEdit()->text();
            trackObj["duration_seconds"] = 180;

            QFile musFile(widget->getfilename());
            QString musBase64;
            if (musFile.open(QIODevice::ReadOnly)) {
                QByteArray musData = musFile.readAll();
                musBase64 = musData.toBase64();
            } else {
                qDebug() << "Не удалось открыть файл трека";
            }

            trackObj["file_blob"] = musBase64;

            QJsonArray trackTags;
            for (const auto &author : *widget->getAuthors()) {
                trackTags.append(author);
            }
            trackObj["usertags"] = trackTags;

            tracksArray.append(trackObj);
        }
        payload["tracks"] = tracksArray;
    }

    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    webSocketStas->sendMessage(message);
    qDebug() << "Данные отправлены";
}

void CreateWidget::resetLineEditStyle(QLineEdit *lineEdit) {
    lineEdit->setStyleSheet("QLineEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");
}

void CreateWidget::on_typeButton_clicked() {
    if (typeOfObject == "single") {
        // Переключаемся на режим альбома
        typeButton->setText("ALBUM");
        nameEdit->show();
        if (trackIndex != 0) {
            addNewTrackButton();
        }
        typeOfObject = "album";
    } else {
        // Подтверждение переключения в режим single с удалением всех треков
        QMessageBox::StandardButton reply;
        if (trackIndex != 0) {
            reply = QMessageBox::question(this, "Confirm action",
                                          "Are you sure you want to switch to SINGLE mode? All tracks will be removed.",
                                          QMessageBox::Yes | QMessageBox::No);
        }
        if (reply == QMessageBox::Yes || trackIndex == 0) {
            typeButton->setText("SINGLE");
            nameEdit->hide();

            // Удаляем все треки из layout и памяти
            for (auto* widget : trackWidgets) {
                trackLayout->removeWidget(widget);
                widget->deleteLater();
            }
            trackWidgets.clear();
            trackIndex = 0;
            typeOfObject = "single";
        }
    }
}

void CreateWidget::addAuthor(const QString &name, int indexOfTrack) {
    // Создаем виджет автора с лейблом и кнопкой удаления
    QWidget *authorItem = new QWidget();
    QHBoxLayout *itemLayout = new QHBoxLayout(authorItem);
    itemLayout->setContentsMargins(5, 0, 5, 0);
    itemLayout->setSpacing(5);

    QLabel *nameLabel = new QLabel(name);
    nameLabel->setStyleSheet("color: white;");

    QPushButton *removeButton = new QPushButton("✖");
    removeButton->setFixedSize(20, 20);
    removeButton->setStyleSheet(
        "QPushButton {"
        "color: white;"
        "background-color: transparent;"
        "border: none;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover { color: red; }"
        );

    itemLayout->addWidget(nameLabel);
    itemLayout->addWidget(removeButton);

    // Вставляем виджет автора в layout перед кнопкой "Add author"
    int insertIndex = 0;
    authorLayout->insertWidget(insertIndex, authorItem);

    // Обработчик удаления автора по нажатию на крестик
    connect(removeButton, &QPushButton::clicked, this, [=]() {
        authorLayout->removeWidget(authorItem);
        authorItem->deleteLater();

        int indexOfAuthor = authors[indexOfTrack - 1]->indexOf(name);
        if (indexOfAuthor != -1) {
            authors[indexOfTrack - 1]->removeAt(indexOfAuthor);
        }
    });
}

void CreateWidget::chooseImage() {
    fileCoverName = QFileDialog::getOpenFileName(this, "Select Cover Image", "", "Images (*.png *.jpg *.jpeg)");
    if (!fileCoverName.isEmpty()) {
        QPixmap originalPixmap(fileCoverName);
        QPixmap scaledPixmap = originalPixmap.scaled(imageLabel->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

        QPixmap roundedPixmap(scaledPixmap.size());
        roundedPixmap.fill(Qt::transparent); // Прозрачный фон

        QPainter painter(&roundedPixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addRoundedRect(QRectF(0, 0, 300, 300), 15, 15); // Радиус скругления 15
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, scaledPixmap);

        imageLabel->setPixmap(roundedPixmap);
    }
}




TrackWidget::TrackWidget(QString fileName, int index, QWidget *parent)
    : QWidget(parent),
    fileName(fileName),
    trackIndex(index)
{
    setFixedHeight(100);

    // Внешний вертикальный layout
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(10, 10, 10, 10);

    // Горизонтальный layout для основной строки трека
    QHBoxLayout *trackrotmLayout = new QHBoxLayout();
    outerLayout->addLayout(trackrotmLayout);

    // Кнопка удаления (крестик)
    QPushButton *removeBtn = new QPushButton("✖");
    removeBtn->setFixedSize(24, 24);
    removeBtn->setStyleSheet(
        "QPushButton { background: transparent; color: white; font-weight: bold; border: none; }"
        "QPushButton:hover { color: red; }"
        );

    // Layout для кнопки удаления (сдвинуть вправо)
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addStretch();
    headerLayout->addWidget(removeBtn);

    // Редактируемое поле заголовка трека
    titleEdit = new QLineEdit();
    titleEdit->setPlaceholderText("Title");
    titleEdit->setStyleSheet(
        "QLineEdit {"
        "color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px;"
        "}"
        );
    connect(titleEdit, &QLineEdit::textChanged, this, [=]() {
        resetLineEditStyle(titleEdit);
    });

    // Метка с названием файла
    QLabel *fileLabel = new QLabel(QFileInfo(fileName).fileName());
    fileLabel->setStyleSheet("color: gray; font-style: italic;");

    // Текстовое поле для текста песни (lyrics)
    tracLyricsEdit = new QTextEdit();
    tracLyricsEdit->setPlaceholderText("Lyrics");
    tracLyricsEdit->setStyleSheet(
        "QTextEdit {"
        "color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px;"
        "}"
        );
    tracLyricsEdit->setFixedHeight(40);
    tracLyricsEdit->setWordWrapMode(QTextOption::WordWrap);
    tracLyricsEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tracLyricsEdit->setContentsMargins(0, 0, 0, 0);
    tracLyricsEdit->installEventFilter(this);

    // Кнопка "OK" для сворачивания lyrics
    QPushButton *collapseButton = new QPushButton("OK");
    collapseButton->setFixedSize(50, 30);
    collapseButton->setStyleSheet(
        "QPushButton {"
        "color: white; background-color: #222; border: 1px solid #555; border-radius: 5px; padding: 5px;"
        "}"
        "QPushButton:hover { background-color: #444; }"
        );
    connect(collapseButton, &QPushButton::clicked, this, [=]() {
        tracLyricsEdit->setFixedHeight(40);
        tracLyricsEdit->clearFocus();
    });

    // Layout для lyrics и кнопки OK
    QHBoxLayout *lyricsLayout = new QHBoxLayout();
    lyricsLayout->addWidget(tracLyricsEdit);
    lyricsLayout->addWidget(collapseButton);

    // Метка с номером трека
    numberLabel = new QLabel(QString::number(trackIndex));
    numberLabel->setStyleSheet(
        "color: white; font-size: 20px; font-family: 'K2D'; font-weight: bold; text-align:left;"
        );

    // Создаем виджет и scroll area для авторов
    QWidget *authorWidget = new QWidget();
    authorWidget->setStyleSheet("border: none;");
    QScrollArea *authorScrollArea = new QScrollArea();
    authorScrollArea->setWidgetResizable(true);
    authorScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    authorScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    authorScrollArea->setFrameStyle(QFrame::NoFrame);
    authorScrollArea->setFixedHeight(40);

    // Контейнер и layout для авторов внутри scroll
    QWidget *authorContent = new QWidget();
    authorLayout = new QHBoxLayout(authorContent);
    authorLayout->setContentsMargins(0, 0, 0, 0);
    authorLayout->setSpacing(5);

    // Кнопка добавления автора
    addAuthorButton = new QPushButton("+ Add author");
    addAuthorButton->setFlat(true);
    addAuthorButton->setStyleSheet(
        "QPushButton {"
        "font-weight: bold;"
        "font-size: 15px;"
        "font-family: 'Tahoma';"
        "background: none;"
        "border: none;"
        "text-decoration: none;"
        "}"
        "QPushButton:hover {"
        "text-decoration: underline;"
        "}"
        );
    authorLayout->addWidget(addAuthorButton);

    authorContent->setLayout(authorLayout);
    authorScrollArea->setWidget(authorContent);

    // Popup для поиска автора
    QWidget *popupWidget = new QWidget(nullptr, Qt::Popup);
    popupWidget->setStyleSheet("background-color: #222; color: white; border: 1px solid #444;");
    popupWidget->setFixedWidth(200);

    QVBoxLayout *popupLayout = new QVBoxLayout(popupWidget);
    QLineEdit *searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search author...");
    popupLayout->addWidget(searchEdit);

    QScrollArea *popupscrollArea = new QScrollArea();
    popupscrollArea->setWidgetResizable(true);
    popupscrollArea->setFixedHeight(150);

    QWidget *resultsWidget = new QWidget();
    QVBoxLayout *resultsLayout = new QVBoxLayout(resultsWidget);

    int indexOfTrack = trackIndex;

    // Кнопки результатов поиска (пример)
    for (int i = 1; i <= 3; ++i) {
        QPushButton *resultButton = new QPushButton(QString("Author %1").arg(i));
        resultButton->setStyleSheet("text-align: left; color: white;");
        resultsLayout->addWidget(resultButton);

        connect(resultButton, &QPushButton::clicked, this, [=]() {
            setAddButtonStyle("white");
            addAuthor(resultButton->text());
            authors.push_back(resultButton->text());
            popupWidget->hide();
        });
    }

    resultsWidget->setLayout(resultsLayout);
    popupscrollArea->setWidget(resultsWidget);
    popupLayout->addWidget(popupscrollArea);

    // Показ popup при клике по кнопке добавления автора
    connect(addAuthorButton, &QPushButton::clicked, this, [=]() {
        QPoint buttonPos = addAuthorButton->mapToGlobal(QPoint(0, addAuthorButton->height()));
        addAuthorButton->setStyleSheet(
            "QPushButton {"
            "font-weight: bold;"
            "font-size: 15px;"
            "font-family: 'Tahoma';"
            "background: none;"
            "border: none;"
            "text-decoration: none;"
            "}"
            "QPushButton:hover {"
            "text-decoration: underline;"
            "}"
            );
        popupWidget->move(buttonPos);
        popupWidget->show();
    });

    // Добавляем расширяющий spacer чтобы авторы не слипались с кнопкой
    QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    authorLayout->addItem(spacer);

    // Подключаем сигнал удаления трека
    connect(removeBtn, &QPushButton::clicked, this, [=]() {
        emit removeButtonClicked();
    });

    // Собираем основной layout трека
    trackrotmLayout->addWidget(numberLabel);
    trackrotmLayout->addWidget(titleEdit);
    trackrotmLayout->addWidget(fileLabel);
    trackrotmLayout->addLayout(lyricsLayout);
    trackrotmLayout->addLayout(headerLayout);

    // Добавляем scroll area с авторами
    outerLayout->addWidget(authorScrollArea);
}

bool TrackWidget::eventFilter(QObject *obj, QEvent *event) {
    if (obj == tracLyricsEdit) {
        if (event->type() == QEvent::FocusIn) {
            tracLyricsEdit->setFixedHeight(80);
            tracLyricsEdit->raise();
        }
    }
    return QWidget::eventFilter(obj, event);
}

void TrackWidget::addAuthor(const QString &name) {
    // Контейнер для автора с именем и кнопкой удаления
    QWidget *authorItem = new QWidget();
    QHBoxLayout *itemLayout = new QHBoxLayout(authorItem);
    itemLayout->setContentsMargins(5, 0, 5, 0);
    itemLayout->setSpacing(5);

    QLabel *nameLabel = new QLabel(name);
    nameLabel->setStyleSheet("color: white;");

    QPushButton *removeButton = new QPushButton("✖");
    removeButton->setFixedSize(20, 20);
    removeButton->setStyleSheet(
        "QPushButton {"
        "color: white;"
        "background-color: transparent;"
        "border: none;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover { color: red; }"
        );

    itemLayout->addWidget(nameLabel);
    itemLayout->addWidget(removeButton);

    // Добавить в layout перед кнопкой "Add author"
    int insertIndex = 0;  // Добавлять сразу после кнопки
    authorLayout->insertWidget(insertIndex, authorItem);

    // Удаление по нажатию на крестик
    connect(removeButton, &QPushButton::clicked, this, [=]() {
        authorLayout->removeWidget(authorItem);
        authorItem->deleteLater();
        int indexOfAuthor = authors.indexOf(name);
        if (indexOfAuthor != -1) {
            authors.removeAt(indexOfAuthor);
        }
    });
}

QLineEdit *TrackWidget::getTitleEdit() {
    return titleEdit;
}

QVector<QString> *TrackWidget::getAuthors() {
    return &authors;
}

int TrackWidget::getIndex() {
    return trackIndex;
}

void TrackWidget::setIndex(int newIndex) {
    trackIndex = newIndex;
    numberLabel->setText(QString::number(trackIndex));
}

void TrackWidget::setAddButtonStyle(QString color) {
    addAuthorButton->setStyleSheet(
        "QPushButton {"
        "color:" + color + ";"
                  "font-weight: bold;"
                  "font-size: 15px;"
                  "font-family: 'Tahoma';"
                  "background: none;"
                  "border: none;"
                  "text-decoration: none;"
                  "}"
                  "QPushButton:hover {"
                  "text-decoration: underline;"
                  "}"
        );
}

void TrackWidget::resetLineEditStyle(QLineEdit *lineEdit) {
    lineEdit->setStyleSheet(
        "QLineEdit {"
        "color: white;"
        "font-size: 18px;"
        "background-color: #111;"
        "border: 1px solid #333;"
        "border-radius: 5px;"
        "padding: 5px;"
        "}"
        );
}

QString TrackWidget::getfilename() {
    return fileName;
}

QTextEdit *TrackWidget::getTracLyricsEdit() {
    return tracLyricsEdit;
}
