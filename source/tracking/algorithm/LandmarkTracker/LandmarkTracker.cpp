#include "LandmarkTracker.h"
#include <QApplication>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "source/tracking/algorithm/algorithms.h"
#include "source/tracking/TrackingThread.h"

namespace {
    auto _ = Algorithms::Registry::getInstance().register_tracker_type<LandmarkTracker>("Landmark Tracker");
}

LandmarkTracker::LandmarkTracker(Settings & settings, std::string &serializationPathName,  QWidget *parent)
    : TrackingAlgorithm( settings, serializationPathName, parent )
    , toolWindow(nullptr)
{
	_showSelectorRec = false;
	_selectorRecStart = cv::Point();
	_selectorRecEnd = cv::Point();

	//KML
	std::cout<<"LANDMARK TRACKER"<<std::endl;
}

void LandmarkTracker::paint		( cv::Mat& image )
{
	if(_showSelectorRec)
	{
		drawRectangle(image);
	}

}

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
		//initialize coordinates for selection tool
		_selectorRecStart.x = e->x();
		_selectorRecStart.y = e->y();
		_selectorRecEnd.x = e->x();
		_selectorRecEnd.y = e->y();
		_showSelectorRec = true;	
		//emit notifyGUI(note,MSGS::NOTIFICATION);
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
			std::string note = "selected area from " + QString::number(_selectorRecStart.x).toStdString() + ":"+ QString::number(_selectorRecStart.y).toStdString()
				+ " to " +  QString::number(_selectorRecEnd.x).toStdString() + ":"+ QString::number(_selectorRecEnd.y).toStdString();
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
	}

	defineROI(emit requestCurrentScreen());
	startTool();
}

void LandmarkTracker::defineROI	(cv::Mat image)
{
	//Define ROI
	box.width	= abs(_selectorRecStart.x - _selectorRecEnd.x);
	box.height	= abs(_selectorRecStart.y - _selectorRecEnd.y);
	box.x	= std::min(_selectorRecStart.x , _selectorRecEnd.x);
	box.y	= std::min(_selectorRecStart.y , _selectorRecEnd.y);

	std::cout <<"Defined ROI" << std::endl;

	//Make an image out of just the selected ROI and display it in a new window
	if(box.width > 0 && box.height > 0){

		cv::Mat roi(image, box);
		selectedRoi = roi.clone();
	}
}

void LandmarkTracker::startTool()
{
	if(toolWindow == nullptr)
	{
		toolWindow =new ToolWindow(this);
		toolWindow->initToolWindow();
		toolWindow->show();
	}
}

void LandmarkTracker::setToolPtr()
{
	toolWindow = nullptr;
}

cv::Mat LandmarkTracker::getSelectedRoi()
{
	return selectedRoi;
}
