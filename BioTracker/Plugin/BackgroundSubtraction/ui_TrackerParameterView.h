/********************************************************************************
** Form generated from reading UI file 'TrackerParameterView.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRACKERPARAMETERVIEW_H
#define UI_TRACKERPARAMETERVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrackerParameterView
{
public:
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_2_binThresh;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QLineEdit *lineEdit_3_SizeErode;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_4;
    QLineEdit *lineEdit_4_SizeDilate;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QLineEdit *lineEdit_7_MogBack;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_8;
    QLineEdit *lineEdit_8_MinBlob;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_9;
    QLineEdit *lineEdit_9MaxBlob;
    QPushButton *pushButton;
    QLabel *label;
    QComboBox *comboBoxSendImage;
    QPushButton *pushButtonResetBackground;

    void setupUi(QWidget *TrackerParameterView)
    {
        if (TrackerParameterView->objectName().isEmpty())
            TrackerParameterView->setObjectName(QStringLiteral("TrackerParameterView"));
        TrackerParameterView->resize(304, 481);
        TrackerParameterView->setAutoFillBackground(false);
        verticalLayout_3 = new QVBoxLayout(TrackerParameterView);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(TrackerParameterView);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setAutoFillBackground(false);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 302, 479));
        horizontalLayout_4 = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(9, 0, -1, -1);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(scrollAreaWidgetContents);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_2_binThresh = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_2_binThresh->setObjectName(QStringLiteral("lineEdit_2_binThresh"));

        horizontalLayout_2->addWidget(lineEdit_2_binThresh);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_3 = new QLabel(scrollAreaWidgetContents);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_6->addWidget(label_3);

        lineEdit_3_SizeErode = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_3_SizeErode->setObjectName(QStringLiteral("lineEdit_3_SizeErode"));

        horizontalLayout_6->addWidget(lineEdit_3_SizeErode);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_4 = new QLabel(scrollAreaWidgetContents);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_7->addWidget(label_4);

        lineEdit_4_SizeDilate = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_4_SizeDilate->setObjectName(QStringLiteral("lineEdit_4_SizeDilate"));

        horizontalLayout_7->addWidget(lineEdit_4_SizeDilate);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_7 = new QLabel(scrollAreaWidgetContents);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_8->addWidget(label_7);

        lineEdit_7_MogBack = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_7_MogBack->setObjectName(QStringLiteral("lineEdit_7_MogBack"));

        horizontalLayout_8->addWidget(lineEdit_7_MogBack);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_8 = new QLabel(scrollAreaWidgetContents);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_10->addWidget(label_8);

        lineEdit_8_MinBlob = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_8_MinBlob->setObjectName(QStringLiteral("lineEdit_8_MinBlob"));

        horizontalLayout_10->addWidget(lineEdit_8_MinBlob);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_9 = new QLabel(scrollAreaWidgetContents);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_3->addWidget(label_9);

        lineEdit_9MaxBlob = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_9MaxBlob->setObjectName(QStringLiteral("lineEdit_9MaxBlob"));

        horizontalLayout_3->addWidget(lineEdit_9MaxBlob);


        verticalLayout->addLayout(horizontalLayout_3);

        pushButton = new QPushButton(scrollAreaWidgetContents);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label);

        comboBoxSendImage = new QComboBox(scrollAreaWidgetContents);
        comboBoxSendImage->setObjectName(QStringLiteral("comboBoxSendImage"));

        verticalLayout->addWidget(comboBoxSendImage);

        pushButtonResetBackground = new QPushButton(scrollAreaWidgetContents);
        pushButtonResetBackground->setObjectName(QStringLiteral("pushButtonResetBackground"));

        verticalLayout->addWidget(pushButtonResetBackground);


        horizontalLayout_4->addLayout(verticalLayout);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_3->addWidget(scrollArea);


        retranslateUi(TrackerParameterView);

        QMetaObject::connectSlotsByName(TrackerParameterView);
    } // setupUi

    void retranslateUi(QWidget *TrackerParameterView)
    {
        TrackerParameterView->setWindowTitle(QApplication::translate("TrackerParameterView", "Form", Q_NULLPTR));
        label_2->setText(QApplication::translate("TrackerParameterView", "BinThreshold", Q_NULLPTR));
        label_3->setText(QApplication::translate("TrackerParameterView", "Size Erode", Q_NULLPTR));
        label_4->setText(QApplication::translate("TrackerParameterView", "Size Dilate", Q_NULLPTR));
        label_7->setText(QApplication::translate("TrackerParameterView", "Background Ratio", Q_NULLPTR));
        label_8->setText(QApplication::translate("TrackerParameterView", "Min Blob Size", Q_NULLPTR));
        label_9->setText(QApplication::translate("TrackerParameterView", "Max Blob Size", Q_NULLPTR));
        pushButton->setText(QApplication::translate("TrackerParameterView", "Set Values", Q_NULLPTR));
        label->setText(QApplication::translate("TrackerParameterView", "Send Image:", Q_NULLPTR));
        comboBoxSendImage->clear();
        comboBoxSendImage->insertItems(0, QStringList()
         << QApplication::translate("TrackerParameterView", "Original", Q_NULLPTR)
         << QApplication::translate("TrackerParameterView", "Binarized", Q_NULLPTR)
         << QApplication::translate("TrackerParameterView", "Eroded", Q_NULLPTR)
         << QApplication::translate("TrackerParameterView", "Dilated", Q_NULLPTR)
         << QApplication::translate("TrackerParameterView", "Foreground", Q_NULLPTR)
         << QApplication::translate("TrackerParameterView", "Background", Q_NULLPTR)
        );
        comboBoxSendImage->setCurrentText(QApplication::translate("TrackerParameterView", "Original", Q_NULLPTR));
        pushButtonResetBackground->setText(QApplication::translate("TrackerParameterView", "Reset Background", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TrackerParameterView: public Ui_TrackerParameterView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRACKERPARAMETERVIEW_H
