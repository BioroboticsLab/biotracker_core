/********************************************************************************
** Form generated from reading UI file 'CameraDeviceSelect.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERADEVICESELECT_H
#define UI_CAMERADEVICESELECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CameraDeviceSelect
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox_VideoDevice;

    void setupUi(QDialog *CameraDeviceSelect)
    {
        if (CameraDeviceSelect->objectName().isEmpty())
            CameraDeviceSelect->setObjectName(QStringLiteral("CameraDeviceSelect"));
        CameraDeviceSelect->resize(400, 300);
        buttonBox = new QDialogButtonBox(CameraDeviceSelect);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        widget = new QWidget(CameraDeviceSelect);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(11, 11, 371, 27));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        comboBox_VideoDevice = new QComboBox(widget);
        comboBox_VideoDevice->setObjectName(QStringLiteral("comboBox_VideoDevice"));

        horizontalLayout->addWidget(comboBox_VideoDevice);


        retranslateUi(CameraDeviceSelect);
        QObject::connect(buttonBox, SIGNAL(accepted()), CameraDeviceSelect, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CameraDeviceSelect, SLOT(reject()));

        QMetaObject::connectSlotsByName(CameraDeviceSelect);
    } // setupUi

    void retranslateUi(QDialog *CameraDeviceSelect)
    {
        CameraDeviceSelect->setWindowTitle(QApplication::translate("CameraDeviceSelect", "Dialog", 0));
        label->setText(QApplication::translate("CameraDeviceSelect", "Select Camera Device", 0));
    } // retranslateUi

};

namespace Ui {
    class CameraDeviceSelect: public Ui_CameraDeviceSelect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERADEVICESELECT_H
