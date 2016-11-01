#include "BioTrackerTrackingAlgorithm.h"

BioTrackerTrackingAlgorithm::BioTrackerTrackingAlgorithm(QObject *parent, ITrackedComponentFactory *factory) :
    ITrackingAlgorithm(parent)
{
    setTrackedComponentFactory(factory);
}

void BioTrackerTrackingAlgorithm::setTextureModel(IModel *model)
{
    m_TextureObject = dynamic_cast<BioTracker::Core::BioTracker3TextureObject *>(model);
}

void BioTrackerTrackingAlgorithm::doTracking(cv::Mat image)
{
    cv::Mat mat;
    cv::applyColorMap(image, mat, cv::COLORMAP_JET);

    m_TextureObject->set(mat);

    QString str;
    str.resize(6);
    for (int s = 0; s < 6 ; ++s) {
        str[s] = QChar('A' + char(qrand() % ('Z' - 'A')));
    }

    m_TrackedElement = dynamic_cast<TrackedElement *>(m_TrackedComponentFactory->getNewTrackedElement());
    m_TrackedElement->setObjectName(str);


}

//void BioTrackerTrackingAlgorithm::doTracking(const cv::Mat &frame, size_t frameNumber) {
//    cv::Mat mat;
//    frame.copyTo(mat);
//    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
//    cv::imshow("Image", frame);

//    cv::waitKey(0);
//}
