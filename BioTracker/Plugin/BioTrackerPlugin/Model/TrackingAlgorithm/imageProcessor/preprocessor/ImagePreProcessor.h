#pragma once

#include <opencv2/opencv.hpp>


#include <QtCore/QSettings>

#include "helper/StringHelper.h"
#include "Model/TrackingAlgorithm/property/SystemProperty.h"
#include "Model/TrackerParameter.h"


class ImagePreProcessor
{
public:

	/**
	 * The standard constructor.
	 */
	ImagePreProcessor(TrackerParameter* p_TrackingParameter);

	/**
	 * The standard destructor.
	 */
	~ImagePreProcessor(void);

	/**
	 * Init function. Sets the property for the imge pre-processing.
	 * @return void.
	 */
	void init();

	/**
	 * A computer vision method to binarize an image.
	 * @param: image, image to binarize,
	 * @return: a binarized image.
	 */
	cv::Mat binarize(cv::Mat& image);

	/**
	 * A mathematical morphology operation using in computer vision to erode an image.
	 * Erode image with 3x3 4-connectivity.
	 * @param: image, image to erode,
	 * @return: an eroded image.
	 */
	cv::Mat erode(cv::Mat& image);	

	/**
	 * A mathematical morphology operation using in computer vision to dilate an image.
	 * Dilate image with 6x6 8-connectivity.
	 * @param: image, image to dilate,
	 * @return: a dilated image.
	 */
	cv::Mat dilate(cv::Mat& image);

	/**
	 * A computer vision methode to calculate the image difference.
	 * Background image subtracts the foreground image.
	 * @param: image, image to background subtract,
	 * @return: the background subtracted image.
	 */
	cv::Mat backgroundSubtraction(cv::Mat& image);

	/**
	 * Pre-process an image, if all methods enabled, this function:
	 * - does the background subtraction
	 * - erodes the image
	 * - dilates the image
	 * @param: image, image to process,
	 * @return: a pre-process image.
	 */
	std::map<std::string, std::shared_ptr<cv::Mat>> preProcess(std::shared_ptr<cv::Mat> p_image);

	/**
	 * The method updates the image background.
	 * @return: void.
	 */
	void resetBackgroundImage();
	TrackerParameter* m_TrackingParameter;
	
private:


	TRACKERPARAM::IMAGE_DISPLAY_TYPE _outputImageType;
	cv::Mat _outputImage;

	int m_Mog2ShadowDetection = true;
	std::shared_ptr<cv::Mat> m_backgroundImage;
	std::shared_ptr<cv::Mat> m_foregroundImage;

	// background subtraction
	cv::Ptr<cv::BackgroundSubtractorMOG2> _pMOG;

	//parameters for image pre-processing
	bool _backgroundSubtractionEnabled;
	bool _backgroundEnabled;
	bool _binarizeEnabled;
	bool _erodeEnabled; 
	bool _dilateEnabled;
	bool _gaussianBlurEnabled;
	bool _resetBackgroundImageEnabled;

	int _maxBackgroundImageInitTime;
	bool _bkgSubMethodMog2;

	TrackerParameter* _TrackingParameter;

	// functions
	void setBkgFrameNum(int);
	int getBkgFrameNum();

	void setEnabledMog2(bool);
	bool isEnabledMog2();
};
