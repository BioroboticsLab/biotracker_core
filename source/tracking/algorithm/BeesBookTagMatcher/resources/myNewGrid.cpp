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
	centerGrid	= CenterGrid;
	alpha		= Alpha;						//new variable, is the orientation of the tag, measured counterclockwise from x-axis.
	theta		= Theta;						//new variable, indicates how tilted is the grid.
	phi			= Phi;							//new variable, is the orientation of the ellipse that represents the tag, measured counterclockwise from x-axis.	
	ID          = std::move(Id);
	  
	angleTag	= -phi * 180 / M_PI;			//this angle denotes the orientation of the ellipses (both ellipses should have the same value) (measured clockwise from the x-axis)
	angleGrid	= (phi-alpha-M_PI/2)*180/M_PI;	//the angle of the grid, is the angle where the bit cells start to be drawn, it is calculated from phi and alpha
	
	axesGrid	= cv::Size2f (Scale*axisTag*(MR/OR),Scale*axisTag*(MR/OR)*cos(theta));
	axesTag		= cv::Size2f (Scale*axisTag,Scale*axisTag*cos(theta));	
	
	//The size of the vectors is set
	absPoints.resize(3);
	relPoints.resize(3);
	realCoord.resize(3);

	//The vectors are initialized
	updatePoints();
	updateVectors();
}
//default constructor
myNewGrid::myNewGrid()
{	
	init(1, cv::Point(100, 100), M_PI / 2, 0, M_PI / 2,  std::vector<bool>(12, 0));
}
//constructor with 1 parameter
myNewGrid::myNewGrid(cv::Point2f CenterGrid, double Alpha)
{	
	init(1, CenterGrid, Alpha, 0, M_PI / 2, std::vector<bool>(12, 0));
}
//constructor with 7 parameters
myNewGrid::myNewGrid(double Scale, cv::Point2f CenterGrid, double Alpha, double Theta, double Phi, std::vector<bool> Id)
{
	init(Scale, CenterGrid, Alpha, Theta, Phi, Id);
}
//destrucor
myNewGrid::~myNewGrid(){}

//Method that update both of the points vectors.
void myNewGrid::updatePoints()
{
	std::vector<cv::Point> tempCont; //auxiliar variable to catch the coordinates	
	//P0 is the center
	relPoints[0] = cv::Point2f(0, 0);		
	absPoints[0] = centerGrid;
	
	for (int i = 1; i < 3; i++)
	{
		//P1 is the orientation
		//P2 is the radius
		ellipse2Poly(cv::Point2f(0, 0), cv::Size(axesTag.width, axesTag.height), angleTag, angleGrid + (i*3) * 30, angleGrid + (i*3+1) * 30, 30, tempCont);
		relPoints[i] = tempCont[0];
		absPoints[i] = relPoints[i] + absPoints[0];					
	}
	return;
}
//Method that updates the vector use to obtain orientation in space.
void myNewGrid::updateVectors()
{	
	//P0 is the center	
	realCoord[0] = cv::Point2f(0, 0);
	for (int i = 1; i < 3; i++)
	{
		//P1 is the orientation
		//P2 is the radius				
		realCoord[i] = ellipsePoint(cv::Point2f(0, 0), cv::Size(axesTag.width, axesTag.height), angleTag, angleGrid + (i * 3) * 30);
	}

	return;
}

//Method that updates the parameters when the tag is translated.
void myNewGrid::translation(cv::Point newCenter)
{
	centerGrid = newCenter;	
	updatePoints();
	return;
}

//Method that updates parameters when the tag orientation is modified.
void myNewGrid::orientation(cv::Point newP1)
{	
	alpha = atan2(newP1.x - centerGrid.x, newP1.y - centerGrid.y) - M_PI / 2;
	angleGrid = (phi - alpha - M_PI / 2) * 180 / M_PI;		
	updatePoints();
	return;
}

//Method that updates the mayor axis using scale and the minor axis using theta as parameter.
void myNewGrid::updateAxes()
{	
	//Updates lengths from scale
	axesTag.width = scale*axisTag;
	axesGrid.width = scale*axisTag*(MR / OR);
	//Updates lengths from theta
	axesTag.height=axesTag.width*cos(theta);
	axesGrid.height=axesGrid.width*cos(theta);	
	return;
}

