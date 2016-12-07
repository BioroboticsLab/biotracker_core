#include "VideoControllWidget.h"
#include "ui_VideoControllWidget.h"
#include "Controller/ControllerPlayer.h"

#include "IStates/IPlayerState.h"

VideoControllWidget::VideoControllWidget(QWidget *parent, IController *controller, IModel *model) :
    IViewWidget(parent, controller, model),
    ui(new Ui::VideoControllWidget)
{
    ui->setupUi(this);

    m_iconPause.addFile(QStringLiteral("qrc:/Images/resources/pause-sign.png"),
                        QSize(), QIcon::Normal, QIcon::Off);
    m_iconPlay.addFile(QStringLiteral("qrc:/Images/resources/arrow-forward1.png"),
                       QSize(), QIcon::Normal, QIcon::Off);
}

VideoControllWidget::~VideoControllWidget()
{
    delete ui;
}

void VideoControllWidget::setSelectedView(QString str)
{
    if (!ui->comboBoxSelectedView->findText(str)) {
        ui->comboBoxSelectedView->addItem(str);
    }
    ui->comboBoxSelectedView->setCurrentText(str);
}

void VideoControllWidget::setVideoViewComboboxModel(QStringListModel *comboboxModel)
{
    ui->comboBoxSelectedView->setModel(comboboxModel);
}

void VideoControllWidget::getNotified()
{
    ui->button_nextFrame->setEnabled(m_Forw);
    ui->button_playPause->setEnabled(m_Play);
    ui->button_previousFrame->setEnabled(m_Back);
    ui->button_stop->setEnabled(m_Stop);


    if (m_Paus) {
        ui->button_playPause->setIcon(QIcon(":/Images/resources/pause-sign.png"));
    }
    else {
        ui->button_playPause->setIcon(QIcon(":/Images/resources/arrow-forward1.png"));
    }
}

void VideoControllWidget::setTotalNumbFrames(size_t numb)
{
    m_TotalNumbFrames = numb;
}

void VideoControllWidget::setCurrentFrameNumber(size_t numb)
{
    m_CurrentFrameNumber = numb;
    ui->frame_num_edit->setText(QString::number(m_CurrentFrameNumber));
}

void VideoControllWidget::setFPS(double fps)
{
    ui->fps_label->setText(QString::number(fps));
}

void VideoControllWidget::setVideoControllsStates(QVector<bool> states)
{

    m_Back = states.at(0);
    m_Forw = states.at(1);
    m_Paus = states.at(2);
    m_Play = states.at(3);
    m_Stop = states.at(4);

}

void VideoControllWidget::on_button_nextFrame_clicked()
{
    ControllerPlayer *controller = dynamic_cast<ControllerPlayer *>(getController());
    controller->nextFrame();
}

void VideoControllWidget::on_button_playPause_clicked()
{
    ControllerPlayer *controller = dynamic_cast<ControllerPlayer *>(getController());

    if (m_Paus) {
        controller->pause();
    }
    else {
        controller->play();
    }
}

void VideoControllWidget::on_button_stop_clicked()
{
    ControllerPlayer *controller = dynamic_cast<ControllerPlayer *>(getController());
    controller->stop();
}

void VideoControllWidget::on_button_previousFrame_clicked()
{
    ControllerPlayer *controller = dynamic_cast<ControllerPlayer *>(getController());
    controller->prevFrame();
}

void VideoControllWidget::on_sld_video_sliderMoved(int position)
{

}

void VideoControllWidget::on_DurationChanged(int position)
{

}

void VideoControllWidget::on_PositionChanged(int position)
{

}


void VideoControllWidget::on_comboBoxSelectedView_currentTextChanged(const QString &arg1)
{
    QString name = arg1;
    ControllerPlayer *controller = dynamic_cast<ControllerPlayer *>(getController());

    controller->changeImageView(name);
}
