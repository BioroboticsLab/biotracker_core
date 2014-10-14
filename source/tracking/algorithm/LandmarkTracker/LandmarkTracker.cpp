#include "LandmarkTracker.h"
#include "source/helper/StringHelper.h"
#include <QApplication>
//#include "source/helper/CvHelper.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "source/tracking/TrackingThread.h"



LandmarkTracker::LandmarkTracker( Settings & settings ) : TrackingAlgorithm( settings )
{
	_showSelectorRec = false;
	_selectorRecStart = cv::Point();
    _selectorRecEnd = cv::Point();
	
	//KML
	std::cout<<"LANDMARK TRACKER"<<std::endl;
	_clonedImage = false;
}


LandmarkTracker::~LandmarkTracker(void)
{
}

void LandmarkTracker::track		( std::vector<TrackedObject> & objectList, ulong frameNumber, cv::Mat& frame ){}
void LandmarkTracker::paint		( cv::Mat& image )
{
	if(_showSelectorRec)
	{
		drawRectangle(image);
	}
	
	if(!_clonedImage)
	{
		_image = image;
		_clonedImage = true;
	}
}


void LandmarkTracker::reset		(){}

//this will draw a basic rectangle onto the diplay image 
void LandmarkTracker::drawRectangle(cv::Mat image)
{
	cv::rectangle( image,
           _selectorRecStart,
           _selectorRecEnd,
           cv::Scalar(0, 0, 255 ),
           1,
           8 );
}

void LandmarkTracker::mouseMoveEvent		( QMouseEvent * e )
{
	if(_showSelectorRec)
		{
			_selectorRecEnd.x = e->x();
			_selectorRecEnd.y = e->y();
			//draw rectangle!
			emit update();
		}
}
void LandmarkTracker::mousePressEvent		( QMouseEvent * e )
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
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
	}
}

void LandmarkTracker::mouseReleaseEvent	( QMouseEvent * e ) 
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
	
	defineROI(_image);
	
}

void LandmarkTracker::mouseWheelEvent ( QWheelEvent *) {}

void LandmarkTracker::defineROI	(cv::Mat image)
{
	//Define ROI
	box.width	= abs(_selectorRecStart.x - _selectorRecEnd.x);
	box.height	= abs(_selectorRecStart.y - _selectorRecEnd.y);
	box.x	= min(_selectorRecStart.x , _selectorRecEnd.x);
	box.y	= min(_selectorRecStart.y , _selectorRecEnd.y);

	std::cout <<"Defined ROI" << std::endl;

	//Make an image out of just the selected ROI and display it in a new window
	cv::Mat roi(image, box);
	cv::namedWindow("ROI");
	cv::imshow("ROI", roi);
}