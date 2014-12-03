#include "BeesBookTagMatcher.h"
#include <QApplication>

#include "source/tracking/algorithm/algorithms.h"

namespace {
    auto _ = Algorithms::Registry::getInstance().register_tracker_type<BeesBookTagMatcher>("BeesBook Tag Matcher");
}

BeesBookTagMatcher::BeesBookTagMatcher(Settings & settings, std::string &serializationPathName, QWidget *parent )
    : TrackingAlgorithm( settings, serializationPathName, parent )
{	
		_ready			= true;  //Ready for a new tag --Ctrl + LCM --
		_setTag			= false; //State while the tag is being set, a red vector is drawn
		_activeTag		= false; //if true, then a new Tag has been defined through the orientation vector and its parameter can now be modified
				
		_setP0			= false; //Set P0 --Left Click--
		_setP1			= false; //Set P1 --Left Click--
		_setP2			= false; //Set P2 --Left Click--
		
		_setOrient		= false; //to modify exclusively the tag orientation.
}


BeesBookTagMatcher::~BeesBookTagMatcher(void)
{
}

void BeesBookTagMatcher::track		( ulong, cv::Mat& ){}
void BeesBookTagMatcher::paint		( cv::Mat& image )
{	
	if (_Grids.size() > 0)
	{
		drawSetTags(image);
	}
	if (_setTag)
	{
		drawOrientation(image, orient);
	}	
	/*else if (_setP1 || _setP2)
	{
		drawSettingTheta(image);
	}*/
	else if(_activeTag)
	{		
		drawActiveTag(image);
	}		
}
void BeesBookTagMatcher::reset		(){}

//check if MOUSE BUTTON IS CLICKED
void BeesBookTagMatcher::mousePressEvent	( QMouseEvent * e )
{
	//check if LEFT button is clicked
	if ( e->button() == Qt::LeftButton)
	{		
		//check for SHIFT modifier
		if(Qt::ShiftModifier == QApplication::keyboardModifiers())
		{
			if (_activeTag) // The Tag is active and can now be modified
			{				
				//if clicked on one of the set points, the point is activated
				if (selectPoint(cv::Point(e->x(), e->y())))
					emit update();
				if (_setP1)
					_setOrient = true;
			}
		}
		//check for CTRL modifier
		else if(Qt::ControlModifier == QApplication::keyboardModifiers())	//a new tag is generated
		{
			if (_ready)
			{
				setTag(cv::Point(e->x(), e->y()));
				_setP1 = true;
			}			
		}
		//without modifier
		else
		{
			if (_activeTag) // The Tag is active and can now be modified
			{
				//if clicked in one of the bit cells, its value is changed
				if (dist(g.centerGrid, cv::Point(e->x(), e->y())) > 2 && dist(g.centerGrid, cv::Point(e->x(), e->y())) < g.axesGrid.height)
					g.updateID(cv::Point(e->x(), e->y()));
				else
					//if clicked on one of the set points, the point is activated
					if (selectPoint(cv::Point(e->x(), e->y())))
						emit update();
				//otherwise checks if one of the other tags is selected
					else
						selectTag(cv::Point(e->x(), e->y()));
			}
		}
	}
	//check if RIGHT button is clicked
	if ( e->button() == Qt::RightButton)
	{			
		//check for SHIFT modifier
		if(Qt::ShiftModifier == QApplication::keyboardModifiers())
		{					
		}			
		//check for CTRL modifier
		if (Qt::ControlModifier == QApplication::keyboardModifiers())
		{
			cancelTag(cv::Point2f(e->x(), e->y())); //The Tag being currently configured is cancelled
		}
	}	
	
}
//check if pointer MOVES
void BeesBookTagMatcher::mouseMoveEvent		( QMouseEvent * e )
{
	if (_setTag) //Orientation of the tag is being set
	{
		orient[1] = cv::Point(e->x(), e->y());	
		emit update();
	}
	else if (_setP0) //The tag is being translated
	{			
		g.translation(cv::Point(e->x(), e->y()));
		emit update();				
	}
	else if (_setP1) //The orientation of the tag is being modified
	{	
		if (_setOrient)
			g.orientation(cv::Point(e->x(), e->y()));
		else
			setTheta(cv::Point(e->x(), e->y()));
		emit update();
	}	
	else if (_setP2) //The space angle (theta) is being modified
	{	
		setTheta(cv::Point(e->x(), e->y()));
		emit update();
	}
}
//check if MOUSE BUTTON IS RELEASED
void BeesBookTagMatcher::mouseReleaseEvent	( QMouseEvent * e )
{
	// left button released
	if (e->button() == Qt::LeftButton)
	{	
		if (_setTag) //center and orientation of the tag were set.
		{
			_setTag = false;
			_activeTag = true;
			_ready = true;			
			g = myNewGrid(orient[0], atan2(orient[1].x - orient[0].x, orient[1].y - orient[0].y) - M_PI / 2);
			
			//length of the vector is taked into consideration
			setTheta(cv::Point(e->x(), e->y()));
			_setP1 = false;			
		}		
		else if (_setP0)//the tag was translated
		{
			_setP0 = false;
			g.updateVectors();
		}
		else if (_setP1) //orientation of the bee and the marker were modified
		{
			_setP1 = false;
			_setOrient = false;
			g.updateVectors();
		}
		else if (_setP2) //orientation of the marker was modified
		{
			_setP2 = false;
			g.updateVectors();
		}
		emit update();
	}
	// right button released
	if (e->button() == Qt::RightButton)
	{			
	}
}
//check if WHEEL IS ACTIVE
void BeesBookTagMatcher::mouseWheelEvent	( QWheelEvent * e)
{
	if (_activeTag) // The Grid is active for draging
	{					
			g.scale = g.scale + e->delta() / 96*0.05;		//scale variable is updated by 0.05
			std::cout << "scale " << g.scale << std::endl;
			g.updateAxes();			
			emit update();		
	}	
}

