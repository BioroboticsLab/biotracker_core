#include "BioTrackerTrackingAlgorithm.h"
#include <future>
//#include "settings/Settings.h"

BioTrackerTrackingAlgorithm::BioTrackerTrackingAlgorithm(IModel *parameter, IModel *trajectory) 
{
	_TrackingParameter = (TrackerParameter*)parameter;
	_TrackedTrajectoryMajor = (TrackedTrajectory*)trajectory;
	_imageX = 100;
	_imageY = 100;
}
/*
BioTrackerTrackingAlgorithm::~BioTrackerTrackingAlgorithm()
{
	_ofs.close();
}*/

void BioTrackerTrackingAlgorithm::doTracking(std::shared_ptr<cv::Mat> p_image, uint framenumber)
{
	auto start = std::chrono::high_resolution_clock::now();

	//dont do nothing if we ain't got an image
	if (p_image->empty()) {
		return;
	}

	if (_imageX != p_image->size().width || _imageY != p_image->size().height) {
		_imageX = p_image->size().width;
		_imageY = p_image->size().height;
		Q_EMIT emitDimensionUpdate(_imageX, _imageY);
	}

	using namespace cv;

	cv::Mat imgHSV;
	std::shared_ptr<cv::Mat> _imgTracked = std::make_shared<cv::Mat>();

	cvtColor(*p_image, imgHSV,
		COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV


	inRange(imgHSV, Scalar(_TrackingParameter->_lowH, _TrackingParameter->_lowS, _TrackingParameter->_lowV), 
		Scalar(_TrackingParameter->_highH, _TrackingParameter->_highS, _TrackingParameter->_highV),
		*_imgTracked); //Threshold the image

					  //morphological opening (remove small objects from the foreground)
	erode(*_imgTracked, *_imgTracked, getStructuringElement(MORPH_ELLIPSE, Size(5,
		5)));
	dilate(*_imgTracked, *_imgTracked, getStructuringElement(MORPH_ELLIPSE, Size(5,
		5)));

	//morphological closing (fill small holes in the foreground)
	cv::dilate(*_imgTracked, *_imgTracked,
		cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	cv::erode(*_imgTracked, *_imgTracked, cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(5, 5)));

	Q_EMIT emitCvMatA(_imgTracked, QString("Tracked"));


	Q_EMIT emitTrackingDone();
}
