#include "BeesBookTagMatcher.h"
#include <QApplication>


BeesBookTagMatcher::BeesBookTagMatcher(Settings & settings, std::string &serializationPathName, QWidget *parent )
    : TrackingAlgorithm( settings, serializationPathName, parent )
{	
		_ready			= true;  //Ready for a new tag --ctrl + Right Click--
		_activeDragGrid = false; //a new Grid has been set and can now be draged and rotated
		_activeTag		= false; //if true, then a new Grid has been defined with the vector of points and the bits can now bi defined
		_activePoints	= false; //a new set of points is being configured
		
		_setP0			= false; //Set P0 --Left Click--
		_setP1			= false; //Set P1 --Left Click--
		_setP2			= false; //Set P2 --Left Click--
		_setP3			= false; //Set P3 --Left Click--
		_setP4			= false; //Set P4 --Left Click--
		_setP5			= false; //Set P5 --Left Click--
		_setP6			= false; //Set P6 --Left Click--
		_setP7			= false; //Set P7 --Left Click--
		_setP8			= false; //Set P8 --Left Click--

		_modPosGrid		= false; //modify position of active Grid (only Grid, inside the Tag)
		_modPosTag		= false; //modify position of active Tag (including Grid)				
}


BeesBookTagMatcher::~BeesBookTagMatcher(void)
{
}

void BeesBookTagMatcher::track		( ulong, cv::Mat& ){}
void BeesBookTagMatcher::paint		( cv::Mat& image )
{	
	if(_activeDragGrid)
	{		
		drawDragGrid(image);
	}
	else if(_activePoints)
	{		
		drawPoints(image);
	}
	else if(_activeTag)
	{
		drawGrid(image);
	}
}
void BeesBookTagMatcher::reset		(){}

//check if MOUSE BUTTON IS CLICKED
void BeesBookTagMatcher::mousePressEvent		( QMouseEvent * e )
{
	//check if LEFT button is clicked
	if ( e->button() == Qt::LeftButton)
	{
		if (_activeDragGrid) // The Grid is active for draging
		{
			if (abs(e->x()-g.absPoints[0].x)<2 && abs(e->y()-g.absPoints[0].y)<2)
				_setP0 = true;			
		}
		else if (_activePoints) //modify points
			for (int i=0; i<9; i++)
				{
                    if (std::abs<double>(e->x()-g.absPoints[i].x)<2 && std::abs<double>(e->y()-g.absPoints[i].y)<2)
					{
						switch (i)
						{
						case 0:
							_setP0 = true;							
							emit update();
							break;
						case 1:
							_setP1 = true;							
							emit update();
							break;
						case 2:
							_setP2 = true;							
							emit update();
							break;
						case 3:
							_setP3 = true;							
							emit update();
							break;
						case 4:
							_setP4 = true;							
							emit update();
							break;
						case 5:
							_setP5 = true;							
							emit update();
							break;
						case 6:
							_setP6 = true;							
							emit update();
							break;
						case 7:
							_setP7 = true;							
							emit update();
							break;
						case 8:
							_setP8 = true;							
							emit update();
							break;
						default:
							break;
						}
						break;
					}			
				}
		if(Qt::ShiftModifier == QApplication::keyboardModifiers())
		{			
		}			
		if(Qt::ControlModifier == QApplication::keyboardModifiers())
		{			
		}			
	}
	//check if RIGHT button is clicked
	if ( e->button() == Qt::RightButton)
	{	//check for CTRL modifier
		if(Qt::ControlModifier == QApplication::keyboardModifiers()) //reset
		{
			if (_ready)
			{
				_ready = false;
				_activeDragGrid = true;
				g=myNewGrid(cv::Point (e->x(),e->y()));	 //a new set of points and parameters is generated		
				emit update(); //the initialized tag is shown
			}			
		}
		//check for SHIFT modifier
		if(Qt::ShiftModifier == QApplication::keyboardModifiers()) //update parameters
		{
		}			
	}	
	
}
//check if pointer MOVES
void BeesBookTagMatcher::mouseMoveEvent		( QMouseEvent * e )
{
	if (_setP0)
	{			
		g.absPoints[0]= cv::Point(e->x(),e->y());	// P0 (center) is updated		
		g.updatePoints(1);
		g.centerGrid = g.absPoints[0];
		g.centerTag = g.absPoints[9];
		emit update();
	}
	else if (_setP1)
	{			
		g.absPoints[1]= cv::Point(e->x(),e->y()); // P1 is updated		
		g.updatePoints(2);
		emit update();
	}
	else if (_setP3)
	{			
		//g.ratP1P3 = dist(g.absPoints[0],cv::Point(e->x(),e->y()))/dist(g.absPoints[0],g.absPoints[1]);
		//if (g.ratP1P3 > 1)
		//	g.ratP1P3 = 1;
		//g.absPoints[3]= cv::Point((g.absPoints[1].x-g.absPoints[0].x)*g.ratP1P3 + g.absPoints[0].x,(g.absPoints[1].y-g.absPoints[0].y)*g.ratP1P3  + g.absPoints[0].y); // P3 is updated
		g.updatePoints(2);
		emit update();
	}
	else if (_setP2)
	{			
		g.absPoints[2]=cv::Point(e->x(),e->y()); // P2 is updated
		//g.absPoints[4]= cv::Point((e->x()-g.absPoints[0].x)*g.ratP2P4 + g.absPoints[0].x,(e->y()-g.absPoints[0].y)*g.ratP2P4  + g.absPoints[0].y); // P4 is updated
		g.updatePoints(2);
		emit update();
	}	
	else if (_setP4)
	{			
		//g.ratP2P4 = dist(g.absPoints[0],cv::Point(e->x(),e->y()))/dist(g.absPoints[0],g.absPoints[2]);
		//if (g.ratP2P4 > 1)
		//	g.ratP2P4 = 1;
		//g.absPoints[4]= cv::Point((g.absPoints[2].x-g.absPoints[0].x)*g.ratP2P4 + g.absPoints[0].x,(g.absPoints[2].y-g.absPoints[0].y)*g.ratP2P4  + g.absPoints[0].y); // P4 is updated		
		g.updatePoints(2);
		emit update();
	}
}
//check if MOUSE BUTTON IS RELEASED
void BeesBookTagMatcher::mouseReleaseEvent	( QMouseEvent * e )
{
	// left button released
	if (e->button() == Qt::LeftButton)
	{
		if (_setP0)
			_setP0 = false;	
		else if (_setP1)
			_setP1 = false;
		else if (_setP2)
			_setP2 = false;
		else if (_setP3)
			_setP3 = false;
		else if (_setP4)
			_setP4 = false;
		else if (_activePoints)
			emit update();
	}
	// right button released
	if (e->button() == Qt::RightButton)
	{		
	}
}

