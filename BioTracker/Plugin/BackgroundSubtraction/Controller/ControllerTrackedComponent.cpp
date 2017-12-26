#include "ControllerTrackedComponent.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackingRectElement.h"
#include "Model/TrackedComponents/TrackedTrajectory.h"
#include "View/TrackedElementView.h"
#include "qdebug.h"

ControllerTrackedComponent::ControllerTrackedComponent(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr)
{
	m_currentFrameNumber = 0;
}

void ControllerTrackedComponent::createView()
{
	m_View = new TrackedElementView(0, this, m_Model);
}

void ControllerTrackedComponent::connectModelToController()
{

}

void ControllerTrackedComponent::connectControllerToController()
{
}

void createTrajectories(int count, TrackedTrajectory* all) {
	//This should be done using a factory, right?
	for (int i = 0; i < count; i++) {
		TrackedTrajectory *t = new TrackedTrajectory();
        t->setValid(true);
        //t->setId(i);
		TrackedElement *e = new TrackedElement(t, "n.a.", t->getId());
		t->add(e);
		all->add(t, i);
	}
}

void ControllerTrackedComponent::createModel()
{
	TrackedTrajectory *t = new TrackedTrajectory(this, "All");
	
	//Add default trajectories
	createTrajectories(2, t);
	m_Model = t;
}


IView *ControllerTrackedComponent::getTrackingElementsWidget()
{
	return m_View;
}

void ControllerTrackedComponent::receiveRemoveTrajectory(IModelTrackedTrajectory * trajectory)
{
	trajectory->setValid(false);
	qDebug() << "track" << trajectory->getId() << "set invalid";
}

void ControllerTrackedComponent::receiveRemoveTrackEntity(IModelTrackedTrajectory * trajectory)
{
	trajectory->getChild(m_currentFrameNumber)->setValid(false);
	qDebug() << "track " << trajectory->getId() << " entity #" << m_currentFrameNumber << "set invalid";
}

void ControllerTrackedComponent::receiveAddTrajectory(QPoint position)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	cv::Point newPosPx = cv::Point(position.x(), position.y());
	cv::Point2f newPosCm = m_areaDescr->pxToCm(newPosPx);
	// ignore blobs outside the tracking area
	//if (!m_areaDescr->inTrackingArea(newPosPx)) {
	//	qDebug() << "Created the entity outside of the tracking area!";
	//	return;
	//}

	TrackedTrajectory* newTraj = new TrackedTrajectory();
	TrackedElement* firstElem = new TrackedElement(newTraj, "n.a.", newTraj->getId());

	FishPose newPose = FishPose(newPosCm, newPosPx, 0, 0, 20, 20, 0.0);

	firstElem->setFishPose(newPose);


	firstElem->setTime(start);
	newTraj->add(firstElem, m_currentFrameNumber);
	TrackedTrajectory* allTraj = qobject_cast<TrackedTrajectory*>(m_Model);
	if (allTraj) {
		allTraj->add(newTraj);
		qDebug() << "TRACKER: Trajectory added at" << firstElem->getXpx() << "," << firstElem->getYpx();
	}
}

void ControllerTrackedComponent::receiveMoveElement(IModelTrackedTrajectory* trajectory, QPoint position)
{
	TrackedTrajectory* traj = dynamic_cast<TrackedTrajectory*>(trajectory);
	// don't move when frame number under 0 and main trajectory (id's: 0)!!
	if (!(traj->getId() == 0) && m_currentFrameNumber >= 0) {
		TrackedElement* element = dynamic_cast<TrackedElement*>(traj->getChild(m_currentFrameNumber));

		//TODO rewrite this when default ipose is implemented...
		FishPose oldPose = element->getFishPose();

		cv::Point newPosPx = cv::Point(position.x(), position.y());
		cv::Point2f newPosCm = m_areaDescr->pxToCm(newPosPx);
		// ignore blobs outside the tracking area
		if (!m_areaDescr->inTrackingArea(newPosPx)) {
			qDebug() << "Moved the entity outside of the tracking area!";
			return;
		}

		FishPose newPose = FishPose(newPosCm, newPosPx, oldPose.orientation_rad(), oldPose.orientation_deg(), oldPose.width(), oldPose.height(), oldPose.getScore());

		element->setFishPose(newPose);
		//qDebug() << "plugin-pos:" << position;
	}
}

void ControllerTrackedComponent::receiveSwapIds(IModelTrackedTrajectory * trajectory0, IModelTrackedTrajectory * trajectory1)
{
	TrackedTrajectory* traj0 = dynamic_cast<TrackedTrajectory*>(trajectory0);
	TrackedTrajectory* traj1 = dynamic_cast<TrackedTrajectory*>(trajectory1);

	if (traj0 && traj1) {
		if (traj0->getId() !=0 && traj1->getId() !=0) {
			int traj0Id = traj0->getId();
			int traj1Id = traj1->getId();

			traj0->setId(traj1Id);
			traj1->setId(traj0Id);

			qDebug() << "Swap IDs " << traj0Id << "and " << traj1Id;
		}
	}
}

void ControllerTrackedComponent::receiveToggleFixTrack(IModelTrackedTrajectory * trajectory, bool toggle)
{
	trajectory->setFixed(toggle);
}

void ControllerTrackedComponent::receiveCurrentFrameNumber(uint framenumber)
{
	m_currentFrameNumber = (int)framenumber;
}

void ControllerTrackedComponent::receiveAreaDescriptorUpdate(IModelAreaDescriptor * areaDescr)
{
	m_areaDescr = areaDescr;
}
