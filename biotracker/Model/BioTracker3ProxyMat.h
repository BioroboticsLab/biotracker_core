#ifndef BIOTRACKER3PROXYMAT_H
#define BIOTRACKER3PROXYMAT_H

#include "Interfaces/IModel/imodel.h"

#include <boost/optional.hpp>

#include <opencv2/opencv.hpp>

class BioTracker3ProxyMat {
  public:
    BioTracker3ProxyMat(cv::Mat const &mat)
        : m_originalMat(mat) {
    }

    BioTracker3ProxyMat(const BioTracker3ProxyMat &) = delete;
    BioTracker3ProxyMat &operator=(const BioTracker3ProxyMat &) = delete;

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

#endif // BIOTRACKER3PROXYMAT_H
