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
        "    text-align: center;"
        "    font-weight: bold;"
        "font-size: " + QString::number(fontsize) + "px;"
        "    font-family: 'Tahoma';"
        "    background-color: #333333;"
        "    border: 1px solid white;"
        "    border-radius: 4px;"
        "    text-decoration: none;"
        "}"
        "QPushButton:hover {"
        "    border: 2px solid white;"
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
    QPixmap scaledPixmap = originalPixmap.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedPixmap(size, size);
    roundedPixmap.fill(Qt::transparent);
    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPainterPath path;
    path.addRoundedRect(QRectF(0, 0, size, size), radius, radius);
    painter.setClipPath(path);

    painter.drawPixmap(0, 0, scaledPixmap);

    label->setPixmap(roundedPixmap);
    label->setFixedSize(size, size);
    label->setAlignment(Qt::AlignHCenter);
}


