#include "ControllerCoreParameter.h"
#include "ControllerTrackedComponentCore.h"
#include "ControllerAreaDescriptor.h"
#include "ControllerDataExporter.h"
#include "ControllerGraphicScene.h"
#include "ControllerPlayer.h"
#include "ControllerMainWindow.h"
#include "ControllerAnnotations.h"
#include "View/CoreParameterView.h"
#include "View/TrackedComponentView.h"
#include "Model/CoreParameter.h"

ControllerCoreParameter::ControllerCoreParameter(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IControllerCfg(parent, context, ctr)
{

}

void ControllerCoreParameter::createView()
{
	assert(m_Model);
	m_View = new CoreParameterView(0, this, m_Model);
}

void ControllerCoreParameter::connectModelToController()
{

}

void ControllerCoreParameter::connectControllerToController()
{
	CoreParameterView* view = static_cast<CoreParameterView*>(m_View);view->triggerUpdate();
	//Connections to the trackedComponentCore
	{
		IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);
		ControllerTrackedComponentCore *tccController = dynamic_cast<ControllerTrackedComponentCore*>(ctr);
		IView* vv = tccController->getView();
		TrackedComponentView* tcview = static_cast<TrackedComponentView*>(vv);

		IController* ctr2 = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
		ControllerGraphicScene* ctrGrphScn = dynamic_cast<ControllerGraphicScene*>(ctr2);


		QObject::connect(view, &CoreParameterView::emitViewSwitch, tcview, &TrackedComponentView::receiveViewSwitch, Qt::DirectConnection);
		//Tracks
		//QObject::connect(view, &CoreParameterView::emitSelectAll, tcview, &TrackedComponentView::receiveSelectAll, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitAddTrack, tcview, &TrackedComponentView::addTrajectory, Qt::DirectConnection);

		//Track dimensions
		QObject::connect(view, &CoreParameterView::emitTrackOrientationLine, tcview, &TrackedComponentView::receiveTrackOrientationLine, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTrackShowId, tcview, &TrackedComponentView::receiveTrackShowId, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTrackDimensionsAll, tcview, &TrackedComponentView::receiveTrackDimensionsAll, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTrackDimensionsSelected, tcview, &TrackedComponentView::receiveTrackDimensionsSelected, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTrackDimensionsSetDefault, tcview, &TrackedComponentView::receiveTrackDimensionsSetDefault, Qt::DirectConnection);

		//Track ignore zoom
		QObject::connect(view, &CoreParameterView::emitIgnoreZoom, tcview, &TrackedComponentView::receiveIgnoreZoom, Qt::DirectConnection);

		//Track color
		QObject::connect(view, &CoreParameterView::emitColorChangeBorderAll, tcview, &TrackedComponentView::receiveColorChangeBorderAll, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitColorChangeBorderSelected, tcview, &TrackedComponentView::receiveColorChangeBorderSelected, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitColorChangeBrushAll, tcview, &TrackedComponentView::receiveColorChangeBrushAll, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitColorChangeBrushSelected, tcview, &TrackedComponentView::receiveColorChangeBrushSelected, Qt::DirectConnection);
		//Tracing
		QObject::connect(view, &CoreParameterView::emitTracingHistoryLength, tcview, &TrackedComponentView::receiveTracingHistoryLength, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTracingStyle, tcview, &TrackedComponentView::receiveTracingStyle, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTracingSteps, tcview, &TrackedComponentView::receiveTracingSteps, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTracingTimeDegradation, tcview, &TrackedComponentView::receiveTracingTimeDegradation, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTracerFrameNumber, tcview, &TrackedComponentView::receiveTracerFrameNumber, Qt::DirectConnection);
		//Tracing dimensions
		QObject::connect(view, &CoreParameterView::emitTracerProportions, tcview, &TrackedComponentView::receiveTracerProportions, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTracerOrientationLine, tcview, &TrackedComponentView::receiveTracerOrientationLine, Qt::DirectConnection);
		//Misc
		QObject::connect(view, &CoreParameterView::emitToggleAntialiasingEntities, tcview, &TrackedComponentView::receiveToggleAntialiasingEntities, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitToggleAntialiasingFull, ctrGrphScn, &ControllerGraphicScene::receiveToggleAntialiasingFull, Qt::DirectConnection);

	}
	//Connections to the AreaDescriptor
	{
		IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::AREADESCRIPTOR);
		ControllerAreaDescriptor *adController = static_cast<ControllerAreaDescriptor*>(ctr);
		QObject::connect(view, &CoreParameterView::emitRectDimensions, adController, &ControllerAreaDescriptor::setRectificationDimensions, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitDisplayTrackingArea, adController, &ControllerAreaDescriptor::setDisplayTrackingAreaDefinition, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitDisplayRectification, adController, &ControllerAreaDescriptor::setDisplayRectificationDefinition, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTrackingAreaAsEllipse, adController, &ControllerAreaDescriptor::setTrackingAreaAsEllipse, Qt::DirectConnection);
    }

	//Connections to the Annotations
	{
		IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::ANNOTATIONS);
		ControllerAnnotations *annoController = static_cast<ControllerAnnotations*>(ctr);
		QObject::connect(view, &CoreParameterView::emitSetAnnoColor, annoController, &ControllerAnnotations::receiveSetAnnotationColor, Qt::DirectConnection);
	}

    //Connections to the DataExporter
    {
        IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::DATAEXPORT);
        ControllerDataExporter *deController = static_cast<ControllerDataExporter*>(ctr);
        QObject::connect(view, &CoreParameterView::emitFinalizeExperiment, deController, &ControllerDataExporter::receiveFinalizeExperiment, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTrialStarted, deController, &ControllerDataExporter::receiveTrialStarted, Qt::DirectConnection);


    }
    //Media Player
    {
        IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
        ControllerPlayer *mpc = static_cast<ControllerPlayer*>(ctr);
        MediaPlayer* mp = static_cast<MediaPlayer*>(mpc->getModel());
        QObject::connect(mp, &MediaPlayer::fwdPlayerParameters, view, &CoreParameterView::rcvPlayerParameters);
        QObject::connect(view, &CoreParameterView::emitStartPlayback, mpc, &ControllerPlayer::play);
        QObject::connect(view, &CoreParameterView::emitStopPlayback, mpc, &ControllerPlayer::stop);
		QObject::connect(view, &CoreParameterView::emitPausePlayback, mpc, &ControllerPlayer::pause);
    }
    //Main Window
    {
        IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
        ControllerMainWindow *mwc = static_cast<ControllerMainWindow*>(ctr);
        QObject::connect(view, &CoreParameterView::emitEnableTracking, mwc, &ControllerMainWindow::activeTracking);
        QObject::connect(view, &CoreParameterView::emitDisableTracking, mwc, &ControllerMainWindow::deactiveTrackring);
		QObject::connect(view, &CoreParameterView::emitActivateTrackingSwitch, mwc, &ControllerMainWindow::activeTrackingCheckBox);
		QObject::connect(view, &CoreParameterView::emitDeactivateTrackingSwitch, mwc, &ControllerMainWindow::deactiveTrackingCheckBox);
		QObject::connect(view, &CoreParameterView::emitSaveDataToFile, mwc, &ControllerMainWindow::receiveSaveTrajData);
    }

    view->triggerUpdate();
}

