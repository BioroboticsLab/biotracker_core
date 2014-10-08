#include "BeesBookTagMatcher.h"
#include "source/helper/StringHelper.h"
#include <QApplication>

//some variables can go out of boundary


BeesBookTagMatcher::BeesBookTagMatcher( Settings & settings, QWidget *parent ) : TrackingAlgorithm( settings, parent )
{
		_activeGrid = false; //a new Grid has been set and can now be modified
		_activeTag = false; //a new Grid has been set and the Tag perimeter can now be modified

		_modPosGrid = false; //modify position of active Grid
		_modPosTag = false; //modify position of active Tag

		_modHeightInn = false; //modify height Inner circle and angleGrid
		_modWidthInn = false; //modify width Inner circle and angleGrid
		_modHeightMid = false; //modify height Middle circle and angleGrid
		_modWidthMid = false; //modify width Middle circle and angleGrid		
		_modHeightOut = false; //modify height Outer circle and angleTag
		_modWidthOut = false; //modify width Outer circle and angleTag

		_ready = true; //Ready for a new tag --ctrl + Right Click--
		_activePoints = false; //a new set of points is being configured
		_setP0 = false; //Set P0 --Left Click--
		_setP1 = false; //Set P1 --Left Click--
		_setP2 = false; //P1 has been set, P0, P1 and P2 are active --ctrl + Right Click--
		_setP3 = false; //Set P3 --Left Click--
		_setP4 = false;	//P3 has been set, P0, P1, P2, P3 and P4 are active --ctrl + Right Click--

		ratP1P2 = 0.5; //ratio P1/P2
		ratP3P4 = 0.5; //ratio P3/P4
}


BeesBookTagMatcher::~BeesBookTagMatcher(void)
{
}

void BeesBookTagMatcher::track		( ulong, cv::Mat& ){}
void BeesBookTagMatcher::paint		( cv::Mat& image )
{	
	if(_activePoints)
	{		
		drawPoints(image);
	}
}
void BeesBookTagMatcher::reset		(){}

