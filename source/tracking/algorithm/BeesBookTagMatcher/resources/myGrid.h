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

class myGrid
{
public:
	//initializer function
	void init(cv::Point Center, cv::Size Axes, double AngleTag, double AngleGrid, std::vector<bool>	Id);
	//default constructor
	myGrid();
	//constructor with 4 parameters
	myGrid(cv::Point Center, cv::Size Axes, double AngleTag, double AngleGrid, std::vector<bool> Id);	
	//default destructor
	~myGrid();

	//Object properties
	cv::Point								center;
	cv::Size								axes;
	double									angleTag; //the angle of the tag
	double									angleGrid; //the angle of the grid
	double									tilt;
	std::vector<bool>						ID;
	std::vector<std::vector <cv::Point> >	cellsContours;

	double getTilt();
	//function that draws a grid, active grids are printed in a different color
	void drawGrid(cv::Mat &img, bool active);	

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
};
