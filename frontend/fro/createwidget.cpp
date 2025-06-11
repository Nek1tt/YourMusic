#include <QScreen> //for Qscreen *screen = QApplication::primaryScreen();
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
// #include <QWebSocket>
#include <QMessageBox>
#include <QDir>


#include "createwidget.h"
#include "myalbumswidget.h"


CreateWidget::CreateWidget(QWidget *parent, QWidget *tab)
    : QWidget(parent)
{
    // Initialize buttons
    button1 = new QPushButton("Add Noize");
    button2 = new QPushButton("Add Anacondaz");
    button3 = new QPushButton("Add Oxxy");
    button1->setFixedWidth(150);
    button2->setFixedWidth(150);
    button3->setFixedWidth(150);

    // Create a vertical layout for the buttons
    createLayout = new QVBoxLayout(tab);

    // Add buttons to the layout
    createLayout->addWidget(button1);
    createLayout->addWidget(button2);
    createLayout->addWidget(button3);

    connect(button1, &QPushButton::clicked, this, &CreateWidget::onAddNoizeClicked);
    connect(button2, &QPushButton::clicked, this, &CreateWidget::onAddAnacondazClicked);
    connect(button3, &QPushButton::clicked, this, &CreateWidget::onAddOxxyClicked);

    // Set the layout of the Create tab to this layout
   // createtab->setLayout(createLayout);
}


void CreateWidget::onAddNoizeClicked()
{
    album noizealbum = {"the_entrance_in_city", "noizeee" " ../resources/imgs/covers/cover1.png"};
    write_album(noizealbum);
    // Action when "Add Noize" button is clicked
}

void CreateWidget::onAddAnacondazClicked()
{
    album anacondazalbum = {"call_me", "anacondaz" " ../resources/imgs/covers/cover0.png"};
    write_album(anacondazalbum);
    // Action when "Add Anacondaz" button is clicked
}

void CreateWidget::onAddOxxyClicked()
{
    album oxxyalbum = {"the_beauty_and_uglyness", "oxxy" " ../resources/imgs/covers/cover2.png"};
    write_album(oxxyalbum);
    // Action when "Add Oxxy" button is clicked
}
