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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MusicMain
{
public:
    QWidget *centralwidget;

    void setupUi(QMainWindow *MusicMain)
    {
        if (MusicMain->objectName().isEmpty())
            MusicMain->setObjectName("MusicMain");
        MusicMain->setEnabled(true);
        MusicMain->resize(1006, 601);
        MusicMain->setAcceptDrops(true);
        centralwidget = new QWidget(MusicMain);
        centralwidget->setObjectName("centralwidget");
        MusicMain->setCentralWidget(centralwidget);

        retranslateUi(MusicMain);

        QMetaObject::connectSlotsByName(MusicMain);
    } // setupUi

    void retranslateUi(QMainWindow *MusicMain)
    {
        MusicMain->setWindowTitle(QCoreApplication::translate("MusicMain", "MusicMain", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MusicMain: public Ui_MusicMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MUSICMAIN_H
