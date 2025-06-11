/********************************************************************************
** Form generated from reading UI file 'musicmain.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MUSICMAIN_H
#define UI_MUSICMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MusicMain
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *Main;
    QWidget *Create;
    QWidget *Profile;
    QPushButton *createTab;
    QPushButton *homeTab;
    QPushButton *profileTab;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MusicMain)
    {
        if (MusicMain->objectName().isEmpty())
            MusicMain->setObjectName("MusicMain");
        MusicMain->resize(800, 600);
        centralwidget = new QWidget(MusicMain);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(220, -30, 581, 631));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Light, brush1);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush1);
        QBrush brush2(QColor(255, 255, 220, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush1);
        QBrush brush3(QColor(255, 255, 255, 127));
        brush3.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush3);
#endif
        palette.setBrush(QPalette::Active, QPalette::Accent, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush3);
#endif
        palette.setBrush(QPalette::Inactive, QPalette::Accent, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush1);
        QBrush brush4(QColor(0, 0, 0, 127));
        brush4.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush4);
#endif
        palette.setBrush(QPalette::Disabled, QPalette::Accent, brush1);
        tabWidget->setPalette(palette);
        Main = new QWidget();
        Main->setObjectName("Main");
        tabWidget->addTab(Main, QString());
        Create = new QWidget();
        Create->setObjectName("Create");
        tabWidget->addTab(Create, QString());
        Profile = new QWidget();
        Profile->setObjectName("Profile");
        tabWidget->addTab(Profile, QString());
        createTab = new QPushButton(centralwidget);
        createTab->setObjectName("createTab");
        createTab->setGeometry(QRect(70, 190, 80, 24));
        homeTab = new QPushButton(centralwidget);
        homeTab->setObjectName("homeTab");
        homeTab->setGeometry(QRect(70, 90, 80, 24));
        profileTab = new QPushButton(centralwidget);
        profileTab->setObjectName("profileTab");
        profileTab->setGeometry(QRect(70, 320, 80, 24));
        MusicMain->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MusicMain);
        statusbar->setObjectName("statusbar");
        MusicMain->setStatusBar(statusbar);

        retranslateUi(MusicMain);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MusicMain);
    } // setupUi

    void retranslateUi(QMainWindow *MusicMain)
    {
        MusicMain->setWindowTitle(QCoreApplication::translate("MusicMain", "MusicMain", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Main), QCoreApplication::translate("MusicMain", "Main", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Create), QCoreApplication::translate("MusicMain", "Search", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Profile), QCoreApplication::translate("MusicMain", "Profile", nullptr));
        createTab->setText(QCoreApplication::translate("MusicMain", "Create", nullptr));
        homeTab->setText(QCoreApplication::translate("MusicMain", "Home", nullptr));
        profileTab->setText(QCoreApplication::translate("MusicMain", "Profile", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MusicMain: public Ui_MusicMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MUSICMAIN_H
