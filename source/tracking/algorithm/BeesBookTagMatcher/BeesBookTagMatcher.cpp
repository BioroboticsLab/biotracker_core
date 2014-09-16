#include "BeesBookTagMatcher.h"
#include "source/helper/StringHelper.h"
#include <QApplication>
#include "source/helper/CvHelper.h"

//some variables can go out of boundary

BeesBookTagMatcher::BeesBookTagMatcher( Settings & settings ) : TrackingAlgorithm( settings )
{
	_newGrid			= false;
	_activeGrid			= false;	
	_modPosGrid = false;
	_modSizeGrid		= false;		
	_modAngleXY		= false;
	_modTiltGrid		= false;
}


BeesBookTagMatcher::~BeesBookTagMatcher(void)
{
}

void BeesBookTagMatcher::track		( std::vector<TrackedObject> & objectList, ulong frameNumber, cv::Mat& frame ){}
void BeesBookTagMatcher::paint		( cv::Mat& image )
{	
	if(_modSizeGrid || _newGrid || _activeGrid || _modPosGrid || _modTiltGrid || _modAngleXY)
	{
		drawGrid(image);
	}
}
void BeesBookTagMatcher::reset		(){}

//this will draw a basic grid onto the display image
//at the moment is drawing an ellipse since I haven't been able to use the Grid object
void BeesBookTagMatcher::drawGrid(cv::Mat image)
{
	//float size = 8;
	//float angle = 160;
	//float tilt = 0;
	//int x = 34;
	//int y = 38;
	//decoder::Ellipse ell(1672,Point2i(34,34),Size(24,23),155.56);
	//bool permutation = 0;	
	//	
	//decoder::Grid g(8, 160, 0, 34, 38, decoder::Ellipse(1672,Point2i(34,34),Size(24,23),155.56),0,decoder::Grid::ScoringMethod(0));	
	//cv::Mat image3=g.drawGrid();

	//for loop to draw the Grids tored in _Grids vector.
	if (_Grids.size()>0)
	{		
		for (size_t i=0; i<_Grids.size(); i++)
		{
			cv::ellipse(image, _Grids[i].center,_Grids[i].axes,_Grids[i].angle,0,360,Scalar(255, 0, 0 ),2,8,0);
			std::string note = "iterating " + StringHelper::iToSS(i) + 
				" center: (" + StringHelper::iToSS(_Grids[i].center.x) + "," + StringHelper::iToSS(_Grids[i].center.y) + ") " +
				" mayor axis: " + StringHelper::iToSS(_Grids[i].axes.width) + " minor axis: " + StringHelper::iToSS(_Grids[i].axes.height)
				+ " angle: " + StringHelper::iToSS(_Grids[i].angle);
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
	}
	
	//A new Grid with default parameters is drawn
	if (_modSizeGrid || _newGrid || _modPosGrid || _modTiltGrid || _modAngleXY)
	{
		cv::ellipse( image, _centerGrid, Size(_axis1,_axis2), _angle, 0, 360, _color, 2, 8, 0);
	}	
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
			_color=cv::Scalar(255, 255, 0 );					
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
			_color=cv::Scalar(255, 255, 0 );
			_modAngleXY = true;	
		}
		//hold LEFT click drags the ellipse -modifies Grid's position-
		else if(Qt::AltModifier == QApplication::keyboardModifiers())
		{			
			std::string note = "left button pressed on: x=" + StringHelper::iToSS(e->x()) + " y=" + StringHelper::iToSS(e->y());
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//initialize coordinates for selection tool					
			_color=cv::Scalar(255, 255, 0 );					
			_modPosGrid = true;
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
				_Grids.push_back(myGrid(_centerGrid,cv::Size(_axis1,_axis2),_angle));
				//propierties of pushed grid are printed
				std::string note = "Tag number: " + StringHelper::iToSS(_Grids.size()) + 
					"center: (" + StringHelper::iToSS(_centerGrid.x) + "," + StringHelper::iToSS(_centerGrid.y) + ") " +
					" mayor axis: " + StringHelper::iToSS(_axis1) + " minor axis: " + StringHelper::iToSS(_axis2) +
					" angle: " + StringHelper::iToSS(_angle);
				emit notifyGUI(note,MSGS::NOTIFICATION);
				//initialize coordinates for selection tool
				_centerGrid.x = e->x();
				_centerGrid.y = e->y();			
				_angle = 0;
				_tilt = 1;				
				_axis1 = 50;
				_axis2 = _axis1 * _tilt;
				_color=cv::Scalar(255, 0, 255 );
				_newGrid = true;
				_activeGrid = false;
				emit update();				
			}
			//a new grid, first grid
			else
			{
				std::string note = "first new tag on: x=" + StringHelper::iToSS(e->x()) + " y=" + StringHelper::iToSS(e->y());
				emit notifyGUI(note,MSGS::NOTIFICATION);
				//initialize coordinates for selection tool
				_centerGrid.x = e->x();
				_centerGrid.y = e->y();
				_angle = 0;
				_tilt = 1;		
				_axis1 = 50;
				_axis2 = _axis1 * _tilt;
				_color=cv::Scalar(255, 0, 255 );
				_newGrid = true;
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
			_color=cv::Scalar(255, 255, 0 );					
			_modTiltGrid = true;			
		}
	}	
}