//check if MOUSE BUTTON IS CLICKED
void BeesBookTagMatcher::mousePressEvent		( QMouseEvent * e )
{
	//check if LEFT button is clicked
	if ( e->button() == Qt::LeftButton)
	{	
		if (_defPoints.size() >0)
			for (int i=_defPoints.size(); i>=0; i--)
				{
					if (abs(e->x()-_defPoints[i].x)<2 && abs(e->y()-_defPoints[i].y)<2)
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
						default:
							break;
						}
						break;
					}			
				}
		//check for SHIFT modifier
		if(Qt::ShiftModifier == QApplication::keyboardModifiers())
		{			
			if(!(_setP0 || _setP1 || _setP2 || _setP3 || _setP4))
			{
				if (_ready)
				{
					_defPoints.clear();	 //a new set of points is being generated
					ratP1P2 = 0.5; //ratio P1/P2
					ratP3P4 = 0.5; //ratio P3/P4
					_defPoints.push_back(cv::Point(e->x(),e->y())); // P0 is pushed back, at this instant _defPoints.size() == 1
					_defPoints.push_back(cv::Point(e->x(),e->y())); // P1 is pushed back, at this instant _defPoints.size() == 2
					_defPoints.push_back(cv::Point(e->x(),e->y())); // P2 is pushed back, at this instant _defPoints.size() == 3		
					_ready = false;
					_activePoints = true;
					_setP1 = true;
					emit update();
				}
				else
				{
					_defPoints.push_back(cv::Point(e->x(),e->y())); // P3 is pushed back, at this instant _defPoints.size() == 4
					_defPoints.push_back(cv::Point(e->x(),e->y())); // P4 is pushed back, at this instant _defPoints.size() == 5
					_setP3 = true;			
					emit update();
				}			
			}
			
		}			
	}
	//check if RIGHT button is clicked
	if ( e->button() == Qt::RightButton)
	{	
		//check for CTRL modifier
		if(Qt::ControlModifier == QApplication::keyboardModifiers()) //reset
		{
			_ready = true;
			_defPoints.clear();	 //a new set of points is being generated
			ratP1P2 = 0.5; //ratio P1/P2
			ratP3P4 = 0.5; //ratio P3/P4
			emit update();
		}
	}	
	
}
//check if pointer MOVES
void BeesBookTagMatcher::mouseMoveEvent		( QMouseEvent * e )
{
	if (_setP0)
	{			
		_defPoints[0]= cv::Point(e->x(),e->y());	// P0 is updated
		_defPoints[2]= cv::Point((_defPoints[1].x-e->x())*ratP1P2 + e->x(),(_defPoints[1].y-e->y())*ratP1P2  + e->y()); // P2 is updated
		if (_defPoints.size()>4)
			_defPoints[4]= cv::Point((_defPoints[3].x-e->x())*ratP3P4 + e->x(),(_defPoints[3].y-e->y())*ratP3P4  + e->y()); // P4 is updated
		emit update();
	}
	else if (_setP1)
	{			
		_defPoints[1]= cv::Point(e->x(),e->y()); // P1 is updated
		_defPoints[2]= cv::Point((e->x()-_defPoints[0].x)*ratP1P2 + _defPoints[0].x,(e->y()-_defPoints[0].y)*ratP1P2  + _defPoints[0].y); // P2 is updated
		emit update();
	}
	else if (_setP2)//////////////////////
	{			
		ratP1P2 = dist(_defPoints[0],cv::Point(e->x(),e->y()))/dist(_defPoints[0],_defPoints[1]);
		if (ratP1P2 > 1)
			ratP1P2 = 1;
		_defPoints[2]= cv::Point((_defPoints[1].x-_defPoints[0].x)*ratP1P2 + _defPoints[0].x,(_defPoints[1].y-_defPoints[0].y)*ratP1P2  + _defPoints[0].y); // P2 is updated
		emit update();
	}
	else if (_setP3)
	{			
		_defPoints[3]=cv::Point(e->x(),e->y()); // P3 is updated
		_defPoints[4]= cv::Point((e->x()-_defPoints[0].x)*ratP3P4 + _defPoints[0].x,(e->y()-_defPoints[0].y)*ratP3P4  + _defPoints[0].y); // P4 is updated
		emit update();
	}
	else if (_setP4)///////////////////////
	{			
		ratP3P4 = dist(_defPoints[0],cv::Point(e->x(),e->y()))/dist(_defPoints[0],_defPoints[3]);
		if (ratP3P4 > 1)
			ratP3P4 = 1;
		_defPoints[4]= cv::Point((_defPoints[3].x-_defPoints[0].x)*ratP3P4 + _defPoints[0].x,(_defPoints[3].y-_defPoints[0].y)*ratP3P4  + _defPoints[0].y); // P4 is updated		
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
		{
			_defPoints[0]=cv::Point(e->x(),e->y()); // P0 is updated
			_defPoints[2]= cv::Point((_defPoints[1].x-e->x())*ratP1P2 + e->x(),(_defPoints[1].y-e->y())*ratP1P2  + e->y()); // P2 is updated
			if (_defPoints.size()>4)
			{
				_defPoints[4]= cv::Point((_defPoints[3].x-e->x())*ratP3P4 + e->x(),(_defPoints[3].y-e->y())*ratP3P4  + e->y()); // P4 is updated
				angleP1P3 = ((_defPoints[1].x-_defPoints[0].x)*(_defPoints[3].x-_defPoints[0].x)+(_defPoints[1].y-_defPoints[0].y)*(_defPoints[3].y-_defPoints[0].y))
					/(dist(_defPoints[0],_defPoints[1])*dist(_defPoints[0],_defPoints[3]));
				angleP1P3 = acos(angleP1P3)*180.0/CV_PI; //angle P1P3 is updated
				std::string note = "New angle: " + QString::number(angleP1P3).toStdString();
				emit notifyGUI(note,MSGS::NOTIFICATION);
			}			
			_setP0 = false;
			emit update();
		}
		else if (_setP1)
		{
			_defPoints[1]=cv::Point(e->x(),e->y());	// P1 is updated
			_defPoints[2]= cv::Point((e->x()-_defPoints[0].x)*ratP1P2 + _defPoints[0].x,(e->y()-_defPoints[0].y)*ratP1P2  + _defPoints[0].y); // P2 is updated
			if (_defPoints.size()==5)
			{
				angleP1P3 = ((_defPoints[1].x-_defPoints[0].x)*(_defPoints[3].x-_defPoints[0].x)+(_defPoints[1].y-_defPoints[0].y)*(_defPoints[3].y-_defPoints[0].y))
						/(dist(_defPoints[0],_defPoints[1])*dist(_defPoints[0],_defPoints[3]));
				angleP1P3 = acos(angleP1P3)*180.0/CV_PI; //angle P1P3 is updated
				std::string note = "New angle: " + QString::number(angleP1P3).toStdString();
				emit notifyGUI(note,MSGS::NOTIFICATION);
			}
			_setP1 = false;
			emit update();
		}
		else if (_setP2)
		{
			_defPoints[2]= cv::Point((_defPoints[1].x-_defPoints[0].x)*ratP1P2 + _defPoints[0].x,(_defPoints[1].y-_defPoints[0].y)*ratP1P2  + _defPoints[0].y); // P2 is updated
			std::string note = "New rat P1P2: " + QString::number(dist(_defPoints[0],_defPoints[2])).toStdString() + "/" 
				+ QString::number(dist(_defPoints[0],_defPoints[1])).toStdString() 
				+ " = " + QString::number(ratP1P2).toStdString(); // the new ratP1P2 is printed in the GUI
			emit notifyGUI(note,MSGS::NOTIFICATION);
			_setP2 = false;			
			emit update();
		}
		else if (_setP3)
		{
			_defPoints[3]=cv::Point(e->x(),e->y());	// P3 is updated	
			_defPoints[4]= cv::Point((e->x()-_defPoints[0].x)*ratP3P4 + _defPoints[0].x,(e->y()-_defPoints[0].y)*ratP3P4  + _defPoints[0].y); // P4 is updated
			angleP1P3 = ((_defPoints[1].x-_defPoints[0].x)*(_defPoints[3].x-_defPoints[0].x)+(_defPoints[1].y-_defPoints[0].y)*(_defPoints[3].y-_defPoints[0].y))
					/(dist(_defPoints[0],_defPoints[1])*dist(_defPoints[0],_defPoints[3]));
			angleP1P3 = acos(angleP1P3)*180.0/CV_PI; //angle P1P3 is updated
			std::string note = "New angle: " + QString::number(angleP1P3).toStdString();
			emit notifyGUI(note,MSGS::NOTIFICATION);
			_setP3 = false;
			_ready = true;
			emit update();
		}
		else if (_setP4)
		{
			_defPoints[4]= cv::Point((_defPoints[3].x-_defPoints[0].x)*ratP3P4 + _defPoints[0].x,(_defPoints[3].y-_defPoints[0].y)*ratP3P4  + _defPoints[0].y); // P4 is updated
			std::string note = "New rat P3P4: " + QString::number(dist(_defPoints[0],_defPoints[4])).toStdString() + "/" 
				+ QString::number(dist(_defPoints[0],_defPoints[3])).toStdString() 
				+ " = " + QString::number(ratP3P4).toStdString(); // the new ratP3P4 is printed in the GUI
			emit notifyGUI(note,MSGS::NOTIFICATION);
			_setP4 = false;
			emit update();
		}
	}		
}

