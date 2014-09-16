#include "myGrid.h"
#include "source/helper/StringHelper.h"
#include <QApplication>
#include "source/helper/CvHelper.h"

#include "cv.h"
#include "source/settings/Settings.h"
#include "source/tracking/TrackingAlgorithm.h"
#include <iostream>
#include <QPolygon>


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
	int ites = 12;
	
	// render the bit cells
	for (int i = 0; i < ites; i++)
	{		
		std::vector<cv::Point> subCont = renderGridCell(i);
		conts.push_back(subCont);
		cellsContours.push_back(subCont);
	}
	// render the diameter of the grid
	std::vector < cv::Point > cont1;
	cv::ellipse2Poly( center, cv::Size((IR/OR)*axes.width,(IR/OR)*axes.height), angleTag, angleGrid, angleGrid+180, 180, cont1);	
	conts.push_back(cont1);
	
	conts.push_back(std::vector<cv::Point>());	//empty vector to allow the first n vector to be printed VS-2012 error

	//the twelve cells are drawn
	//cv::drawContours(img, conts, -1, cv::Scalar(0, 0, 0), 1);
	for (int i=0; i<12;i++)
	{		
		//each of the cells contour is colored according to its value
		cv::drawContours(img, conts, i, cv::Scalar(ID[i]*255, ID[i]*255, ID[i]*255), 1);
	}

	//half white circle
	cv::ellipse(img, center, cv::Size((IR/OR)*axes.width,(IR/OR)*axes.height), angleTag, angleGrid, angleGrid-180, cv::Scalar(255, 255, 255), 1, 8, 0);	
	//half black circle
	cv::ellipse(img, center, cv::Size((IR/OR)*axes.width,(IR/OR)*axes.height), angleTag, angleGrid, angleGrid+180, cv::Scalar(0, 0, 0), 1, 8, 0);	
	
	//at the end the diameter is redrawn in green
	cv::drawContours(img, conts, 12, cv::Scalar(0, 255, 0), 1);
	
	//an ellipse is drawn as the tag contour, color according to active parameter
	if (active)
		cv::ellipse(img, center, axes, angleTag, 0, 360, cv::Scalar(0, 0, 255), 1, 8, 0);
	else
		cv::ellipse(img, center, axes, angleTag, 0, 360, cv::Scalar(255, 255, 0), 1, 8, 0);
}

std::vector<cv::Point> myGrid::renderGridCell(unsigned short cell){

	std::vector<cv::Point> globCont;
	std::vector<cv::Point> globCont2;
	//std::vector<cv::Point> endCont;

	// Outer cells
	if (cell < 12) {
		
		int arcStart = -180+ angleGrid + cell * 30;
		int arcEnd = -180 + angleGrid + (cell + 1) * 30;

		// outer arc
		ellipse2Poly(center, cv::Size((MR/OR)*axes.width,(MR/OR)*axes.height), angleTag, arcStart, arcEnd, 1, globCont);
		// inner arc
		ellipse2Poly(center, cv::Size((IR/OR)*axes.width,(IR/OR)*axes.height), angleTag, arcStart, arcEnd, 1, globCont2);
		// join outer and inner arc
		globCont.insert(globCont.end(), globCont2.rbegin(), globCont2.rend());	
	}

	// Hier ist irgendein doofes problem auf das ich einfach nicht komme... der debugger sagt was von free aber ich versteh das nicht -.-;
	// Inzwischen geht alles, nur zwischen dem return und dem ruecksprung in die andere Methode kommt es zum crash. Vermutung: ellipse2Poly benutzt irgend nen dynamischen speicherallokier zauber, wodurch beim ruecksprung in die aufrufene methode die freigabe des speichers nicht richtig funktioniert

	return globCont;
}