#include "BeesBookTagMatcher.h"
#include "source/helper/StringHelper.h"
#include <QApplication>
#include "source/helper/CvHelper.h"

//some variables can go out of boundary

BeesBookTagMatcher::BeesBookTagMatcher( Settings & settings ) : TrackingAlgorithm( settings )
{
	_newGrid			= false;
	_activeGrid			= false;	
	_modPosGrid			= false;
	_modSizeGrid		= false;		
	_modAngleTag		= false;
	_modAngleGrid		= false;
	_modTiltGrid		= false;
}


BeesBookTagMatcher::~BeesBookTagMatcher(void)
{
}

void BeesBookTagMatcher::track		( std::vector<TrackedObject> & objectList, ulong frameNumber, cv::Mat& frame ){}
void BeesBookTagMatcher::paint		( cv::Mat& image )
{	
	if(_modSizeGrid || _newGrid || _activeGrid || _modPosGrid || _modTiltGrid || _modAngleTag)
	{
		drawGrid(image);
	}
}
void BeesBookTagMatcher::reset		(){}

//this draws a basic grid onto the display image
void BeesBookTagMatcher::drawGrid(cv::Mat image)
{
	//for loop to draw the Grids tored in _Grids vector.
	if (_Grids.size()>0)
	{
		for (size_t i=0; i<_Grids.size(); i++)
		{
			gtemp.init(_Grids[i].center, _Grids[i].axes, _Grids[i].angleTag, _Grids[i].angleGrid, _Grids[i].ID);
			gtemp.drawGrid(image,0);		
		}
	}
	//The current Grid is drawn
	g.drawGrid(image,1);
}

