#include "SettingsWindow.h"

#include "settings/Settings.h"

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

	this->close();
}


SettingsWindow::~SettingsWindow()
{
}
