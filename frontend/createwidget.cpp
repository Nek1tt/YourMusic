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
#include <QKeyEvent>


CreateWidget::CreateWidget(QString *usertag, WebSocketClient *webSocketStas, QWidget *parent)
    : QWidget(parent),
    webSocketStas(webSocketStas),
    usertag(usertag)
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
    connect(nameEdit, &QLineEdit::textChanged, this, [=]() {
        resetLineEditStyle(nameEdit);
    });
    nameEdit->hide();
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
    lyricsEdit->hide();
    lyricsEdit->setPlaceholderText("Lyrics");
    lyricsEdit->setStyleSheet("QTextEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; }");
    lyricsEdit->setFixedHeight(32);
    lyricsEdit->setWordWrapMode(QTextOption::WordWrap);
    //lyricsEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //lyricsEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

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
    connect(uploadButton, &QPushButton::clicked, this, &CreateWidget::on_upload_clicked);
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
    trackContainer->setFixedWidth(1000);

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

    connect(webSocketStas, &WebSocketClient::messageReceived,
            this, &CreateWidget::onTextMessageReceived);
}

void CreateWidget::onTextMessageReceived(const QString &type, const QJsonObject &dataObj) {
    qDebug()<<"type:";
    qDebug()<<type;
    // if(type=="auth_response"){
    //     serverStatus = dataObj.value("status").toString();
    //     serverMessage = dataObj.value("message").toString();
    //     if(current_query == "register"){
    //         if(serverStatus=="ok"){
    //             accept();
    //         }else{
    //             regErrorLabel->setText(serverMessage);
    //             regErrorLabel->setStyleSheet("color: red;");
    //             qDebug()<<"error: "<<serverMessage;
    //         }
    //     }
    //     if(current_query=="login"){
    //         if(serverStatus=="ok"){
    //             userTag = dataObj.value("usertag").toString();
    //             accept();
    //         }else{
    //             logErrorLabel->setText(serverMessage);
    //             logErrorLabel->setStyleSheet("color: red;");
    //             qDebug()<<"error: "<<serverMessage;
    //         }
    //     }
    // }

}

void CreateWidget::addNewTrackButton() {

    addTrackBtn = new QPushButton();
    // addTrackBtn->setFixedWidth(1000);

    addTrackBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    addTrackBtn->setFixedHeight(100);

    // Задать иконку слева + текст
    QIcon icon("../resources/icons/playbutton.png"); // путь к иконке
    addTrackBtn->setIcon(icon);
    addTrackBtn->setIconSize(QSize(70, 70)); // Размер иконки при необходимости подстрой

    // Установить текст с плюсом и словами (можно использовать HTML для доп. стиля)
    addTrackBtn->setText("         ADD AUDIO RECORDING"); // с пробелами для отступа между иконкой и плюсом + текстом

    // Стиль кнопки
    addTrackBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"  // полностью прозрачный фон
        "   border-top: 1px solid #555;"      // верхняя граница
        "   border-bottom: 1px solid #555;"   // нижняя граница
        "   border-left: none;"
        "   border-right: none;"
        "   color: white;"                    // цвет текста
        "   font-size: 30px;"                 // размер шрифта
        "   font-weight: bold;"               // жирный текст
        "   text-align: left;"                // текст слева
        "   padding-left: 10px;"              // отступ от левого края (для иконки + текста)
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255, 255, 255, 0.05);" // легкое затемнение при наведении
        "}"
        );

    trackLayout->addWidget(addTrackBtn);

    connect(addTrackBtn, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Выберите MP3-файл", "", "MP3 Files (*.mp3)");
        if (!fileName.isEmpty()) {
            // Создание нового виджета трека
            trackIndex++;
            auto *trackWidget = new TrackWidget(fileName, trackIndex);

            int index = trackLayout->indexOf(addTrackBtn);
            trackLayout->removeWidget(addTrackBtn);
            addTrackBtn->deleteLater();
            trackLayout->insertWidget(index, trackWidget);

            // Кнопка удаления трека
            connect(trackWidget, &TrackWidget::removeButtonClicked, this, [=]() {
                trackLayout->removeWidget(trackWidget);
                trackWidget->deleteLater();
                int indexOfWidget = trackWidgets.indexOf(trackWidget);
                if (indexOfWidget != -1) {
                    trackWidgets.removeAt(indexOfWidget);
                }
                for (int i=0; i<trackWidgets.size(); i++){
                    trackWidgets[i]->setIndex(i+1);
                }
                trackIndex--;
                if(trackIndex==0 && (typeOfObject == "single")){
                    addNewTrackButton();
                }

            });

            if (!(typeOfObject == "single")){
                addNewTrackButton();
            }

            trackWidgets.push_back(trackWidget);

        }
    });
}

