#include "TrackerParameterView.h"
#include "ui_TrackerParameterView.h"

#include <iostream>

TrackerParameterView::TrackerParameterView(QWidget *parent, IController *controller, IModel *model) :
    IViewWidget(parent, controller, model),
    ui(new Ui::TrackerParameterView)
{
    ui->setupUi(this);

    //For the spinboxes we don't need this
	/*ui->lineEdit_2_binThresh->setValidator(new QIntValidator(this));
	ui->lineEdit_3_SizeErode->setValidator(new QIntValidator(this));
	ui->lineEdit_4_SizeDilate->setValidator(new QIntValidator(this));
	ui->lineEdit_8_MinBlob->setValidator(new QIntValidator(this));
	ui->lineEdit_9MaxBlob->setValidator(new QIntValidator(this));
	ui->lineEdit_7_MogBack->setValidator(new QDoubleValidator(this));	
    */
    getNotified();


    QObject::connect(ui->lineEdit_2_binThresh, SIGNAL(valueChanged(int)), this, SLOT(on_pushButton_clicked()));
    QObject::connect(ui->lineEdit_3_SizeErode, SIGNAL(valueChanged(int)), this, SLOT(on_pushButton_clicked()));
    QObject::connect(ui->lineEdit_4_SizeDilate, SIGNAL(valueChanged(int)), this, SLOT(on_pushButton_clicked()));
    QObject::connect(ui->lineEdit_8_MinBlob, SIGNAL(valueChanged(int)), this, SLOT(on_pushButton_clicked()));
    QObject::connect(ui->lineEdit_9MaxBlob, SIGNAL(valueChanged(int)), this, SLOT(on_pushButton_clicked()));
    QObject::connect(ui->lineEdit_7_MogBack, SIGNAL(valueChanged(double)), this, SLOT(on_pushButton_clicked()));

    ui->pushButton->setVisible(false);
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
}


void TrackerParameterView::on_checkBoxTrackingArea_stateChanged(int v) {
	Q_EMIT trackingAreaType(v);
}

void TrackerParameterView::on_pushButton_clicked()
{
	TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());

	int setBinarizationThreshold = ui->lineEdit_2_binThresh->text().toInt();
	int setSizeErode = ui->lineEdit_3_SizeErode->text().toInt();
	int setSizeDilate = ui->lineEdit_4_SizeDilate->text().toInt();
	int setmog2History = 0;// ui->lineEdit_5_MogHist->text().toInt();
	int setmog2VarThresh = 0;// ui->lineEdit_6_MogThresh->text().toInt();
	int setMinBlobSize = ui->lineEdit_8_MinBlob->text().toInt();
	int setMaxBlobSize = ui->lineEdit_9MaxBlob->text().toInt();
		
	double setmog2BackgroundRatio = ui->lineEdit_7_MogBack->value();

	parameter->setAll(0, setBinarizationThreshold, setSizeErode, setSizeDilate, setmog2History, setmog2VarThresh, 
		setmog2BackgroundRatio, setMinBlobSize, setMaxBlobSize);

    Q_EMIT parametersChanged();
}

void TrackerParameterView::getNotified()
{
    TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());

	int val = parameter->getBinarizationThreshold();
	ui->lineEdit_2_binThresh->setValue(val);

	val = parameter->getSizeErode();
	ui->lineEdit_3_SizeErode->setValue(val);

	val = parameter->getSizeDilate();
	ui->lineEdit_4_SizeDilate->setValue(val);

	double dval = parameter->getmog2BackgroundRatio();
	ui->lineEdit_7_MogBack->setValue(dval);

	val = parameter->getMinBlobSize();
	ui->lineEdit_8_MinBlob->setValue(val);

	val = parameter->getMaxBlobSize();
	ui->lineEdit_9MaxBlob->setValue(val);
}
