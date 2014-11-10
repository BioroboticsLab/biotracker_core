#include "myNewGrid.h"
#include <QApplication>

#include "cv.h"
#include "source/settings/Settings.h"
#include "source/tracking/TrackingAlgorithm.h"
#include <iostream>
#include <QPolygon>
#include <utility>

//default constructor
//THIS IS CPP 11
//myNewGrid::myNewGrid()
//: myNewGrid(cv::Point (0,0), cv::Size (axisTag*(MR/OR),axisTag*(MR/OR)), 0, cv::Point (0,0), cv::Size (axisTag,axisTag), 0, std::vector<bool> (12,0))
//{}
//Tobias temporary fix:
//void myNewGrid::init(cv::Point2f CenterGrid, cv::Size2f AxesGrid, double Alpha, cv::Point2f CenterTag, cv::Size2f AxesTag, double Phi, std::vector<bool> Id)
void myNewGrid::init(double Scale, cv::Point2f CenterGrid, double Alpha, double Theta, double Phi, std::vector<bool> Id)
{
	scale		= Scale;						//new variable, is the scale of the tag referenced to axisTag (25 pixels)
	centerTag	= CenterGrid;
	centerGrid	= CenterGrid;
	alpha		= Alpha;						//new variable, is the orientation of the tag, measured counterclockwise from x-axis.
	theta		= Theta;						//new variable, indicates how tilted is the grid.
	phi			= Phi;							//new variable, is the orientation of the ellipse that represents the tag, measured counterclockwise from x-axis.
	ID           = std::move(Id);
	  
	angleTag	= -(phi*180/M_PI);				//this angle denotes the orientation of the ellipses (both ellipses should have the same value) (measured clockwise from the x-axis)
	angleGrid	= (phi-alpha-M_PI/2)*180/M_PI;	//the angle of the grid, is the angle where the bit cells start to be drawn, it is calculated from phi and alpha
	  
	axesGrid	= cv::Size2f (Scale*axisTag*(MR/OR),Scale*axisTag*(MR/OR)*cos(theta));
	axesTag		= cv::Size2f (Scale*axisTag,Scale*axisTag*cos(theta));	
  
	initPoints();  
}

