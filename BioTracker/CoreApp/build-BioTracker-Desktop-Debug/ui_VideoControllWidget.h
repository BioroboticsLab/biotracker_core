/********************************************************************************
** Form generated from reading UI file 'VideoControllWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOCONTROLLWIDGET_H
#define UI_VIDEOCONTROLLWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VideoControllWidget
{
public:
    QVBoxLayout *verticalLayout;
    QSlider *sld_video;
    QGroupBox *playback;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *button_previousFrame;
    QPushButton *button_playPause;
    QPushButton *button_stop;
    QPushButton *button_nextFrame;
    QPushButton *button_screenshot;
    QLabel *lbl_frame;
    QLineEdit *frame_num_edit;
    QSpacerItem *horizontalSpacer;
    QLabel *labelView;
    QComboBox *comboBoxSelectedView;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *button_panZoom;
    QGroupBox *speed;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QSlider *sld_speed;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_5;
    QLabel *fps_label;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label;
    QLabel *fps_edit;

    void setupUi(QWidget *VideoControllWidget)
    {
        if (VideoControllWidget->objectName().isEmpty())
            VideoControllWidget->setObjectName(QStringLiteral("VideoControllWidget"));
        VideoControllWidget->resize(775, 193);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(VideoControllWidget->sizePolicy().hasHeightForWidth());
        VideoControllWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(VideoControllWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        sld_video = new QSlider(VideoControllWidget);
        sld_video->setObjectName(QStringLiteral("sld_video"));
        sld_video->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sld_video->sizePolicy().hasHeightForWidth());
        sld_video->setSizePolicy(sizePolicy1);
        sld_video->setAutoFillBackground(false);
        sld_video->setMaximum(2500);
        sld_video->setPageStep(25);
        sld_video->setOrientation(Qt::Horizontal);
        sld_video->setInvertedAppearance(false);
        sld_video->setInvertedControls(false);
        sld_video->setTickPosition(QSlider::TicksAbove);
        sld_video->setTickInterval(25);

        verticalLayout->addWidget(sld_video);

        playback = new QGroupBox(VideoControllWidget);
        playback->setObjectName(QStringLiteral("playback"));
        sizePolicy.setHeightForWidth(playback->sizePolicy().hasHeightForWidth());
        playback->setSizePolicy(sizePolicy);
        playback->setFlat(false);
        horizontalLayout_4 = new QHBoxLayout(playback);
        horizontalLayout_4->setSpacing(3);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(3, 3, 3, 3);
        button_previousFrame = new QPushButton(playback);
        button_previousFrame->setObjectName(QStringLiteral("button_previousFrame"));
        button_previousFrame->setEnabled(false);
        button_previousFrame->setStyleSheet(QStringLiteral(""));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Images/resources/arrows-skip-back.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_previousFrame->setIcon(icon);
        button_previousFrame->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(button_previousFrame);

        button_playPause = new QPushButton(playback);
        button_playPause->setObjectName(QStringLiteral("button_playPause"));
        button_playPause->setEnabled(false);
        button_playPause->setStyleSheet(QStringLiteral(""));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Images/resources/arrow-forward1.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_playPause->setIcon(icon1);
        button_playPause->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(button_playPause);

        button_stop = new QPushButton(playback);
        button_stop->setObjectName(QStringLiteral("button_stop"));
        button_stop->setEnabled(false);
        button_stop->setStyleSheet(QStringLiteral(""));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Images/resources/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_stop->setIcon(icon2);
        button_stop->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(button_stop);

        button_nextFrame = new QPushButton(playback);
        button_nextFrame->setObjectName(QStringLiteral("button_nextFrame"));
        button_nextFrame->setEnabled(false);
        button_nextFrame->setStyleSheet(QStringLiteral(""));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/Images/resources/arrows-skip-forward.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_nextFrame->setIcon(icon3);
        button_nextFrame->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(button_nextFrame);

        button_screenshot = new QPushButton(playback);
        button_screenshot->setObjectName(QStringLiteral("button_screenshot"));
        button_screenshot->setEnabled(true);
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        button_screenshot->setFont(font);
        button_screenshot->setStyleSheet(QStringLiteral(""));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/Images/resources/screenshot.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_screenshot->setIcon(icon4);
        button_screenshot->setIconSize(QSize(30, 30));

        horizontalLayout_4->addWidget(button_screenshot);

        lbl_frame = new QLabel(playback);
        lbl_frame->setObjectName(QStringLiteral("lbl_frame"));

        horizontalLayout_4->addWidget(lbl_frame);

        frame_num_edit = new QLineEdit(playback);
        frame_num_edit->setObjectName(QStringLiteral("frame_num_edit"));
        frame_num_edit->setEnabled(false);
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_num_edit->sizePolicy().hasHeightForWidth());
        frame_num_edit->setSizePolicy(sizePolicy2);
        frame_num_edit->setMaximumSize(QSize(60, 30));

        horizontalLayout_4->addWidget(frame_num_edit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        labelView = new QLabel(playback);
        labelView->setObjectName(QStringLiteral("labelView"));

        horizontalLayout_4->addWidget(labelView);

        comboBoxSelectedView = new QComboBox(playback);
        comboBoxSelectedView->setObjectName(QStringLiteral("comboBoxSelectedView"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(comboBoxSelectedView->sizePolicy().hasHeightForWidth());
        comboBoxSelectedView->setSizePolicy(sizePolicy3);
        comboBoxSelectedView->setMinimumSize(QSize(100, 0));
        comboBoxSelectedView->setBaseSize(QSize(0, 0));

        horizontalLayout_4->addWidget(comboBoxSelectedView);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        button_panZoom = new QPushButton(playback);
        button_panZoom->setObjectName(QStringLiteral("button_panZoom"));
        button_panZoom->setFont(font);
        button_panZoom->setStyleSheet(QStringLiteral(""));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/Images/resources/panZoom.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_panZoom->setIcon(icon5);
        button_panZoom->setIconSize(QSize(30, 30));
        button_panZoom->setCheckable(true);

        horizontalLayout_4->addWidget(button_panZoom);


        verticalLayout->addWidget(playback);

        speed = new QGroupBox(VideoControllWidget);
        speed->setObjectName(QStringLiteral("speed"));
        sizePolicy.setHeightForWidth(speed->sizePolicy().hasHeightForWidth());
        speed->setSizePolicy(sizePolicy);
        speed->setMinimumSize(QSize(0, 0));
        horizontalLayout_3 = new QHBoxLayout(speed);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_3 = new QLabel(speed);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setEnabled(true);

        horizontalLayout->addWidget(label_3);

        sld_speed = new QSlider(speed);
        sld_speed->setObjectName(QStringLiteral("sld_speed"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(sld_speed->sizePolicy().hasHeightForWidth());
        sld_speed->setSizePolicy(sizePolicy4);
        sld_speed->setMinimum(1);
        sld_speed->setMaximum(61);
        sld_speed->setValue(30);
        sld_speed->setSliderPosition(30);
        sld_speed->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(sld_speed);

        label_4 = new QLabel(speed);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);


        horizontalLayout_3->addLayout(horizontalLayout);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        label_5 = new QLabel(speed);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_3->addWidget(label_5);

        fps_label = new QLabel(speed);
        fps_label->setObjectName(QStringLiteral("fps_label"));
        fps_label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(fps_label);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        label = new QLabel(speed);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(label);

        fps_edit = new QLabel(speed);
        fps_edit->setObjectName(QStringLiteral("fps_edit"));
        fps_edit->setMinimumSize(QSize(60, 0));

        horizontalLayout_3->addWidget(fps_edit);


        verticalLayout->addWidget(speed);


        retranslateUi(VideoControllWidget);

        QMetaObject::connectSlotsByName(VideoControllWidget);
    } // setupUi

    void retranslateUi(QWidget *VideoControllWidget)
    {
        VideoControllWidget->setWindowTitle(QApplication::translate("VideoControllWidget", "Form", 0));
        playback->setTitle(QApplication::translate("VideoControllWidget", "Video Controls", 0));
#ifndef QT_NO_TOOLTIP
        button_previousFrame->setToolTip(QApplication::translate("VideoControllWidget", "Previous Frame", 0));
#endif // QT_NO_TOOLTIP
        button_previousFrame->setText(QString());
#ifndef QT_NO_TOOLTIP
        button_playPause->setToolTip(QApplication::translate("VideoControllWidget", "Play", 0));
#endif // QT_NO_TOOLTIP
        button_playPause->setText(QString());
#ifndef QT_NO_TOOLTIP
        button_stop->setToolTip(QApplication::translate("VideoControllWidget", "Stop", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        button_stop->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        button_stop->setText(QString());
#ifndef QT_NO_TOOLTIP
        button_nextFrame->setToolTip(QApplication::translate("VideoControllWidget", "Next Frame", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        button_nextFrame->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        button_nextFrame->setText(QString());
#ifndef QT_NO_TOOLTIP
        button_screenshot->setToolTip(QApplication::translate("VideoControllWidget", "Screenshot", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        button_screenshot->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        button_screenshot->setText(QString());
        lbl_frame->setText(QApplication::translate("VideoControllWidget", "Current frame:", 0));
        frame_num_edit->setText(QApplication::translate("VideoControllWidget", "0", 0));
        labelView->setText(QApplication::translate("VideoControllWidget", "View:", 0));
        comboBoxSelectedView->clear();
        comboBoxSelectedView->insertItems(0, QStringList()
         << QApplication::translate("VideoControllWidget", "Original", 0)
        );
#ifndef QT_NO_TOOLTIP
        button_panZoom->setToolTip(QApplication::translate("VideoControllWidget", "Pan/Zoom Mode", 0));
#endif // QT_NO_TOOLTIP
        button_panZoom->setText(QString());
        speed->setTitle(QApplication::translate("VideoControllWidget", "Playback Speed", 0));
        label_3->setText(QApplication::translate("VideoControllWidget", "slow", 0));
        label_4->setText(QApplication::translate("VideoControllWidget", "fast", 0));
        label_5->setText(QApplication::translate("VideoControllWidget", "target fps: ", 0));
        fps_label->setText(QApplication::translate("VideoControllWidget", "30", 0));
        label->setText(QApplication::translate("VideoControllWidget", "measured fps:", 0));
        fps_edit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VideoControllWidget: public Ui_VideoControllWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOCONTROLLWIDGET_H
