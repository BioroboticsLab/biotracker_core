#include "myGrid.h"
#include <QApplication>

#include "cv.h"
#include "source/settings/Settings.h"
#include "source/tracking/TrackingAlgorithm.h"
#include <iostream>
#include <QPolygon>

// current tag design -- without inner border
/** outer radius to grid size ratio (tag design constant) */
#define IR 1.2
/** tag radius to grid size ratio */
#define MR 2.2
/** inner outer radius to grid size ratio */
#define OR 3

#define axisTag 30

void myGrid::init(cv::Point Center, cv::Size Axes, double AngleTag, double AngleGrid, std::vector<bool>	Id){
	center		= Center;
	axes		= Axes;
	angleTag	= AngleTag;
	angleGrid	= AngleGrid;
	tilt		=double(axes.height)/axes.width;
	ID			= Id;
}
//default constructor
myGrid::myGrid(){
	init(cv::Point(0,0), cv::Size(axisTag,axisTag), 0, 0, std::vector<bool> (12,0));	
}
//constructor with 4 parameters
myGrid::myGrid(cv::Point Center, cv::Size Axes, double AngleTag, double AngleGrid, std::vector<bool>	Id){
	init(Center, Axes, AngleTag, AngleGrid, Id);	
}
//default destrucor
myGrid::~myGrid(){}

//updates and returns Tilt's value
double myGrid::getTilt(){
	return(tilt	=double(axes.height)/axes.width);
}

void myGrid::drawGrid(cv::Mat &img, bool active) {
			
	// contour vector
	std::vector<std::vector <cv::Point> >	conts;	
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
		//only the bit cells area stored in cellsContours
		if (i<12 || i==15)			
			cellsContours.push_back(subCont);
	}	
	conts.push_back(std::vector<cv::Point>());	//empty vector to allow the first n vector to be printed VS-2012 error

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
}

std::vector<cv::Point> myGrid::renderGridCell(unsigned short cell){

	std::vector<cv::Point> globCont;
	std::vector<cv::Point> globCont2;	

	// Bit cells
	if (cell < 12)
	{	
		int arcStart = -180+ angleGrid + cell * 30;
		int arcEnd = -180 + angleGrid + (cell + 1) * 30;
		// outer arc
		ellipse2Poly(center, cv::Size((MR/OR)*axes.width,(MR/OR)*axes.height), angleTag, arcStart, arcEnd, 1, globCont);
		// inner arc
		ellipse2Poly(center, cv::Size((IR/OR)*axes.width,(IR/OR)*axes.height), angleTag, arcStart, arcEnd, 1, globCont2);
		// join outer and inner arc
		globCont.insert(globCont.end(), globCont2.rbegin(), globCont2.rend());	
	}
	// White semicircle
	if (cell == 12)		
		ellipse2Poly(center, cv::Size((IR/OR)*axes.width,(IR/OR)*axes.height), angleTag, angleGrid, angleGrid-180, 1, globCont);
	// Black semicircle
	if (cell == 13)		
		ellipse2Poly(center, cv::Size((IR/OR)*axes.width,(IR/OR)*axes.height), angleTag, angleGrid, angleGrid+180, 1, globCont);
	// Green diamater
	if (cell == 14)
	cv::ellipse2Poly( center, cv::Size((IR/OR)*axes.width,(IR/OR)*axes.height), angleTag, angleGrid, angleGrid+180, 180, globCont);
	// Outer perimeter
	if (cell == 15)
		ellipse2Poly(center, axes, angleTag, 0, 360, 1, globCont);

	return globCont;
}