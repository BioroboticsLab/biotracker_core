#include "ContoursDetector.h"
#include "source/helper/CvHelper.h"

std::vector<ContourPose> ContoursDetector::findContours(cv::Mat image_mat)
{
	std::vector<ContourPose> contourPoses;

	

	return contourPoses;
}

void ContoursDetector::createMask(std::vector<cv::Point> points)
{

}

std::vector<ContourPose> ContoursDetector::getPoses(cv::Mat image_mat)
{	
	return findContours(image_mat);
}