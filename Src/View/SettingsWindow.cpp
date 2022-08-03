#include "SettingsWindow.h"

#include "util/Config.h"

#include <QFileDialog>

SettingsWindow::SettingsWindow(QWidget* parent, Config* cfg)
: QWidget(parent)
, ui(new Ui::SettingsForm)
{
    _cfg = cfg;
    ui->setupUi(this);

    for (int i = 0; i < codecList.size(); i++)
        ui->comboBoxVideoCodec->addItem(codecList[i].first.c_str());

    int defaultIndex = _cfg->VideoCodecUsed;
    ui->comboBoxVideoCodec->setCurrentIndex(defaultIndex);

    bool dropFrames = _cfg->DropFrames;
    ui->checkBoxDropFrames->setChecked(dropFrames);

    bool recordScaled = _cfg->RecordScaledOutput;
    ui->checkBox_scaledOutput->setChecked(recordScaled);

    for (int i = 0; i < exporterList.size(); i++)
        ui->comboBoxDefaultExporter->addItem(exporterList[i].c_str());

    int exporter = _cfg->DataExporter;
    ui->comboBoxDefaultExporter->setCurrentIndex(exporter);

    ui->lineEdit_nthFrame->setValidator(new QIntValidator(this));
    int stride = _cfg->FrameStride;
    ui->lineEdit_nthFrame->setText(std::to_string(stride).c_str());

    ui->lineEdit_qp->setValidator(new QIntValidator(this));
    int qp = _cfg->GpuQp;
    ui->lineEdit_qp->setText(std::to_string(qp).c_str());

    ui->lineEditLocationManSave->setText(_cfg->DefaultLocationManualSave);

#ifndef WITH_CUDA
    ui->lineEdit_qp->setVisible(false);
    ui->label_qp->setVisible(false);
#endif
}

void SettingsWindow::on_buttonSaveClose_clicked()
{

    _cfg->RecordScaledOutput = ui->checkBox_scaledOutput->isChecked();
    _cfg->DropFrames         = ui->checkBoxDropFrames->isChecked();
    _cfg->VideoCodecUsed     = ui->comboBoxVideoCodec->currentIndex();
    _cfg->DataExporter       = ui->comboBoxDefaultExporter->currentIndex();
    _cfg->FrameStride        = (ui->lineEdit_nthFrame->text()).toInt();
    _cfg->GpuQp              = (ui->lineEdit_qp->text()).toInt();
    _cfg->DefaultLocationManualSave = (ui->lineEditLocationManSave->text());

    this->close();
}

void SettingsWindow::on_pushButtonSetNewLocManSave_clicked()
{
    QString dirStr = QFileDialog::getExistingDirectory(
        this,
        tr("Select Directory"),
        "",
        QFileDialog::ShowDirsOnly);
    if (dirStr != "") {
        ui->lineEditLocationManSave->setText(dirStr);
    }
}

SettingsWindow::~SettingsWindow()
{
}
