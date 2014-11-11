#pragma once

#include <cv.h>
#include <opencv2/opencv.hpp>

#include <QtCore/QSettings>

#include "source/helper/stdext.h"
#include "source/settings/Settings.h"


class ImagePreProcessor
{
public:

	/**
	 * The constructor with provided parameters.
	 * @param params, parameters for the image pre-processing.
	 */
	ImagePreProcessor(Settings &settings);

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
	cv::Mat binarize(cv::Mat image);

	/**
	 * Sets the threshold value for a parameter
	 * @param: paramName, the parameter name,
	 * @param: threshValue, threshold value,
	 * @return: void.
	 */
	void setThreshold(std::string paramName, std::string threshValue);

	/**
	 * A mathematical morphology operation using in computer vision to erode an image.
	 * Erode image with 3x3 4-connectivity.
	 * @param: image, image to erode,
	 * @return: an eroded image.
	 */
	cv::Mat erode(cv::Mat image);	

	/**
	 * A mathematical morphology operation using in computer vision to dilate an image.
	 * Dilate image with 6x6 8-connectivity.
	 * @param: image, image to dilate,
	 * @return: a dilated image.
	 */
	cv::Mat dilate(cv::Mat image);

	/**
	 * A computer vision methode to calculate the image difference.
	 * Background image subtracts the foreground image.
	 * @param: image, image to background subtract,
	 * @return: the background subtracted image.
	 */
	cv::Mat backgroundSubtraction(cv::Mat image);

	/**
	 * Pre-process an image, if all methods enabled, this function:
	 * - does the background subtraction
	 * - erodes the image
	 * - dilates the image
	 * @param: image, image to process,
	 * @return: a pre-process image.
	 */
	void preProcess(cv::Mat image);

	/**
	 * The method updates the image background.
	 * @return: void.
	 */
	void resetBackgroundImage();

	/**
	 * Method to enable image pre-processing methods
	 * @ param: enableProcess, specifies the process to enable or disable,
	 * @ param: enable, true to enable, false to disable.
	 * @return void.
	 */
	void setEnable(std::string enableProcess, bool enable = true);

	/**
	 * Gets displayed image type for displaying it.
	 * @param: imageDisplayType, kind of image need to retrieve.
	 * Hint: IMAGE_BI, binarized image,
	 *		 IMAGE_ER, eroded image,
	 *		 IMAGE_DI, dilated image,
	 *		 IMAGE_FG, foreground mask image,
	 *		 IMAGE_BG, background image,
	 *		 IMAGE_PP, final proceedied image,
	 * @return: type of image.
	 */
	cv::Mat imageDisplayType(TRACKERPARAM::IMAGE_DISPLAY_TYPE imageDisplayType);

private:
	// System properties
	Settings& _settings;

	// variable of the various processing image
	cv::Mat _original;
	cv::Mat _proceededImage;
	cv::Mat _fgMaskMOG;
	cv::Mat _backgroundImage;
	cv::Mat _binarizedImage;
	cv::Mat _erodedImage;
	cv::Mat _dilatedImage;

	// background subtraction
    std::unique_ptr<cv::BackgroundSubtractorMOG2> _pMOG;

	//parameters for image pre-processing
	bool _backgroundSubtractionEnabled;
	bool _backgroundEnabled;
	bool _binarizeEnabled;
	bool _erodeEnabled;
	bool _dilateEnabled;
};
