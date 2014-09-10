#include "BeesBookTagMatcher.h"
#include "source/helper/StringHelper.h"
#include <QApplication>
#include "source/helper/CvHelper.h"

BeesBookTagMatcher::BeesBookTagMatcher( Settings & settings ) : TrackingAlgorithm( settings )
{
	_activeGrid			= false;
	_newGrid			= false;
	_modifyPositionGrid = false;
	_modifySizeGrid		= false;		
	_modifyAngleXY		= false;
	_modifyTiltGrid		= false;
	
	_tilt				= 1; // _axis1 and _axis2 have the same length for all new tag
	_angle				= 0; //
}


BeesBookTagMatcher::~BeesBookTagMatcher(void)
{
}

void BeesBookTagMatcher::track		( std::vector<TrackedObject> & objectList, ulong frameNumber, cv::Mat& frame ){}
void BeesBookTagMatcher::paint		( cv::Mat& image )
{	
	if(_modifySizeGrid || _newGrid || _activeGrid || _modifyPositionGrid || _modifyTiltGrid || _modifyAngleXY)
	{
		drawGrid(image);
	}
}
void BeesBookTagMatcher::reset		(){}

//this will draw a basic grid onto the display image 
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

	//Here only enters when the vector is not empty
	if (_Grids.size()>0)
	{		
		for (int i=0; i<_Grids.size(); i++)
		{
			cv::ellipse(image, _Grids[i].center,_Grids[i].axes,_angle,0,360,Scalar(255, 0, 0 ),2,8,0);
			std::string note = "iterating " + StringHelper::iToSS(i) + 
				" center: (" + StringHelper::iToSS(_Grids[i].center.x) + "," + StringHelper::iToSS(_Grids[i].center.y) + ") " +
				"mayor axis: " + StringHelper::iToSS(_Grids[i].axes.width) + "minor axis: " + StringHelper::iToSS(_Grids[i].axes.height);
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
	}
	//raius is initialized to 50 when it is a new grid
	if (_modifySizeGrid || _newGrid || _modifyPositionGrid || _modifyTiltGrid || _modifyAngleXY)
	{
		cv::ellipse( image, _centerGrid, Size(_axis1,_axis2), _angle, 0, 360, _color, 2, 8, 0);
	}	
}

