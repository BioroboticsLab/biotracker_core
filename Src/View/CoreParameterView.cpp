#include "CoreParameterView.h"
#include "ui_CoreParameterView.h"
#include "qdebug.h"
#include "Model/CoreParameter.h"

#include <iostream>
#include "util/types.h"
#include <limits>
#include "Controller/ControllerCoreParameter.h"
#include <qmessagebox.h>
#include <QColorDialog>
#include "Controller/IControllerCfg.h"

CoreParameterView::CoreParameterView(QWidget*     parent,
                                     IController* controller,
                                     IModel*      model)
: IViewWidget(parent, controller, model)
, ui(new Ui::CoreParameterView)
{
    _cfg = static_cast<IControllerCfg*>(controller)->getConfig();
    ui->setupUi(this);

    ui->lineEditRectWidth->setValidator(new QDoubleValidator(this));
    ui->lineEditRectHeight->setValidator(new QDoubleValidator(this));

    double      h  = std::max(_cfg->RectificationHeight,
                        std::numeric_limits<double>::epsilon());
    double      w  = std::max(_cfg->RectificationWidth,
                        std::numeric_limits<double>::epsilon());
    std::string sw = std::to_string(w);
    std::string sh = std::to_string(h);
    sh.erase(sh.find_last_not_of('0') + 1, std::string::npos);
    sw.erase(sw.find_last_not_of('0') + 1, std::string::npos);
    ui->lineEditRectWidth->setText(sw.c_str());
    ui->lineEditRectHeight->setText(sh.c_str());

    // TODO implement it correctly before enabling again
    ui->checkBoxIgnoreZoom->hide();

    CoreParameter* coreParams = dynamic_cast<CoreParameter*>(model);

    int trial =
        dynamic_cast<ControllerCoreParameter*>(controller)->getTrialNumber();
    ui->label_ExpTrialNo->setText(std::to_string(trial).c_str());

    fillUI();
    setStyle();

    getNotified();
}

CoreParameterView::~CoreParameterView()
{
    delete ui;
}

QWidget* CoreParameterView::getTrackerHook()
{
    // return ui->tabWidgetParameters;
    // dead code
    return nullptr;
}

void CoreParameterView::setPermission(
    std::pair<ENUMS::COREPERMISSIONS, bool> permission)
{

    // first check if permission is for view, if not pass permission to shapes
    // -> view has all permissions, shapes only certain ones
    if (permission.first == ENUMS::COREPERMISSIONS::COMPONENTVIEW &&
        permission.second == false) {
        // disable everything but the area descriptor groupbox
        ui->checkBoxEnableCoreView->setEnabled(false);
        ui->groupBoxTracks->setEnabled(false);
        ui->groupBoxTracing->setEnabled(false);
        ui->groupBoxMiscellaneous->setEnabled(false);
        ui->groupBoxAnno->setEnabled(false);
        ui->groupBoxRectificationParm->setEnabled(true);
        _expertSwitch->setEnabled(false);
        return;
    }
    // does not need to be propagated to shapes; only handled by view
    // TODO: move this to controller
    if (permission.first == ENUMS::COREPERMISSIONS::COMPONENTADD &&
        permission.second == false) {
        this->ui->pushButton_addTraj->setDisabled(true);
        return;
    }
}

void CoreParameterView::triggerUpdate()
{
    on_checkBoxDisplayTrackingArea_stateChanged(0);
    on_checkBoxDisplayRectification_stateChanged(0);
    on_checkboxTrackingAreaAsEllipse_stateChanged(0);
}

void CoreParameterView::areaDescriptorTypeChanged(QString type)
{
    if (type != "0") {
        ui->checkboxTrackingAreaAsEllipse->setChecked(true);
    } else {
        ui->checkboxTrackingAreaAsEllipse->setChecked(false);
    }
}

