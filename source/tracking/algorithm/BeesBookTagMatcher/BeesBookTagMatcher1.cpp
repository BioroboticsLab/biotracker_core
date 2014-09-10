#include "BeesBookTagMatcher.h"
#include "source/helper/StringHelper.h"
#include <QApplication>
#include "source/helper/CvHelper.h"


BeesBookTagMatcher::BeesBookTagMatcher( Settings & settings ) : TrackingAlgorithm( settings )
{
}


BeesBookTagMatcher::~BeesBookTagMatcher(void)
{
}

void BeesBookTagMatcher::track		( std::vector<TrackedObject> & objectList, ulong frameNumber, cv::Mat& frame ){}
void BeesBookTagMatcher::paint		( cv::Mat& image )
{
	if(_showSelectorRec)
	{
		drawRectangle(image);
	}
	else if(_showSelectorCirc)
	{
		drawCircle(image);
	}
	else if(_showSelectorGrid)
	{
		drawGrid(image);
	}
}
void BeesBookTagMatcher::reset		(){}

//this will draw a basic rectangle onto the diplay image 
void BeesBookTagMatcher::drawRectangle(cv::Mat image)
{
	cv::rectangle( image,
           _selectorRecStart,
           _selectorRecEnd,
           _color,
           -1,
           8 );
}
//this will draw a basic circle onto the diplay image 
void BeesBookTagMatcher::drawCircle(cv::Mat image)
{
	int radius;
	cv::Point diff = _selectorRecStart-_selectorRecEnd;
	radius = sqrt(diff.x*diff.x + diff.y*diff.y);

	cv::circle( image,
           _selectorRecStart,
		   radius,
           _color,
           -1,
           8 );
}
//this will draw a basic grid onto the diplay image 
void BeesBookTagMatcher::drawGrid(cv::Mat image)
{
	float size = 8;
	float angle = 160;
	float tilt = 0;
	int x = 34;
	int y = 38;
	decoder::Ellipse ell(1672,Point2i(34,34),Size(24,23),155.56);
	bool permutation = 0;	


	int radius;
	decoder::Grid g(8, 160, 0, 34, 38, decoder::Ellipse(1672,Point2i(34,34),Size(24,23),155.56),0,decoder::Grid::ScoringMethod(0));
	//decoder::Grid g(8, decoder::Grid::ScoringMethod(0));
	cv::Point diff = _selectorRecStart-_selectorRecEnd;
	radius = sqrt(diff.x*diff.x + diff.y*diff.y);
	//cv::Mat image3=g.drawGrid();
	cv::ellipse( image,
           _selectorRecStart,
		   Size(radius,radius),
		   155.1,
		   0,
		   360,
		   _color,
           1,
           8,
		   0);	
}


