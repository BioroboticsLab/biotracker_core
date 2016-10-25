#include "BioTrackerTrackingAlgorithm.h"

BioTrackerTrackingAlgorithm::BioTrackerTrackingAlgorithm() {

}

void BioTrackerTrackingAlgorithm::doTracking(const cv::Mat &frame, size_t frameNumber) {
    cv::Mat mat;
    frame.copyTo(mat);
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", frame);

    cv::waitKey(0);
}