void CoreParameterView::on_checkBoxEnableCoreView_stateChanged(int v)
{
    CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
    // disable
    if (ui->checkBoxEnableCoreView->checkState() == Qt::Unchecked) {
        // disable all groupboxes but the area descriptor one
        ui->groupBoxTracks->setEnabled(false);
        ui->groupBoxTracing->setEnabled(false);
        ui->groupBoxMiscellaneous->setEnabled(false);
        ui->groupBoxAnno->setEnabled(false);
        ui->groupBoxRectificationParm->setEnabled(true);
        _expertSwitch->setEnabled(false);

        emitViewSwitch(false);
        coreParams->m_viewSwitch = false;
    }
    // enable
    else if (ui->checkBoxEnableCoreView->checkState() == Qt::Checked) {
        // enable all groupboxes
        ui->groupBoxTracks->setEnabled(true);
        ui->groupBoxTracing->setEnabled(true);
        ui->groupBoxMiscellaneous->setEnabled(true);
        ui->groupBoxAnno->setEnabled(true);
        ui->groupBoxRectificationParm->setEnabled(true);
        _expertSwitch->setEnabled(true);

        emitViewSwitch(true);
        coreParams->m_viewSwitch = true;
    }
}

void CoreParameterView::on_comboBoxTracingStyle_currentIndexChanged(
    const QString& text)
{
    CoreParameter* coreParams  = dynamic_cast<CoreParameter*>(getModel());
    coreParams->m_tracingStyle = text;
    emitTracingStyle(text);
}

void CoreParameterView::on_spinBoxTracingHistoryLength_valueChanged(int i)
{
    CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
    // int value = ui->spinBoxTracingHistoryLength->value();
    coreParams->m_tracingHistory = i;
    emitTracingHistoryLength(i);
}

void CoreParameterView::on_spinBoxTracingSteps_valueChanged(int i)
{
    CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
    // int value = ui->spinBoxTracingSteps->value();
    coreParams->m_tracingSteps = i;
    emitTracingSteps(i);
}

void CoreParameterView::on_comboBoxTracingTimeDegradation_currentIndexChanged(
    const QString& text)
{
    CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
    coreParams->m_tracingTimeDegradation = text;
    emitTracingTimeDegradation(text);
}

void CoreParameterView::on_checkBoxTracerFrameNumber_stateChanged(int toggle)
{
    CoreParameter* coreParams       = dynamic_cast<CoreParameter*>(getModel());
    coreParams->m_tracerFrameNumber = toggle;
    emitTracerFrameNumber(toggle);
}

void CoreParameterView::on_spinBoxTracerProportions_valueChanged(double d)
{
    CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
    // float value = (float)ui->spinBoxTracerProportions->value();
    coreParams->m_tracerProportions = d;
    emitTracerProportions(d);
}

void CoreParameterView::on_checkBoxTracerOrientationLine_stateChanged(
    int toggle)
{
    CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
    coreParams->m_tracerOrientationLine = toggle;
    emitTracerOrientationLine(toggle);
}

// void CoreParameterView::on_pushButtonSelectAll_clicked()
// {
// 	emitSelectAll();
// }

// void CoreParameterView::on_pushButtonAddTrack_clicked()
// {
// 	emitAddTrack();
// }

void CoreParameterView::on_checkBoxIgnoreZoom_stateChanged(int v)
{
    CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
    coreParams->m_ignoreZoom  = v;
    emitIgnoreZoom(v);
}
// TODO show current color
void CoreParameterView::on_pushButtonColorChangeBorder_clicked()
{
    emitColorChangeBorderAll();
}

void CoreParameterView::on_pushButtonColorChangeBorderSelected_clicked()
{
    emitColorChangeBorderSelected();
}
// TODO show current color
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

void CoreParameterView::on_checkBoxShowId_stateChanged(int v)
{
    CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
    coreParams->m_trackShowId = v;
    emitTrackShowId(v);
}

// if dimensions are set for all tracks they are going to be set for new tracks
// aswell
void CoreParameterView::on_pushButtonTrackDimensionSetterAll_clicked()
{
    int width  = ui->spinboxTrackWidth->value();
    int height = ui->spinBoxTrackHeight->value();

    CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
    if (coreParams) {
        coreParams->m_trackWidth  = width;
        coreParams->m_trackHeight = height;

        emitTrackDimensionsAll(width, height);
    }
}

void CoreParameterView::on_pushButtonTrackDimensionSetterSelected_clicked()
{
    int width  = ui->spinboxTrackWidth->value();
    int height = ui->spinBoxTrackHeight->value();
    emitTrackDimensionsSelected(width, height);
}

void CoreParameterView::on_pushButtonDefaultDimensions_clicked()
{
    emitTrackDimensionsSetDefault();
}

