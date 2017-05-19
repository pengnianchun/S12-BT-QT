/********************************************************************************
** Form generated from reading UI file 'devaddrinputdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVADDRINPUTDIALOG_H
#define UI_DEVADDRINPUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DevAddrInputDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpinBox *spinBoxStartAddr;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonConfirm;
    QSpacerItem *horizontalSpacer1;
    QPushButton *pushButtonCancel;
    QSpacerItem *horizontalSpacer2;

    void setupUi(QDialog *DevAddrInputDialog)
    {
        if (DevAddrInputDialog->objectName().isEmpty())
            DevAddrInputDialog->setObjectName(QStringLiteral("DevAddrInputDialog"));
        DevAddrInputDialog->resize(278, 107);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/easyicon_net.png"), QSize(), QIcon::Normal, QIcon::Off);
        DevAddrInputDialog->setWindowIcon(icon);
        gridLayout = new QGridLayout(DevAddrInputDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 12, -1, 18);
        label = new QLabel(DevAddrInputDialog);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        spinBoxStartAddr = new QSpinBox(DevAddrInputDialog);
        spinBoxStartAddr->setObjectName(QStringLiteral("spinBoxStartAddr"));
        spinBoxStartAddr->setMinimum(1);
        spinBoxStartAddr->setMaximum(65535);
        spinBoxStartAddr->setDisplayIntegerBase(16);

        horizontalLayout_2->addWidget(spinBoxStartAddr);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonConfirm = new QPushButton(DevAddrInputDialog);
        pushButtonConfirm->setObjectName(QStringLiteral("pushButtonConfirm"));

        horizontalLayout->addWidget(pushButtonConfirm);

        horizontalSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer1);

        pushButtonCancel = new QPushButton(DevAddrInputDialog);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));

        horizontalLayout->addWidget(pushButtonCancel);

        horizontalSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer2);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 5);
        verticalLayout->setStretch(1, 1);

        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(DevAddrInputDialog);

        QMetaObject::connectSlotsByName(DevAddrInputDialog);
    } // setupUi

    void retranslateUi(QDialog *DevAddrInputDialog)
    {
        DevAddrInputDialog->setWindowTitle(QApplication::translate("DevAddrInputDialog", "Scan Node", Q_NULLPTR));
        label->setText(QApplication::translate("DevAddrInputDialog", "\350\257\267\350\276\223\345\205\245\347\273\223\347\202\271\345\234\260\345\235\200(Hex)\357\274\232", Q_NULLPTR));
        pushButtonConfirm->setText(QApplication::translate("DevAddrInputDialog", "\347\241\256\345\256\232", Q_NULLPTR));
        pushButtonCancel->setText(QApplication::translate("DevAddrInputDialog", "\345\217\226\346\266\210", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DevAddrInputDialog: public Ui_DevAddrInputDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVADDRINPUTDIALOG_H
