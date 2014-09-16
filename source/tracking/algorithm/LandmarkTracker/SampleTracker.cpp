#include "SampleTracker.h"
#include "source/helper/StringHelper.h"
#include <QApplication>
#include "source/helper/CvHelper.h"


SampleTracker::SampleTracker( Settings & settings ) : TrackingAlgorithm( settings )
{
	_showSelectorRec = false;
	_selectorRecStart = cv::Point();
    _selectorRecEnd = cv::Point();
}


SampleTracker::~SampleTracker(void)
{
}

void SampleTracker::track		( std::vector<TrackedObject> & objectList, ulong frameNumber, cv::Mat& frame ){}
void SampleTracker::paint		( cv::Mat& image )
{
	if(_showSelectorRec)
	{
		drawRectangle(image);
	}
}
void SampleTracker::reset		(){}

//this will draw a basic rectangle onto the diplay image 
void SampleTracker::drawRectangle(cv::Mat image)
{
	cv::rectangle( image,
           _selectorRecStart,
           _selectorRecEnd,
           cv::Scalar(255, 255, 0 ),
           -1,
           8 );
}

void SampleTracker::mouseMoveEvent		( QMouseEvent * e )
{
	if(_showSelectorRec)
		{
			_selectorRecEnd.x = e->x();
			_selectorRecEnd.y = e->y();
			//draw rectangle!
			emit update();
		}
}
void SampleTracker::mousePressEvent		( QMouseEvent * e )
{
	//check if left button is clicked
	if ( e->button() == Qt::LeftButton)
	{
		//check for shift modifier
		if(Qt::ShiftModifier == QApplication::keyboardModifiers())
		{
			int x = e->x(); int y = e->y();
			std::string note = "shift + left button press on: x=" + StringHelper::iToSS(x) + " y=" + StringHelper::iToSS(y);
			//initialize coordinates for selection tool
			_selectorRecStart.x = e->x();
			_selectorRecStart.y = e->y();
			_selectorRecEnd.x = e->x();
			_selectorRecEnd.y = e->y();
			_showSelectorRec = true;		
		}
	}
}
void SampleTracker::mouseReleaseEvent	( QMouseEvent * e )
{	
	if ( e->button() == Qt::LeftButton)
	{
		if(_showSelectorRec)
		{
			_selectorRecEnd.x = e->x();
			_selectorRecEnd.y = e->y();
			_showSelectorRec = false;
			//next draw will delete rectangle!
			emit update();
			std::string note = "selected area from " + StringHelper::iToSS(_selectorRecStart.x) + ":"+ StringHelper::iToSS(_selectorRecStart.y)
				+ " to " +  StringHelper::iToSS(_selectorRecEnd.x) + ":"+ StringHelper::iToSS(_selectorRecEnd.y);
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
	}
}