void CoreParameterView::on_lineEditRectWidth_textChanged(QString s)
{
    double w = std::max(
        ::atof(ui->lineEditRectWidth->text().toStdString().c_str()),
        std::numeric_limits<double>::epsilon());
    double h = std::max(
        ::atof(ui->lineEditRectHeight->text().toStdString().c_str()),
        std::numeric_limits<double>::epsilon());
    Q_EMIT emitRectDimensions(w, h);
}

void CoreParameterView::on_lineEditRectHeight_textChanged(QString s)
{
    double w = std::max(
        ::atof(ui->lineEditRectWidth->text().toStdString().c_str()),
        std::numeric_limits<double>::epsilon());
    double h = std::max(
        ::atof(ui->lineEditRectHeight->text().toStdString().c_str()),
        std::numeric_limits<double>::epsilon());
    Q_EMIT emitRectDimensions(w, h);
}

void CoreParameterView::on_checkBoxDisplayTrackingArea_stateChanged(int v)
{
    Q_EMIT emitDisplayTrackingArea(
        ui->checkBoxDisplayTrackingArea->isChecked());
}

void CoreParameterView::on_checkBoxDisplayRectification_stateChanged(int v)
{
    Q_EMIT emitDisplayRectification(
        ui->checkBoxDisplayRectification->isChecked());
}

void CoreParameterView::on_checkboxTrackingAreaAsEllipse_stateChanged(int v)
{
    Q_EMIT emitTrackingAreaAsEllipse(
        ui->checkboxTrackingAreaAsEllipse->isChecked());
}

void CoreParameterView::on_pushButtonAnnoColor_clicked()
{
    QPalette pal          = ui->pushButtonAnnoColor->palette();
    QColor   oldColor     = pal.color(QPalette::Button);
    QColor   newAnnoColor = QColorDialog::getColor(
        oldColor,
        Q_NULLPTR,
        "Set new annotation color",
        QColorDialog::ShowAlphaChannel);

    if (newAnnoColor.isValid()) {
        QPalette pal = ui->pushButtonAnnoColor->palette();
        pal.setColor(QPalette::Button, newAnnoColor);
        ui->pushButtonAnnoColor->setAutoFillBackground(true);
        ui->pushButtonAnnoColor->setPalette(pal);
        ui->pushButtonAnnoColor->update();
        ui->pushButtonAnnoColor->setAutoFillBackground(true);
        ui->pushButtonAnnoColor->setFlat(true);

        Q_EMIT emitSetAnnoColor(newAnnoColor);
    }
}

void CoreParameterView::toggleExpertOptions(bool toggle)
{
    ui->groupBoxTracerDimensions->setVisible(toggle);
    ui->groupBoxMiscellaneous->setVisible(toggle);
    ui->groupBoxTrackDimensions->setVisible(toggle);
    ui->groupBoxTracerDimensions->setVisible(toggle);
    ui->groupBoxAnno->setVisible(toggle);

    ui->checkBoxTracerFrameNumber->setVisible(toggle);
    ui->checkBoxShowId->setVisible(toggle);
}

void CoreParameterView::on_checkBoxAntialiasingEntities_toggled(bool toggle)
{
    CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
    coreParams->m_antialiasingEntities = toggle;
    emitToggleAntialiasingEntities(toggle);
}

void CoreParameterView::on_checkBoxAntialiasingFull_toggled(bool toggle)
{
    CoreParameter* coreParams      = dynamic_cast<CoreParameter*>(getModel());
    coreParams->m_antialiasingFull = toggle;
    emitToggleAntialiasingFull(toggle);
}

