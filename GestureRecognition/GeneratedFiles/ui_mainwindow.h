/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionExit;
    QAction *actionAboutSDK;
    QAction *actionAboutQt;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGraphicsView *depthView;
    QGraphicsView *colorView;
    QGraphicsView *handView;
    QGraphicsView *matchView;
    QMenuBar *menuBar;
    QMenu *menuKinect;
    QMenu *menuAbout;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(812, 566);
        actionStart = new QAction(MainWindowClass);
        actionStart->setObjectName(QString::fromUtf8("actionStart"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/MainWindow/Resources/Images/start.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart->setIcon(icon);
        actionStop = new QAction(MainWindowClass);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/MainWindow/Resources/Images/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon1);
        actionExit = new QAction(MainWindowClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/MainWindow/Resources/Images/exit.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon2);
        actionAboutSDK = new QAction(MainWindowClass);
        actionAboutSDK->setObjectName(QString::fromUtf8("actionAboutSDK"));
        actionAboutQt = new QAction(MainWindowClass);
        actionAboutQt->setObjectName(QString::fromUtf8("actionAboutQt"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        depthView = new QGraphicsView(centralWidget);
        depthView->setObjectName(QString::fromUtf8("depthView"));

        gridLayout->addWidget(depthView, 0, 0, 1, 1);

        colorView = new QGraphicsView(centralWidget);
        colorView->setObjectName(QString::fromUtf8("colorView"));

        gridLayout->addWidget(colorView, 0, 1, 1, 1);

        handView = new QGraphicsView(centralWidget);
        handView->setObjectName(QString::fromUtf8("handView"));

        gridLayout->addWidget(handView, 1, 0, 1, 1);

        matchView = new QGraphicsView(centralWidget);
        matchView->setObjectName(QString::fromUtf8("matchView"));

        gridLayout->addWidget(matchView, 1, 1, 1, 1);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 812, 21));
        menuKinect = new QMenu(menuBar);
        menuKinect->setObjectName(QString::fromUtf8("menuKinect"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuKinect->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuKinect->addAction(actionStart);
        menuKinect->addAction(actionStop);
        menuKinect->addAction(actionExit);
        menuAbout->addAction(actionAboutSDK);
        menuAbout->addAction(actionAboutQt);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionStart->setText(QApplication::translate("MainWindowClass", "Start", 0, QApplication::UnicodeUTF8));
        actionStop->setText(QApplication::translate("MainWindowClass", "Stop", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindowClass", "Exit", 0, QApplication::UnicodeUTF8));
        actionAboutSDK->setText(QApplication::translate("MainWindowClass", "AboutSDK", 0, QApplication::UnicodeUTF8));
        actionAboutQt->setText(QApplication::translate("MainWindowClass", "AboutQt", 0, QApplication::UnicodeUTF8));
        menuKinect->setTitle(QApplication::translate("MainWindowClass", "Kinect", 0, QApplication::UnicodeUTF8));
        menuAbout->setTitle(QApplication::translate("MainWindowClass", "About", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
