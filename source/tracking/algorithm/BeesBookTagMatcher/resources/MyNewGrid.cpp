#include "myNewGrid.h"
#include "source/helper/StringHelper.h"
#include <QApplication>

#include "cv.h"
#include "source/settings/Settings.h"
#include "source/tracking/TrackingAlgorithm.h"
#include <iostream>
#include <QPolygon>

//initializion function
void myNewGrid::init(cv::Point CenterGrid, cv::Size AxesGrid, double AngleGrid, cv::Point CenterTag, cv::Size AxesTag,double AngleTag, std::vector<bool> Id){
	//Object properties are initialized
	centerGrid = CenterGrid;
	axesGrid = AxesGrid;
	angleGrid = AngleGrid;
	tiltGrid = getTilt(axesGrid);

	centerTag = CenterTag;
	axesTag = AxesTag;	
	angleTag = AngleTag;
	tiltTag = getTilt(axesTag);

	ID = Id;	
}
//default constructor
myNewGrid::myNewGrid()
{
	init(cv::Point (0,0), cv::Size (axisTag*(MR/OR),axisTag*(MR/OR)), 0, cv::Point (0,0), cv::Size (axisTag,axisTag), 0, std::vector<bool> (12,0));	
}
//constructor with 7 parameters
myNewGrid::myNewGrid(cv::Point CenterGrid, cv::Size AxesGrid, double AngleGrid, cv::Point CenterTag, cv::Size AxesTag, double AngleTag, std::vector<bool> Id)
{
	init(CenterGrid, AxesGrid, AngleGrid, CenterTag, AxesTag, AngleTag,  Id);	
}
//default destrucor
myNewGrid::~myNewGrid(){}

//updates and returns Tilt's value
double myNewGrid::getTilt(cv::Size Axes)
{
	return(double(Axes.height)/Axes.width);
}

void myNewGrid::drawModGrid(cv::Mat &img)
{
	renderModTag();
	
	for (int i=0; i<4;i++)
	{		
		switch (i)
		{
		case 0:	case 1:	//draw middle axes
			//cv::line(img, gridAxes[i][0], gridAxes[i][1], cv::Scalar(0, 255, 0), 1);
			cv::line(img, centerGrid, gridAxes[i][1], cv::Scalar(0, 255, 0), 1);			
			cv::circle(img,gridAxes[i][1], 1, cv::Scalar(0, 0, 255), 1);
			break;				
		
		case 2: case 3:	//draw inner axes				
			cv::circle(img,gridAxes[i][1], 1, cv::Scalar(0, 0, 255), 1);
			break;		
		
		default:
			break;
		}		
	}
	cv::circle(img,centerGrid, 1, cv::Scalar(0, 0, 255), 1);
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
		int arcStart = -180 + cell * 30;
		int arcEnd = -180 + (cell + 1) * 30;
		// outer arc
		ellipse2Poly(centerGrid, cv::Size(axesGrid.width, axesGrid.height), angleGrid, arcStart, arcEnd, 1, globCont);
		// inner arc
		ellipse2Poly(centerGrid, cv::Size((IR/MR)*axesGrid.width,(IR/MR)*axesGrid.height), angleGrid, arcStart, arcEnd, 1, globCont2);
		// join outer and inner arc
		globCont.insert(globCont.end(), globCont2.rbegin(), globCont2.rend());	
	}
	// White semicircle
	if (cell == 12)		
		ellipse2Poly(centerGrid, cv::Size((IR/MR)*axesGrid.width,(IR/MR)*axesGrid.height), angleGrid, 0, -180, 1, globCont);
	// Black semicircle
	if (cell == 13)		
		ellipse2Poly(centerGrid, cv::Size((IR/MR)*axesGrid.width,(IR/MR)*axesGrid.height), angleGrid, 0, 180, 1, globCont);
	// Green diamater
	if (cell == 14)
	cv::ellipse2Poly( centerGrid, cv::Size((IR/MR)*axesGrid.width,(IR/MR)*axesGrid.height), angleGrid, 0, 180, 180, globCont);
	// Outer perimeter
	if (cell == 15)
		ellipse2Poly(centerTag, axesTag, angleTag, 0, 360, 1, globCont);

	return globCont;
}

void myNewGrid::renderModTag()
{
	// axes vector	
	std::vector<cv::Point> subCont;

	gridAxes.clear();	
	//middle axes
	//height
	ellipse2Poly(centerGrid, cv::Size(axesGrid.width, axesGrid.height), angleGrid, 90, 270, 180, subCont);
	gridAxes.push_back(subCont);	
	//width
	ellipse2Poly(centerGrid, cv::Size(axesGrid.width, axesGrid.height), angleGrid, -180, 180, 180, subCont);
	gridAxes.push_back(subCont);	
	//inner axes
	//height
	ellipse2Poly(centerGrid, cv::Size((IR/MR)*axesGrid.width,(IR/MR)*axesGrid.height), angleGrid, 90, 270, 180, subCont);
	gridAxes.push_back(subCont);	
	//width
	ellipse2Poly(centerGrid, cv::Size((IR/MR)*axesGrid.width,(IR/MR)*axesGrid.height), angleGrid, -180, 180, 180, subCont);
	gridAxes.push_back(subCont);	
	//outer axes
	//height
	ellipse2Poly(centerGrid, cv::Size((OR/MR)*axesGrid.width,(OR/MR)*axesGrid.height), angleGrid, 90, 270, 180, subCont);
	gridAxes.push_back(subCont);	
	//width
	ellipse2Poly(centerGrid, cv::Size((OR/MR)*axesGrid.width,(OR/MR)*axesGrid.height), angleGrid, -180, 180, 180, subCont);
	gridAxes.push_back(subCont);
	
	//axesTag parameter is updated
	axesTag.width = axesGrid.width * OR/MR;
	axesTag.height = axesGrid.height * OR/MR;
	return;
}

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

void initPoints(std::vector<cv::Point> points){}