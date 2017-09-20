#include "BioTrackerPlugin.h"
#include "PluginContext.h"

#include "Controller/ControllerTrackingAlgorithm.h"
#include "Controller/ControllerTrackedComponent.h"

#include "View/TrackedElementView.h"

	BioTrackerPlugin::BioTrackerPlugin() {
	}

	IView* BioTrackerPlugin::getTrackerParameterWidget() {
		return qobject_cast<sampleTracker::ControllerTrackingAlgorithm*> (m_TrackerController)->getTrackingParameterWidget();
	}
	IView *BioTrackerPlugin::getTrackerElementsWidget()
	{
		return qobject_cast<sampleTracker::ControllerTrackedComponent *> (m_ComponentController)->getTrackingElementsWidget();
	}

#if QT_VERSION < 0x050000
	Q_EXPORT_PLUGIN2(BioTrackerPlugin, BioTrackerPlugin)
#endif // QT_VERSION < 0x050000


		void BioTrackerPlugin::createPlugin() {
		m_PluginContext = new PluginContext();
		m_PluginContext->createApplication();

		IController * ctr = m_PluginContext->requestController(ENUMS::CONTROLLERTYPE::COMPONENT);
		m_ComponentController = qobject_cast<sampleTracker::ControllerTrackedComponent *>(ctr);

		IController * ctr2 = m_PluginContext->requestController(ENUMS::CONTROLLERTYPE::TRACKING);
		m_TrackerController = qobject_cast<sampleTracker::ControllerTrackingAlgorithm *>(ctr2);

		connectInterfaces();

	}

	void BioTrackerPlugin::connectInterfaces() {
		sampleTracker::ControllerTrackingAlgorithm* ctrAlg = qobject_cast<sampleTracker::ControllerTrackingAlgorithm*> (m_TrackerController);
		QObject::connect(ctrAlg, &sampleTracker::ControllerTrackingAlgorithm::emitCvMat, this, &BioTrackerPlugin::receiveCvMatFromController);

		QObject::connect(ctrAlg, &sampleTracker::ControllerTrackingAlgorithm::emitTrackingDone, this, &BioTrackerPlugin::receiveTrackingDone);

		//TODO Hauke I'm not sure if these are the correct partners
		//BioTrackerTrackingAlgorithm *trackingAlg = qobject_cast<BioTrackerTrackingAlgorithm *>(m_TrackerController->getModel());
		//TrackedElementView *view = (TrackedElementView *)(m_ComponentController->getView());
		//QObject::connect(view, &TrackedElementView::emitUpdateCornersChanged, trackingAlg, &BioTrackerTrackingAlgorithm::updateCorner);
	}

	void BioTrackerPlugin::receiveCurrentFrameFromMainApp(std::shared_ptr<cv::Mat> mat, uint frameNumber) {
		qobject_cast<sampleTracker::ControllerTrackingAlgorithm*> (m_TrackerController)->doTracking(mat, frameNumber);
	}

	void BioTrackerPlugin::receiveCvMatFromController(std::shared_ptr<cv::Mat> mat, QString name) {
		Q_EMIT emitCvMat(mat, name);
	}

	void BioTrackerPlugin::receiveTrackingDone() {
		Q_EMIT emitTrackingDone();
	}