#include "CameraDeviceSelect.h"
#include "ui_CameraDeviceSelect.h"

CameraDeviceSelect::CameraDeviceSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraDeviceSelect)
{
    ui->setupUi(this);




}

CameraDeviceSelect::~CameraDeviceSelect()
{
    delete ui;
}

void CameraDeviceSelect::on_buttonBox_accepted()
{
    int i = ui->comboBox_VideoDevice->currentIndex();
}
