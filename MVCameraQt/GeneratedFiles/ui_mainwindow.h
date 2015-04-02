/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLive;
    QAction *actionWhitBalance;
    QAction *actionAutoExposure;
    QWidget *centralWidget;
    QWidget *widget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(664, 554);
        actionLive = new QAction(MainWindow);
        actionLive->setObjectName(QStringLiteral("actionLive"));
        actionLive->setCheckable(true);
        QIcon icon;
        icon.addFile(QStringLiteral("playback_play.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionLive->setIcon(icon);
        actionWhitBalance = new QAction(MainWindow);
        actionWhitBalance->setObjectName(QStringLiteral("actionWhitBalance"));
        actionWhitBalance->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral("picture.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionWhitBalance->setIcon(icon1);
        actionAutoExposure = new QAction(MainWindow);
        actionAutoExposure->setObjectName(QStringLiteral("actionAutoExposure"));
        actionAutoExposure->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral("sun.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionAutoExposure->setIcon(icon2);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 640, 480));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 664, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setMovable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionLive->setText(QApplication::translate("MainWindow", "Live", 0));
#ifndef QT_NO_TOOLTIP
        actionLive->setToolTip(QApplication::translate("MainWindow", "Start Live Capture", 0));
#endif // QT_NO_TOOLTIP
        actionWhitBalance->setText(QApplication::translate("MainWindow", "WhiteBalance", 0));
#ifndef QT_NO_TOOLTIP
        actionWhitBalance->setToolTip(QApplication::translate("MainWindow", "Auto White Balance", 0));
#endif // QT_NO_TOOLTIP
        actionAutoExposure->setText(QApplication::translate("MainWindow", "AutoExposure", 0));
#ifndef QT_NO_TOOLTIP
        actionAutoExposure->setToolTip(QApplication::translate("MainWindow", "Auto Exposure", 0));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
