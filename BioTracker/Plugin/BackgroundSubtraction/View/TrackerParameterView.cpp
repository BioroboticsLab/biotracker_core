#include "TrackerParameterView.h"
#include "ui_TrackerParameterView.h"

#include <iostream>

TrackerParameterView::TrackerParameterView(QWidget *parent, IController *controller, IModel *model) :
    IViewWidget(parent, controller, model),
    _ui(new Ui::TrackerParameterView)
{
    _ui->setupUi(this);
    getNotified();


    QObject::connect(_ui->lineEdit_2_binThresh, SIGNAL(valueChanged(int)), this, SLOT(on_pushButton_clicked()));
    QObject::connect(_ui->lineEdit_3_SizeErode, SIGNAL(valueChanged(int)), this, SLOT(on_pushButton_clicked()));
    QObject::connect(_ui->lineEdit_4_SizeDilate, SIGNAL(valueChanged(int)), this, SLOT(on_pushButton_clicked()));
    QObject::connect(_ui->lineEdit_8_MinBlob, SIGNAL(valueChanged(int)), this, SLOT(on_pushButton_clicked()));
    QObject::connect(_ui->lineEdit_9MaxBlob, SIGNAL(valueChanged(int)), this, SLOT(on_pushButton_clicked()));
    QObject::connect(_ui->lineEdit_7_MogBack, SIGNAL(valueChanged(double)), this, SLOT(on_pushButton_clicked()));

    _ui->pushButton->setVisible(false);
}

TrackerParameterView::~TrackerParameterView()
{
    delete _ui;
}

void TrackerParameterView::on_pushButtonResetBackground_clicked() {
	TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());
	parameter->setResetBackground(true);
}

void TrackerParameterView::on_comboBoxSendImage_currentIndexChanged(int v) {
	TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());
	parameter->setSendImage(v);
	parameter->setNewSelection(_ui->comboBoxSendImage->currentText().toStdString());
}


void TrackerParameterView::on_pushButton_clicked()
{
	TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());

	int setBinarizationThreshold = _ui->lineEdit_2_binThresh->text().toInt();
	int setSizeErode = _ui->lineEdit_3_SizeErode->text().toInt();
	int setSizeDilate = _ui->lineEdit_4_SizeDilate->text().toInt();
	int setmog2History = 0;// ui->lineEdit_5_MogHist->text().toInt();
	int setmog2VarThresh = 0;// ui->lineEdit_6_MogThresh->text().toInt();
	int setMinBlobSize = _ui->lineEdit_8_MinBlob->text().toInt();
	int setMaxBlobSize = _ui->lineEdit_9MaxBlob->text().toInt();
		
	double setmog2BackgroundRatio = _ui->lineEdit_7_MogBack->value();

	parameter->setAll(0, setBinarizationThreshold, setSizeErode, setSizeDilate, setmog2History, setmog2VarThresh, 
		setmog2BackgroundRatio, setMinBlobSize, setMaxBlobSize);

    Q_EMIT parametersChanged();
}

void TrackerParameterView::getNotified()
{
    TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());

	int val = parameter->getBinarizationThreshold();
	_ui->lineEdit_2_binThresh->setValue(val);

	val = parameter->getSizeErode();
	_ui->lineEdit_3_SizeErode->setValue(val);

	val = parameter->getSizeDilate();
	_ui->lineEdit_4_SizeDilate->setValue(val);

	double dval = parameter->getmog2BackgroundRatio();
	_ui->lineEdit_7_MogBack->setValue(dval);

	val = parameter->getMinBlobSize();
	_ui->lineEdit_8_MinBlob->setValue(val);

	val = parameter->getMaxBlobSize();
	_ui->lineEdit_9MaxBlob->setValue(val);
}
