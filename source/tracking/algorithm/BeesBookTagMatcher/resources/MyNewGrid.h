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
#include <math.h>
#include <bitset>

// current tag design 
/** inner radius */
#define IR 1.2
/** middle radius */
#define MR 2.2
/** outer radius */
#define OR 3

#define axisTag 30

class myNewGrid
{
public:
	//initializer function
	void init(cv::Point CenterGrid, cv::Size AxesGrid, double AngleGrid, cv::Point CenterTag, cv::Size AxesTag,double AngleTag, std::vector<bool> Id);
	//default constructor
	myNewGrid();
	//constructor with 7 parameters
	myNewGrid(cv::Point CenterGrid, cv::Size AxesGrid, double AngleGrid, cv::Point CenterTag, cv::Size AxesTag,double AngleTag, std::vector<bool> Id);	
	//default destructor
	~myNewGrid();

	//Object properties
	cv::Point								centerGrid;
	cv::Point								centerTag;

	cv::Size								axesGrid;
	cv::Size								axesTag;

	double									angleGrid; //the angle of the grid
	double									angleTag; //the angle of the tag

	double									tiltGrid;
	double									tiltTag;

	std::vector<bool>						ID;

	std::vector<std::vector <cv::Point> >	cellsContours;
	std::vector<std::vector <cv::Point> >	gridAxes;

	//function that calculates the tilt for an ellipse with the given axes.
	double getTilt(cv::Size Axes);
	//function that draws the grid axes while being modified.
	void drawModGrid(cv::Mat &img);
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

	//function that initialize a new grid from a vector of five points where
	//P0 -- grid center
	//P1 -- middle radius 1
	//P2 -- inner radius 1
	//P3 -- middle radius 2
	//P4 -- inner radius 2	
	void initPoints(std::vector<cv::Point> points);
};