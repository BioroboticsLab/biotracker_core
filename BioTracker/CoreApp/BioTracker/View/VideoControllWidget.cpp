#include "VideoControllWidget.h"
#include "ui_VideoControllWidget.h"
#include "Controller/ControllerPlayer.h"

//#include "IStates/IPlayerState.h"

#include "Model/MediaPlayer.h"

VideoControllWidget::VideoControllWidget(QWidget* parent, IController* controller, IModel* model) :
    IViewWidget(parent, controller, model),
    ui(new Ui::VideoControllWidget) {
    ui->setupUi(this);
	m_RecO = false;
	m_RecI = false;
	m_Paus = false;

    m_iconPause.addFile(QStringLiteral("qrc:/Images/resources/pause-sign.png"),
                        QSize(), QIcon::Normal, QIcon::Off);
    m_iconPlay.addFile(QStringLiteral("qrc:/Images/resources/arrow-forward1.png"),
                       QSize(), QIcon::Normal, QIcon::Off);

    ui->sld_video->setMinimum(0);
	ui->comboBoxSelectedView->setVisible(false);
	ui->labelView->setVisible(false);
	this->setSelectedView("Original");
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

	m_RecI = mediaPlayer->getRecIState();
	if (m_RecI) {
		ui->button_record_cam->setIcon(QIcon(":/Images/resources/recordingCam.png"));
	}
	else {
		ui->button_record_cam->setIcon(QIcon(":/Images/resources/recordCam.png"));
	}

    int currentFrameNr = mediaPlayer->getCurrentFrameNumber();
    ui->frame_num_edit->setText(QString::number(currentFrameNr));
    ui->sld_video->setValue(currentFrameNr);

	ui->fps_label->setText(QString::number(mediaPlayer->getFpsOfSourceFile()));
	double cfps = mediaPlayer->getCurrentFPS();
	ui->label_currentFpsNum->setText(QString::number(mediaPlayer->getCurrentFPS()));

    int totalNumberOfFrames = mediaPlayer->getTotalNumberOfFrames();

    if(totalNumberOfFrames >= 1) {
        ui->sld_video->setEnabled(true);
        ui->sld_video->setMaximum(totalNumberOfFrames-1);
		
		int intervalPower = floor(log10(totalNumberOfFrames));
		int tickInterval = pow(10, intervalPower > 0? floor(log10(totalNumberOfFrames))-1: 0);

		ui->sld_video->setTickInterval(tickInterval);
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


void VideoControllWidget::on_button_record_clicked() {
	//QString name = arg1;
	ControllerPlayer* controller = dynamic_cast<ControllerPlayer*>(getController());

	int success = controller->recordOutput();
	if (success == 1) {
		QPixmap pix(":/Images/resources/recording.png");
		QIcon icon(pix);
		ui->button_record->setIcon(icon);
		ui->button_record->setIconSize(QSize(32,32));
	}
	else {
		QPixmap pix(":/Images/resources/record.png");
		QIcon icon(pix);
		ui->button_record->setIcon(icon);
		ui->button_record->setIconSize(QSize(32, 32));
	}
}

void VideoControllWidget::on_button_record_cam_clicked() {
	//QString name = arg1;
	ControllerPlayer* controller = dynamic_cast<ControllerPlayer*>(getController());

	int success = controller->recordInput();
}

void VideoControllWidget::on_button_screenshot_clicked() {
    ControllerPlayer* controller = dynamic_cast<ControllerPlayer*>(getController());
    controller->takeScreenshot();
}

void VideoControllWidget::on_comboBoxSelectedView_currentTextChanged(const QString& arg1) {
	QString name = arg1;
	ControllerPlayer* controller = dynamic_cast<ControllerPlayer*>(getController());

	controller->changeImageView(name);
}

void VideoControllWidget::on_sld_video_sliderReleased() {
   // int position = ui->sld_video->sliderPosition();
    //ControllerPlayer* controller = dynamic_cast<ControllerPlayer*>(getController());
    //controller->setGoToFrame(position);
}

void VideoControllWidget::on_sld_video_actionTriggered(int action)
{
	ControllerPlayer* controller = dynamic_cast<ControllerPlayer*>(getController());
	int position = ui->sld_video->sliderPosition();
	controller->setGoToFrame(position);
}


/**
 * If the video slider is moved, this function sets the value to the current frame number lable
 */
void VideoControllWidget::on_sld_video_sliderMoved(int position) {
    ui->frame_num_edit->setText(QString::number(position));
}