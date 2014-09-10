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

class myGrid
{
public:
	myGrid(cv::Point center, cv::Size Axes);
	~myGrid();

	cv::Point center;	
	cv::Size axes;
};