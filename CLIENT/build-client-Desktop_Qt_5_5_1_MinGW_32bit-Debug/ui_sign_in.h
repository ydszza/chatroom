/********************************************************************************
** Form generated from reading UI file 'sign_in.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGN_IN_H
#define UI_SIGN_IN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sign_in
{
public:
    QPushButton *pushButton_sign_in;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit_account;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_password;

    void setupUi(QWidget *sign_in)
    {
        if (sign_in->objectName().isEmpty())
            sign_in->setObjectName(QStringLiteral("sign_in"));
        sign_in->resize(400, 300);
        pushButton_sign_in = new QPushButton(sign_in);
        pushButton_sign_in->setObjectName(QStringLiteral("pushButton_sign_in"));
        pushButton_sign_in->setGeometry(QRect(170, 180, 60, 23));
        pushButton_sign_in->setMaximumSize(QSize(60, 16777215));
        layoutWidget = new QWidget(sign_in);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(110, 90, 165, 22));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit_account = new QLineEdit(layoutWidget);
        lineEdit_account->setObjectName(QStringLiteral("lineEdit_account"));

        horizontalLayout->addWidget(lineEdit_account);

        layoutWidget1 = new QWidget(sign_in);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(110, 130, 165, 22));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_password = new QLineEdit(layoutWidget1);
        lineEdit_password->setObjectName(QStringLiteral("lineEdit_password"));
        lineEdit_password->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(lineEdit_password);

        layoutWidget->raise();
        layoutWidget->raise();
        pushButton_sign_in->raise();

        retranslateUi(sign_in);

        QMetaObject::connectSlotsByName(sign_in);
    } // setupUi

    void retranslateUi(QWidget *sign_in)
    {
        sign_in->setWindowTitle(QApplication::translate("sign_in", "\347\231\273\345\275\225", 0));
        pushButton_sign_in->setText(QApplication::translate("sign_in", "\347\231\273\345\275\225", 0));
        label->setText(QApplication::translate("sign_in", "\350\264\246\345\217\267", 0));
        lineEdit_account->setText(QApplication::translate("sign_in", "1454865804", 0));
        label_2->setText(QApplication::translate("sign_in", "\345\257\206\347\240\201", 0));
        lineEdit_password->setText(QApplication::translate("sign_in", "1234567890", 0));
    } // retranslateUi

};

namespace Ui {
    class sign_in: public Ui_sign_in {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGN_IN_H