void CoreParameterView::fillUI()
{
    // add switchbutton for expert options
    _expertSwitch = new SwitchButton("not advanced", "advanced");
    ui->widgetParameterLayout->addWidget(_expertSwitch);
    QObject::connect(_expertSwitch,
                     &SwitchButton::emitSetEnabled,
                     this,
                     &CoreParameterView::toggleExpertOptions,
                     Qt::DirectConnection);

    // setup ui from params
    CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());

    // view toggle
    ui->checkBoxEnableCoreView->setChecked(coreParams->m_viewSwitch);
    // track ignore zoom
    ui->checkBoxIgnoreZoom->setChecked(coreParams->m_ignoreZoom);
    // track orientation line
    ui->checkBoxTrackOrientationLine->setChecked(
        coreParams->m_trackOrientationLine);
    // track show id
    ui->checkBoxShowId->setChecked(coreParams->m_trackShowId);
    // tracer proportions
    ui->spinBoxTracerProportions->setValue(coreParams->m_tracerProportions);
    // tracer orientation line
    ui->checkBoxTracerOrientationLine->setChecked(
        coreParams->m_tracerOrientationLine);
    // tracing style
    if (coreParams->m_tracingStyle == "No tracing") {
        ui->comboBoxTracingStyle->setCurrentIndex(0);
    }
    // tracing time degradation
    if (coreParams->m_tracingTimeDegradation == "None") {
        ui->comboBoxTracingTimeDegradation->setCurrentIndex(0);
    }
    // tracing history
    if (coreParams->m_tracingHistory) {
        ui->spinBoxTracingHistoryLength->setValue(
            coreParams->m_tracingHistory);
    }
    // tracing steps
    if (coreParams->m_tracingSteps) {
        ui->spinBoxTracingSteps->setValue(coreParams->m_tracingSteps);
    }
    // tracer frame number
    ui->checkBoxTracerFrameNumber->setChecked(coreParams->m_tracerFrameNumber);
    // antialiasing
    ui->checkBoxAntialiasingEntities->setChecked(
        coreParams->m_antialiasingEntities);
    ui->checkBoxAntialiasingFull->setChecked(coreParams->m_antialiasingFull);
    // track width
    if (coreParams->m_trackWidth) {
        ui->spinboxTrackWidth->setValue(coreParams->m_trackWidth);
    }
    // track height
    if (coreParams->m_trackHeight) {
        ui->spinBoxTrackHeight->setValue(coreParams->m_trackHeight);
    }

    // annotation color button
    QPalette pal = ui->pushButtonAnnoColor->palette();
    pal.setColor(QPalette::Button, Qt::yellow);
    ui->pushButtonAnnoColor->setAutoFillBackground(true);
    ui->pushButtonAnnoColor->setPalette(pal);
    ui->pushButtonAnnoColor->setFlat(true);
    ui->pushButtonAnnoColor->update();

    // enable/disable widgets

    // expert options
    if (_expertSwitch->state()) {
        ui->groupBoxTracerDimensions->show();
        ui->groupBoxMiscellaneous->show();
        ui->groupBoxTrackDimensions->show();
        ui->groupBoxAnno->show();
        ui->checkBoxTracerFrameNumber->show();
        ui->checkBoxShowId->show();
    } else {
        ui->groupBoxTracerDimensions->hide();
        ui->groupBoxMiscellaneous->hide();
        ui->groupBoxTrackDimensions->hide();
        ui->groupBoxAnno->hide();
        ui->checkBoxTracerFrameNumber->hide();
        ui->checkBoxShowId->hide();
    }
}

void CoreParameterView::setStyle()
{
    // main groupboxes
    ui->groupBoxTracks->setStyleSheet(
        "QGroupBox { background-color: #bbdefb;}");
    ui->groupBoxRectificationParm->setStyleSheet(
        "QGroupBox { background-color: #d1c4e9; }");
    ui->groupBoxTracing->setStyleSheet(
        "QGroupBox { background-color: #c8e6c9; }");
    ui->groupBoxMiscellaneous->setStyleSheet(
        "QGroupBox { background-color: #ffecb3; }");
    ui->groupBoxAnno->setStyleSheet(
        "QGroupBox { background-color: #fad8ba; }");

    // expert options are slightly darker
    ui->groupBoxTrackDimensions->setStyleSheet(
        "QGroupBox { background-color: #90caf9;}");
    ui->groupBoxTracerDimensions->setStyleSheet(
        "QGroupBox { background-color: #a5d6a7;}");

    // default groupbox style
    ui->widgetParameter->setStyleSheet(
        "QGroupBox"
        " {border: 1px solid #e5e5e5; border-radius: 5px; margin-top: 1ex; /* "
        "leave space at the top for the title */}"
        "QGroupBox::title {subcontrol-origin: margin; padding: -1px 3px; "
        "background-color: #e5e5e5; }");
}

void CoreParameterView::resetTrial()
{
    _trialActive  = false;
    _trialStarted = false;
    Q_EMIT emitTrialStarted(false);
    ui->pushButton_startExp->setText("Start a new Trial");
}

