#include "BeesBookTagMatcher.h"
#include "source/helper/StringHelper.h"
#include <QApplication>

//some variables can go out of boundary


BeesBookTagMatcher::BeesBookTagMatcher( Settings & settings ) : TrackingAlgorithm( settings )
{
	_newGrid			= true;
	_activeGrid			= false;
	_activeTag			= false;

	_modPosGrid			= false;
	_modHeightMid		= false;
	_modWidthMid		= false;
	_modHeightInn		= false;
	_modWidthInn		= false;
}


BeesBookTagMatcher::~BeesBookTagMatcher(void)
{
}

void BeesBookTagMatcher::track		( std::vector<TrackedObject> & objectList, ulong frameNumber, cv::Mat& frame ){}
void BeesBookTagMatcher::paint		( cv::Mat& image )
{
	if(_activeGrid || _activeTag)
	{
		drawGrid(image);		
	}
}
void BeesBookTagMatcher::reset		(){}

//this draws a basic grid onto the display image
void BeesBookTagMatcher::drawGrid(cv::Mat image)
{
	if (_activeGrid)
	{	
		gNew.drawModGrid(image);
	}
	if (_activeTag)
	{
		gNew.drawFullTag(image,1);
	}
}

//check if MOUSE BUTTON IS CLICKED
void BeesBookTagMatcher::mousePressEvent		( QMouseEvent * e )
{
	//check if LEFT button is clicked
	if ( e->button() == Qt::LeftButton && _activeGrid)
	{
		//check for SHIFT modifier
		if(Qt::ShiftModifier == QApplication::keyboardModifiers()) //-modifies Grid's position-
		{		
			if (abs(e->x()- gNew.centerGrid.x) < 2 && abs(e->y()- gNew.centerGrid.y) < 2)
			{				
				//a differente tag is selected when clicking inside its area
			/*for (int i=0; i<_Grids.size();i++)
				{
					if (cv::pointPolygonTest(_Grids[i].cellsContours[12],cv::Point(e->x(),e->y()),false)>0)
					{
						_Grids.push_back(g);
						g.init(_Grids[i].center, _Grids[i].axes, _Grids[i].angleTag, _Grids[i].angleGrid, _Grids[i].ID);
						_Grids.erase(_Grids.begin()+i);
						break;
					}
				}	*/				
				_modPosGrid = true;
				emit update();
			}
			else if(abs(e->x()- gNew.gridAxes[0][1].x) < 2 && abs(e->y()- gNew.gridAxes[0][1].y) < 2)
			{				
				_modHeightMid = true;
			}
			else if(abs(e->x()- gNew.gridAxes[1][1].x) < 2 && abs(e->y()- gNew.gridAxes[1][1].y) < 2)
			{			
				_modWidthMid = true;
			}
			else if(abs(e->x()- gNew.gridAxes[2][1].x) < 2 && abs(e->y()- gNew.gridAxes[2][1].y) < 2)
			{			
				_modHeightInn = true;
			}
			else if(abs(e->x()- gNew.gridAxes[3][1].x) < 2 && abs(e->y()- gNew.gridAxes[3][1].y) < 2)
			{							
				_modWidthInn = true;
			}

		}
		//check for CTRL modifier
		else if(Qt::ControlModifier == QApplication::keyboardModifiers())
		{				
		}
		//check for ALT modifier
		else if(Qt::AltModifier == QApplication::keyboardModifiers())
		{
		}	
	}

	//check if RIGHT button is clicked
	else if ( e->button() == Qt::RightButton)
	{
		//check for SHIFT modifier
		if(Qt::ShiftModifier == QApplication::keyboardModifiers())		
		{		
		}
		//check for CTRL modifier		
		else if(Qt::ControlModifier == QApplication::keyboardModifiers())
		{
			if (_newGrid && !_activeGrid)
			{				
				_newGrid = false;
				_activeGrid = true;
				gNew.init(cv::Point (e->x(),e->y()), cv::Size (axisTag*(MR/OR),axisTag*(MR/OR)), 0, 
					cv::Point (e->x(),e->y()), cv::Size (axisTag,axisTag), 0, std::vector<bool> (12,0));
				emit update();
			}
			else if (_activeGrid)
			{				
				_activeGrid = false;
				_activeTag = true;				
				emit update();
			}
		}
		//check for ALT modifier
		else if(Qt::AltModifier == QApplication::keyboardModifiers())
		{
		}
	}
		
	//check if MIDDLE button is clicked
	else if ( e->button() == Qt::MiddleButton)
	{
		if (_activeTag)
		{
			_activeTag = false;
			_activeGrid = true;
			emit update();
		}
	}
}

