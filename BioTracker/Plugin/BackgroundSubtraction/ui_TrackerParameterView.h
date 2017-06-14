/********************************************************************************
** Form generated from reading UI file 'TrackerParameterView.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef TRACKERPARAMETERVIEW_H
#define TRACKERPARAMETERVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrackerParameterView
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_13;
    QLineEdit *lineEdit_areaw;
    QLabel *label_14;
    QLineEdit *lineEdit_areah;
    QLabel *label_15;
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
    QLabel *label;
    QComboBox *comboBoxSendImage;
    QPushButton *pushButtonResetBackground;
    QHBoxLayout *horizontalLayout;
    QLabel *label_12;
    QLineEdit *lineEditNoFish;
    QPushButton *pushButtonNoFish;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_10;
    QLabel *label_11;

    void setupUi(QWidget *TrackerParameterView)
    {
        if (TrackerParameterView->objectName().isEmpty())
            TrackerParameterView->setObjectName(QStringLiteral("TrackerParameterView"));
        TrackerParameterView->resize(304, 489);
        gridLayout = new QGridLayout(TrackerParameterView);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(TrackerParameterView);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tab->sizePolicy().hasHeightForWidth());
        tab->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_13 = new QLabel(tab);
        label_13->setObjectName(QStringLiteral("label_13"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy1);

        horizontalLayout_9->addWidget(label_13);

        lineEdit_areaw = new QLineEdit(tab);
        lineEdit_areaw->setObjectName(QStringLiteral("lineEdit_areaw"));

        horizontalLayout_9->addWidget(lineEdit_areaw);

        label_14 = new QLabel(tab);
        label_14->setObjectName(QStringLiteral("label_14"));
        sizePolicy1.setHeightForWidth(label_14->sizePolicy().hasHeightForWidth());
        label_14->setSizePolicy(sizePolicy1);

        horizontalLayout_9->addWidget(label_14);

        lineEdit_areah = new QLineEdit(tab);
        lineEdit_areah->setObjectName(QStringLiteral("lineEdit_areah"));

        horizontalLayout_9->addWidget(lineEdit_areah);

        label_15 = new QLabel(tab);
        label_15->setObjectName(QStringLiteral("label_15"));
        sizePolicy1.setHeightForWidth(label_15->sizePolicy().hasHeightForWidth());
        label_15->setSizePolicy(sizePolicy1);

        horizontalLayout_9->addWidget(label_15);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_2_binThresh = new QLineEdit(tab);
        lineEdit_2_binThresh->setObjectName(QStringLiteral("lineEdit_2_binThresh"));

        horizontalLayout_2->addWidget(lineEdit_2_binThresh);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_3 = new QLabel(tab);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_6->addWidget(label_3);

        lineEdit_3_SizeErode = new QLineEdit(tab);
        lineEdit_3_SizeErode->setObjectName(QStringLiteral("lineEdit_3_SizeErode"));

        horizontalLayout_6->addWidget(lineEdit_3_SizeErode);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_4 = new QLabel(tab);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_7->addWidget(label_4);

        lineEdit_4_SizeDilate = new QLineEdit(tab);
        lineEdit_4_SizeDilate->setObjectName(QStringLiteral("lineEdit_4_SizeDilate"));

        horizontalLayout_7->addWidget(lineEdit_4_SizeDilate);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_5 = new QLabel(tab);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_5->addWidget(label_5);

        lineEdit_5_MogHist = new QLineEdit(tab);
        lineEdit_5_MogHist->setObjectName(QStringLiteral("lineEdit_5_MogHist"));

        horizontalLayout_5->addWidget(lineEdit_5_MogHist);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_6 = new QLabel(tab);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_4->addWidget(label_6);

        lineEdit_6_MogThresh = new QLineEdit(tab);
        lineEdit_6_MogThresh->setObjectName(QStringLiteral("lineEdit_6_MogThresh"));

        horizontalLayout_4->addWidget(lineEdit_6_MogThresh);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_7 = new QLabel(tab);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_8->addWidget(label_7);

        lineEdit_7_MogBack = new QLineEdit(tab);
        lineEdit_7_MogBack->setObjectName(QStringLiteral("lineEdit_7_MogBack"));

        horizontalLayout_8->addWidget(lineEdit_7_MogBack);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_8 = new QLabel(tab);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_10->addWidget(label_8);

        lineEdit_8_MinBlob = new QLineEdit(tab);
        lineEdit_8_MinBlob->setObjectName(QStringLiteral("lineEdit_8_MinBlob"));

        horizontalLayout_10->addWidget(lineEdit_8_MinBlob);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_9 = new QLabel(tab);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_3->addWidget(label_9);

        lineEdit_9MaxBlob = new QLineEdit(tab);
        lineEdit_9MaxBlob->setObjectName(QStringLiteral("lineEdit_9MaxBlob"));

        horizontalLayout_3->addWidget(lineEdit_9MaxBlob);


        verticalLayout->addLayout(horizontalLayout_3);

        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(label);

        comboBoxSendImage = new QComboBox(tab);
        comboBoxSendImage->setObjectName(QStringLiteral("comboBoxSendImage"));

        verticalLayout->addWidget(comboBoxSendImage);

        pushButtonResetBackground = new QPushButton(tab);
        pushButtonResetBackground->setObjectName(QStringLiteral("pushButtonResetBackground"));

        verticalLayout->addWidget(pushButtonResetBackground);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_12 = new QLabel(tab);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout->addWidget(label_12);

        lineEditNoFish = new QLineEdit(tab);
        lineEditNoFish->setObjectName(QStringLiteral("lineEditNoFish"));

        horizontalLayout->addWidget(lineEditNoFish);


        verticalLayout->addLayout(horizontalLayout);

        pushButtonNoFish = new QPushButton(tab);
        pushButtonNoFish->setObjectName(QStringLiteral("pushButtonNoFish"));

        verticalLayout->addWidget(pushButtonNoFish);


        verticalLayout_2->addLayout(verticalLayout);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QStringLiteral("label_10"));

        verticalLayout_3->addWidget(label_10);

        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QStringLiteral("label_11"));

        verticalLayout_3->addWidget(label_11);

        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(TrackerParameterView);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TrackerParameterView);
    } // setupUi

    void retranslateUi(QWidget *TrackerParameterView)
    {
        TrackerParameterView->setWindowTitle(QApplication::translate("TrackerParameterView", "Form", Q_NULLPTR));
        label_13->setText(QApplication::translate("TrackerParameterView", "Area dimensions:", Q_NULLPTR));
        label_14->setText(QApplication::translate("TrackerParameterView", "x", Q_NULLPTR));
        label_15->setText(QApplication::translate("TrackerParameterView", "cm", Q_NULLPTR));
        label_2->setText(QApplication::translate("TrackerParameterView", "BinThreshold", Q_NULLPTR));
        label_3->setText(QApplication::translate("TrackerParameterView", "Size Erode", Q_NULLPTR));
        label_4->setText(QApplication::translate("TrackerParameterView", "Size Dilate", Q_NULLPTR));
        label_5->setText(QApplication::translate("TrackerParameterView", "mog2 History", Q_NULLPTR));
        label_6->setText(QApplication::translate("TrackerParameterView", "mog2 Var Threshold", Q_NULLPTR));
        label_7->setText(QApplication::translate("TrackerParameterView", "mog2 Background Ratio", Q_NULLPTR));
        label_8->setText(QApplication::translate("TrackerParameterView", "Min Blob Size", Q_NULLPTR));
        label_9->setText(QApplication::translate("TrackerParameterView", "Max Blob Size", Q_NULLPTR));
        pushButton->setText(QApplication::translate("TrackerParameterView", "Set Values", Q_NULLPTR));
        label->setText(QApplication::translate("TrackerParameterView", "Send Image:", Q_NULLPTR));
        comboBoxSendImage->clear();
        comboBoxSendImage->insertItems(0, QStringList()
         << QApplication::translate("TrackerParameterView", "0. None", Q_NULLPTR)
         << QApplication::translate("TrackerParameterView", "1. Binarized", Q_NULLPTR)
         << QApplication::translate("TrackerParameterView", "2. Eroded", Q_NULLPTR)
         << QApplication::translate("TrackerParameterView", "3. Dilated", Q_NULLPTR)
         << QApplication::translate("TrackerParameterView", "4. Background Sub", Q_NULLPTR)
        );
        comboBoxSendImage->setCurrentText(QApplication::translate("TrackerParameterView", "0. None", Q_NULLPTR));
        pushButtonResetBackground->setText(QApplication::translate("TrackerParameterView", "Reset Background", Q_NULLPTR));
        label_12->setText(QApplication::translate("TrackerParameterView", "# of Objects", Q_NULLPTR));
        lineEditNoFish->setText(QApplication::translate("TrackerParameterView", "2", Q_NULLPTR));
        pushButtonNoFish->setText(QApplication::translate("TrackerParameterView", "Set # (Erases History)", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("TrackerParameterView", "Tracking", Q_NULLPTR));
        label_10->setText(QApplication::translate("TrackerParameterView", "GUI options go here", Q_NULLPTR));
        label_11->setText(QApplication::translate("TrackerParameterView", "E.g. Size of markers", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("TrackerParameterView", "GUI", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TrackerParameterView: public Ui_TrackerParameterView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // TRACKERPARAMETERVIEW_H
