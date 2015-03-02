#include "LandmarkTracker.h"
#include <QApplication>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "source/tracking/algorithm/algorithms.h"
#include "source/tracking/TrackingThread.h"

namespace {
    auto _ = Algorithms::Registry::getInstance().register_tracker_type<LandmarkTracker>("Landmark Tracker");
}

LandmarkTracker::LandmarkTracker(Settings & settings, QWidget *parent)
	: TrackingAlgorithm(settings, parent)
	, _glwidget(std::make_shared<GLWidget>())
	, _selectorRecStart()
	, _selectorRecEnd()
	, _showSelectorRec(false)
		
{
	//KML
	std::cout<<"LANDMARK TRACKER"<<std::endl;
}

void LandmarkTracker::paint	(cv::Mat& image , const View&)
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
	const bool ctrlModifier = QApplication::keyboardModifiers().testFlag(Qt::ControlModifier);
	const bool _leftMouseHold = QApplication::mouseButtons().testFlag(Qt::LeftButton);
	const bool _rightMouseHold = QApplication::mouseButtons().testFlag(Qt::RightButton);

	//if (_showSelectorRec)
	if (_leftMouseHold)
	{
		_selectorRecEnd.x = e->x();
		_selectorRecEnd.y = e->y();		
		//draw rectangle!
		emit update();
	}

	if (_rightMouseHold && ctrlModifier)
	{
		if (cv::Rect(_selectorRecStart, _selectorRecEnd).contains(cv::Point(e->x(),e->y())))
		{
			std::cout << "IN" << std::endl;

			_mouseStop.x = e->x();
			_mouseStop.y = e->y();

			_selectorRecStart += _mouseStop - _mouseStart;
			_selectorRecEnd += _mouseStop - _mouseStart;

			_showSelectorRec = true;

			emit update();
		}
		else 
		{
			std::cout << "OUT" << std::endl;
		}

		_mouseStart = _mouseStop;

		//std::cout << "mouse pointer: " << e->x() << ":" << e->y() << std::endl;
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
	}

	
	if (e->button() == Qt::RightButton)
	{
		_mouseStart.x = e->x();
		_mouseStart.y = e->y();

		std::cout << "Right mouse pointer: " << e->x() << ":" << e->y() << std::endl;
	}
}

void LandmarkTracker::mouseReleaseEvent	( QMouseEvent * e ) 
{	
	if ( e->button() == Qt::LeftButton)
	{
		
			emit update();
			std::string note = "selected area from " + QString::number(_selectorRecStart.x).toStdString() + ":"+ QString::number(_selectorRecStart.y).toStdString()
				+ " to " +  QString::number(_selectorRecEnd.x).toStdString() + ":"+ QString::number(_selectorRecEnd.y).toStdString();
			emit notifyGUI(note,MSGS::NOTIFICATION);
		

		boost::optional<cv::Mat> img = getCurrentImageCopy();
		if (img) {
			defineROI(img.get());
			//startTool();
		}
	}
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
		//cv::Mat roi(image);
		selectedRoi = roi.clone();
		_glwidget->roiMat = selectedRoi;
	}	
}

void LandmarkTracker::startTool()
{
	std::cout << "Starting CalcMap" << std::endl;
	_glwidget->getRoiCalcMap();
}
/*
void LandmarkTracker::startTool()
{
	if(toolWindow == nullptr)
	{
		toolWindow =new ToolWindow(this);
		toolWindow->initToolWindow();
		toolWindow->show();
	}
	else
	{
		toolWindow->updateToolWindow();
		//cube->updateCubes();
	}
}


void LandmarkTracker::setToolPtr()
{
	toolWindow = nullptr;
}
*/
cv::Mat LandmarkTracker::getSelectedRoi()
{
	return selectedRoi;
}
