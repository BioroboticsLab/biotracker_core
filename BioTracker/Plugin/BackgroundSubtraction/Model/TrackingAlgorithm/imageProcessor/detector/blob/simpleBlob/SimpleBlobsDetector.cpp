#include "SimpleBlobsDetector.h"
#include "helper/CvHelper.h"
#include <limits>

SimpleBlobsDetector::SimpleBlobsDetector(void) : _mask(nullptr)
{
	initParams();
}

void SimpleBlobsDetector::initParams()
{
	_params.minThreshold = 15;//SystemProperty::instance().getValueOfParam<double>(TRACKERPARAM::THRESHOLD_BINARIZING);
	_params.maxThreshold = 50;//SystemProperty::instance().getValueOfParam<double>(TRACKERPARAM::THRESHOLD_BINARIZING); // THRESHOLD_BINARIZING=30
	_params.thresholdStep = 5;	

	_params.filterByArea = true;
	// blobs bigger than the provided blob size
	_params.minArea = 1;//SystemProperty::instance().getValueOfParam<double>(TRACKERPARAM::MIN_BLOB_SIZE);
	_params.maxArea = 999999;//SystemProperty::instance().getValueOfParam<double>(TRACKERPARAM::MAX_BLOB_SIZE);

	//_params.filterByConvexity = true;	
	//_params.minConvexity = SystemProperty::instance().getValueOfParam<double>(TRACKERPARAM::MIN_BLOB_SIZE);
	//_params.maxConvexity = SystemProperty::instance().getValueOfParam<double>(TRACKERPARAM::MAX_BLOB_SIZE);
	
	_params.filterByInertia = false;
	//_params.minInertiaRatio = 0.0f;
	//_params.maxInertiaRatio = 1.0f;
	
	_params.filterByColor = true;
	_params.blobColor = 255;
	
	_params.filterByCircularity = false;

	_params.minDistBetweenBlobs = 0; //std::numeric_limits<float>::min();
}


std::vector<BlobPose> SimpleBlobsDetector::findBlobs(const cv::Mat& binImage, const cv::Mat& oriImage)
{
	std::vector<BlobPose> blobPoses;

	std::vector<cv::KeyPoint> keyPoints;
	//std::vector<std::vector<cv::Point>> contours;
	//std::vector<std::vector<cv::Point>> approxContours;

	//initParams();
	cv::Ptr<cv::SimpleBlobDetector> blobDetector = cv::SimpleBlobDetector::create(_params);

	blobDetector->detect( binImage, keyPoints/*, (_mask ? &IplImage(*_mask) : nullptr) */);

	for (int i = 0; i < keyPoints.size(); i++)
	{
		// gets blob center
		int x = keyPoints.at(i).pt.x;
		int y = keyPoints.at(i).pt.y;
		cv::Point blobPose_px = cv::Point(x, y);		

		// apply homography
		cv::Point2f blobPose_cm = Rectification::instance().pxToCm(blobPose_px);

		// ignore blobs outside the tracking area
		//if (!Rectification::instance().inArea(blobPose_cm))
		//	continue;
		if (!_areaInfo->_apperture->insideElement(blobPose_px))
			continue;

		float blobPose_angleDegree = keyPoints.at(i).angle; // as degree (0..360)
		float blobPose_angleRadian = keyPoints.at(i).angle * float(CV_PI) / float(180.0);
		float blobPose_width =  keyPoints.at(i).size;
		float blobPose_height = keyPoints.at(i).size;

		blobPoses.push_back(BlobPose(blobPose_cm, blobPose_px, blobPose_angleDegree, blobPose_angleRadian, blobPose_width, blobPose_height));
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


std::vector<BlobPose> SimpleBlobsDetector::getPoses(cv::Mat& binarized_image_mat, cv::Mat& original_image_mat)
{	
	return findBlobs(binarized_image_mat,original_image_mat);
}

void SimpleBlobsDetector::setDouble(std::string spec_param, double value)
{	
	if(spec_param.compare("1") == 0) { //TRACKERPARAM::MIN_BLOB_SIZE
		_params.minArea = value;
	} else {
		std::cout << "SimpleBlobsDetector::Warning - Parameter: " << spec_param << " not found!" << std::endl;
	}

	if(spec_param.compare("999999") == 0) { //TRACKERPARAM::MAX_BLOB_SIZE
		_params.maxArea = value;
	} else {
		std::cout << "SimpleBlobsDetector::Warning - Parameter: " << spec_param << " not found!" << std::endl;
	}
}
