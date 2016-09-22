#include "BioTrackerTrackingAlgorithm.h"

BioTrackerTrackingAlgorithm::BioTrackerTrackingAlgorithm() {

}

void BioTrackerTrackingAlgorithm::track(const cv::Mat &frame) {
    cv::Mat mat;
    frame.copyTo(mat);
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", frame);

    cv::waitKey(0);
}
