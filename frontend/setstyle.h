#include <QLabel>
#include <QPushButton>

void set_button_style(QPushButton *button, int fontsize, QString color = "white", QString textalign = "left", QString padb= "0px");

void set_border_button_style(QPushButton *button, int fontsize, QString color = "white", QString padb= "");

void setRoundedImage(QLabel* label, const QString& imagePath, int size, qreal radius);
