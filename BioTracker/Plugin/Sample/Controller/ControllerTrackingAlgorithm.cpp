#include "ControllerTrackingAlgorithm.h"
#include "ControllerTrackedComponent.h"

#include "Model/TrackerParameter.h"
#include "View/TrackerParameterView.h"
#include "ControllerTrackedComponent.h"
#include "View/TrackedElementView.h"

namespace sampleTracker {
	ControllerTrackingAlgorithm::ControllerTrackingAlgorithm(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
		IController(parent, context, ctr)
	{
		m_BioTrackerContext = context;
	}

	void ControllerTrackingAlgorithm::connectControllerToController()
	{
		IController * ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COMPONENT);
		QPointer< ControllerTrackedComponent > ctrComponent = qobject_cast<ControllerTrackedComponent *>(ctr);

		m_TrackedTrajectoryMajor = ctrComponent->getModel();
	}

	void ControllerTrackingAlgorithm::doTracking(std::shared_ptr<cv::Mat> mat, uint number)
	{
		qobject_cast<BioTrackerTrackingAlgorithm *>(m_Model)->doTracking(mat, number);
	}

	IView *ControllerTrackingAlgorithm::getTrackingParameterWidget()
	{
		return m_View;
	}

	void ControllerTrackingAlgorithm::createModel()
	{
		connectControllerToController();

		m_TrackingParameter = new TrackerParameter(this);
		m_Model = new BioTrackerTrackingAlgorithm(m_TrackingParameter, m_TrackedTrajectoryMajor);
	}

	void ControllerTrackingAlgorithm::createView()
	{
		m_View = new TrackerParameterView(0, this, m_TrackingParameter);
	}

	void ControllerTrackingAlgorithm::connectModelToController()
	{
		BioTrackerTrackingAlgorithm *trackingAlg = qobject_cast<BioTrackerTrackingAlgorithm *>(m_Model);
		QObject::connect(trackingAlg, &BioTrackerTrackingAlgorithm::emitCvMatA, this, &ControllerTrackingAlgorithm::receiveCvMatFromTrackingAlgorithm);
		QObject::connect(trackingAlg, &BioTrackerTrackingAlgorithm::emitTrackingDone, this, &ControllerTrackingAlgorithm::receiveTrackingDone);
		QObject::connect(trackingAlg, &BioTrackerTrackingAlgorithm::emitChangeDisplayImage, this, &ControllerTrackingAlgorithm::receiveChangeDisplayImage);

		//enable the tracker to send video dimension updates to the view via signal
		IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COMPONENT);
		IView *v = qobject_cast<ControllerTrackedComponent*>(ctr)->getView();
		TrackedElementView *v2 = dynamic_cast<TrackedElementView *>(v);
		QObject::connect(trackingAlg, SIGNAL(emitDimensionUpdate(int, int)), v2, SLOT(rcvDimensionUpdate(int, int)));
	}

	void ControllerTrackingAlgorithm::receiveCvMatFromTrackingAlgorithm(std::shared_ptr<cv::Mat> mat, QString name)
	{
		Q_EMIT emitCvMat(mat, name);
	}

	void ControllerTrackingAlgorithm::receiveTrackingDone()
	{
		Q_EMIT emitTrackingDone();
	}

	void ControllerTrackingAlgorithm::receiveChangeDisplayImage(QString str) {
		Q_EMIT emitChangeDisplayImage(str);
	}
}