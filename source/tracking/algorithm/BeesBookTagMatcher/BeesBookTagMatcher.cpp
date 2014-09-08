#include "BeesBookTagMatcher.h"
#include "source/helper/StringHelper.h"


BeesBookTagMatcher::BeesBookTagMatcher( Settings & settings ) : TrackingAlgorithm( settings )
{
}


BeesBookTagMatcher::~BeesBookTagMatcher(void)
{
}

void BeesBookTagMatcher::track		( std::vector<TrackedObject> & objectList, ulong frameNumber, cv::Mat& frame ){}
void BeesBookTagMatcher::paint		( cv::Mat& image ){}
void BeesBookTagMatcher::reset		(){}

void BeesBookTagMatcher::mouseMoveEvent		( QMouseEvent * e )
{
}
void BeesBookTagMatcher::mousePressEvent		( QMouseEvent * e )
{
	if ( e->button() == Qt::LeftButton)
	{
		int x = e->x(); int y = e->y();
		std::string note = "left button press on: x=" + StringHelper::iToSS(x) + " y=" + StringHelper::iToSS(y);
		emit notifyGUI(note,MSGS::NOTIFICATION);
	}
}
void BeesBookTagMatcher::mouseReleaseEvent	( QMouseEvent * e )
{	
}