//check if pointer MOVES
void BeesBookTagMatcher::mouseMoveEvent		( QMouseEvent * e )
{
	if (_modPosGrid) // "ALT + LEFT button + MOVE" modifies grid center
	{
		gNew.centerGrid.x = e->x();
		gNew.centerGrid.y = e->y();
		//centerTag is also updated
		gNew.centerTag.x = e->x();
		gNew.centerTag.y = e->y();
		//draw figure!
		emit update();
	}
	else if (_modHeightMid) // "ALT + LEFT button + MOVE" modifies "height middle axis" length
	{
		//new length
		gNew.axesGrid.height = dist(gNew.centerGrid,cv::Point(e->x(),e->y()));
		//axesTag is also updated
		gNew.axesTag.width = gNew.axesGrid.height * OR/MR;
		//new angle
		double angle = atan2(e->y() - gNew.centerGrid.y, e->x() - gNew.centerGrid.x) * 180.0 / CV_PI;			
		gNew.angleGrid = angle+90;
		//angleTag is also updated
		gNew.angleTag = angle+90;
	
		//draw figure!
		emit update();
	}
	else if (_modWidthMid) // "ALT + LEFT button + MOVE" modifies "width middle axis" length
	{
		//new length
		gNew.axesGrid.width = dist(gNew.centerGrid,cv::Point(e->x(),e->y()));
		//axesTag is also updated
		gNew.axesTag.width = gNew.axesGrid.width * OR/MR;
		//new angle
		double angle = atan2(e->y() - gNew.centerGrid.y, e->x() - gNew.centerGrid.x) * 180.0 / CV_PI;		
		gNew.angleGrid = angle;
		//angleTag is also updated
		gNew.angleTag = angle;
		//draw figure!
		emit update();
	}
	else if (_modHeightInn) // "ALT + LEFT button + MOVE" modifies "height inner axis" length
	{
		//new length
		gNew.axesGrid.height = dist(gNew.centerGrid,cv::Point(e->x(),e->y()))*MR/IR;
		//axesTag is also updated
		gNew.axesTag.width = gNew.axesGrid.height * OR/MR;
		//new angle
		double angle = atan2(e->y() - gNew.centerGrid.y, e->x() - gNew.centerGrid.x) * 180.0 / CV_PI;		
		gNew.angleGrid = angle+90;
		//angleTag is also updated
		gNew.angleTag = angle+90;
		//draw figure!
		emit update();
	}
	else if (_modWidthInn) // "ALT + LEFT button + MOVE" modifies "width inner axis" length
	{
		//new length
		gNew.axesGrid.width = dist(gNew.centerGrid,cv::Point(e->x(),e->y()))*MR/IR;
		//axesTag is also updated
		gNew.axesTag.width = gNew.axesGrid.width * OR/MR;
		//new angle
		double angle = atan2(e->y() - gNew.centerGrid.y, e->x() - gNew.centerGrid.x) * 180.0 / CV_PI;		
		gNew.angleGrid = angle;
		//angleTag is also updated
		gNew.angleTag = angle;
		//draw figure!
		emit update();
	}
}

//check if MOUSE BUTTON IS RELEASED
void BeesBookTagMatcher::mouseReleaseEvent	( QMouseEvent * e )
{
	// left button released
	if(e->button() == Qt::LeftButton && _modPosGrid)
	{				
		_modPosGrid = false;		
	}
	else if(e->button() == Qt::LeftButton && _modHeightMid)
	{				
		_modHeightMid = false;		
	}
	else if(e->button() == Qt::LeftButton && _modWidthMid)
	{				
		_modWidthMid = false;		
	}
	else if(e->button() == Qt::LeftButton && _modHeightInn)
	{				
		_modHeightInn = false;		
	}
	else if(e->button() == Qt::LeftButton && _modWidthInn)
	{				
		_modWidthInn = false;		
	}
	// right button released
	else if (e->button() == Qt::RightButton && _newGrid)
	{
		//_newGrid=false;

	}
}

double BeesBookTagMatcher::dist(cv::Point p1, cv::Point p2)
{
	double dist;
	diff = p1-p2;
	dist = sqrt(diff.x*diff.x + diff.y*diff.y);
	return dist;
}