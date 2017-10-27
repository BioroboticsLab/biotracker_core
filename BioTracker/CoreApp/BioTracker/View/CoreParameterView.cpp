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
	//ui->checkBoxEnableCoreView->setChecked(true);
	//ui->spinBoxTracingHistoryLength->setMinimum(0);
	//ui->spinBoxTracingHistoryLength->setMaximum(1000);

	

	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(model);

	fillUI();

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

void CoreParameterView::on_spinBoxTracerProportions_valueChanged(double proportion)
{
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
	coreParams->m_tracerProportions = (float)proportion;
	emitTracerProportions((float)proportion);
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
	emitTrackOrientationLine(v);
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

void CoreParameterView::on_pushButtonDefaultDimensions_clicked()
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
	//disable
	if (ui->checkBoxExpertOptions->checkState() == Qt::Unchecked) {
		ui->groupBoxTracerDimensions->hide();
		ui->groupBoxMiscellaneous->hide();
	}
	//enable
	else if (ui->checkBoxExpertOptions->checkState() == Qt::Checked) {
		ui->groupBoxTracerDimensions->show();
		ui->groupBoxMiscellaneous->show();
	}
}

void CoreParameterView::on_checkBoxAntialiasing_stateChanged(int v)
{
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
	if (ui->checkBoxExpertOptions->checkState() == Qt::Unchecked) {
		coreParams->m_antialiasing = false;
	}
	//enable
	else if (ui->checkBoxExpertOptions->checkState() == Qt::Checked) {
		coreParams->m_antialiasing = true;

	}
	emitToggleAntialiasing(v);
}

void CoreParameterView::fillUI() 
{
	//setup ui from params
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());

	//view toggle
	if (coreParams->m_viewSwitch) { ui->checkBoxEnableCoreView->setChecked(true); }
	else { ui->checkBoxEnableCoreView->setChecked(false); }
	//track orientation line
	if (coreParams->m_trackOrientationLine) { ui->checkBoxTrackOrientationLine->setChecked(true); }
	else {ui->checkBoxTrackOrientationLine->setChecked(false);}
	//tracer proportions
	ui->spinBoxTracerProportions->setValue(0.5);
	//tracer orientation line
	if (coreParams->m_tracerOrientationLine) { ui->checkBoxTracerOrientationLine->setChecked(true); }
	else { ui->checkBoxTracerOrientationLine->setChecked(false); }
	//tracing style
	if (coreParams->m_tracingStyle == "None") { ui->comboBoxTracingStyle->setCurrentIndex(0); }
	//tracing time degradation
	if (coreParams->m_tracingTimeDegradation == "None") { ui->comboBoxTracingTimeDegradation->setCurrentIndex(0); }
	//tracing history
	if (coreParams->m_tracingHistory) { ui->spinBoxTracingHistoryLength->setValue(coreParams->m_tracingHistory); }
	//tracing steps
	if (coreParams->m_tracingSteps) { ui->spinBoxTracingSteps->setValue(coreParams->m_tracingSteps); }
	//antialiasing
	if (coreParams->m_antialiasing) { ui->checkBoxAntialiasing->setChecked(true); }
	else { ui->checkBoxAntialiasing->setChecked(false); }



	//enable/disable widgets

	//expert options
	if (ui->checkBoxExpertOptions->isChecked()) {
		ui->groupBoxTracerDimensions->show();
		ui->groupBoxMiscellaneous->show();
	}
	else {
		ui->groupBoxTracerDimensions->hide();
		ui->groupBoxMiscellaneous->hide();
	}
}

void CoreParameterView::getNotified()
{
	qDebug() << "Core parameter notified";
}