void BeesBookTagMatcher::mouseMoveEvent		( QMouseEvent * e )
{
	if(_showSelectorRec || _showSelectorCirc || _showSelectorGrid)
		{
			_selectorRecEnd.x = e->x();
			_selectorRecEnd.y = e->y();
			//draw figure!
			emit update();
		}	
}
void BeesBookTagMatcher::mousePressEvent		( QMouseEvent * e )
{
	//check if left button is clicked
	if ( e->button() == Qt::LeftButton)
	{
		//check for shift modifier
		if(Qt::ShiftModifier == QApplication::keyboardModifiers())
		{
			int x = e->x(); int y = e->y();
			std::string note = "shift + left button press on: x=" + StringHelper::iToSS(x) + " y=" + StringHelper::iToSS(y);
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//initialize coordinates for selection tool
			_selectorRecStart.x = e->x();
			_selectorRecStart.y = e->y();
			_selectorRecEnd.x = e->x();
			_selectorRecEnd.y = e->y();
			_color=cv::Scalar(255, 255, 0 );
			_showSelectorRec = true;		
		}
		else if(Qt::ControlModifier == QApplication::keyboardModifiers())
		{
			int x = e->x(); int y = e->y();
			std::string note = "ctrl + left button press on: x=" + StringHelper::iToSS(x) + " y=" + StringHelper::iToSS(y);
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//initialize coordinates for selection tool
			_selectorRecStart.x = e->x();
			_selectorRecStart.y = e->y();
			_selectorRecEnd.x = e->x();
			_selectorRecEnd.y = e->y();
			_color=cv::Scalar(255, 0, 255 );
			//_newRec = true;		
			_showSelectorRec = true;
		}
	}
	else if ( e->button() == Qt::RightButton)
	{
		//check for shift modifier
		if(Qt::ShiftModifier == QApplication::keyboardModifiers())
		{
			int x = e->x(); int y = e->y();
			std::string note = "shift + right button press on: x=" + StringHelper::iToSS(x) + " y=" + StringHelper::iToSS(y);
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//initialize coordinates for selection tool
			_selectorRecStart.x = e->x();
			_selectorRecStart.y = e->y();
			_selectorRecEnd.x = e->x();
			_selectorRecEnd.y = e->y();
			_color=cv::Scalar(255, 255, 0 );
			_showSelectorCirc = true;		
		}
		else if(Qt::ControlModifier == QApplication::keyboardModifiers())
		{
			int x = e->x(); int y = e->y();
			std::string note = "ctrl + right button press on: x=" + StringHelper::iToSS(x) + " y=" + StringHelper::iToSS(y);
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//initialize coordinates for selection tool
			_selectorRecStart.x = e->x();
			_selectorRecStart.y = e->y();
			_selectorRecEnd.x = e->x();
			_selectorRecEnd.y = e->y();
			_color=cv::Scalar(255, 0, 255 );					
			_showSelectorCirc = true;
		}
	}
	else if ( e->button() == Qt::MidButton)
	{		
		if(Qt::ShiftModifier == QApplication::keyboardModifiers())
		{
			int x = e->x(); int y = e->y();
			std::string note = "shift + middle button press on: x=" + StringHelper::iToSS(x) + " y=" + StringHelper::iToSS(y);
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//initialize coordinates for selection tool
			_selectorRecStart.x = e->x();
			_selectorRecStart.y = e->y();
			_selectorRecEnd.x = e->x();
			_selectorRecEnd.y = e->y();
			_color=cv::Scalar(0, 255, 255 );					
			_showSelectorGrid = true;
		}
		else if(Qt::ControlModifier == QApplication::keyboardModifiers())
		{
			int x = e->x(); int y = e->y();
			std::string note = "ctrl + middle button press on: x=" + StringHelper::iToSS(x) + " y=" + StringHelper::iToSS(y);
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//initialize coordinates for selection tool
			_selectorRecStart.x = e->x();
			_selectorRecStart.y = e->y();
			_selectorRecEnd.x = e->x();
			_selectorRecEnd.y = e->y();
			_color=cv::Scalar(0, 255, 255 );					
			_showSelectorGrid = true;
		}
	}
}
void BeesBookTagMatcher::mouseReleaseEvent	( QMouseEvent * e )
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
	else if ( e->button() == Qt::RightButton)
	{
		if(_showSelectorCirc)
		{
			_selectorRecEnd.x = e->x();
			_selectorRecEnd.y = e->y();
			_showSelectorCirc = false;
			//next draw will delete circle!
			emit update();
			std::string note = "selected area from " + StringHelper::iToSS(_selectorRecStart.x) + ":"+ StringHelper::iToSS(_selectorRecStart.y)
				+ " to " +  StringHelper::iToSS(_selectorRecEnd.x) + ":"+ StringHelper::iToSS(_selectorRecEnd.y);
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
	}
	else if ( e->button() == Qt::MidButton)
	{
		if(_showSelectorGrid)
		{
			_selectorRecEnd.x = e->x();
			_selectorRecEnd.y = e->y();
			_showSelectorGrid = false;
			//next draw will delete ellipse!
			emit update();
			std::string note = "selected area from " + StringHelper::iToSS(_selectorRecStart.x) + ":"+ StringHelper::iToSS(_selectorRecStart.y)
				+ " to " +  StringHelper::iToSS(_selectorRecEnd.x) + ":"+ StringHelper::iToSS(_selectorRecEnd.y);
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
	}
}