void BeesBookTagMatcher::mouseWheelEvent	( QWheelEvent * e)
{
	if (_activeDragGrid) // The Grid is active for draging
	{			
			g.angleGrid=g.angleGrid-e->delta()/96;
			g.updatePoints(3);
			emit update();
		//std::cout<<"alpha "<<g.alpha<<" angleGrid " <<g.angleGrid<<" extra "<< e->delta()/96<<std::endl;
	}	
}

//BeesBookTagMatcher private member functions
//this draws a basic grid onto the display image
void BeesBookTagMatcher::drawDragGrid(cv::Mat image)
{		
	std::cout<<"DRAWDRAGGRID"<<std::endl;	
	g.drawFullTag(image,1); //the grid is drawn as active
	cv::circle(image, g.absPoints[0], 1, cv::Scalar(255, 255, 255), 1); //the center is drawn in white
	cv::circle(image, g.absPoints[5], 1, cv::Scalar(0, 255, 255), 1); //the orientation point is printed in yellow
	cv::circle(image, g.absPoints[1], 1, cv::Scalar(0, 255, 255), 1); //the orientation point is printed in yellow
}
//BeesBookTagMatcher private member functions
//this draws a basic grid onto the display image
void BeesBookTagMatcher::drawGrid(cv::Mat image)
{
	g.updateParam();
	std::cout<<"DRAWGRID"<<std::endl;
	g.drawFullTag(image,1); //the grid is drawn	
}
void BeesBookTagMatcher::drawPoints(cv::Mat image)
{
	if (_activePoints)
	{
		g.drawModPoints(image);
	}		
}


double BeesBookTagMatcher::dist(cv::Point p1, cv::Point p2)
{
	double dist;
	diff = p1-p2;
	dist = sqrt(diff.x*diff.x + diff.y*diff.y);
	return dist;
}
