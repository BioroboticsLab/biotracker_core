/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_Video;
    QAction *actionOpen_Picture;
    QAction *actionLoad_tracking_data;
    QAction *actionSave_tracking_data;
    QAction *actionQuit;
    QAction *actionLoad_Tracker;
    QAction *actionOpen_Camera;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_6;
    QWidget *videoAreaWidget;
    QVBoxLayout *verticalLayout_2;
    QFrame *trackingArea;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *videoViewLayout;
    QWidget *videoControls;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QDockWidget *dockWidgetAlgorithm;
    QWidget *dockWidgetAlgorithmContents;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_alg;
    QGridLayout *gridLayout;
    QComboBox *comboBox_TrackerSelect;
    QHBoxLayout *horizontalLayout;
    QCheckBox *checkBox_TrackingActivated;
    QGroupBox *groupBox_tools;
    QVBoxLayout *verticalLayout_4;
    QScrollArea *scrollArea;
    QWidget *groupBoxContents;
    QDockWidget *dockWidgetNotification;
    QWidget *dockWidgetNotificationContents;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(982, 818);
        MainWindow->setDockNestingEnabled(true);
        MainWindow->setUnifiedTitleAndToolBarOnMac(false);
        actionOpen_Video = new QAction(MainWindow);
        actionOpen_Video->setObjectName(QStringLiteral("actionOpen_Video"));
        actionOpen_Picture = new QAction(MainWindow);
        actionOpen_Picture->setObjectName(QStringLiteral("actionOpen_Picture"));
        actionLoad_tracking_data = new QAction(MainWindow);
        actionLoad_tracking_data->setObjectName(QStringLiteral("actionLoad_tracking_data"));
        actionSave_tracking_data = new QAction(MainWindow);
        actionSave_tracking_data->setObjectName(QStringLiteral("actionSave_tracking_data"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionLoad_Tracker = new QAction(MainWindow);
        actionLoad_Tracker->setObjectName(QStringLiteral("actionLoad_Tracker"));
        actionOpen_Camera = new QAction(MainWindow);
        actionOpen_Camera->setObjectName(QStringLiteral("actionOpen_Camera"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        horizontalLayout_5 = new QHBoxLayout(centralWidget);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_6 = new QHBoxLayout(widget_2);
        horizontalLayout_6->setSpacing(3);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(3, 3, 3, 3);
        videoAreaWidget = new QWidget(widget_2);
        videoAreaWidget->setObjectName(QStringLiteral("videoAreaWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(videoAreaWidget->sizePolicy().hasHeightForWidth());
        videoAreaWidget->setSizePolicy(sizePolicy1);
        verticalLayout_2 = new QVBoxLayout(videoAreaWidget);
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(3, 3, 3, 3);
        trackingArea = new QFrame(videoAreaWidget);
        trackingArea->setObjectName(QStringLiteral("trackingArea"));
        sizePolicy1.setHeightForWidth(trackingArea->sizePolicy().hasHeightForWidth());
        trackingArea->setSizePolicy(sizePolicy1);
        trackingArea->setMinimumSize(QSize(0, 150));
        verticalLayout = new QVBoxLayout(trackingArea);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        videoViewLayout = new QVBoxLayout();
        videoViewLayout->setSpacing(6);
        videoViewLayout->setObjectName(QStringLiteral("videoViewLayout"));

        verticalLayout->addLayout(videoViewLayout);


        verticalLayout_2->addWidget(trackingArea);

        videoControls = new QWidget(videoAreaWidget);
        videoControls->setObjectName(QStringLiteral("videoControls"));
        videoControls->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(videoControls->sizePolicy().hasHeightForWidth());
        videoControls->setSizePolicy(sizePolicy2);
        videoControls->setMinimumSize(QSize(0, 0));

        verticalLayout_2->addWidget(videoControls);


        horizontalLayout_6->addWidget(videoAreaWidget);


        horizontalLayout_5->addWidget(widget_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 982, 19));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        dockWidgetAlgorithm = new QDockWidget(MainWindow);
        dockWidgetAlgorithm->setObjectName(QStringLiteral("dockWidgetAlgorithm"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(dockWidgetAlgorithm->sizePolicy().hasHeightForWidth());
        dockWidgetAlgorithm->setSizePolicy(sizePolicy3);
        dockWidgetAlgorithm->setMinimumSize(QSize(200, 207));
        dockWidgetAlgorithm->setSizeIncrement(QSize(0, 0));
        dockWidgetAlgorithm->setBaseSize(QSize(0, 0));
        dockWidgetAlgorithmContents = new QWidget();
        dockWidgetAlgorithmContents->setObjectName(QStringLiteral("dockWidgetAlgorithmContents"));
        sizePolicy3.setHeightForWidth(dockWidgetAlgorithmContents->sizePolicy().hasHeightForWidth());
        dockWidgetAlgorithmContents->setSizePolicy(sizePolicy3);
        verticalLayout_3 = new QVBoxLayout(dockWidgetAlgorithmContents);
        verticalLayout_3->setSpacing(2);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(3, 3, 3, 3);
        widget_alg = new QWidget(dockWidgetAlgorithmContents);
        widget_alg->setObjectName(QStringLiteral("widget_alg"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(widget_alg->sizePolicy().hasHeightForWidth());
        widget_alg->setSizePolicy(sizePolicy4);
        widget_alg->setMinimumSize(QSize(0, 50));
        gridLayout = new QGridLayout(widget_alg);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        comboBox_TrackerSelect = new QComboBox(widget_alg);
        comboBox_TrackerSelect->setObjectName(QStringLiteral("comboBox_TrackerSelect"));

        gridLayout->addWidget(comboBox_TrackerSelect, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        checkBox_TrackingActivated = new QCheckBox(widget_alg);
        checkBox_TrackingActivated->setObjectName(QStringLiteral("checkBox_TrackingActivated"));

        horizontalLayout->addWidget(checkBox_TrackingActivated);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        verticalLayout_3->addWidget(widget_alg);

        groupBox_tools = new QGroupBox(dockWidgetAlgorithmContents);
        groupBox_tools->setObjectName(QStringLiteral("groupBox_tools"));
        QSizePolicy sizePolicy5(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(groupBox_tools->sizePolicy().hasHeightForWidth());
        groupBox_tools->setSizePolicy(sizePolicy5);
        groupBox_tools->setFlat(false);
        verticalLayout_4 = new QVBoxLayout(groupBox_tools);
        verticalLayout_4->setSpacing(3);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(3, 3, 3, 3);
        scrollArea = new QScrollArea(groupBox_tools);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        sizePolicy5.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy5);
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setLineWidth(0);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        groupBoxContents = new QWidget();
        groupBoxContents->setObjectName(QStringLiteral("groupBoxContents"));
        groupBoxContents->setGeometry(QRect(0, 0, 180, 563));
        sizePolicy5.setHeightForWidth(groupBoxContents->sizePolicy().hasHeightForWidth());
        groupBoxContents->setSizePolicy(sizePolicy5);
        scrollArea->setWidget(groupBoxContents);

        verticalLayout_4->addWidget(scrollArea);


        verticalLayout_3->addWidget(groupBox_tools);

        dockWidgetAlgorithm->setWidget(dockWidgetAlgorithmContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidgetAlgorithm);
        dockWidgetNotification = new QDockWidget(MainWindow);
        dockWidgetNotification->setObjectName(QStringLiteral("dockWidgetNotification"));
        dockWidgetNotification->setFloating(false);
        dockWidgetNotificationContents = new QWidget();
        dockWidgetNotificationContents->setObjectName(QStringLiteral("dockWidgetNotificationContents"));
        sizePolicy1.setHeightForWidth(dockWidgetNotificationContents->sizePolicy().hasHeightForWidth());
        dockWidgetNotificationContents->setSizePolicy(sizePolicy1);
        dockWidgetNotification->setWidget(dockWidgetNotificationContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidgetNotification);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_Video);
        menuFile->addAction(actionOpen_Picture);
        menuFile->addAction(actionOpen_Camera);
        menuFile->addSeparator();
        menuFile->addAction(actionLoad_Tracker);
        menuFile->addSeparator();
        menuFile->addAction(actionLoad_tracking_data);
        menuFile->addAction(actionSave_tracking_data);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "BioTracker", 0));
        actionOpen_Video->setText(QApplication::translate("MainWindow", "Open &Video...", 0));
        actionOpen_Picture->setText(QApplication::translate("MainWindow", "Open &Picture...", 0));
        actionLoad_tracking_data->setText(QApplication::translate("MainWindow", "&Load tracking data...", 0));
        actionSave_tracking_data->setText(QApplication::translate("MainWindow", "&Save tracking data...", 0));
        actionQuit->setText(QApplication::translate("MainWindow", "E&xit", 0));
        actionLoad_Tracker->setText(QApplication::translate("MainWindow", "L&oad Tracker...", 0));
        actionOpen_Camera->setText(QApplication::translate("MainWindow", "Open &Camera...", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "&File", 0));
        dockWidgetAlgorithm->setWindowTitle(QApplication::translate("MainWindow", "A&lgorithm", 0));
        checkBox_TrackingActivated->setText(QApplication::translate("MainWindow", "Activate Tracking", 0));
        groupBox_tools->setTitle(QApplication::translate("MainWindow", "Tools", 0));
        dockWidgetNotification->setWindowTitle(QApplication::translate("MainWindow", "&Notifications", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
