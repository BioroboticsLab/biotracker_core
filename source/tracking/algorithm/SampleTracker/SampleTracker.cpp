#include "SampleTracker.h"
#include "source/helper/StringHelper.h"
#include <QApplication>

SampleTracker::SampleTracker( Settings & settings, QWidget *parent ) : TrackingAlgorithm( settings, parent )
{
	_showSelectorRec = false;
	_selectorRecStart = cv::Point();
    _selectorRecEnd = cv::Point();
}


SampleTracker::~SampleTracker(void)
{
}

void SampleTracker::track		( std::vector<TrackedObject> &, ulong, cv::Mat& ){}
void SampleTracker::paint		( cv::Mat& image )
{
	if(_showSelectorRec)
	{
		drawRectangle(image);
	}
}
void SampleTracker::reset		(){}

void SampleTracker::forcePointIntoPicture(cv::Point & point, cv::Mat & image)
{
	//check if any coordinates outside picture
	//and if so change them!
	if ( point.x < 0 )
		point.x = 0;
	if ( point.x > image.cols )
		point.x = image.cols;
	if ( point.y < 0 )
		point.y = 0;
	if ( point.y > image.rows )
		point.y = image.rows;
}

//this will draw a basic rectangle onto the diplay image 
void SampleTracker::drawRectangle(cv::Mat image)
{
	forcePointIntoPicture(_selectorRecStart,image);
	forcePointIntoPicture(_selectorRecEnd,image);

	int width	= _selectorRecEnd.x - _selectorRecStart.x;
	int height	= _selectorRecEnd.y - _selectorRecStart.y;
	int x,y;
	//check into which direction rectangle is stretched
	if		( width < 0 && height > 0 )
	{		x = _selectorRecEnd.x;		y = _selectorRecStart.y;	}
	else if ( width > 0 && height < 0 )
	{		x = _selectorRecStart.x;		y = _selectorRecEnd.y;	}
	else if ( width < 0 && height < 0 )
	{		x = _selectorRecEnd.x;		y = _selectorRecEnd.y;	}
	else
	{		x = _selectorRecStart.x;		y = _selectorRecStart.y;	}
	
	cv::Mat roi = image(cv::Rect(x, y, abs(width), abs(height)));	
	
	cv::Mat color(roi.size(), CV_8UC3, cv::Scalar(255, 255, 0)); 
    double alpha = 0.3;
    cv::addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi); 
	//cv::rectangle( image,
 //          _selectorRecStart,
 //          _selectorRecEnd,
 //          cv::Scalar(255, 255, 0 ),           
 //          4 );
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

void SampleTracker::mouseWheelEvent ( QWheelEvent *) {}
