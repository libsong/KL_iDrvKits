/********************************************************************************
** Form generated from reading UI file 'targetwininfo.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TARGETWININFO_H
#define UI_TARGETWININFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <qiplineedit.h>

QT_BEGIN_NAMESPACE

class Ui_TargetWinInfo
{
public:
    QPushButton *pushButton_Tyes;
    QGroupBox *groupBox_2;
    QIPLineEdit *lineEdit_Tipaddr;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit_Tpwd;
    QLineEdit *lineEdit_Tusr;
    QLabel *label;

    void setupUi(QDialog *TargetWinInfo)
    {
        if (TargetWinInfo->objectName().isEmpty())
            TargetWinInfo->setObjectName(QStringLiteral("TargetWinInfo"));
        TargetWinInfo->resize(309, 184);
        pushButton_Tyes = new QPushButton(TargetWinInfo);
        pushButton_Tyes->setObjectName(QStringLiteral("pushButton_Tyes"));
        pushButton_Tyes->setGeometry(QRect(110, 140, 75, 23));
        groupBox_2 = new QGroupBox(TargetWinInfo);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 281, 111));
        lineEdit_Tipaddr = new QIPLineEdit(groupBox_2);
        lineEdit_Tipaddr->setObjectName(QStringLiteral("lineEdit_Tipaddr"));
        lineEdit_Tipaddr->setGeometry(QRect(100, 20, 151, 20));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 50, 71, 16));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 80, 71, 16));
        lineEdit_Tpwd = new QLineEdit(groupBox_2);
        lineEdit_Tpwd->setObjectName(QStringLiteral("lineEdit_Tpwd"));
        lineEdit_Tpwd->setGeometry(QRect(100, 80, 151, 20));
        lineEdit_Tusr = new QLineEdit(groupBox_2);
        lineEdit_Tusr->setObjectName(QStringLiteral("lineEdit_Tusr"));
        lineEdit_Tusr->setGeometry(QRect(100, 50, 151, 20));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 20, 61, 16));

        retranslateUi(TargetWinInfo);

        QMetaObject::connectSlotsByName(TargetWinInfo);
    } // setupUi

    void retranslateUi(QDialog *TargetWinInfo)
    {
        TargetWinInfo->setWindowTitle(QApplication::translate("TargetWinInfo", "\347\231\273\345\275\225\344\277\241\346\201\257", Q_NULLPTR));
        pushButton_Tyes->setText(QApplication::translate("TargetWinInfo", "\347\241\256\345\256\232", Q_NULLPTR));
        groupBox_2->setTitle(QString());
        label_2->setText(QApplication::translate("TargetWinInfo", "\347\231\273\345\275\225\347\224\250\346\210\267\345\220\215", Q_NULLPTR));
        label_3->setText(QApplication::translate("TargetWinInfo", "\347\224\250\346\210\267\345\220\215\345\257\206\347\240\201", Q_NULLPTR));
        label->setText(QApplication::translate("TargetWinInfo", "IP \345\234\260\345\235\200", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TargetWinInfo: public Ui_TargetWinInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TARGETWININFO_H