//check if pointer MOVES
void BeesBookTagMatcher::mouseMoveEvent		( QMouseEvent * e )
{
	if (_modPosGrid) // "HOLD LEFT click" drags the ellipse
	{
		_centerGrid.x = e->x();
		_centerGrid.y = e->y();
		//draw figure!
		emit update();
	}
	else if(_modSizeGrid) // "SHIFT + LEFT button + MOVE" modifies size of the ellipse
	{
		_endGrid.x = e->x();
		_endGrid.y = e->y();
		diff = _centerGrid-_endGrid;
		_axis1 = sqrt(diff.x*diff.x + diff.y*diff.y);
		_axis2 = _axis1 * _tilt;
		//draw figure!
		emit update();
	}
	else if (_modAngleXY) // "CTRL + LEFT button + MOVE"  rotates the ellipse
	{	
		//drag in right or down direction = clockwise rotation
		if (e->x() - prevPosition.x > 0 || e->y() - prevPosition.y > 0)
			_angle +=2;
		//drag in left or up direction = counter clockwise rotation
		else if (e->x() - prevPosition.x < 0 || e->y() - prevPosition.y < 0)
			_angle -=2;
		//update pointer position
		prevPosition.x = e->x();
		prevPosition.y = e->y();
		//draw figure!
		emit update();
	}
	else if (_modTiltGrid) // "SHIFT + RIGHT click + move" tilts the ellipse
	{	
		if (e->x() - prevPosition.x > 0) //drag in right direction increases _axis1
			_axis1 += 2;
		else if (e->x() - prevPosition.x < 0) //drag in left direction decreases _axis1
			_axis1 = abs(_axis1-2);
		else if (e->y() - prevPosition.y < 0) //drag in up direction increases _axis2
			_axis2 += 2;
		else if (e->y() - prevPosition.y > 0) //drag in down direction decreases _axis2
			_axis2 = abs(_axis2-2);
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
			std::string note = "Left button released, center of the Grid: (" + StringHelper::iToSS(_centerGrid.x) + ","+ StringHelper::iToSS(_centerGrid.y) + ")";
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
		else if(_modAngleXY) //deactivates ANGLE modification
		{			
			_angle = int(_angle)%360;
			_modAngleXY = false;						
			std::string note = "Left button released, center of the Grid: (" + StringHelper::iToSS(_centerGrid.x) + ","+ StringHelper::iToSS(_centerGrid.y) + ")" 
				+ " angle " + StringHelper::iToSS(_angle);			
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}		
		else if(_modPosGrid)
		{			
			_modPosGrid = false;			
			std::string note = "Left button released, center of the Grid: (" + StringHelper::iToSS(_centerGrid.x) + ","+ StringHelper::iToSS(_centerGrid.y) + ")";
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
	}
	else if ( e->button() == Qt::RightButton)
	{
		if(_newGrid)	//NEW Grid set
		{			
			_newGrid = false;
			_activeGrid = true;			
			std::string note = "new active Grid on: (" + StringHelper::iToSS(_centerGrid.x) + ","+ StringHelper::iToSS(_centerGrid.y) + ")";
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
		else if(_modTiltGrid)
		{			
			_tilt = double(_axis2)/_axis1; //_tilt value is calculated at the end of the modification
			_modTiltGrid = false;			
			std::string note = "Rigth button released, center of the Grid: (" + StringHelper::iToSS(_centerGrid.x) + ","+ StringHelper::iToSS(_centerGrid.y) + ")" + 
				" Tilt " + QString::number(_tilt).toStdString();
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
	}
}