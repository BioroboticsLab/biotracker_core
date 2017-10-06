#include "CoreParameterView.h"
#include "ui_CoreParameterView.h"
#include "qdebug.h"
#include "Model/CoreParameter.h"

#include <iostream>

CoreParameterView::CoreParameterView(QWidget *parent, IController *controller, IModel *model) :
	IViewWidget(parent, controller, model),
	ui(new Ui::CoreParameterView)
{
	ui->setupUi(this);

	////ui->lineEdit->setValidator(new QIntValidator(this));
	//ui->lineEdit_2_binThresh->setValidator(new QIntValidator(this));
	//ui->lineEdit_3_SizeErode->setValidator(new QIntValidator(this));
	//ui->lineEdit_4_SizeDilate->setValidator(new QIntValidator(this));
	////	ui->lineEdit_5_MogHist->setValidator(new QIntValidator(this));
	////	ui->lineEdit_6_MogThresh->setValidator(new QIntValidator(this));
	//ui->lineEdit_8_MinBlob->setValidator(new QIntValidator(this));
	//ui->lineEdit_9MaxBlob->setValidator(new QIntValidator(this));
	//ui->lineEditNoFish->setValidator(new QIntValidator(this));

	//ui->lineEdit_7_MogBack->setValidator(new QDoubleValidator(this));

	//BioCore::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CONFIGPARAM::CONFIG_INI_FILE);
	//int v = _settings->getValueOrDefault<int>(TRACKERPARAM::CN_APPERTURE_TYPE, 0);
	ui->checkBoxEnableCoreView->setChecked(true);
	ui->spinBoxTracingHistoryLength->setMinimum(0);
	ui->spinBoxTracingHistoryLength->setMaximum(1000);

	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(model);

	coreParams->m_viewSwitch = ui->checkBoxEnableCoreView->isChecked();
	coreParams->m_tracingStyle = ui->comboBoxTracingStyle->currentText();
	coreParams->m_tracingHistory = ui->spinBoxTracingHistoryLength->value();
	//TODO  colors
	coreParams->m_tracingSteps = ui->spinBoxTracingSteps->value();
	coreParams->m_colorBorder = new QColor();
	coreParams->m_colorBrush =  new QColor();

	getNotified();
}

CoreParameterView::~CoreParameterView()
{
	delete ui;
}

void CoreParameterView::on_checkBoxEnableCoreView_stateChanged(int v) 
{
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
	//disable
	if (ui->checkBoxEnableCoreView->checkState() == Qt::Unchecked) {
		ui->widgetParameter->setEnabled(false);
		emitViewSwitch(false);
		coreParams->m_viewSwitch = false;
	}
	//enable
	else if (ui->checkBoxEnableCoreView->checkState() == Qt::Checked){
		ui->widgetParameter->setEnabled(true);
		emitViewSwitch(true);
		coreParams->m_viewSwitch = true;

	}
}

void CoreParameterView::on_comboBoxTracingStyle_currentIndexChanged(const QString & text)
{
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
	coreParams->m_tracingStyle = text;
	emitTracingStyle(text);
}

void CoreParameterView::on_spinBoxTracingHistoryLength_valueChanged(int i)
{
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
	coreParams->m_tracingHistory = i;
	emitTracingHistoryLength(i);
}

void CoreParameterView::on_spinBoxTracingSteps_valueChanged(int i)
{
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
	coreParams->m_tracingSteps = i;
	emitTracingSteps(i);
}

void CoreParameterView::on_pushButtonSelectAll_clicked()
{
	emitSelectAll();
}
//TODO show current color
void CoreParameterView::on_pushButtonColorChangeBorder_clicked()
{
	emitColorChangeBorderAll();
}
void CoreParameterView::on_pushButtonColorChangeBorderSelected_clicked()
{
	emitColorChangeBorderSelected();
}
//TODO show current color
void CoreParameterView::on_pushButtonColorChangeBrush_clicked()
{
	emitColorChangeBrushAll();
}

void CoreParameterView::on_pushButtonColorchangeBrushSelected_clicked()
{
	emitColorChangeBrushSelected();
}

//void CoreParameterView::on_pushButton_clicked()
//{
//	//int threshold = ui->lineEdit->text().toInt();
//	CoreParameter *parameter = qobject_cast<CoreParameter *>(getModel());
//	//parameter->setThreshold(threshold);
//
//	int setBinarizationThreshold = ui->lineEdit_2_binThresh->text().toInt();
//	int setSizeErode = ui->lineEdit_3_SizeErode->text().toInt();
//	int setSizeDilate = ui->lineEdit_4_SizeDilate->text().toInt();
//	int setmog2History = 0;// ui->lineEdit_5_MogHist->text().toInt();
//	int setmog2VarThresh = 0;// ui->lineEdit_6_MogThresh->text().toInt();
//	int setMinBlobSize = ui->lineEdit_8_MinBlob->text().toInt();
//	int setMaxBlobSize = ui->lineEdit_9MaxBlob->text().toInt();
//	int setAreaW = ui->lineEdit_areaw->text().toInt();
//	int setAreaH = ui->lineEdit_areah->text().toInt();
//
//	double setmog2BackgroundRatio = ui->lineEdit_7_MogBack->text().toDouble();
//
//	parameter->setAll(0, setBinarizationThreshold, setSizeErode, setSizeDilate, setmog2History, setmog2VarThresh,
//		setmog2BackgroundRatio, setMinBlobSize, setMaxBlobSize, setAreaW, setAreaH);
//}

void CoreParameterView::getNotified()
{
	qDebug() << "Core parameter notified";
	//CoreParameter *parameter = qobject_cast<CoreParameter *>(getModel());
	////int threshold = parameter->getThreshold();
	////ui->lineEdit->setText(QString::number(threshold));

	//int val = parameter->getBinarizationThreshold();
	//ui->lineEdit_2_binThresh->setText(QString::number(val));

	//val = parameter->getSizeErode();
	//ui->lineEdit_3_SizeErode->setText(QString::number(val));

	//val = parameter->getSizeDilate();
	//ui->lineEdit_4_SizeDilate->setText(QString::number(val));

	//val = parameter->getmog2History();
	////ui->lineEdit_5_MogHist->setText(QString::number(val));

	//val = parameter->getmog2VarThresh();
	////ui->lineEdit_6_MogThresh->setText(QString::number(val));

	//double dval = parameter->getmog2BackgroundRatio();
	//ui->lineEdit_7_MogBack->setText(QString::number(dval));

	//val = parameter->getMinBlobSize();
	//ui->lineEdit_8_MinBlob->setText(QString::number(val));

	//val = parameter->getMaxBlobSize();
	//ui->lineEdit_9MaxBlob->setText(QString::number(val));

	//val = parameter->getAreaWidth();
	//ui->lineEdit_areaw->setText(QString::number(val));

	//val = parameter->getAreaHeight();
	//ui->lineEdit_areah->setText(QString::number(val));
}