myNewGrid::myNewGrid()
{	
	init(1, cv::Point (0,0), M_PI/2, 0, M_PI/2, std::vector<bool> (12,0));	
}
//constructor with 1 parameter
myNewGrid::myNewGrid(cv::Point2f CenterGrid)
{	
	init(1, CenterGrid, M_PI/2, 0, M_PI/2, std::vector<bool> (12,0));
	//init(CenterGrid, cv::Size2f (axisTag*(MR/OR),axisTag*(MR/OR)), M_PI/2, CenterGrid, cv::Size2f (axisTag,axisTag), M_PI/2, std::vector<bool> (12,0));	
}
//constructor with 7 parameters
myNewGrid::myNewGrid(double Scale, cv::Point2f CenterGrid, double Alpha, double Theta, double Phi, std::vector<bool> Id)
{
	init(Scale, CenterGrid, Alpha, Theta, Phi, Id);
	//init(CenterGrid, AxesGrid, AngleGrid, CenterTag, AxesTag, AngleTag, Id);  
}
//destrucor
myNewGrid::~myNewGrid(){}
//This function initializes the vector with the coordinates of the points that define the grid
//Use this function only when a new tag is initialized
void myNewGrid::initPoints()
{
	absPoints.clear();
	relPoints.clear();
	//The coordinates of the center are absolute in the image coordinates
	absPoints.push_back(centerGrid);							// P0 is pushed back, at this instant absPoints.size() == 1
	absPoints.push_back(centerGrid+cv::Point2f(0,-axisTag*(MR/OR)));// P1 is pushed back, at this instant absPoints.size() == 2
	//absPoints.push_back(center+cv::Point2f(axisTag*(MR/OR),0)); // P2 is pushed back, at this instant absPoints.size() == 3
	//absPoints.push_back(center+cv::Point2f(0,axisTag*(MR/OR))); // P3 is pushed back, at this instant absPoints.size() == 4
	//absPoints.push_back(center+cv::Point2f(-axisTag*(MR/OR),0));// P4 is pushed back, at this instant absPoints.size() == 5	
	//absPoints.push_back(center+cv::Point2f(0,-axisTag));		// P5 is pushed back, at this instant absPoints.size() == 6
	//absPoints.push_back(center+cv::Point2f(axisTag,0));			// P6 is pushed back, at this instant absPoints.size() == 7
	//absPoints.push_back(center+cv::Point2f(0,axisTag));			// P7 is pushed back, at this instant absPoints.size() == 8
	//absPoints.push_back(center+cv::Point2f(-axisTag,0));		// P8 is pushed back, at this instant absPoints.size() == 9
	////absPoints.push_back(center);								// P9 is pushed back, at this instant absPoints.size() == 10
	
	//The coordinates of the following four points shall be interpratated as relative to the center
	relPoints.push_back(cv::Point2f(0,0));						// R0 is pushed back, at this instant relPoints.size() == 1
	relPoints.push_back(cv::Point2f(0,-axisTag*(MR/OR)));		// R1 is pushed back, at this instant relPoints.size() == 2
	//relPoints.push_back(cv::Point2f(axisTag*(MR/OR),0));		// R2 is pushed back, at this instant relPoints.size() == 3
	//relPoints.push_back(cv::Point2f(0,axisTag*(MR/OR)));		// R3 is pushed back, at this instant relPoints.size() == 4
	//relPoints.push_back(cv::Point2f(-axisTag*(MR/OR),0));		// R4 is pushed back, at this instant relPoints.size() == 5	
	//relPoints.push_back(cv::Point2f(0,-axisTag));				// R5 is pushed back, at this instant relPoints.size() == 6
	//relPoints.push_back(cv::Point2f(axisTag,0));				// R6 is pushed back, at this instant relPoints.size() == 7
	//relPoints.push_back(cv::Point2f(0,axisTag));				// R7 is pushed back, at this instant relPoints.size() == 8
	//relPoints.push_back(cv::Point2f(-axisTag,0));				// R8 is pushed back, at this instant relPoints.size() == 9
	////relPoints.push_back(cv::Point2f(0,0));						// R5 is pushed back, at this instant relPoints.size() == 10
}
//updates one of the two points vector
//m = 1 --> updates absPoints from relPoints (when the center coordinates are modified)
//m = 2 --> updates relPoints from absPoints (when the coordinates of one of the outter points is modified)
//m = 3 --> updates relPoints and absPoints from Parameters (option called by drawDragGrid)
void myNewGrid::updatePoints(int m)
{
	if (m==1)
		for (int i=1; i<10; i++)
			absPoints[i]=relPoints[i]+absPoints[0];
	else if (m==2)
		for (int i=1; i<10; i++)
			relPoints[i]=absPoints[i]-absPoints[0];
	else if (m==3)
	{
		alpha = -angleGrid*M_PI/180 - M_PI/2;
		std::cout<<"alpha "<<alpha<<" angleGrid " <<angleGrid<<std::endl;		
		/*for (int i=1; i<9; i++)
		{
			relPoints[i]=cv::Point2f(relPoints[i].x*cos(alpha)-relPoints[i].y*sin(alpha),relPoints[i].x*sin(alpha)+relPoints[i].y*cos(alpha));
			absPoints[i]=relPoints[i]+absPoints[0];
			std::cout<<relPoints[i]<<std::endl;
		}
		updateParam();*/
	}
	return;
}
//updates the tag parameters from the points positions
//s --> scale parameter relative to axisTag
//(x0,y0) --> center of the tag and grid
//alpha --> bee orientation (angle between the "head" of the bee and the x axis)
//theta --> tilt of the tag
//phi --> orientation of the ellipse mayor axis (tag and grid)
void myNewGrid::updateParam()
{
	cv::Point3d			v1;
	cv::Point3d			v2;
	cv::Point3d			norm;

	//calculate the Z component for P3 and P4 in order to calculate a normal vector to them through cross product
	v1.x=relPoints[3].x;
	v1.y=relPoints[3].y;
	v1.z=sqrt(pow(axisTag*(MR/OR),2)-(pow(v1.x,2)+pow(v1.y,2)));
	std::cout <<"radius grid"<<axisTag*(MR/OR)<<std::endl;
	std::cout << "vector1 "<< v1.x <<" "<< v1.y <<" "<< v1.z <<std::endl;
	
	v2.x=relPoints[4].x;
	v2.y=relPoints[4].y;
	v2.z=sqrt(pow(axisTag*(MR/OR),2)-(pow(v2.x,2)+pow(v2.y,2)));
	std::cout << "vector2 "<< v2.x <<" "<< v2.y <<" "<< v2.z <<std::endl;
	//cross product and normal to the plane
	norm = v2.cross(v1);
	//to assure that the normal vector points upwards
	if (norm.z<0)
	{
		norm.x=-norm.x;
		norm.y=-norm.y;
		norm.z=-norm.z;
	}
	std::cout << "normal "<< norm.x <<" "<< norm.y <<" "<< norm.z <<std::endl;
	//roll (theta) and yaw (phi) angles
	cv::Point			x2d(1,0);
	cv::Point			y2d(0,-1); //we use (0,-1) to get the angle relative to the vertical of the hive, were upwards is 0
	cv::Point3d			z3d(0,0,1);
	//roll angle, the angle between the normal to the plane and the z-axis is 
	theta=acos(norm.dot(z3d)/(sqrt(norm.dot(norm))));
	std::cout<<"theta "<<theta*180/M_PI<<std::endl;

	//yaw angle
	if (norm.x == 0 && norm.y==0)
		phi =0;
	else
		phi=acos(y2d.dot(cv::Point2d(norm.x,norm.y))/sqrt(pow(norm.x,2)+pow(norm.y,2)));
	std::cout<<"phi "<<phi*180/M_PI<<std::endl;

	//alpha angle (bee orientation)
	alpha=acos(x2d.dot(cv::Point2d(relPoints[3]))/sqrt(relPoints[3].dot(relPoints[3])));
	if (relPoints[3].y>0)
		alpha =2*M_PI-alpha;
	std::cout<<"alpha "<<alpha*180/M_PI<<std::endl;

	updateAxes();

	centerGrid = absPoints[0];
	centerTag = absPoints[0];
	angleGrid = -(alpha-phi)*180/M_PI-90;
	angleTag = -phi*180/M_PI;
	return;
}
//function that updates the minor axis of the ellipse using as parameter theta.
void myNewGrid::updateAxes()
{
	axesTag.height=axisTag*cos(theta);
	axesGrid.height=axesGrid.width*cos(theta);
	return;
}

