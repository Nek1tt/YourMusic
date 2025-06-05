#include <QPainter>
#include <QPainterPath>

#include "setstyle.h"


void set_button_style(QPushButton *button, int fontsize, QString color, QString textalign, QString padb){
    button->setStyleSheet(
        "QPushButton {"
        "text-align: "+ textalign + ";"
        "padding-left: 0px;"
        "padding-bottom: " + padb +";"
        "font-weight: bold;"
        "font-size: " + QString::number(fontsize) + "px;"
        "font-family: 'Tahoma';"
        "background: none;"
        "border: none;"
        "color: " + color + ";"
        "text-decoration: none;"
        "}"
        "QPushButton:hover {"
        "text-decoration: underline;"
        "}");
}


void set_border_button_style(QPushButton *button, int fontsize, QString color, QString padb){
    button->setStyleSheet(
        "QPushButton {"
        "    text-align: center;"  // Центрируем текст
        "    font-weight: bold;"
        "font-size: " + QString::number(fontsize) + "px;"
        "    font-family: 'Tahoma';"
        "    background-color: #333333;"  // Темно-серый фон
        "    border: 1px solid white;"     // Белая рамка 1px
        "    border-radius: 4px;"          // Скругленные углы радиусом 4px
        "    text-decoration: none;"       // Убираем подчеркивание по умолчанию
        "}"
        "QPushButton:hover {"
        "    border: 2px solid white;"      // Увеличенная рамка до 2px при наведении
        "}"
        );
}


void setRoundedImage(QLabel* label, const QString& imagePath, int size, qreal radius) {
    if (!label) {
        qWarning() << "Label is null!";
        return;
    }

    QPixmap originalPixmap;
    if (!originalPixmap.load(imagePath)) {
        qWarning() << "Не удалось загрузить картинку:" << imagePath;
        return;
    }

    // Масштабируем с сохранением пропорций
    QPixmap scaledPixmap = originalPixmap.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Создаём новый пустой прозрачный QPixmap
    QPixmap roundedPixmap(size, size);
    roundedPixmap.fill(Qt::transparent);

    // Готовим Painter
    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPainterPath path;
    path.addRoundedRect(QRectF(0, 0, size, size), radius, radius);
    painter.setClipPath(path);

    // Отрисовываем исходную картинку
    painter.drawPixmap(0, 0, scaledPixmap);

    // Устанавливаем готовую картинку в label
    label->setPixmap(roundedPixmap);
    label->setFixedSize(size, size);
    label->setAlignment(Qt::AlignHCenter);
}


