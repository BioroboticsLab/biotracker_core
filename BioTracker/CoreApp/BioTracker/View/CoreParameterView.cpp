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

	//TODO do this the other way around!
	//tracing
	coreParams->m_tracingStyle = ui->comboBoxTracingStyle->currentText();
	coreParams->m_tracingHistory = ui->spinBoxTracingHistoryLength->value();
	coreParams->m_tracingSteps = ui->spinBoxTracingSteps->value();
	coreParams->m_tracingTimeDegradation = ui->comboBoxTracingTimeDegradation->currentText();
	coreParams->m_tracerWidth = ui->spinBoxTracerWidth->value();
	coreParams->m_tracerHeight = ui->spinBoxTracerHeight->value();
	coreParams->m_tracerOrientationLine = ui->checkBoxTracerOrientationLine->isChecked();

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

QWidget * CoreParameterView::getTrackerHook()
{
	return ui->tabWidgetParameters;
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

void CoreParameterView::on_comboBoxTracingTimeDegradation_currentIndexChanged(const QString & text)
{
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
	coreParams->m_tracingTimeDegradation = text;
	emitTracingTimeDegradation(text);
}

void CoreParameterView::on_spinBoxTracerWidth_valueChanged(int width)
{
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
	coreParams->m_tracerWidth = width;
	emitTracerWidth(width);
}

void CoreParameterView::on_spinBoxTracerHeight_valueChanged(int height)
{
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
	coreParams->m_tracerHeight = height;
	emitTracerHeight(height);
}

void CoreParameterView::on_checkBoxTracerOrientationLine_stateChanged(int toggle)
{
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
	coreParams->m_tracerOrientationLine = toggle;
	emitTracerOrientationLine(toggle);
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

void CoreParameterView::on_checkBoxTrackOrientationLine_stateChanged(int v)
{
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
	coreParams->m_trackOrientationLine = v;
	emitTracerOrientationLine(v);
}

// if dimensions are set for all tracks they are going to be set for new tracks aswell
void CoreParameterView::on_pushButtonTrackDimensionSetterAll_clicked()
{
	int width = ui->spinboxTrackWidth->value();
	int height = ui->spinBoxTrackHeight->value();

	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
	coreParams->m_trackWidth = width;
	coreParams->m_trackHeight = height;

	emitTrackDimensionsAll(width, height);
}

void CoreParameterView::on_pushButtonTrackDimensionSetterSelected_clicked()
{
	int width = ui->spinboxTrackWidth->value();
	int height = ui->spinBoxTrackHeight->value();
	emitTrackDimensionsSelected(width, height);
}

void CoreParameterView::on_pushButtonTrackDefaultDimensions_clicked()
{
	emitTrackDimensionsSetDefault();
}

void CoreParameterView::on_lineEditRectWidth_textChanged(QString s) {
	double w = ::atof(ui->lineEditRectWidth->text().toStdString().c_str());
	double h = ::atof(ui->lineEditRectHeight->text().toStdString().c_str());
	Q_EMIT emitRectDimensions(w,h);
}

void CoreParameterView::on_lineEditRectHeight_textChanged(QString s) {
	double w = ::atof(ui->lineEditRectWidth->text().toStdString().c_str());
	double h = ::atof(ui->lineEditRectHeight->text().toStdString().c_str());
	Q_EMIT emitRectDimensions(w, h); 
}

void CoreParameterView::on_checkBoxDisplayTrackingArea_stateChanged(int v) {
	Q_EMIT emitDisplayTrackingArea(ui->checkBoxDisplayTrackingArea->isChecked());
}

void CoreParameterView::on_checkBoxDisplayRectification_stateChanged(int v) {
	Q_EMIT emitDisplayRectification(ui->checkBoxDisplayRectification->isChecked());
}

void CoreParameterView::on_checkboxTrackingAreaAsEllipse_stateChanged(int v) {
	Q_EMIT emitTrackingAreaAsEllipse(ui->checkboxTrackingAreaAsEllipse->isChecked());
}

void CoreParameterView::on_pushButtonFinalizeExperiment_clicked() {
    Q_EMIT emitFinalizeExperiment();
}

void CoreParameterView::on_checkBoxExpertOptions_stateChanged(int v)
{
	//TODO HIDE STUFF HERE, NOT ALL STUFF 

	if (ui->checkBoxExpertOptions->checkState() == Qt::Unchecked) {
		ui->widgetParameter->hide();
	}
	//enable
	else if (ui->checkBoxExpertOptions->checkState() == Qt::Checked) {
		ui->widgetParameter->show();
	}
}

void CoreParameterView::on_checkBoxAntialiasing_stateChanged(int v)
{
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
	coreParams->m_antialiasing = v;
	emitToggleAntialiasing(v);
}

void CoreParameterView::getNotified()
{
	qDebug() << "Core parameter notified";
}