void CoreParameterView::getNotified()
{
    CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
    ui->labelNumberOfTracks->setText(
        QString::number(coreParams->m_trackNumber));
    ui->label_ExpObjCnt->setText(QString::number(coreParams->m_trackNumber));

    int trial = dynamic_cast<ControllerCoreParameter*>(getController())
                    ->getTrialNumber() +
                1;
    ui->label_ExpTrialNo->setText(std::to_string(trial).c_str());
}

/************EXPERIMENT TAB*******************/

void CoreParameterView::on_pushButton_startExp_clicked()
{
    if (!_trialActive) {

        if (_currentFile == "No Media") {
            int ret = QMessageBox::information(
                this,
                tr("BioTracker"),
                tr("Please select a source video first. \nYou can do so in "
                   "the \"File\" menu."),
                QMessageBox::Ok);
        } else {
            if (!_trialStarted) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(
                    this,
                    "Confirmation",
                    "Are you sure you want to start a new trial?"
                    "\nThis will reset the previous tracking data!",
                    QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::No) {
                    qDebug() << "CORE: New trial aborted";
                    return;
                }

                _trialStarted = true;
                emitTrialStarted(true);
                emitFinalizeExperiment();
                emitPausePlayback();
                QMessageBox::information(
                    0,
                    "New trial",
                    "You started a new trial! Previous data was saved and "
                    "reset.\n\nGo to the timespot in the video you want to "
                    "track at."
                    "\n\nPlease add the number of objects you want to track "
                    "as trajectories.\nResume the video.");
            } else {
                emitStartPlayback();
            }
            emitDeactivateTrackingSwitch();
            emitEnableTracking();
            ui->pushButton_startExp->setText("Pause Trial");
            ui->label_ExpSt->setText("Active - tracking");
            _trialActive = true;
        }
    } else {

        emitDisableTracking();
        emitPausePlayback();
        ui->pushButton_startExp->setText("Resume Trial");
        ui->label_ExpSt->setText("Paused - not tracking");
        _trialActive = false;
    }
}

void CoreParameterView::on_pushButton_finalizeExp_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Confirmation",
        "Are you sure you want to finalize the trial?"
        "\nThis will save the current trial tracking"
        " data in a new file in the 'Trials' directory"
        " and reset it afterwards!",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        qDebug() << "CORE: Finalize trial aborted!";
        return;
    }

    Q_EMIT emitStopPlayback();
    Q_EMIT emitActivateTrackingSwitch();
    Q_EMIT emitDisableTracking();
    _trialActive  = false;
    _trialStarted = false;
    ui->pushButton_startExp->setText("Start a new Trial");
    ui->label_ExpSt->setText("No Trial started!");
    Q_EMIT emitTrialStarted(false);
    Q_EMIT emitFinalizeExperiment();
}

void CoreParameterView::on_trialHelpButton_clicked()
{
    QMessageBox::information(
        0,
        "Trials",
        "'Start a new Trial' will cause all the previous data to be saved and "
        "then reset.\n\n"
        "Go to the time spot you want to track and create a trajectory for "
        "each object you want to track.\n"
        "Resume the video and the tracker will track your objects.\n\n"
        "If you want to pause the tracking you can just pause the video. "
        "Resuming the video will continue the tracking.\n"
        "If you want to pause the video and resume without tracking you will "
        "have to click 'Pause Trial' causing the tracking to be disabled.\n"
        "You can now watch the video without tracking.\n\n"
        "'Resume trial' will cause the tracking to be activated AND the video "
        "to be resumed");
}

void CoreParameterView::on_pushButton_saveData_clicked()
{
    emitSaveDataToFile();
}

void CoreParameterView::on_pushButton_resetData_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Confirmation",
        "Are you sure you want to reset the current tracking data?"
        "This will firstly save the data and then reset it!",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        qDebug() << "CORE: Data reset aborted";
        return;
    }
    Q_EMIT emitFinalizeExperiment();
}

void CoreParameterView::on_pushButton_addTraj_clicked()
{
    emitAddTrack();
}

void CoreParameterView::rcvPlayerParameters(
    std::shared_ptr<const playerParameters> parameters)
{
    QFileInfo f(parameters->m_CurrentFilename);
    _currentFile = f.baseName();
    ui->label_ExpSrcCnt->setText(_currentFile);
}
