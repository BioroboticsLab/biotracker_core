#include "SettingsWindow.h"

#include "settings/Settings.h"

#include <QFileDialog>

SettingsWindow::SettingsWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SettingsForm)
{
	ui->setupUi(this);

	for (int i=0; i<codecList.size(); i++)
		ui->comboBoxVideoCodec->addItem(codecList[i].first.c_str());

	BioTracker::Core::Settings *set = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	int defaultIndex = set->getValueOrDefault<int>(CFG_CODEC, 0);
	ui->comboBoxVideoCodec->setCurrentIndex(defaultIndex);

	bool dropFrames = set->getValueOrDefault<bool>(CFG_DROPFRAMES, false);
	ui->checkBoxDropFrames->setChecked(dropFrames);

	bool recordScaled = set->getValueOrDefault<bool>(CFG_RECORDSCALEDOUT, false);
	ui->checkBox_scaledOutput->setChecked(recordScaled);


	for (int i = 0; i<exporterList.size(); i++)
		ui->comboBoxDefaultExporter->addItem(exporterList[i].c_str());

	int exporter = set->getValueOrDefault<int>(CFG_EXPORTER, 0);
	ui->comboBoxDefaultExporter->setCurrentIndex(exporter);


	ui->lineEdit_nthFrame->setValidator(new QIntValidator(this));
	int stride = set->getValueOrDefault<int>(CFG_INPUT_FRAME_STRIDE, CFG_INPUT_FRAME_STRIDE_VAL);
	ui->lineEdit_nthFrame->setText(std::to_string(stride).c_str());

	ui->lineEdit_qp->setValidator(new QIntValidator(this));
	int qp = set->getValueOrDefault<int>(CFG_GPU_QP, CFG_GPU_QP_VAL);
	ui->lineEdit_qp->setText(std::to_string(qp).c_str());

	std::string locStr= set->getValueOrDefault<std::string>(CFG_DEFAULT_LOC_MAN_SAVE, CFG_DEFAULT_LOC_MAN_SAVE_VAL);
	ui->lineEditLocationManSave->setText(QString::fromStdString(locStr));

#ifndef WITH_CUDA
	ui->lineEdit_qp->setVisible(false);
	ui->label_qp->setVisible(false);
#endif
}

void SettingsWindow::on_buttonSaveClose_clicked() {

	BioTracker::Core::Settings *set = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	bool recordScaled = ui->checkBox_scaledOutput->isChecked();
	set->setParam(CFG_RECORDSCALEDOUT, recordScaled);

	bool dropFrames = ui->checkBoxDropFrames->isChecked();
	set->setParam(CFG_DROPFRAMES, dropFrames);

	int codec = ui->comboBoxVideoCodec->currentIndex();
	set->setParam(CFG_CODEC, codec);

	int exporter = ui->comboBoxDefaultExporter->currentIndex();
	set->setParam(CFG_EXPORTER, exporter);

	int stride = (ui->lineEdit_nthFrame->text()).toInt();
	set->setParam(CFG_INPUT_FRAME_STRIDE, stride);

	int qp = (ui->lineEdit_qp->text()).toInt();
	set->setParam(CFG_GPU_QP, qp);

	std::string defaultManSave = (ui->lineEditLocationManSave->text()).toStdString();
	set->setParam(CFG_DEFAULT_LOC_MAN_SAVE, defaultManSave);

	this->close();
}

void SettingsWindow::on_pushButtonSetNewLocManSave_clicked() {
	QString dirStr = QFileDialog::getExistingDirectory(this, tr("Select Directory"), "", QFileDialog::ShowDirsOnly);
	if (dirStr) {
		ui->lineEditLocationManSave->setText(dirStr);
	}
}



SettingsWindow::~SettingsWindow()
{
}