void BeesBookTagMatcher:: mouseWheelEvent	( QWheelEvent *){}

//BeesBookTagMatcher private member functions
//this draws a basic grid onto the display image
void BeesBookTagMatcher::drawGrid(cv::Mat image)
{
	if (_activeGrid)
	{	
		gNew.drawModGrid(image);
		gNew.drawFullTag(image,1);
	}
	if (_activeTag)
	{
		gNew.drawFullTag(image,1);
	}
}
void BeesBookTagMatcher::drawPoints(cv::Mat image)
{	
	//lines are drawn in blue	
	if (_defPoints.size() > 2)
		cv::line(image, _defPoints[0], _defPoints[1], cv::Scalar(255, 0, 0), 1);
	if (_defPoints.size() > 4)
		cv::line(image, _defPoints[0], _defPoints[3], cv::Scalar(255, 0, 0), 1);
	//points are drawn in yellow
	for (const auto &p : _defPoints)
	{
		cv::circle(image, p, 1, cv::Scalar(0, 255, 255), 1);
	}
	if (_defPoints.size() > 0)
	{
		cv::circle(image,_defPoints[0], 1, cv::Scalar(255, 255, 255), 1);
		cv::circle(image,_defPoints[1], 1, cv::Scalar(200, 100, 200), 1);
	}
	//the colors are set according to the point being modified
	if (_setP0)
	{
		if (_defPoints.size() > 2)
			cv::line(image, _defPoints[0], _defPoints[1], cv::Scalar(0, 255, 0), 1);
		if (_defPoints.size() > 4)
			cv::line(image, _defPoints[0], _defPoints[3], cv::Scalar(0, 255, 0), 1);
		//points are drawn in yellow
		for (const auto &p : _defPoints)
		{
			cv::circle(image, p, 1, cv::Scalar(0, 255, 255), 1);
		}
		cv::circle(image,_defPoints[0], 1, cv::Scalar(0, 0, 255), 1);
		cv::circle(image,_defPoints[1], 1, cv::Scalar(200, 100, 200), 1);
	}
	else if (_setP1 || _setP2)
	{		
		cv::line(image, _defPoints[0], _defPoints[1], cv::Scalar(0, 255, 0), 1);	
		//points are drawn in yellow
		for (const auto &p : _defPoints)
		{
			cv::circle(image, p, 1, cv::Scalar(0, 255, 255), 1);
		}
		cv::circle(image,_defPoints[0], 1, cv::Scalar(255, 255, 255), 1);
		cv::circle(image,_defPoints[1], 1, cv::Scalar(200, 100, 200), 1);
		if (_setP1)
			cv::circle(image,_defPoints[1], 1, cv::Scalar(0, 0, 255), 1);
		else
			cv::circle(image,_defPoints[2], 1, cv::Scalar(0, 0, 255), 1);
	}
	else if (_setP3 || _setP4)
	{		
		cv::line(image, _defPoints[0], _defPoints[3], cv::Scalar(0, 255, 0), 1);	
		//points are drawn in yellow
		for (const auto &p : _defPoints)
		{
			cv::circle(image, p, 1, cv::Scalar(0, 255, 255), 1);
		}
		cv::circle(image,_defPoints[0], 1, cv::Scalar(255, 255, 255), 1);
		cv::circle(image,_defPoints[1], 1, cv::Scalar(200, 100, 200), 1);
		if (_setP3)
			cv::circle(image,_defPoints[3], 1, cv::Scalar(0, 0, 255), 1);
		else
			cv::circle(image,_defPoints[4], 1, cv::Scalar(0, 0, 255), 1);
	}
}


double BeesBookTagMatcher::dist(cv::Point p1, cv::Point p2)
{
	double dist;
	diff = p1-p2;
	dist = sqrt(diff.x*diff.x + diff.y*diff.y);
	return dist;
}