void ControllerCoreParameter::changeAreaDescriptorType(QString type) {
    if (dynamic_cast<CoreParameterView*>(m_View))
        dynamic_cast<CoreParameterView*>(m_View)->areaDescriptorTypeChanged(type);
}

void ControllerCoreParameter::receiveResetTrial()
{
	CoreParameterView* view = static_cast<CoreParameterView*>(m_View);
	view->resetTrial();
}

void ControllerCoreParameter::triggerUpdate() {

    CoreParameterView* view = static_cast<CoreParameterView*>(m_View);
    view->triggerUpdate();
}

void ControllerCoreParameter::receiveTrackNumber(int number)
{
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(getModel());
	if (coreParams) {
		coreParams->setTrackNumber(number);
	}
}

void ControllerCoreParameter::createModel()
{

	m_Model = new CoreParameter(this);
}

void ControllerCoreParameter::updateView() 
{
}

void ControllerCoreParameter::setCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission)
{
	if (dynamic_cast<CoreParameterView*>(m_View)) {
		dynamic_cast<CoreParameterView*>(m_View)->setPermission(permission);
	}
	else {
		//qDebug() << "no view yet";
		//This should never happen, actually
		assert(false);
	}
}

int ControllerCoreParameter::getTrialNumber() {
    return (static_cast<ControllerDataExporter*>(m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::DATAEXPORT)))->getNumber(true);
}