void BeesBookTagMatcher::mousePressEvent		( QMouseEvent * e )
{
	//check if left button is clicked
	if ( e->button() == Qt::LeftButton && _activeGrid)
	{
		//check for shift modifier
		if(Qt::ShiftModifier == QApplication::keyboardModifiers())
		{			
			std::string note = "shift + left button press on: x=" + StringHelper::iToSS(e->x()) + " y=" + StringHelper::iToSS(e->y());
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//initialize coordinates for selection tool						
			_color=cv::Scalar(255, 255, 0 );					
			_modifySizeGrid = true;
		}
		else if(Qt::ControlModifier == QApplication::keyboardModifiers())
		{			
			std::string note = "control + left button press on: x=" + StringHelper::iToSS(e->x()) + " y=" + StringHelper::iToSS(e->y());
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//initialize coordinates for selection tool					
			prevPosition.x = e->x();
			prevPosition.y = e->y();						
			_color=cv::Scalar(255, 255, 0 );					
			_modifyTiltGrid = true;
		}
		else
		{			
			std::string note = "shift + left button press on: x=" + StringHelper::iToSS(e->x()) + " y=" + StringHelper::iToSS(e->y());
			emit notifyGUI(note,MSGS::NOTIFICATION);
			//initialize coordinates for selection tool					
			_color=cv::Scalar(255, 255, 0 );					
			_modifyPositionGrid = true;
		}
	}
	//check if right button is clicked
	else if ( e->button() == Qt::RightButton)
	{
		//check for ctrl modifier
		if(Qt::ControlModifier == QApplication::keyboardModifiers())
		{
			//a new grid, the provious one is pushed in the vector and a new one is generated
			if (_activeGrid)
			{
				_Grids.push_back(myGrid(_centerGrid,cv::Size(_axis1,_axis2)));
				std::string note = "Tag number: " + StringHelper::iToSS(_Grids.size()) + " on: x=" + 
					StringHelper::iToSS(_centerGrid.x) + " y=" + StringHelper::iToSS(_centerGrid.y);
				emit notifyGUI(note,MSGS::NOTIFICATION);
				//initialize coordinates for selection tool
				_centerGrid.x = e->x();
				_centerGrid.y = e->y();			
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
				_tilt = 1;		
				_axis1 = 50;
				_axis2 = _axis1 * _tilt;
				_color=cv::Scalar(255, 0, 255 );
				_newGrid = true;
				emit update();
			}
		}
		if(Qt::ShiftModifier == QApplication::keyboardModifiers())
		{
			//rotation of the ellipse in XY
			if (_activeGrid)
			{				
				std::string note = "Tag number: " + StringHelper::iToSS(_Grids.size()) + " on: x=" + 
					StringHelper::iToSS(_centerGrid.x) + " y=" + StringHelper::iToSS(_centerGrid.y);
				emit notifyGUI(note,MSGS::NOTIFICATION);
				//load the previous position of the pointer
				prevPosition.x = e->x();
				prevPosition.y = e->y();						
				_color=cv::Scalar(255, 255, 0 );
				_modifyAngleXY = true;				
			}
		}
	}	
}
void BeesBookTagMatcher::mouseMoveEvent		( QMouseEvent * e )
{
	if(_modifySizeGrid)
	{
		_endGrid.x = e->x();
		_endGrid.y = e->y();
		diff = _centerGrid-_endGrid;
		_axis1 = sqrt(diff.x*diff.x + diff.y*diff.y);
		_axis2 = _axis1 * _tilt;
		//draw figure!
		emit update();
	}
	else if (_modifyTiltGrid)
	{		
		_tilt = (prevPosition.x-e->x())/_axis1;
		_axis2 = _axis1 * _tilt;
		//draw figure!
		emit update();
	}
	else if (_modifyPositionGrid)
	{
		_centerGrid.x = e->x();
		_centerGrid.y = e->y();
		//draw figure!
		emit update();
	}
	else if (_modifyAngleXY)
	{		
		if (prevPosition.y - (e->y()) > 0)
			_angle -=2;
		else if (prevPosition.y - (e->y()) < 0)
			_angle +=2;
		//draw figure!
		emit update();
	}
}
void BeesBookTagMatcher::mouseReleaseEvent	( QMouseEvent * e )
{
	if ( e->button() == Qt::LeftButton)
	{
		if(_modifySizeGrid)
		{			
			_modifySizeGrid = false;
			//next draw will delete rectangle!
			//emit update();
			std::string note = "Left button released, center of the Grid: (" + StringHelper::iToSS(_centerGrid.x) + ","+ StringHelper::iToSS(_centerGrid.y) + ")";
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
		else if(_modifyTiltGrid)
		{			
			_modifyTiltGrid = false;
			//next draw will delete rectangle!
			//emit update();
			std::string note = "Left button released, center of the Grid: (" + StringHelper::iToSS(_centerGrid.x) + ","+ StringHelper::iToSS(_centerGrid.y) + ")";
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
		else if(_modifyPositionGrid)
		{			
			_modifyPositionGrid = false;
			//next draw will delete rectangle!
			//emit update();
			std::string note = "Left button released, center of the Grid: (" + StringHelper::iToSS(_centerGrid.x) + ","+ StringHelper::iToSS(_centerGrid.y) + ")";
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
	}
	else if ( e->button() == Qt::RightButton)
	{
		if(_newGrid)
		{			
			_newGrid = false;
			_activeGrid = true;			
			std::string note = "new active Grid on: (" + StringHelper::iToSS(_centerGrid.x) + ","+ StringHelper::iToSS(_centerGrid.y) + ")";
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
		else if(_modifyAngleXY)
		{			
			_modifyAngleXY = false;						
			std::string note = "Right button released, center of the Grid: (" + StringHelper::iToSS(_centerGrid.x) + ","+ StringHelper::iToSS(_centerGrid.y) + ")";
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
	}
}