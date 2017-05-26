/********************************************************************************
** Form generated from reading UI file 'CameraDevice.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERADEVICE_H
#define UI_CAMERADEVICE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CameraDevice
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QDialogButtonBox *buttonBox;
    QWidget *widget;

    void setupUi(QWidget *CameraDevice)
    {
        if (CameraDevice->objectName().isEmpty())
            CameraDevice->setObjectName(QStringLiteral("CameraDevice"));
        CameraDevice->resize(493, 300);
        layoutWidget = new QWidget(CameraDevice);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(19, 250, 461, 31));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(layoutWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout->addWidget(comboBox);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout->addWidget(buttonBox);

        widget = new QWidget(CameraDevice);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 9, 471, 231));

        retranslateUi(CameraDevice);

        QMetaObject::connectSlotsByName(CameraDevice);
    } // setupUi

    void retranslateUi(QWidget *CameraDevice)
    {
        CameraDevice->setWindowTitle(QApplication::translate("CameraDevice", "Form", 0));
        label->setText(QApplication::translate("CameraDevice", "Select Camera Device", 0));
    } // retranslateUi

};

namespace Ui {
    class CameraDevice: public Ui_CameraDevice {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERADEVICE_H
