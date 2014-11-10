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
#define _USE_MATH_DEFINES
#include <bitset>

// current tag design
// These parameters are only used when a new tag is set.
// To draw tags from the vector use the parameters from each tag
/* inner radius */
#define IR 1.2
/* middle radius */
#define MR 2.2
/* outer radius */
#define OR 3
/* default size */
#define axisTag 25

// current tag design -- without inner border
/** outer radius to grid size ratio (tag design constant) */

class myNewGrid
{
public:
	//initializer function
	void init(double scale, cv::Point2f CenterGrid, double Alpha, double Theta, double Phi, std::vector<bool> Id);
	//void init(cv::Point2f CenterGrid, cv::Size2f AxesGrid, double Alpha, cv::Point2f CenterTag, cv::Size2f AxesTag, double Phi, std::vector<bool> Id);
	//default constructor
	myNewGrid();
	//constructor with 1 parameter
	myNewGrid(cv::Point2f centerGrid);
	//constructor with 7 parameters
	myNewGrid(double Scale, cv::Point2f CenterGrid, double Alpha, double Theta, double Phi, std::vector<bool> Id);	
	//myNewGrid(cv::Point2f CenterGrid, cv::Size2f AxesGrid, double Alpha, cv::Point2f CenterTag, cv::Size2f AxesTag, double Phi, std::vector<bool> Id);	
	//destructor
	~myNewGrid();

	//Object properties	
	cv::Point2f								centerGrid; //center of the grid
	cv::Point2f								centerTag;  //center of the ellipse defined by the tag itself, usually is equal to centerGrid but it might be diferent

	cv::Size2f								axesGrid;	//major and minor axes of the ellipse defined by the Grid
	cv::Size2f								axesTag;	//major and minor axes of the ellipse defined by the Tag

	double									angleGrid;	//the angle of the grid, is the angle where the bit cells start to be drawn, it is calculated from phi and alpha
	double									angleTag;	//this angle denotes the orientation of the ellipses (both ellipses should have the same phi parameter) (measured from the y-axis)
	
	double									scale;		//scale parameter for the tag, referenced to axisTag.
	double									alpha;		//bee orientation, where 0 degrees is the x-axis and from there the angle is measured counterclockwise
	double									theta;		//this angle denotes the tilt of the grid
	double									phi;		//this angle denotes the orientation of the ellipses (both ellipses should have the same phi parameter) (measured from the y-axis)
	
	double									angleP1P2;	//angle between P0P1 and P0P2


	std::vector<bool>						ID;

	std::vector<std::vector <cv::Point> >	cellsContours;
	
	//std::vector<std::vector <cv::Point> >	gridAxes;
	std::vector<cv::Point2f>				absPoints; //vector of points which are used by the user to define a new grid (coordinates relative to the image)
	std::vector<cv::Point2f>				relPoints; //vector of points which are used by the user to define a new grid (coordinates relative to the center of the grid P0)
	
	//function that generates the set of 9 points that define a grid
	void initPoints();
	//function that updates the set of 9 points that define a grid from parameters.
	void updatePoints(int m);
	//function that calculates the vector of parameters of the grid from points configuration.
	void updateParam();	
	//function that updates the minor axis of the ellipse using theta as parameter.
	void updateAxes();
	//function that draws the points without the tag.
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

	////function that renders tag axes while being modified
	//void renderModTag();		
};