void myNewGrid::drawModPoints(cv::Mat &img)
{	
	cv::circle(img, absPoints[0], 1, cv::Scalar(255, 255, 255), 1);
	for (int i=1;i<5;i++)
	{
		cv::circle(img, absPoints[i], 1, cv::Scalar(0, 255, 0), 1);
		cv::circle(img, absPoints[i+4], 1, cv::Scalar(0, 255, 255), 1);
	}
	return;
}

void myNewGrid::drawGrid(cv::Mat &img)
{	
	//render of the Tag is generated
	std::vector<std::vector <cv::Point>> conts = renderFullTag();
	//the twelve bit cells are drawn	
	for (int i=0; i<12;i++)
	{		
		//each of the cells contour is colored according to its value
		cv::drawContours(img, conts, i, cv::Scalar(ID[i]*255, ID[i]*255, ID[i]*255), 1);
	}

	//half white circle
	cv::drawContours(img, conts, 12, cv::Scalar(255, 255, 255), 1);	
	//half black circle
	cv::drawContours(img, conts, 13, cv::Scalar(0, 0, 0), 1);	
	//at the end the diameter is redrawn in green
	cv::drawContours(img, conts, 14, cv::Scalar(0, 255, 0), 1);
	return;
}

void myNewGrid::drawFullTag(cv::Mat &img, bool active)
{	
	std::vector<std::vector <cv::Point>> conts = renderFullTag();
	//the twelve bit cells are drawn	
	for (int i=0; i<12;i++)
	{		
		//each of the cells contour is colored according to its value
		cv::drawContours(img, conts, i, cv::Scalar(ID[i]*255, ID[i]*255, ID[i]*255), 1);
	}

	//half white circle
	cv::drawContours(img, conts, 12, cv::Scalar(255, 255, 255), 1);	
	//half black circle
	cv::drawContours(img, conts, 13, cv::Scalar(0, 0, 0), 1);	
	
	//at the end the diameter is redrawn in green
	cv::drawContours(img, conts, 14, cv::Scalar(0, 255, 0), 1);
	
	//an ellipse is drawn as the tag contour, color according to active parameter
	if (active)
		cv::drawContours(img, conts, 15, cv::Scalar(0, 0, 255), 1);		
	else
		cv::drawContours(img, conts, 15, cv::Scalar(255, 255, 0), 1);
	return;
}