//function that is called to set the binary ID
void myNewGrid::updateID(cv::Point newID)
{
	for (int i = 0; i < 12; i++)
	{
		if (cv::pointPolygonTest(cellsContours[i], newID, 0) > 0) //check for every cell		
			ID[i] = !ID[i]; //the current value of the cell is changed		
	}	
	return;
}

/////////function that determines coordinates for a point on an ellipse's contour
//Paramters:
//Center -- cv::Point2f -- Xc = Center.x, Yc = Center.y
//SemiAxes -- cv::Size2f -- a = SemiAxes.width, b = SemiAxes.height
//Phi -- double -- phi (angle between the X-axis and a) = Phi -- in degrees, CLOCKWISE
//T -- double -- excentric anomally = T -- in degrees, CLOCKWISE
cv::Point2f myNewGrid::ellipsePoint(cv::Point2f Center, cv::Size2f SemiAxes, double Phi, double T)
{
	cv::Point2f ellPoint;
	//Conversion to radians
	Phi = Phi * M_PI / 180;
	T = T * M_PI / 180;

	ellPoint.x = Center.x + SemiAxes.width*cos(T)*cos(Phi) - SemiAxes.height*sin(T)*sin(Phi);
	ellPoint.y = Center.y + SemiAxes.width*cos(T)*sin(Phi) + SemiAxes.height*sin(T)*cos(Phi);

	//std::cout << "ellPoint.x = " << ellPoint.x << ", ellPoint.y = " << ellPoint.y << std::endl;

	return ellPoint;
}

//updates the tag parameters from the points positions
//s --> scale parameter relative to axisTag
//(x0,y0) --> center of the tag and grid
//alpha --> bee orientation (angle between the "head" of the bee and the x axis)
//theta --> tilt of the tag
//phi --> orientation of the ellipse mayor axis (tag and grid)
void myNewGrid::updateParam()
{
	cv::Point3d		v1;
	cv::Point3d		v2;
	cv::Point3d		norm;
	
	double			temp1;
	double			temp2;

	//calculate the Z component for P1 in order to calculate a normal vector to them through cross product
	v1.x = realCoord[1].x;
	v1.y = realCoord[1].y;
	temp1 = pow(scale*axisTag, 2) - (pow(v1.x, 2) + pow(v1.y, 2));
	if (temp1 < 0)
		v1.z = 0;
	else
		v1.z = sqrt(temp1);	
	//std::cout << "vector1: x = " << relPoints[1].x << ", y = " << relPoints[1].y << "(for image)" << std::endl;
	//std::cout << "vector1: x = " << v1.x << ", y = " << v1.y << ", z = " << v1.z << "(for trigonometry)" << std::endl;
	//calculate the Z component for P2 in order to calculate a normal vector to them through cross product
	v2.x = realCoord[2].x;
	v2.y = realCoord[2].y;
	temp1 = pow(scale*axisTag, 2) - (pow(v2.x, 2) + pow(v2.y, 2));
	if (temp1 < 0)
		v2.z = 0;
	else
		v2.z = sqrt(temp1);		
	//angle between the projection of both vectors in XY	
	temp2 = acos(realCoord[1].dot(realCoord[2]) / (sqrt(realCoord[1].dot(realCoord[1]))*sqrt(realCoord[2].dot(realCoord[2]))));	
	// protection, acos only operates in the range [1,-1]
	if (temp2 >= -1 && temp2 <= 1)
	{
		temp2 = acos(temp2);
		//std::cout << "--------- angle V1 and V2 = " << temp2 << std::endl;
		if (temp2 < M_PI / 2)
			v2.z = -v2.z;
	}	
	//std::cout << "vector2: x = " << relPoints[2].x << ", y = " << relPoints[2].y << "(for image)" << std::endl;
	//std::cout << "vector2: x = " << v2.x << ", y = " << v2.y << ", z = " << v2.z << "(for trigonometry)" << std::endl;
	//cross product and normal to the plane
	norm = v2.cross(v1);
	//to assure that the normal vector points upwards
	if (norm.z<0)
	{
		norm.x = -norm.x;
		norm.y = -norm.y;
		norm.z = -norm.z;
	}
	//std::cout << "normal: x = " << norm.x << ", y = " << norm.y << ", z = " << norm.z << std::endl;	
	
	//roll (theta) and yaw (phi) angles
	cv::Point			x2d(1, 0);
	cv::Point			y2d(0, 1); //we use (0,-1) to get the angle relative to the vertical of the hive, were upwards is 0
	cv::Point3d			z3d(0, 0, 1);
	
	//roll angle, the angle between the normal to the plane and the z-axis is
	temp2 = norm.dot(z3d) / (sqrt(norm.dot(norm)));
	// protection, acos only takes values in the range [1,-1]
	if (temp2 >= -1 && temp2 <= 1)
		theta = acos(temp2);
	//std::cout << "theta " << theta * 180 / M_PI << std::endl;
	//yaw angle
	if (norm.x == 0 && norm.y == 0)
			phi = 0;
	else		
	{		
		temp2 = y2d.dot(cv::Point2d(norm.x, norm.y)) / sqrt(pow(norm.x, 2) + pow(norm.y, 2));		
		// protection, acos only opetrates in the range [1,-1]
		if (temp2 >= -1 && temp2 <= 1)
			phi = acos(temp2);
			//if norm.x component is negative the obtained angle should be taken as negative
			if (norm.x < 0)
				phi = -phi;
		//std::cout << "phi " << phi * 180 / M_PI << std::endl;
	}	
	//alpha is not updated
	//std::cout << "alpha " << alpha * 180 / M_PI << std::endl << std::endl;
	//axes are updated to display the ellipse
	updateAxes();
	//the angles of the ellipse and the grid are calculated for displaying purposes
	angleGrid = (phi-alpha-M_PI/2)*180/M_PI;
	//std::cout << "angleGrid " << angleGrid << std::endl;
	angleTag = -phi * 180 / M_PI;
	//std::cout << "angleTag " << angleTag << std::endl;
	//std::cout << std::endl << "From updateParameters " << std::endl;

	updatePoints();
	return;
}