//BeesBookTagMatcher private member functions

//DRAWING FUNCTIONS

//function that draws the set Tags so far.
void BeesBookTagMatcher::drawSetTags(cv::Mat image)
{
    for (size_t i = 0; i < _Grids.size(); i++)
	{
		gtemp = myNewGrid(_Grids[i].scale, _Grids[i].centerGrid, _Grids[i].alpha, _Grids[i].theta, _Grids[i].phi, _Grids[i].ID);
		gtemp.drawFullTag(image, 2); //the grids are drawn as set
	}	
}
//function that draws the orientation vector while being set.
void BeesBookTagMatcher::drawOrientation(cv::Mat image, std::vector<cv::Point> orient)
{
	//std::cout << "DRAW ORIENTAION" << std::endl;
	cv::line(image, orient[0], orient[1], cv::Scalar(0, 0, 255), 1);		//the orientation vector is printed in red
}
//function that draws an active tag calling an instance of MyNewGrid
void BeesBookTagMatcher::drawActiveTag(cv::Mat image)
{		
	//std::cout<<"DRAW ACTIVE GRID"<<std::endl;	
	g.drawFullTag(image,1); //the grid is drawn as active
	g.updatePoints();
	for (int i = 0; i < 3; i++)
	{
		cv::circle(image, g.absPoints[i], 1, cv::Scalar(0, 0, 255), 1); //the point is drawn in red					
	}
	//active point in blue
	if (_setP0)
		cv::circle(image, g.absPoints[0], 1, cv::Scalar(255, 0, 0), 1);
	else if (_setP1)
		cv::circle(image, g.absPoints[1], 1, cv::Scalar(255, 0, 0), 1);
	else if (_setP2)
		cv::circle(image, g.absPoints[2], 1, cv::Scalar(255, 0, 0), 1);
}

//function that draws the tag while being rotated in space
void BeesBookTagMatcher::drawSettingTheta(cv::Mat &img)
{
	cv::ellipse(img, g.absPoints[0], g.axesTag, g.angleTag, 0, 360, cv::Scalar(0, 255, 255), 1);
	cv::line(img, g.absPoints[0], g.absPoints[0]+g.realCoord[1], cv::Scalar(0, 0, 255), 1);	//the orientation vector is printed in red
	cv::line(img, g.absPoints[0], g.absPoints[0]+g.realCoord[2], cv::Scalar(0, 255, 0), 1);	//the redius vector is printed in green

	for (int i = 0; i < 3; i++)
	{
		cv::circle(img, g.absPoints[0]+g.realCoord[i], 1, cv::Scalar(0, 0, 255), 1); //the point is drawn in red					
	}
	if (_setP1)
		cv::circle(img, g.absPoints[0] + g.realCoord[1], 1, cv::Scalar(255, 0, 0), 1);
	else if (_setP2)
		cv::circle(img, g.absPoints[0] + g.realCoord[2], 1, cv::Scalar(255, 0, 0), 1);
}

//TAG CONFIGURATION FUNCTIONS

