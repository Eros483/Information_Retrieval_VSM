/********************************************************************************
** Form generated from reading UI file 'vsm.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSM_H
#define UI_VSM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_vsm
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *vsm)
    {
        if (vsm->objectName().isEmpty())
            vsm->setObjectName("vsm");
        vsm->resize(800, 600);
        centralwidget = new QWidget(vsm);
        centralwidget->setObjectName("centralwidget");
        vsm->setCentralWidget(centralwidget);
        menubar = new QMenuBar(vsm);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        vsm->setMenuBar(menubar);
        statusbar = new QStatusBar(vsm);
        statusbar->setObjectName("statusbar");
        vsm->setStatusBar(statusbar);

        retranslateUi(vsm);

        QMetaObject::connectSlotsByName(vsm);
    } // setupUi

    void retranslateUi(QMainWindow *vsm)
    {
        vsm->setWindowTitle(QCoreApplication::translate("vsm", "vsm", nullptr));
    } // retranslateUi

};

namespace Ui {
    class vsm: public Ui_vsm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSM_H
