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

// current tag design -- without inner border
/** outer radius to grid size ratio (tag design constant) */
#define IR 1.2
/** tag radius to grid size ratio */
#define MR 2.2
/** inner outer radius to grid size ratio */
#define OR 3

#define axisTag 30
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
	 * @param cell ID between [0,11]; which cell of the grid is to be rendered?
	 * @param offset angle offset to draw inner half circles with different angles 1 offset = 30°
	 * @return a vector with the contours of the cell
	 */
	std::vector<cv::Point> renderGridCell(unsigned short cell);
};