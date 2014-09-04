#include "TrackingAlgorithm.h"
#include "source/helper/StringHelper.h"

TrackingAlgorithm::TrackingAlgorithm(Settings & settings) : _settings(settings) 
{
}

TrackingAlgorithm::~TrackingAlgorithm() 
{
}

void TrackingAlgorithm::mouseMoveEvent		( QMouseEvent * e ){}
void TrackingAlgorithm::mousePressEvent		( QMouseEvent * e )
{
	if ( e->button() == Qt::LeftButton)
	{
		int x = e->x(); int y = e->y();
		std::string note = "left button press on: x=" + StringHelper::iToSS(x) + " y=" + StringHelper::iToSS(y);
		emit notifyGUI(note,MSGS::NOTIFICATION);
	}
}
void TrackingAlgorithm::mouseReleaseEvent	( QMouseEvent * e ){}