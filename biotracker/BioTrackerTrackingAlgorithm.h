#ifndef BIOTRACKERTRACKINGALGORITHM_H
#define BIOTRACKERTRACKINGALGORITHM_H
#include "Interfaces/imodel.h"

#include <boost/optional.hpp>

#include <opencv2/opencv.hpp>

class ProxyMat {
  public:
    ProxyMat(cv::Mat const &mat)
        : m_originalMat(mat) {
    }

    ProxyMat(const ProxyMat &) = delete;
    ProxyMat &operator=(const ProxyMat &) = delete;

    cv::Mat &getMat() {
        if (!isModified()) {
            m_modifiedMat = m_originalMat.clone();
        }
        return m_modifiedMat.get();
    }

    void setMat(cv::Mat mat) {
        m_modifiedMat = mat;
    }

    bool isModified() const {
        return m_modifiedMat.is_initialized();
    }

  private:
    cv::Mat const &m_originalMat;
    boost::optional<cv::Mat> m_modifiedMat;
};


class BioTrackerTrackingAlgorithm : public IModel {
  public:
    BioTrackerTrackingAlgorithm();

    void track(const cv::Mat &frame);
};

#endif // BIOTRACKERTRACKINGALGORITHM_H