//function that generates a vector of points for a specific cell from the grid
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
		ellipse2Poly(centerGrid, axesTag, angleTag, 0, 360, 1, globCont);

	return globCont;
}
//function that renders all the necessary cells in a Tag calling renderGridCell
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
//function that draws a grid, active grids are printed in a different color.
void myNewGrid::drawFullTag(cv::Mat &img, int active)
{
	std::vector<std::vector <cv::Point>> conts = renderFullTag();
	//the twelve bit cells are drawn	
	for (int i = 0; i<12; i++)
	{
		//each of the cells contour is colored according to its value
		cv::drawContours(img, conts, i, cv::Scalar(ID[i] * 255, ID[i] * 255, ID[i] * 255), 1);
	}

	//half white circle
	cv::drawContours(img, conts, 12, cv::Scalar(255, 255, 255), 1);
	//half black circle
	cv::drawContours(img, conts, 13, cv::Scalar(0, 0, 0), 1);

	//at the end the diameter is redrawn in green
	cv::drawContours(img, conts, 14, cv::Scalar(0, 255, 0), 1);

	//an ellipse is drawn as the tag contour, color according to active parameter
	if (active == 1) //the tag is active
		cv::drawContours(img, conts, 15, cv::Scalar(0, 255, 255), 1);
	else if (active == 2) //the tag is already set
		cv::drawContours(img, conts, 15, cv::Scalar(0, 255, 0), 1);
	else if (active == 3) //the tag hasn't been set yet
		cv::drawContours(img, conts, 15, cv::Scalar(0, 0, 255), 1);
	return;
}


//function that obtains the cartesians coordinates from polar elements (for arithmetic purposes).
cv::Point2f myNewGrid::polar2rect(double radius, double angle)
{
	cv::Point2f  point;
	point.x = radius * cos(angle);
	point.y = -radius * sin(angle);
	return point;
}