std::vector<cv::Point> myNewGrid::renderGridCell(unsigned short cell)
{
	std::vector<cv::Point> globCont;
	std::vector<cv::Point> globCont2;	

	// Bit cells
	if (cell < 12)
	{	
		int arcStart = angleGrid+cell * 30;
		int arcEnd = angleGrid+(cell + 1) * 30;
		// outer arc
		ellipse2Poly(centerGrid, cv::Size(axesGrid.width, axesGrid.height), angleTag, arcStart, arcEnd, 1, globCont);
		// inner arc
		ellipse2Poly(centerGrid, cv::Size((IR/MR)*axesGrid.width,(IR/MR)*axesGrid.height), angleTag, arcStart, arcEnd, 1, globCont2);
		// join outer and inner arc
		globCont.insert(globCont.end(), globCont2.rbegin(), globCont2.rend());	
	}
	// White semicircle
	if (cell == 12)		
		ellipse2Poly(centerGrid, cv::Size((IR/MR)*axesGrid.width,(IR/MR)*axesGrid.height), angleTag, angleGrid, angleGrid+180, 1, globCont);
	// Black semicircle
	if (cell == 13)		
		ellipse2Poly(centerGrid, cv::Size((IR/MR)*axesGrid.width,(IR/MR)*axesGrid.height), angleTag, angleGrid, angleGrid-180, 1, globCont);
	// Green diamater
	if (cell == 14)
	cv::ellipse2Poly(centerGrid, cv::Size((IR/MR)*axesGrid.width,(IR/MR)*axesGrid.height), angleTag, angleGrid, angleGrid-180, 180, globCont);
	// Outer perimeter
	if (cell == 15)
		ellipse2Poly(centerTag, axesTag, angleTag, 0, 360, 1, globCont);

	return globCont;
}

//void myNewGrid::renderModTag()
//{
//	// axes vector	
//	std::vector<cv::Point> subCont;
//
//	gridAxes.clear();	
//	//middle axes
//	//height
//	ellipse2Poly(centerGrid, cv::Size(axesGrid.width, axesGrid.height), angleGrid, 90, 270, 180, subCont);
//	gridAxes.push_back(subCont);	
//	//width
//	ellipse2Poly(centerGrid, cv::Size(axesGrid.width, axesGrid.height), angleGrid, -180, 180, 180, subCont);
//	gridAxes.push_back(subCont);	
//	//inner axes
//	//height
//	ellipse2Poly(centerGrid, cv::Size((IR/MR)*axesGrid.width,(IR/MR)*axesGrid.height), angleGrid, 90, 270, 180, subCont);
//	gridAxes.push_back(subCont);	
//	//width
//	ellipse2Poly(centerGrid, cv::Size((IR/MR)*axesGrid.width,(IR/MR)*axesGrid.height), angleGrid, -180, 180, 180, subCont);
//	gridAxes.push_back(subCont);	
//	//outer axes
//	//height
//	ellipse2Poly(centerGrid, cv::Size((OR/MR)*axesGrid.width,(OR/MR)*axesGrid.height), angleGrid, 90, 270, 180, subCont);
//	gridAxes.push_back(subCont);	
//	//width
//	ellipse2Poly(centerGrid, cv::Size((OR/MR)*axesGrid.width,(OR/MR)*axesGrid.height), angleGrid, -180, 180, 180, subCont);
//	gridAxes.push_back(subCont);
//	
//	//axesTag parameter is updated
//	axesTag.width = axesGrid.width * OR/MR;
//	axesTag.height = axesGrid.height * OR/MR;
//	return;
//}

std::vector<std::vector <cv::Point>> myNewGrid::renderFullTag()
{
	// contour vector
	std::vector<std::vector <cv::Point>> conts;	
	cellsContours.clear();
	//ites 0-11 bit cells
	//ites 12 white semicircle
	//ites 13 black semicircle
	//ites 14 diamater
	//ites 15 tag perimeter
	int ites = 16;
	
	
	// render the bit cells
	for (int i = 0; i < ites; i++)	
	{
		std::vector<cv::Point> subCont = renderGridCell(i);
		conts.push_back(subCont);
		//only the bit cells  and the tag perimter are stored in cellsContours
		if (i<12 || i==15)			
			cellsContours.push_back(subCont);
	}	
	conts.push_back(std::vector<cv::Point>());	//empty vector to allow the first n vector to be printed VS-2012 error
	return conts;
}