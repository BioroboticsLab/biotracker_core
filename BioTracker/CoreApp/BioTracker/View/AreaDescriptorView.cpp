#include "AreaDescriptorView.h"

//#include "Model/TrackingAlgorithm/property/Rectification.h"
#include "QBrush"
#include "QPainter"
#include <QGraphicsSceneHoverEvent>
#include "AreaDesciptor/RectDescriptor.h"
#include "settings/Settings.h"
//#include "Model/TrackingAlgorithm/property/ParamNames.h"
#include "util/misc.h"
#include "util/types.h"

#include "Model/AreaDescriptor/AreaInfo.h"


AreaDescriptorView::AreaDescriptorView(IController *controller, IModel *model) :
	IViewTrackedComponent( 0 , controller, model)
{

}


void AreaDescriptorView::updateRectification() {
	//TODO PORT
	//Rectification::instance().setArea(((AreaDescriptor*)_rectif)->getRect());
	//Rectification::instance().setupRecitification(100, 100, 2040, 2040);
}

void AreaDescriptorView::updateApperture() {
	std::vector<cv::Point> p = _tracking->getRect();

	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	_settings->setParam(AREADESCRIPTOR::CN_APPERTURE, cvPointsToString(p).c_str());
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