void CreateWidget::on_upload_clicked(){
    qDebug()<<"1";


    if(!(typeOfObject == "single") && nameEdit->text().trimmed().isEmpty()){
        qDebug()<<"2";

        nameEdit->setStyleSheet("QLineEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; border: 1px solid red; }");
        nameEdit->setFocus();
        qDebug()<<"3";

        return;

    }
    if(trackIndex!=0){
        qDebug()<<"4";
        for (auto *trackwidget:trackWidgets){
            if (trackwidget->getTitleEdit()->text().trimmed().isEmpty()) {
                trackwidget->getTitleEdit()->setStyleSheet("QLineEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; border: 1px solid red; }");
                trackwidget->getTitleEdit()->setFocus();
                qDebug()<<"5";
                return;
            }
            if(trackwidget->getAuthors()->isEmpty()){
                trackwidget->setAddButtonStyle("red");
                return;
            }
        }
    }


    for (auto *trackwidget:trackWidgets){
        qDebug()<<"Titles";
        qDebug()<<trackwidget->getTitleEdit()->text();

        qDebug()<<"Titles";
        for (auto author:*trackwidget->getAuthors()){
            qDebug()<<author;
        }
    }


    QJsonObject payload;
    payload["endpoint"] = "/catalog";
    payload["action"] = "create";

    QFile file(fileCoverName);
    QString base64String;
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileData = file.readAll();
        base64String = fileData.toBase64();
        //payload["cover_blob"] = base64String;
    } else {
        qDebug() << "Failed to open cover image!";
    }

    if(typeOfObject == "single"){
        payload["title"] = trackWidgets[0]->getTitleEdit()->text();
        payload["entity"] = "track";
        payload["duration_seconds"] = 3453;
        payload["file_blob"] = base64String;
        QJsonArray trackTags;
        for (auto author:*trackWidgets[0]->getAuthors()){
            trackTags.append(author);
        }
        payload["usertags"] = trackTags;
        payload["cover_blob"] = base64String;
    }else{
        payload["entity"] = typeOfObject;
        payload["album_title"] = nameEdit->text();
        QJsonArray albumtags;
        albumtags.append(*usertag);
        payload["usertags"] = albumtags;
        payload["cover_blob"] = base64String;

        QJsonArray tracksArray;
        for (auto widget:trackWidgets){
            // Пример 1 трека
            QJsonObject track1;
            track1["title"] = widget->getTitleEdit()->text();
            track1["duration_seconds"] = 180;
            track1["file_blob"] = base64String;

            QJsonArray trackTags;

            for (auto author:*widget->getAuthors()){
                trackTags.append(author);
            }
            track1["usertags"] = trackTags;

            tracksArray.append(track1);
        }
        payload["tracks"] = tracksArray;
    }


    QJsonDocument doc(payload);
    QString message = QString::fromUtf8(doc.toJson());
    qDebug()<<message;
    webSocketStas->sendMessage(message);
    qDebug()<<"6";


}

void CreateWidget::resetLineEditStyle(QLineEdit *lineEdit) {
    lineEdit->setStyleSheet("QLineEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");
}

void CreateWidget::on_typeButton_clicked(){
    if(typeOfObject == "single"){
        typeButton->setText("ALBUM");
        nameEdit->show();
        if(trackIndex!=0){
            addNewTrackButton();
        }
        typeOfObject = "album";

    }else{
        QMessageBox::StandardButton reply;
        if(trackIndex!=0){
        reply = QMessageBox::question(this, "Confirm action",
                                      "Are you sure you want to switch to SINGLE mode? All tracks will be removed.",
                                      QMessageBox::Yes | QMessageBox::No);
        }
        if (reply == QMessageBox::Yes || trackIndex==0) {
            typeButton->setText("SINGLE");
            nameEdit->hide();
            for (auto *widget : trackWidgets) {
                trackIndex--;
                trackLayout->removeWidget(widget);
                widget->deleteLater();
            }

            trackWidgets.clear();
            trackIndex=0;
            typeOfObject = "single";
        }

    }
}

