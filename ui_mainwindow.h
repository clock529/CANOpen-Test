/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *connect;
    QPushButton *close;
    QPushButton *enable;
    QPushButton *running;
    QPushButton *stop;
    QPushButton *PDO;
    QPushButton *reset;
    QPushButton *gotozero;
    QPushButton *gotozero_2;
    QPushButton *basic;
    QPushButton *reset_2;
    QPushButton *running_2;
    QPushButton *running_3;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(576, 535);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        connect = new QPushButton(centralWidget);
        connect->setObjectName(QString::fromUtf8("connect"));
        connect->setGeometry(QRect(20, 30, 91, 41));
        close = new QPushButton(centralWidget);
        close->setObjectName(QString::fromUtf8("close"));
        close->setGeometry(QRect(130, 30, 89, 41));
        enable = new QPushButton(centralWidget);
        enable->setObjectName(QString::fromUtf8("enable"));
        enable->setGeometry(QRect(20, 100, 91, 41));
        running = new QPushButton(centralWidget);
        running->setObjectName(QString::fromUtf8("running"));
        running->setGeometry(QRect(250, 230, 91, 41));
        stop = new QPushButton(centralWidget);
        stop->setObjectName(QString::fromUtf8("stop"));
        stop->setGeometry(QRect(360, 240, 91, 41));
        PDO = new QPushButton(centralWidget);
        PDO->setObjectName(QString::fromUtf8("PDO"));
        PDO->setGeometry(QRect(360, 170, 91, 41));
        reset = new QPushButton(centralWidget);
        reset->setObjectName(QString::fromUtf8("reset"));
        reset->setGeometry(QRect(20, 170, 91, 41));
        gotozero = new QPushButton(centralWidget);
        gotozero->setObjectName(QString::fromUtf8("gotozero"));
        gotozero->setGeometry(QRect(240, 170, 91, 41));
        gotozero_2 = new QPushButton(centralWidget);
        gotozero_2->setObjectName(QString::fromUtf8("gotozero_2"));
        gotozero_2->setGeometry(QRect(130, 230, 91, 41));
        basic = new QPushButton(centralWidget);
        basic->setObjectName(QString::fromUtf8("basic"));
        basic->setGeometry(QRect(20, 230, 91, 41));
        reset_2 = new QPushButton(centralWidget);
        reset_2->setObjectName(QString::fromUtf8("reset_2"));
        reset_2->setGeometry(QRect(140, 170, 91, 41));
        running_2 = new QPushButton(centralWidget);
        running_2->setObjectName(QString::fromUtf8("running_2"));
        running_2->setGeometry(QRect(170, 310, 91, 41));
        running_3 = new QPushButton(centralWidget);
        running_3->setObjectName(QString::fromUtf8("running_3"));
        running_3->setGeometry(QRect(50, 310, 91, 41));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 576, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        connect->setText(QApplication::translate("MainWindow", "\350\277\236\346\216\245", nullptr));
        close->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255", nullptr));
        enable->setText(QApplication::translate("MainWindow", "\344\275\277\350\203\275", nullptr));
        running->setText(QApplication::translate("MainWindow", "\350\277\220\350\241\214", nullptr));
        stop->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242", nullptr));
        PDO->setText(QApplication::translate("MainWindow", "PDO", nullptr));
        reset->setText(QApplication::translate("MainWindow", "reset", nullptr));
        gotozero->setText(QApplication::translate("MainWindow", "gotozero", nullptr));
        gotozero_2->setText(QApplication::translate("MainWindow", "guide", nullptr));
        basic->setText(QApplication::translate("MainWindow", "basic", nullptr));
        reset_2->setText(QApplication::translate("MainWindow", "reset2", nullptr));
        running_2->setText(QApplication::translate("MainWindow", "\350\277\220\350\241\2143", nullptr));
        running_3->setText(QApplication::translate("MainWindow", "\350\277\220\350\241\2142", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