//function called while setting the tag (it initializes orient vector)
void BeesBookTagMatcher::setTag(cv::Point location)
{
	//If there is an active Tag, this is pushed into the _Grids vector and a new Tag is generated
	if (_activeTag)
		_Grids.push_back(g);

	_ready = false;
	_setTag = true;

	orient.clear();
	orient.push_back(location);
	orient.push_back(location);
	emit update();	//the orientation vector is drawn.
	return;
}
//function that allows P1 and P2 to be modified to calculate the tag's angle in space.
void BeesBookTagMatcher::setTheta(cv::Point location)
{
	//std::cout << " --------------------------------------------------------------- " << std::endl;
	double		prop;
	double		angle;
	//when P1 is being modified
	if (_setP1)
	{		
		prop = dist(g.absPoints[0], location) / (g.scale*axisTag); //the length of the vector is equal to the distance to the pointer, limited to axisTag and 0.5*axisTag 
		if (prop > 1)
			prop = 1;
		else if (prop < 0.5)
			prop = 0.5;
		//P1 updates the orientation of the tag too.
		g.orientation(location);
		g.realCoord[1] = g.polar2rect(prop*axisTag, g.alpha);
		//std::cout << "realCoord 1: x = " << g.realCoord[1].x << ", y = " << g.realCoord[1].y << "(New value)" << std::endl;
		//std::cout << " test  --- " << g.polar2rect(prop*axisTag, g.alpha) << std::endl;
	}
	//when P2 is being modified
	else if (_setP2)
	{	
		prop = dist(g.absPoints[0], location) / (g.scale*axisTag); //the length of the vector is equal to the distance to the pointer, limited to axisTag and 0.5*axisTag 
		if (prop > 1)
			prop = 1;
		else if (prop < 0.5)
			prop = 0.5;		
		//P2 doesn't update alpha
		angle = atan2(location.x - g.centerGrid.x, location.y - g.centerGrid.y) - M_PI / 2;
		g.realCoord[2] = g.polar2rect(prop*axisTag, angle);
		//std::cout << "realCoord 2: x = " << g.realCoord[2].x << ", y = " << g.realCoord[2].y << "(New value)" << std::endl;
	}
	//updates parameters
	g.updateParam();
	return;
}


//function that cancels the active tag and activates the previous one.
void BeesBookTagMatcher::cancelTag(cv::Point2f location)
{
	if (_Grids.size() > 0)
	{
		g = myNewGrid(_Grids[_Grids.size() - 1].scale, _Grids[_Grids.size() - 1].centerGrid, _Grids[_Grids.size() - 1].alpha, _Grids[_Grids.size() - 1].theta, _Grids[_Grids.size() - 1].phi, _Grids[_Grids.size() - 1].ID); //previous Tag is loaded
		_Grids.pop_back(); //last tag is set as active and removed from the vector
	}
	else //when there is just one tag, this is reset
		g = myNewGrid(location, M_PI / 2);
	emit update();
	return;
}


//function that checks if one of the set Points is selected, returns true when one of the points is selected
bool BeesBookTagMatcher::selectPoint(cv::Point location)
{
	bool answer = false;
	for (int i = 0; i<3; i++)
	{
		if (dist(location, g.absPoints[i])<2) //check if the pointer is on one of the points
		{
			switch (i)
			{
			case 0:
				_setP0 = true;
				answer = true;
				break;
			case 1:
				_setP1 = true;
				answer = true;	
				break;
			case 2:
				_setP2 = true;
				answer = true;		
				break;
			default:
				answer = false;
				break;
			}
			break;
		}
	}
	return answer;
}
//function that checks if one of the already set Tags is selected.
void BeesBookTagMatcher::selectTag(cv::Point location)
{
	if (_Grids.size()>0)
        for (size_t i = 0; i < _Grids.size(); i++)
		{
		//if (dist(location, _Grids[i].centerTag) < _Grids[i].axesTag.height)
		if (dist(location, _Grids[i].centerGrid) < _Grids[i].axesTag.height)
			{
				_Grids.push_back(g); //active tag is pushed back in the vector of Grids
				g = myNewGrid(_Grids[i].scale, _Grids[i].centerGrid, _Grids[i].alpha, _Grids[i].theta, _Grids[i].phi, _Grids[i].ID); //selected Tag is loaded
				_Grids.erase(_Grids.begin()+i); //The selected tag is erased from the vector
				emit update();
			}			
		}	
	return;
}

//AUXILIAR FUNCTION

//function that calculates the distance between two points
double BeesBookTagMatcher::dist(cv::Point p1, cv::Point p2)
{	
	diff = p1-p2;
	return sqrt(diff.x*diff.x + diff.y*diff.y);	
}
