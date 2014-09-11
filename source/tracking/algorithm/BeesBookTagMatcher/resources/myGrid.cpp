#include "myGrid.h"

myGrid::myGrid(cv::Point Center, cv::Size Axes, double Angle){
	center	= Center;
	axes	= Axes;
	angle	= Angle;
}

myGrid::~myGrid(){}
