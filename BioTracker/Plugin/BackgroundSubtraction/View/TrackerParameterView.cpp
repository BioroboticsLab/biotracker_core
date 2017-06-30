#include "TrackerParameterView.h"
#include "ui_TrackerParameterView.h"

#include <iostream>

TrackerParameterView::TrackerParameterView(QWidget *parent, IController *controller, IModel *model) :
    IViewWidget(parent, controller, model),
    ui(new Ui::TrackerParameterView)
{
    ui->setupUi(this);

	//ui->lineEdit->setValidator(new QIntValidator(this));
	ui->lineEdit_2_binThresh->setValidator(new QIntValidator(this));
	ui->lineEdit_3_SizeErode->setValidator(new QIntValidator(this));
	ui->lineEdit_4_SizeDilate->setValidator(new QIntValidator(this));
	ui->lineEdit_5_MogHist->setValidator(new QIntValidator(this));
	ui->lineEdit_6_MogThresh->setValidator(new QIntValidator(this));
	ui->lineEdit_8_MinBlob->setValidator(new QIntValidator(this));
	ui->lineEdit_9MaxBlob->setValidator(new QIntValidator(this));
	ui->lineEditNoFish->setValidator(new QIntValidator(this));

	ui->lineEdit_7_MogBack->setValidator(new QDoubleValidator(this));	
	
    getNotified();
}

TrackerParameterView::~TrackerParameterView()
{
    delete ui;
}

void TrackerParameterView::on_pushButtonResetBackground_clicked() {
	TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());
	parameter->setResetBackground(true);
}

void TrackerParameterView::on_comboBoxSendImage_currentIndexChanged(int v) {
	TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());
	parameter->setSendImage(v);
	parameter->setNewSelection(ui->comboBoxSendImage->currentText().toStdString());
}

void TrackerParameterView::on_checkBoxNetwork_stateChanged(int v) {
	TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());
	parameter->setDoNetwork(v);
}

void TrackerParameterView::on_checkBoxBackground_stateChanged(int v) {
	TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());
	parameter->setDoBackground(v);
}

void TrackerParameterView::on_pushButtonNoFish_clicked() {
	TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());
	int noFish = ui->lineEditNoFish->text().toInt();
	parameter->setNoFish(noFish);
}

void TrackerParameterView::on_pushButton_clicked()
{
	//int threshold = ui->lineEdit->text().toInt();
	TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());
	//parameter->setThreshold(threshold);

	int setBinarizationThreshold = ui->lineEdit_2_binThresh->text().toInt();
	int setSizeErode = ui->lineEdit_3_SizeErode->text().toInt();
	int setSizeDilate = ui->lineEdit_4_SizeDilate->text().toInt();
	int setmog2History = ui->lineEdit_5_MogHist->text().toInt();
	int setmog2VarThresh = ui->lineEdit_6_MogThresh->text().toInt();
	int setMinBlobSize = ui->lineEdit_8_MinBlob->text().toInt();
	int setMaxBlobSize = ui->lineEdit_9MaxBlob->text().toInt();
	int setAreaW = ui->lineEdit_areaw->text().toInt();
	int setAreaH = ui->lineEdit_areah->text().toInt();
		
	double setmog2BackgroundRatio = ui->lineEdit_7_MogBack->text().toDouble();

	parameter->setAll(0, setBinarizationThreshold, setSizeErode, setSizeDilate, setmog2History, setmog2VarThresh, 
		setmog2BackgroundRatio, setMinBlobSize, setMaxBlobSize, setAreaW, setAreaH);
}

void TrackerParameterView::getNotified()
{
    TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());
	//int threshold = parameter->getThreshold();
	//ui->lineEdit->setText(QString::number(threshold));

	int val = parameter->getBinarizationThreshold();
	ui->lineEdit_2_binThresh->setText(QString::number(val));

	val = parameter->getSizeErode();
	ui->lineEdit_3_SizeErode->setText(QString::number(val));

	val = parameter->getSizeDilate();
	ui->lineEdit_4_SizeDilate->setText(QString::number(val));

	val = parameter->getmog2History();
	ui->lineEdit_5_MogHist->setText(QString::number(val));

	val = parameter->getmog2VarThresh();
	ui->lineEdit_6_MogThresh->setText(QString::number(val));

	double dval = parameter->getmog2BackgroundRatio();
	ui->lineEdit_7_MogBack->setText(QString::number(dval));

	val = parameter->getMinBlobSize();
	ui->lineEdit_8_MinBlob->setText(QString::number(val));

	val = parameter->getMaxBlobSize();
	ui->lineEdit_9MaxBlob->setText(QString::number(val));

	val = parameter->getAreaWidth();
	ui->lineEdit_areaw->setText(QString::number(val));

	val = parameter->getAreaHeight();
	ui->lineEdit_areah->setText(QString::number(val));
}
