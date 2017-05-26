/********************************************************************************
** Form generated from reading UI file 'TrackerParameterView.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRACKERPARAMETERVIEW_H
#define UI_TRACKERPARAMETERVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrackerParameterView
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_2_binThresh;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QLineEdit *lineEdit_3_SizeErode;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_4;
    QLineEdit *lineEdit_4_SizeDilate;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *lineEdit_5_MogHist;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_6;
    QLineEdit *lineEdit_6_MogThresh;
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

    void setupUi(QWidget *TrackerParameterView)
    {
        if (TrackerParameterView->objectName().isEmpty())
            TrackerParameterView->setObjectName(QStringLiteral("TrackerParameterView"));
        TrackerParameterView->resize(208, 360);
        gridLayout = new QGridLayout(TrackerParameterView);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(TrackerParameterView);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(TrackerParameterView);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(TrackerParameterView);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_2_binThresh = new QLineEdit(TrackerParameterView);
        lineEdit_2_binThresh->setObjectName(QStringLiteral("lineEdit_2_binThresh"));

        horizontalLayout_2->addWidget(lineEdit_2_binThresh);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_3 = new QLabel(TrackerParameterView);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_6->addWidget(label_3);

        lineEdit_3_SizeErode = new QLineEdit(TrackerParameterView);
        lineEdit_3_SizeErode->setObjectName(QStringLiteral("lineEdit_3_SizeErode"));

        horizontalLayout_6->addWidget(lineEdit_3_SizeErode);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_4 = new QLabel(TrackerParameterView);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_7->addWidget(label_4);

        lineEdit_4_SizeDilate = new QLineEdit(TrackerParameterView);
        lineEdit_4_SizeDilate->setObjectName(QStringLiteral("lineEdit_4_SizeDilate"));

        horizontalLayout_7->addWidget(lineEdit_4_SizeDilate);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_5 = new QLabel(TrackerParameterView);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_5->addWidget(label_5);

        lineEdit_5_MogHist = new QLineEdit(TrackerParameterView);
        lineEdit_5_MogHist->setObjectName(QStringLiteral("lineEdit_5_MogHist"));

        horizontalLayout_5->addWidget(lineEdit_5_MogHist);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_6 = new QLabel(TrackerParameterView);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_4->addWidget(label_6);

        lineEdit_6_MogThresh = new QLineEdit(TrackerParameterView);
        lineEdit_6_MogThresh->setObjectName(QStringLiteral("lineEdit_6_MogThresh"));

        horizontalLayout_4->addWidget(lineEdit_6_MogThresh);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_7 = new QLabel(TrackerParameterView);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_8->addWidget(label_7);

        lineEdit_7_MogBack = new QLineEdit(TrackerParameterView);
        lineEdit_7_MogBack->setObjectName(QStringLiteral("lineEdit_7_MogBack"));

        horizontalLayout_8->addWidget(lineEdit_7_MogBack);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_8 = new QLabel(TrackerParameterView);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_10->addWidget(label_8);

        lineEdit_8_MinBlob = new QLineEdit(TrackerParameterView);
        lineEdit_8_MinBlob->setObjectName(QStringLiteral("lineEdit_8_MinBlob"));

        horizontalLayout_10->addWidget(lineEdit_8_MinBlob);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_9 = new QLabel(TrackerParameterView);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_3->addWidget(label_9);

        lineEdit_9MaxBlob = new QLineEdit(TrackerParameterView);
        lineEdit_9MaxBlob->setObjectName(QStringLiteral("lineEdit_9MaxBlob"));

        horizontalLayout_3->addWidget(lineEdit_9MaxBlob);


        verticalLayout->addLayout(horizontalLayout_3);


        gridLayout->addLayout(verticalLayout, 1, 1, 1, 1);

        pushButton = new QPushButton(TrackerParameterView);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);


        retranslateUi(TrackerParameterView);

        QMetaObject::connectSlotsByName(TrackerParameterView);
    } // setupUi

    void retranslateUi(QWidget *TrackerParameterView)
    {
        TrackerParameterView->setWindowTitle(QApplication::translate("TrackerParameterView", "Form", 0));
        label->setText(QApplication::translate("TrackerParameterView", "Threshold", 0));
        label_2->setText(QApplication::translate("TrackerParameterView", "BinThreshold", 0));
        label_3->setText(QApplication::translate("TrackerParameterView", "Size Erode", 0));
        label_4->setText(QApplication::translate("TrackerParameterView", "Size Dilate", 0));
        label_5->setText(QApplication::translate("TrackerParameterView", "mog2 History", 0));
        label_6->setText(QApplication::translate("TrackerParameterView", "mog2 Var Threshold", 0));
        label_7->setText(QApplication::translate("TrackerParameterView", "mog2 Background Ratio", 0));
        label_8->setText(QApplication::translate("TrackerParameterView", "Min Blob Size", 0));
        label_9->setText(QApplication::translate("TrackerParameterView", "Max Blob Size", 0));
        pushButton->setText(QApplication::translate("TrackerParameterView", "Set Values", 0));
    } // retranslateUi

};

namespace Ui {
    class TrackerParameterView: public Ui_TrackerParameterView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRACKERPARAMETERVIEW_H
