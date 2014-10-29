#ifndef MYNEWGRID_H_
#define MYNEWGRID_H_

// basic file operations
#include <iostream>
//random..
#include <stdio.h>
//openCV
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//tools
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <bitset>

// current tag design -- without inner border
/** outer radius to grid size ratio (tag design constant) */

class myNewGrid
{
public:
	//initializer function
	void init(cv::Point2d CenterGrid, cv::Size2d AxesGrid, double AngleGrid, cv::Point2d CenterTag, cv::Size2d AxesTag,double AngleTag, std::vector<bool> Id);
	//default constructor
	myNewGrid();
	//constructor with 1 parameter
	myNewGrid(cv::Point2d centerGrid);
	//constructor with 7 parameters
	myNewGrid(cv::Point2d CenterGrid, cv::Size2d AxesGrid, double AngleGrid, cv::Point CenterTag, cv::Size2d AxesTag,double AngleTag, std::vector<bool> Id);	
	//destructor
	~myNewGrid();

	//Object properties	
	cv::Point2d								centerGrid;
	cv::Point2d								centerTag;

	cv::Size2d								axesGrid;	//alpha --> the orientation of the bee
	cv::Size2d								axesTag;	//phi -->The 

	double									alpha;		//bee orientation, where 0 degrees is the x-axis and from there the angle is measured counterclockwise
	double									theta;		//this angle denotes the tilt of the grid
	double									phi;		//this angle denotes the orientation of the ellipse (measured from the y-axis)
	double									angleGrid;	//the angle of the grid
	double									angleTag;	//the angle of the tag
	
	double									ratP1P3;	//ratio P0P1/P0P3
	double									ratP2P4;	//ratio P0P2/P0P4	
	double									angleP1P2;	//angle between P0P1 and P0P2


	std::vector<bool>						ID;

	std::vector<std::vector <cv::Point> >	cellsContours;
	std::vector<std::vector <cv::Point> >	gridAxes;
	std::vector<cv::Point2d>				absPoints; //vector of points which are used by the user to define a new grid (coordinates relative to the image)
	std::vector<cv::Point2d>				relPoints; //vector of points which are used by the user to define a new grid (coordinates relative to the center)
	
	//function that generates the set of 9 points that define a grid
	void initPoints(cv::Point2d center);
	//function that updates the set of 9 points that define a grid
	void updatePoints(int m);
	//function that calculates the vector of parameters of the grid.
	void updateParam();
	//function that updates the minor axis of the ellipse using theta as parameter.
	void updateAxes();
	//function that draws the grid without the tag.
	void drawModPoints(cv::Mat &img);
	//function that draws the grid without the tag.
	void drawGrid(cv::Mat &img);
	//function that draws a grid, active grids are printed in a different color.
	void drawFullTag(cv::Mat &img, bool active);

	//function that generates a vector of points for a specific cell from the grid
	/**	 
	 * @ param cell ID between [0,14]; which cell of the grid is to be rendered?
	 * @ param offset angle offset to draw inner half circles with different angles 1 offset = 30°
	 * @ return a vector with the contours of the cell
	 * @ 0-11 bit cells
	 * @ 12 white semicircle
	 * @ 13 black semicircle
	 * @ 14 diameter
	 * @ 15 tag perimeter
	 */
	std::vector<cv::Point> renderGridCell(unsigned short cell);
	
	//function that renders all the necessary cells in a Tag calling renderGridCell
	std::vector<std::vector <cv::Point>> renderFullTag();

	//function that renders tag axes while being modified
	void renderModTag();		
};

#endif
