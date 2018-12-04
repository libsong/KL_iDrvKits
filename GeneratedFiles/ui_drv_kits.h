/********************************************************************************
** Form generated from reading UI file 'drv_kits.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRV_KITS_H
#define UI_DRV_KITS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_drv_kitsClass
{
public:
    QAction *action_usrdoc;
    QAction *action_about;
    QAction *action_ftp;
    QWidget *centralWidget;
    QPushButton *pushButton_nextstp;
    QPushButton *pushButton_cancel;
    QLabel *label_disp;
    QStackedWidget *stackedWidget_center;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *drv_kitsClass)
    {
        if (drv_kitsClass->objectName().isEmpty())
            drv_kitsClass->setObjectName(QStringLiteral("drv_kitsClass"));
        drv_kitsClass->resize(847, 508);
        action_usrdoc = new QAction(drv_kitsClass);
        action_usrdoc->setObjectName(QStringLiteral("action_usrdoc"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/drv_kits/pic/wenhao.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_usrdoc->setIcon(icon);
        action_about = new QAction(drv_kitsClass);
        action_about->setObjectName(QStringLiteral("action_about"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/drv_kits/pic/about.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_about->setIcon(icon1);
        action_ftp = new QAction(drv_kitsClass);
        action_ftp->setObjectName(QStringLiteral("action_ftp"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/drv_kits/pic/ftp.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_ftp->setIcon(icon2);
        centralWidget = new QWidget(drv_kitsClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton_nextstp = new QPushButton(centralWidget);
        pushButton_nextstp->setObjectName(QStringLiteral("pushButton_nextstp"));
        pushButton_nextstp->setGeometry(QRect(650, 430, 75, 23));
        pushButton_cancel = new QPushButton(centralWidget);
        pushButton_cancel->setObjectName(QStringLiteral("pushButton_cancel"));
        pushButton_cancel->setGeometry(QRect(750, 430, 75, 23));
        label_disp = new QLabel(centralWidget);
        label_disp->setObjectName(QStringLiteral("label_disp"));
        label_disp->setGeometry(QRect(10, 10, 821, 31));
        label_disp->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"background-color: rgb(97,195,195);"));
        stackedWidget_center = new QStackedWidget(centralWidget);
        stackedWidget_center->setObjectName(QStringLiteral("stackedWidget_center"));
        stackedWidget_center->setGeometry(QRect(10, 70, 811, 331));
        drv_kitsClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(drv_kitsClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 847, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        drv_kitsClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(drv_kitsClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        drv_kitsClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(action_ftp);
        menu_2->addAction(action_usrdoc);
        menu_2->addAction(action_about);

        retranslateUi(drv_kitsClass);

        stackedWidget_center->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(drv_kitsClass);
    } // setupUi

    void retranslateUi(QMainWindow *drv_kitsClass)
    {
        drv_kitsClass->setWindowTitle(QApplication::translate("drv_kitsClass", "drv_kits", Q_NULLPTR));
        action_usrdoc->setText(QApplication::translate("drv_kitsClass", "\344\275\277\347\224\250\350\257\264\346\230\216", Q_NULLPTR));
        action_about->setText(QApplication::translate("drv_kitsClass", "\345\205\263\344\272\216...", Q_NULLPTR));
        action_ftp->setText(QApplication::translate("drv_kitsClass", "SO \346\226\207\344\273\266\344\274\240\350\276\223", Q_NULLPTR));
        pushButton_nextstp->setText(QApplication::translate("drv_kitsClass", "\344\270\213\344\270\200\346\255\245", Q_NULLPTR));
        pushButton_cancel->setText(QApplication::translate("drv_kitsClass", "\345\217\226\346\266\210", Q_NULLPTR));
        label_disp->setText(QApplication::translate("drv_kitsClass", "TextLabel", Q_NULLPTR));
        menu->setTitle(QApplication::translate("drv_kitsClass", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("drv_kitsClass", "\345\270\256\345\212\251", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class drv_kitsClass: public Ui_drv_kitsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRV_KITS_H
