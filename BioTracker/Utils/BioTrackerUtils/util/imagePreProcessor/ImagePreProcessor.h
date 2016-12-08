#pragma once

#include <opencv2/opencv.hpp>

#include "util/stdext.h"

namespace BioTracker {
namespace Core {

namespace PREPROCESSORPARAM {
// Parameter for the opencv BackgroundSubtractorMOG2 class
static const std::string BG_MOG2_HISTORY            =
    "TRACKERPARAM.BG_MOG2_HISTORY";
static const std::string BG_MOG2_VAR_THRESHOLD      =
    "TRACKERPARAM.BG_MOG2_VAR_THRESHOLD";
static const std::string BG_MOG2_SHADOW_DETECTION   =
    "TRACKERPARAM.BG_MOG2_SHADOW_DETECTION";
static const std::string BG_MOG2_BACKGROUND_RATIO   =
    "TRACKERPARAM.BG_MOG2_BACKGROUND_RATIO";

// Parameters for enabling methods
static const std::string ENABLE_BG_SUBTRACTION      =
    "TRACKERPARAM.ENABLE_BG_SUBTRACTION";
static const std::string ENABLE_BG_IMAGE            =
    "TRACKERPARAM.ENABLE_BG_IMAGE";
static const std::string ENABLE_BINARIZING          =
    "TRACKERPARAM.ENABLE_BINARIZING";
static const std::string ENABLE_ERODE               =
    "TRACKERPARAM.ENABLE_ERODE";
static const std::string ENABLE_DILATE              =
    "TRACKERPARAM.ENABLE_DILATE";
static const std::string IMAGE_VIEW_TYPE            =
    "TRACKERPARAM.IMAGE_VIEW_TYPE";

// Mapping issue
static const std::string MAX_MAPPED_DISTANCE_PX =
    "TRACKERPARAM.MAX_MAPPED_DISTANCE_PX";
static const std::string MAX_MAPPED_DISTANCE_CM =
    "TRACKERPARAM.MAX_MAPPED_DISTANCE_CM";

// Blob dectection issue
static const std::string MAX_BLOB_SIZE              =
    "TRACKERPARAM.MAX_BLOB_SIZE";
static const std::string MIN_BLOB_SIZE              =
    "TRACKERPARAM.MIN_BLOB_SIZE";

// Parameters for image pre-processing step
static const std::string SIZE_ERODE                 = "TRACKERPARAM.SIZE_ERODE";
static const std::string SIZE_DILATE                =
    "TRACKERPARAM.SIZE_DILATE";
static const std::string THRESHOLD_BINARIZING       =
    "TRACKERPARAM.THRESHOLD_BINARIZING";

// Image processing type
enum IMAGE_DISPLAY_TYPE {
    IMAGE_OR = 0,
    IMAGE_BI = 1,
    IMAGE_ER = 2,
    IMAGE_DI = 3,
    IMAGE_FG = 4,
    IMAGE_BG = 5,
    IMAGE_PP = 6,
};
}

class Settings;

class ImagePreProcessor {
  public:

    /**
     * The constructor with provided parameters.
     * @param settings reference to the biotracker settings object
     */
    ImagePreProcessor(Settings &settings);

    /**
     * Init function. Sets the property for the imge pre-processing.
     */
    void init();

    /**
     * A computer vision method to binarize an image.
     * @param image image to binarize,
     * @return a binarized image.
     */
    cv::Mat binarize(cv::Mat image);

    /**
     * Sets the threshold value for a parameter
     * @param paramName the parameter name,
     * @param threshValue threshold value,
     */
    void setThreshold(std::string paramName, std::string threshValue);

    /**
     * A mathematical morphology operation using in computer vision to erode an image.
     * Erode image with 3x3 4-connectivity.
     * @param image image to erode,
     * @return an eroded image.
     */
    cv::Mat erode(cv::Mat image);

    /**
     * A mathematical morphology operation using in computer vision to dilate an image.
     * Dilate image with 6x6 8-connectivity.
     * @param image image to dilate,
     * @return a dilated image.
     */
    cv::Mat dilate(cv::Mat image);

    /**
     * A computer vision methode to calculate the image difference.
     * Background image subtracts the foreground image.
     * @param image image to background subtract,
     * @return the background subtracted image.
     */
    cv::Mat backgroundSubtraction(cv::Mat image);

    /**
     * Pre-process an image, if all methods enabled, this function:
     * - does the background subtraction
     * - erodes the image
     * - dilates the image
     * @param image image to process,
     */
    void preProcess(cv::Mat image);

    /**
     * The method updates the image background.
     */
    void resetBackgroundImage();

    /**
     * Method to enable image pre-processing methods
     * @param enableProcess specifies the process to enable or disable,
     * @param enable true to enable, false to disable.
     */
    void setEnable(std::string enableProcess, bool enable = true);

    /**
     * Gets displayed image type for displaying it.
     * @param imageDisplayType kind of image need to retrieve.
     * Hint: IMAGE_BI, binarized image,
     *       IMAGE_ER, eroded image,
     *       IMAGE_DI, dilated image,
     *       IMAGE_FG, foreground mask image,
     *       IMAGE_BG, background image,
     *       IMAGE_PP, final proceedied image,
     * @return type of image.
     */
    cv::Mat imageDisplayType(PREPROCESSORPARAM::IMAGE_DISPLAY_TYPE
                             imageDisplayType);

  private:
    // System properties
    Settings &_settings;

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

}
}

