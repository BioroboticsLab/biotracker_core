#include "BioTracker3VideoControllWidget.h"
#include "ui_BioTracker3VideoControllWidget.h"
#include "Controller/ControllerPlayer.h"

#include "Interfaces/IStates/IPlayerState.h"

BioTracker3VideoControllWidget::BioTracker3VideoControllWidget(QWidget *parent, IController *controller, IModel *model) :
    IViewWidget(parent, controller, model),
    ui(new Ui::BioTracker3VideoControllWidget)
{
    ui->setupUi(this);

    m_iconPause.addFile(QStringLiteral(":/BioTracker/resources/pause-sign.png"),
                        QSize(), QIcon::Normal, QIcon::Off);
    m_iconPlay.addFile(QStringLiteral(":/BioTracker/resources/arrow-forward1.png"),
                       QSize(), QIcon::Normal, QIcon::Off);
}

BioTracker3VideoControllWidget::~BioTracker3VideoControllWidget()
{
    delete ui;
}

void BioTracker3VideoControllWidget::setSelectedView(QString str)
{
    if (!ui->comboBoxSelectedView->findText(str)) {
        ui->comboBoxSelectedView->addItem(str);
    }
    ui->comboBoxSelectedView->setCurrentText(str);
}

void BioTracker3VideoControllWidget::setVideoViewComboboxModel(QStringListModel *comboboxModel)
{
    ui->comboBoxSelectedView->setModel(comboboxModel);
}

void BioTracker3VideoControllWidget::getNotified()
{
    ui->button_nextFrame->setEnabled(m_Forw);
    ui->button_playPause->setEnabled(m_Play);
    ui->button_previousFrame->setEnabled(m_Back);
    ui->button_stop->setEnabled(m_Stop);


    if (m_Paus) {
        ui->button_playPause->setIcon(m_iconPause);
    }
    else {
        ui->button_playPause->setIcon(m_iconPlay);
    }
}

void BioTracker3VideoControllWidget::setTotalNumbFrames(size_t numb)
{
    m_TotalNumbFrames = numb;
}

void BioTracker3VideoControllWidget::setCurrentFrameNumber(size_t numb)
{
    m_CurrentFrameNumber = numb;
    ui->frame_num_edit->setText(QString::number(m_CurrentFrameNumber));
}

void BioTracker3VideoControllWidget::setFPS(double fps)
{
    ui->fps_label->setText(QString::number(fps));
}

void BioTracker3VideoControllWidget::setVideoControllsStates(QVector<bool> states)
{

    m_Back = states.at(0);
    m_Forw = states.at(1);
    m_Paus = states.at(2);
    m_Play = states.at(3);
    m_Stop = states.at(4);

}

void BioTracker3VideoControllWidget::on_button_nextFrame_clicked()
{
    ControllerPlayer *controller = dynamic_cast<ControllerPlayer *>(getController());
    controller->nextFrame();
}

void BioTracker3VideoControllWidget::on_button_playPause_clicked()
{
    ControllerPlayer *controller = dynamic_cast<ControllerPlayer *>(getController());

    if (m_Paus) {
        controller->pause();
    }
    else {
        controller->play();
    }
}

void BioTracker3VideoControllWidget::on_button_stop_clicked()
{
    ControllerPlayer *controller = dynamic_cast<ControllerPlayer *>(getController());
    controller->stop();
}

void BioTracker3VideoControllWidget::on_button_previousFrame_clicked()
{
    ControllerPlayer *controller = dynamic_cast<ControllerPlayer *>(getController());
    controller->prevFrame();
}

void BioTracker3VideoControllWidget::on_sld_video_sliderMoved(int position)
{

}

void BioTracker3VideoControllWidget::on_DurationChanged(int position)
{

}

void BioTracker3VideoControllWidget::on_PositionChanged(int position)
{

}


void BioTracker3VideoControllWidget::on_comboBoxSelectedView_currentTextChanged(const QString &arg1)
{
    QString name = arg1;
    ControllerPlayer *controller = dynamic_cast<ControllerPlayer *>(getController());

    controller->changeImageView(name);
}
