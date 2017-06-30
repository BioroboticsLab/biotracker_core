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
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_2_lowH;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QLineEdit *lineEdit_3_highH;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_4;
    QLineEdit *lineEdit_4_lowS;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *lineEdit_5_HighS;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_6;
    QLineEdit *lineEdit_6_LowV;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QLineEdit *lineEdit_7_HighV;
    QPushButton *pushButton;
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
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_2_lowH = new QLineEdit(tab);
        lineEdit_2_lowH->setObjectName(QStringLiteral("lineEdit_2_lowH"));

        horizontalLayout_2->addWidget(lineEdit_2_lowH);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_3 = new QLabel(tab);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_6->addWidget(label_3);

        lineEdit_3_highH = new QLineEdit(tab);
        lineEdit_3_highH->setObjectName(QStringLiteral("lineEdit_3_highH"));

        horizontalLayout_6->addWidget(lineEdit_3_highH);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_4 = new QLabel(tab);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_7->addWidget(label_4);

        lineEdit_4_lowS = new QLineEdit(tab);
        lineEdit_4_lowS->setObjectName(QStringLiteral("lineEdit_4_lowS"));

        horizontalLayout_7->addWidget(lineEdit_4_lowS);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_5 = new QLabel(tab);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_5->addWidget(label_5);

        lineEdit_5_HighS = new QLineEdit(tab);
        lineEdit_5_HighS->setObjectName(QStringLiteral("lineEdit_5_HighS"));

        horizontalLayout_5->addWidget(lineEdit_5_HighS);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_6 = new QLabel(tab);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_4->addWidget(label_6);

        lineEdit_6_LowV = new QLineEdit(tab);
        lineEdit_6_LowV->setObjectName(QStringLiteral("lineEdit_6_LowV"));

        horizontalLayout_4->addWidget(lineEdit_6_LowV);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_7 = new QLabel(tab);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_8->addWidget(label_7);

        lineEdit_7_HighV = new QLineEdit(tab);
        lineEdit_7_HighV->setObjectName(QStringLiteral("lineEdit_7_HighV"));

        horizontalLayout_8->addWidget(lineEdit_7_HighV);


        verticalLayout->addLayout(horizontalLayout_8);

        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);


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
        label_2->setText(QApplication::translate("TrackerParameterView", "LowH", Q_NULLPTR));
        label_3->setText(QApplication::translate("TrackerParameterView", "HighH", Q_NULLPTR));
        label_4->setText(QApplication::translate("TrackerParameterView", "LowS", Q_NULLPTR));
        label_5->setText(QApplication::translate("TrackerParameterView", "HighS", Q_NULLPTR));
        label_6->setText(QApplication::translate("TrackerParameterView", "LowV", Q_NULLPTR));
        label_7->setText(QApplication::translate("TrackerParameterView", "HighV", Q_NULLPTR));
        pushButton->setText(QApplication::translate("TrackerParameterView", "Set Values", Q_NULLPTR));
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

#endif // UI_TRACKERPARAMETERVIEW_H