void CreateWidget::addAuthor(const QString &name, int indexOfTrack) {
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
        int indexOfAouthor = authors[indexOfTrack-1]->indexOf(name);
        if (indexOfAouthor != -1) {
            authors[indexOfTrack-1]->removeAt(indexOfAouthor);
        }
        // for (auto *vec : authors) {
        //     delete vec;
        // }
        // authors.clear();
    });
}


void CreateWidget::chooseImage()
{
    fileCoverName = QFileDialog::getOpenFileName(this, "Select Cover Image", "", "Images (*.png *.jpg *.jpeg)");
    if (!fileCoverName.isEmpty()) {
        QPixmap originalPixmap(fileCoverName);
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




TrackWidget::TrackWidget(QString fileName, int index, QWidget *parent)
    :QWidget(parent),
    fileName(fileName),
    trackIndex(index)
{
    setFixedHeight(100);

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    auto *trackrotmLayout = new QHBoxLayout();
    outerLayout->addLayout(trackrotmLayout);
    outerLayout->setContentsMargins(10, 10, 10, 10);
    // outerLayout->setSpacing(5);

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

    titleEdit = new QLineEdit();
    connect(titleEdit, &QLineEdit::textChanged, this, [=]() {
        resetLineEditStyle(titleEdit);
    });
    titleEdit->setPlaceholderText("Title");
    titleEdit->setStyleSheet("QLineEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");

    // Название файла
    QLabel *fileLabel = new QLabel(QFileInfo(fileName).fileName());
    fileLabel->setStyleSheet("color: gray; font-style: italic;");
    //fileLabel->setMaximumWidth(100);
    // fileLabel->setStyleSheet(
    //     "QLabel {"
    //     " color: gray;"
    //     " font-style: italic;"
    //     " qproperty-alignment: AlignLeft | AlignVCenter;"
    //     " max-width: 200px;"  // пример ширины
    //     " text-overflow: ellipsis;"  // работает в некоторых стилях
    //     " white-space: nowrap;"
    //     " overflow: hidden;"
    //     "}"
    //     );

    tracLyricsEdit = new QTextEdit();
    tracLyricsEdit->setPlaceholderText("Lyrics");
    tracLyricsEdit->setStyleSheet("QTextEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; }");
    tracLyricsEdit->setFixedHeight(50); // стартовая высота
    tracLyricsEdit->setWordWrapMode(QTextOption::WordWrap);
    tracLyricsEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tracLyricsEdit->setContentsMargins(0, 0, 0, 0);
    tracLyricsEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);


    // connect(tracLyricsEdit, &QTextEdit::textChanged, this, [=]() {
    //     QTextDocument *doc = tracLyricsEdit->document();
    //     doc->setTextWidth(tracLyricsEdit->viewport()->width());
    //     QSize docSize = doc->size().toSize();

    //     int newHeight = docSize.height(); // добавим чуть-чуть на отступ
    //     if (newHeight < 30)
    //         newHeight = 30;
    //     else if (newHeight > 100) {
    //         newHeight = 80;
    //         tracLyricsEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //     } else {
    //         tracLyricsEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //     }

    //     tracLyricsEdit->setFixedHeight(newHeight);
    // });

    // подключаем eventFilter
    tracLyricsEdit->installEventFilter(this);

    // отдельно кнопку Collapse:
    QPushButton *collapseButton = new QPushButton("OK");
    collapseButton->setStyleSheet("QPushButton { color: white; background-color: #222; border: 1px solid #555; border-radius: 5px; padding: 5px; }"
                                  "QPushButton:hover { background-color: #444; }");
    collapseButton->setFixedHeight(30);
    collapseButton->setFixedWidth(50);

    connect(collapseButton, &QPushButton::clicked, this, [=]() {
        tracLyricsEdit->setFixedHeight(50);
        tracLyricsEdit->clearFocus();
    });

    // Layout для QTextEdit + кнопка
    QHBoxLayout *lyricsLayout = new QHBoxLayout();
    lyricsLayout->addWidget(tracLyricsEdit);
    lyricsLayout->addWidget(collapseButton);


    // Название файла
    QVector<QString> *trackAuthors = new QVector<QString>;
    numberLabel = new QLabel(QString::number(trackIndex));
    numberLabel->setStyleSheet("color: white; font-size: 20px; font-family: 'K2D'; font-weight: bold; text-align:left;");

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

    addAuthorButton = new QPushButton("+ Add author");
    addAuthorButton->setFlat(true);
    addAuthorButton->setStyleSheet(
        "QPushButton {"
        "font-weight: bold;"
        "font-size: 15 px;"
        "font-family: 'Tahoma';"
        "background: none;"
        "border: none;"
        "text-decoration: none;"
        "}"
        "QPushButton:hover {"
        "text-decoration: underline;"
        "}");
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
    int indexOfTrack = trackIndex;

    // Примерные кнопки результатов
    for (int i = 1; i <= 3; ++i) {
        QPushButton *resultButton = new QPushButton(QString("Author %1").arg(i));
        resultButton->setStyleSheet("text-align: left; color: white;");
        resultsLayout->addWidget(resultButton);

        // Добавление автора по нажати

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

    // Показ popup при нажатии
    connect(addAuthorButton, &QPushButton::clicked, this, [=]() {
        QPoint buttonPos = addAuthorButton->mapToGlobal(QPoint(0, addAuthorButton->height()));
        addAuthorButton->setStyleSheet(
            "QPushButton {"
            "font-weight: bold;"
            "font-size: 15 px;"
            "font-family: 'Tahoma';"
            "background: none;"
            "border: none;"
            "text-decoration: none;"
            "}"
            "QPushButton:hover {"
            "text-decoration: underline;"
            "}");
        popupWidget->move(buttonPos);
        popupWidget->show();
    });

    authorLayout->addWidget(addAuthorButton);
    QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    authorLayout->addItem(spacer);

    connect(removeBtn, &QPushButton::clicked, this, [=]() {
        emit removeButtonClicked();
    });

    // Компоновка
    trackrotmLayout->addWidget(numberLabel);
    trackrotmLayout->addWidget(titleEdit);
    trackrotmLayout->addWidget(fileLabel);
    trackrotmLayout->addLayout(lyricsLayout);
    trackrotmLayout->addLayout(headerLayout);
    outerLayout->addWidget(authorScrollArea);
}

bool TrackWidget::eventFilter(QObject *obj, QEvent *event) {
    if (obj == tracLyricsEdit) {
        //this->parentWidget()->setFixedHeight(200);
        if (event->type() == QEvent::FocusIn) {
            tracLyricsEdit->setFixedHeight(100);
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
        int indexOfAouthor = authors.indexOf(name);
        if (indexOfAouthor != -1) {
            authors.removeAt(indexOfAouthor);
        }

    });
}

QLineEdit *TrackWidget::getTitleEdit(){
    return titleEdit;
}

QVector<QString> *TrackWidget::getAuthors(){
    return &authors;
}

int TrackWidget::getIndex(){
    return trackIndex;
}

void TrackWidget::setIndex(int newIndex){
    trackIndex = newIndex;
    numberLabel->setText(QString::number(trackIndex));
}

void TrackWidget::setAddButtonStyle(QString color){
    addAuthorButton->setStyleSheet(
        "QPushButton {"
        "color:" + color + ";"
        "font-weight: bold;"
        "font-size: 15 px;"
        "font-family: 'Tahoma';"
        "background: none;"
        "border: none;"
        "text-decoration: none;"
        "}"
        "QPushButton:hover {"
        "text-decoration: underline;"
    "}");
}

void TrackWidget::resetLineEditStyle(QLineEdit *lineEdit){
    lineEdit->setStyleSheet("QLineEdit {color: white; font-size: 18px; background-color: #111; border: 1px solid #333; border-radius: 5px; padding: 5px; }");
}





