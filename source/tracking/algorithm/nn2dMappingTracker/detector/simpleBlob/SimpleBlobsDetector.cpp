#include "SimpleBlobsDetector.h"
#include "source/helper/CvHelper.h"
#include <limits>

SimpleBlobsDetector::SimpleBlobsDetector(Rectification &rectification, Settings &settings) : 
	IDetector<BlobPose>(rectification,settings),
	_mask(nullptr)
{
	initParams();
}

void SimpleBlobsDetector::initParams()
{
	_params.minThreshold = 0;
	_params.maxThreshold = 255;
	//params.thresholdStep = 100;

	_params.filterByArea = true;
	_params.minArea = 0;
	_params.maxArea = 100;

	_params.filterByConvexity = true;
	_params.minConvexity = 0.0;
	_params.maxConvexity = 100.0;

	//params.maxArea = 8000;
	//params.maxConvexity = 10;
	
	_params.filterByColor = true;
	_params.blobColor = 255;
	
	_params.filterByCircularity = false;
	_params.minDistBetweenBlobs = std::numeric_limits<float>::min();
}

std::vector<BlobPose> SimpleBlobsDetector::findBlobs(const cv::Mat& binImage)
{
	std::vector<BlobPose> blobPoses;

	std::vector<cv::KeyPoint> keyPoints;
	//std::vector<std::vector<cv::Point>> contours;
	//std::vector<std::vector<cv::Point>> approxContours;

	cv::SimpleBlobDetector blobDetector( _params );
	blobDetector.create("SimpleBlob");

	blobDetector.detect( binImage, keyPoints, (_mask ? &IplImage(*_mask) : nullptr) );

	for (int i = 0; i < keyPoints.size(); i++)
	{
		// gets blob center
		int x = keyPoints.at(i).pt.x;
		int y = keyPoints.at(i).pt.y;
		cv::Point blobPose_px = cv::Point(x, y);		

		// apply homography
		cv::Point2f blobPose_cm = _rectification.pxToCm(blobPose_px);

		// ignore blobs outside the tracking area
		if (!_rectification.inArea(blobPose_cm))
			continue;

		float blobPose_angle = keyPoints.at(i).angle;
		float blobPose_width =  keyPoints.at(i).size;
		float blobPose_height = keyPoints.at(i).size;

		blobPoses.push_back(BlobPose(blobPose_cm, blobPose_px, blobPose_angle, blobPose_width, blobPose_height));
	}

	/*std::cout << keyPoints.size() << std::endl;
		
	cv::drawKeypoints( processedImage, keyPoints, out, CV_RGB(255,0,0),cv::DrawMatchesFlags::DEFAULT);*/
		
	/*approxContours.resize( contours.size() );

	for( int i = 0; i < contours.size(); ++i )
	{
			cv::approxPolyDP( cv::Mat(contours[i]), approxContours[i], 4, 1 );
			cv::drawContours( out, contours, i, CV_RGB(rand()&255, rand()&255, rand()&255) );
			cv::drawContours( out, approxContours, i, CV_RGB(rand()&255, rand()&255, rand()&255) );
	}*/

	return blobPoses;
}

std::vector<BlobPose> SimpleBlobsDetector::getPoses(cv::Mat binImage)
{	
	return findBlobs(binImage);
}