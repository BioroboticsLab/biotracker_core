#include "AreaDescriptorView.h"

#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackingRectElement.h"
#include "Model/TrackedComponents/TrackedTrajectory.h"
#include "Model/TrackingAlgorithm/property/Rectification.h"
#include "QBrush"
#include "QPainter"
#include <QGraphicsSceneHoverEvent>
#include "AreaDesciptor/RectDescriptor.h"
#include "settings/Settings.h"
#include "Model/TrackingAlgorithm/property/ParamNames.h"
#include "util/misc.h"

#include "Model/AreaInfo.h"


AreaDescriptorView::AreaDescriptorView(QGraphicsItem *parent, IController *controller, IModel *model) :
	IViewTrackedComponent(parent, controller, model)
{

}


void AreaDescriptorView::updateRectification() {

	Rectification::instance().setArea(((AreaDescriptor*)_rectif)->getRect());
	Rectification::instance().setupRecitification(100, 100, 2040, 2040);
}

void AreaDescriptorView::updateApperture() {
	std::vector<cv::Point> p = _tracking->getRect();

	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CONFIGPARAM::CONFIG_INI_FILE);
	_settings->setParam(TRACKERPARAM::CN_APPERTURE, cvPointsToString(p).c_str());
}


QRectF AreaDescriptorView::boundingRect() const
{

	return _boundingRect;
}


void AreaDescriptorView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

void AreaDescriptorView::getNotified()
{
	update();
}