//check if MOUSE BUTTON IS CLICKED
void BeesBookTagMatcher::mousePressEvent		( QMouseEvent * e )
{
	//check if LEFT button is clicked
	if ( e->button() == Qt::LeftButton && _activeGrid)
	{
		//check for SHIFT modifier -modifies Grid's Size-
		if(Qt::ShiftModifier == QApplication::keyboardModifiers())
		{			
			std::string note = "shift + left button pressed on: x=" + StringHelper::iToSS(e->x()) + " y=" + StringHelper::iToSS(e->y());
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//initialize coordinates for selection tool				
			_modSizeGrid = true;
		}
		//check for CTRL modifier -modifies ellipse angle-
		else if(Qt::ControlModifier == QApplication::keyboardModifiers())
		{			
			std::string note = "control + left button pressed on: x=" + StringHelper::iToSS(e->x()) + " y=" + StringHelper::iToSS(e->y());
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//load the previous position of the pointer
			prevPosition.x = e->x();
			prevPosition.y = e->y();			
			_modAngleTag = true;	
		}
		//check for ALT modifier, hold LEFT click drags the ellipse -modifies Grid's position-
		else if(Qt::AltModifier == QApplication::keyboardModifiers())
		{			
			std::string note = "left button pressed on: x=" + StringHelper::iToSS(e->x()) + " y=" + StringHelper::iToSS(e->y());
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//a differente tag is selected when clicking inside its area
			for (int i=0; i<_Grids.size();i++)
				{
					if (cv::pointPolygonTest(_Grids[i].cellsContours[12],cv::Point(e->x(),e->y()),false)>0)
					{
						_Grids.push_back(g);
						g.init(_Grids[i].center, _Grids[i].axes, _Grids[i].angleTag, _Grids[i].angleGrid, _Grids[i].ID);
						_Grids.erase(_Grids.begin()+i);
						break;
					}
				}	
			_modPosGrid = true;
			emit update();
		}
	}

	//check if RIGHT button is clicked
	else if ( e->button() == Qt::RightButton)
	{
		//check for CTRL modifier
		if(Qt::ControlModifier == QApplication::keyboardModifiers())
		{
			//a new grid, the provious one is pushed into _Grids vector and a new one is generated
			if (_activeGrid)
			{				
				_Grids.push_back(g);
				//propierties of pushed grid are printed
				std::string note = "Tag number: " + StringHelper::iToSS(_Grids.size()) + 
					"center: (" + StringHelper::iToSS(g.center.x) + "," + StringHelper::iToSS(g.center.y) + ") " +
					" mayor axis: " + StringHelper::iToSS(g.axes.width) + " minor axis: " + StringHelper::iToSS(g.axes.height) +
					" angle: " + StringHelper::iToSS(g.angleTag);
				emit notifyGUI(note,MSGS::NOTIFICATION);				
				// a new Tag				
				_newGrid = true;
				_activeGrid = false;
				g.init(cv::Point(e->x(),e->y()), cv::Size(axisTag,axisTag), 0, 0, std::vector<bool> (12,0));
				emit update();				
			}
			//a new grid, first grid
			else
			{
				std::string note = "first new tag on: x=" + StringHelper::iToSS(e->x()) + " y=" + StringHelper::iToSS(e->y());
				emit notifyGUI(note,MSGS::NOTIFICATION);
				// a new Tag				
				_newGrid = true;			
				g.init(cv::Point(e->x(),e->y()), cv::Size(axisTag,axisTag), 0, 0, std::vector<bool> (12,0));
				emit update();
			}
		}
		//check for SHIFT modifier -modifies Grid's tilt-
		else if(Qt::ShiftModifier == QApplication::keyboardModifiers() && _activeGrid)
		{	
			std::string note = "shift + right button pressed on: x=" + StringHelper::iToSS(e->x()) + " y=" + StringHelper::iToSS(e->y());
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//initialize coordinates for selection tool				
			prevPosition.x = e->x();
			prevPosition.y = e->y();							
			_modTiltGrid = true;			
		}
		//check for ALT modifier -modifies Grid's angle
		else if(Qt::AltModifier == QApplication::keyboardModifiers())
		{			
			std::string note = "alt + right button pressed on: x=" + StringHelper::iToSS(e->x()) + " y=" + StringHelper::iToSS(e->y());
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//load the previous position of the pointer
			prevPosition.x = e->x();
			prevPosition.y = e->y();			
			_modAngleGrid = true;	
		}
	}
	//check if MIDDLE button is clicked
	else if ( e->button() == Qt::MiddleButton && _activeGrid)
	{
		//modify bit value
		//check if the pointer is inside one of the cellsContours
		for (int i=0; i<g.cellsContours.size();i++)
		{			
			if (cv::pointPolygonTest(g.cellsContours[i],cv::Point(e->x(),e->y()),false)>0)
			{
				g.ID[i]	= !g.ID[i];
				cout<<endl;
				for(int i2=0; i2 < 12; i2++)
					cout<<g.ID[i2];
				emit update();
			}
		}		
	}
}

