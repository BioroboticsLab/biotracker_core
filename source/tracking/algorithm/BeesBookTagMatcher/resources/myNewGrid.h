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

#include <math.h>
#define _USE_MATH_DEFINES
#include <bitset>

// current tag design -- without inner border
/** inner radius */
#define IR 1.2
/** middle radius */
#define MR 2.2
/** outer radius */
#define OR 3

#define axisTag 25

class myNewGrid
{
public:
	//initializer function
	void init(double scale, cv::Point2f CenterGrid, double Alpha, double Theta, double Phi, std::vector<bool> Id);	
	//default constructor
	myNewGrid();
	//constructor with 1 parameter
	myNewGrid(cv::Point2f centerGrid, double Alpha);
	//constructor with 7 parameters
	myNewGrid(double Scale, cv::Point2f CenterGrid, double Alpha, double Theta, double Phi, std::vector<bool> Id);	
	//destructor
	~myNewGrid();

	//Object properties	
	cv::Point2f								centerGrid; //center of the grid	

	cv::Size2f								axesGrid;	//major and minor axes of the ellipse defined by the Grid
	cv::Size2f								axesTag;	//major and minor axes of the ellipse defined by the Tag

	double									angleGrid;	//the angle of the grid, is the angle where the bit cells start to be drawn, it is calculated from phi and alpha
	double									angleTag;	//this angle denotes the orientation of the ellipses (both ellipses should have the same phi parameter) (measured from the y-axis)
	
	double									scale;		//scale parameter for the tag, referenced to axisTag.
	double									alpha;		//bee orientation, where 0 degrees is the x-axis and from there the angle is measured counterclockwise
	double									theta;		//this angle denotes the tilt of the grid
	double									phi;		//this angle denotes the orientation of the ellipses (both ellipses should have the same phi parameter) (measured from the y-axis)
	
	//test
	//double									rho;		//this is the angle between orientation and mid-circle vectors.
	
	std::vector<bool>						ID;

	std::vector<std::vector <cv::Point> >	cellsContours;
		
	std::vector<cv::Point2f>				absPoints;	//vector of points which are used by the user to define a new grid (absolute coordinates, relative to the image)	
	std::vector<cv::Point2f>				relPoints;	//vector of points which are used by the user to define a new grid (coordinates relative to the center of the grid P0)	
	std::vector<cv::Point2f>				realCoord;	//vector of points which are used by the user to define a new grid (relative and exact coordinates for computing purposes)
	
	//Object methods
	//function that updates the set of 3 points that define a grid from parameters (for displaying pursoses).
	void updatePoints();
	//function that updates the set of 3 points that define a grid from parameters (for arithmetic purposes).
	void updateVectors();
	//function that updates parameters when the tag is translated.
	void translation(cv::Point newCenter);
	//function that updates parameters when the tag orientation is modified.
	void orientation(cv::Point newP1);
	//function that updates the mayor axis and the minor axis using theta and scale as parameters.
	void updateAxes();
	//function that is called to set the binary ID
	void updateID(cv::Point newID);

	//function that determines coordinates for a point on an ellipse's contour (for arithmetic purposes).
	cv::Point2f ellipsePoint(cv::Point2f Center, cv::Size2f SemiAxes, double Phi, double T);

	//function that calculates the vector of parameters of the grid from points configuration.
	void updateParam();		

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
	//function that draws a grid, active grids are printed in a different color.
	void drawFullTag(cv::Mat &img, int active);

	//function that obtains the cartesians coordinates from polar elements (for arithmetic purposes).
	cv::Point2f polar2rect(double radius, double angle);
};

#endif
