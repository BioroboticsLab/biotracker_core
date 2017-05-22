#include "VideoControllWidget.h"
#include "ui_VideoControllWidget.h"
#include "Controller/ControllerPlayer.h"

//#include "IStates/IPlayerState.h"

#include "Model/MediaPlayer.h"

VideoControllWidget::VideoControllWidget(QWidget* parent, IController* controller, IModel* model) :
    IViewWidget(parent, controller, model),
    ui(new Ui::VideoControllWidget) {
    ui->setupUi(this);

    m_iconPause.addFile(QStringLiteral("qrc:/Images/resources/pause-sign.png"),
                        QSize(), QIcon::Normal, QIcon::Off);
    m_iconPlay.addFile(QStringLiteral("qrc:/Images/resources/arrow-forward1.png"),
                       QSize(), QIcon::Normal, QIcon::Off);

    ui->sld_video->setMinimum(0);
}

VideoControllWidget::~VideoControllWidget() {
    delete ui;
}

void VideoControllWidget::setSelectedView(QString str) {
    if (!ui->comboBoxSelectedView->findText(str)) {
        ui->comboBoxSelectedView->addItem(str);
    }
    ui->comboBoxSelectedView->setCurrentText(str);
}

void VideoControllWidget::setVideoViewComboboxModel(QStringListModel* comboboxModel) {
    ui->comboBoxSelectedView->setModel(comboboxModel);
}

void VideoControllWidget::getNotified() {
    MediaPlayer* mediaPlayer = dynamic_cast<MediaPlayer*>(getModel());

    ui->button_nextFrame->setEnabled(mediaPlayer->getForwardState());
    ui->button_previousFrame->setEnabled(mediaPlayer->getBackwardState());
    ui->button_playPause->setEnabled(mediaPlayer->getPlayState());
    ui->button_stop->setEnabled(mediaPlayer->getStopState());

    m_Paus = mediaPlayer->getPauseState();

    if (m_Paus) {
        ui->button_playPause->setIcon(QIcon(":/Images/resources/pause-sign.png"));
    } else {
        ui->button_playPause->setIcon(QIcon(":/Images/resources/arrow-forward1.png"));
    }

    int currentFrameNr = mediaPlayer->getCurrentFrameNumber();
    ui->frame_num_edit->setText(QString::number(currentFrameNr));
    ui->sld_video->setValue(currentFrameNr);

    ui->fps_label->setText(QString::number(mediaPlayer->getCurrentFPS()));

    int totalNumberOfFrames = mediaPlayer->getTotalNumberOfFrames();

    if(totalNumberOfFrames >= 1) {
        ui->sld_video->setEnabled(true);
        ui->sld_video->setMaximum(totalNumberOfFrames);
    }



}


void VideoControllWidget::on_button_nextFrame_clicked() {
    ControllerPlayer* controller = dynamic_cast<ControllerPlayer*>(getController());
    controller->nextFrame();
}

void VideoControllWidget::on_button_playPause_clicked() {
    ControllerPlayer* controller = dynamic_cast<ControllerPlayer*>(getController());

    if (m_Paus) {
        controller->pause();
    } else {
        controller->play();
    }
}

void VideoControllWidget::on_button_stop_clicked() {
    ControllerPlayer* controller = dynamic_cast<ControllerPlayer*>(getController());
    controller->stop();
}

void VideoControllWidget::on_button_previousFrame_clicked() {
    ControllerPlayer* controller = dynamic_cast<ControllerPlayer*>(getController());
    controller->prevFrame();
}

void VideoControllWidget::on_DurationChanged(int position) {

}

void VideoControllWidget::on_PositionChanged(int position) {

}


void VideoControllWidget::on_comboBoxSelectedView_currentTextChanged(const QString& arg1) {
    QString name = arg1;
    ControllerPlayer* controller = dynamic_cast<ControllerPlayer*>(getController());

    controller->changeImageView(name);
}

void VideoControllWidget::on_sld_video_sliderReleased() {
    int position = ui->sld_video->value();
    ControllerPlayer* controller = dynamic_cast<ControllerPlayer*>(getController());
    controller->setGoToFrame(position);
}

/**
 * If the video slider is moved, this function sets the value to the current frame number lable
 */
void VideoControllWidget::on_sld_video_sliderMoved(int position) {
    ui->frame_num_edit->setText(QString::number(position));
}