//check if pointer MOVES
void BeesBookTagMatcher::mouseMoveEvent		( QMouseEvent * e )
{
	if (_modPosGrid) // "ALT + LEFT button + MOVE" drags the ellipse
	{
		g.center.x = e->x();
		g.center.y = e->y();
		//draw figure!
		emit update();
	}
	else if(_modSizeGrid) // "SHIFT + LEFT button + MOVE" modifies size of the ellipse
	{
		_endGrid.x = e->x();
		_endGrid.y = e->y();
		diff = g.center-_endGrid;
		g.axes.width = sqrt(diff.x*diff.x + diff.y*diff.y);
		g.axes.height = g.axes.width * g.tilt;
		//draw figure!
		emit update();
	}
	else if (_modAngleTag) // "CTRL + LEFT button + MOVE"  rotates the ellipse
	{	
		//drag in right or down direction = clockwise rotation
		if (e->x() - prevPosition.x > 0 || e->y() - prevPosition.y > 0)
			g.angleTag +=2;
		//drag in left or up direction = counter clockwise rotation
		else if (e->x() - prevPosition.x < 0 || e->y() - prevPosition.y < 0)
			g.angleTag -=2;
		//update pointer position
		prevPosition.x = e->x();
		prevPosition.y = e->y();
		//draw figure!
		emit update();
	}
	else if (_modAngleGrid) // "ALT + RIGHT button + MOVE"  rotates the grid
	{	
		//drag in right or down direction = clockwise rotation
		if (e->x() - prevPosition.x > 0 || e->y() - prevPosition.y > 0)
			g.angleGrid +=2;
		//drag in left or up direction = counter clockwise rotation
		else if (e->x() - prevPosition.x < 0 || e->y() - prevPosition.y < 0)
			g.angleGrid -=2;
		//update pointer position
		prevPosition.x = e->x();
		prevPosition.y = e->y();
		//draw figure!
		emit update();
	}
	else if (_modTiltGrid) // "SHIFT + RIGHT click + move" tilts the ellipse
	{	
		if (e->x() - prevPosition.x > 0) //drag in right direction increases _axis1
			g.axes.width += 2;
		else if (e->x() - prevPosition.x < 0) //drag in left direction decreases _axis1
			g.axes.width = abs(g.axes.width-2);
		else if (e->y() - prevPosition.y < 0) //drag in up direction increases _axis2
			g.axes.height += 2;
		else if (e->y() - prevPosition.y > 0) //drag in down direction decreases _axis2
			g.axes.height = abs(g.axes.height-2);
		//update pointer position
		prevPosition.x = e->x();
		prevPosition.y = e->y();
		//draw figure!
		emit update();	
	}	
	
}

//check if MOUSE BUTTON IS RELEASED
void BeesBookTagMatcher::mouseReleaseEvent	( QMouseEvent * e )
{
	if ( e->button() == Qt::LeftButton)
	{
		if(_modSizeGrid) 
		{			
			_modSizeGrid = false;			
			std::string note = "Left button released, center of the Grid: (" + StringHelper::iToSS(g.center.x) + ","+ StringHelper::iToSS(g.center.y) + ")";
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
		else if(_modAngleTag) //deactivates ANGLE modification
		{			
			g.angleTag = int(g.angleTag)%360;
			_modAngleTag = false;						
			std::string note = "Left button released, center of the Grid: (" + StringHelper::iToSS(g.center.x) + ","+ StringHelper::iToSS(g.center.y) + ")" 
				+ " angle " + StringHelper::iToSS(g.angleTag);			
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}		
		else if(_modPosGrid)
		{				
			_modPosGrid = false;			
			std::string note = "Left button released, center of the Grid: (" + StringHelper::iToSS(g.center.x) + ","+ StringHelper::iToSS(g.center.y) + ")";
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
	}
	else if ( e->button() == Qt::RightButton)
	{
		if(_newGrid)	//NEW Grid set
		{			
			_newGrid = false;
			_activeGrid = true;			
			std::string note = "new active Grid on: (" + StringHelper::iToSS(g.center.x) + ","+ StringHelper::iToSS(g.center.y) + ")";			
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
		else if(_modAngleGrid) //deactivates ANGLE modification
		{			
			g.angleGrid = int(g.angleGrid)%360;
			_modAngleGrid = false;						
			std::string note = "Left button released, center of the Grid: (" + StringHelper::iToSS(g.center.x) + ","+ StringHelper::iToSS(g.center.y) + ")" 
				+ " angle " + StringHelper::iToSS(g.angleGrid);			
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
		else if(_modTiltGrid)
		{			
			g.getTilt(); //_tilt value is updated at the end of the modification
			_modTiltGrid = false;			
			std::string note = "Rigth button released, center of the Grid: (" + StringHelper::iToSS(g.center.x) + ","+ StringHelper::iToSS(g.center.y) + ")" + 
				" Tilt " + QString::number(g.tilt).toStdString();
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
	}
}