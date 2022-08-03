/****************************************************************************
 **
 ** This file is part of the BioTracker Framework
 ** Origin of this class is BioTracker in version 2.
 **
 ****************************************************************************/

#ifndef BIOTRACKER3PROXYMAT_H
#define BIOTRACKER3PROXYMAT_H

#include "Interfaces/IModel/IModel.h"

#include <boost/optional.hpp>

#include <opencv2/opencv.hpp>

/**
 * The BioTracker3ProxyMat class is currently not used in BioTracker. It is an
 * old class of the BioTracker in version 2.
 */
class BioTracker3ProxyMat
{
public:
    BioTracker3ProxyMat(cv::Mat const& mat)
    : m_originalMat(mat)
    {
    }

    BioTracker3ProxyMat(const BioTracker3ProxyMat&) = delete;
    BioTracker3ProxyMat& operator=(const BioTracker3ProxyMat&) = delete;

    cv::Mat& getMat()
    {
        if (!isModified()) {
            m_modifiedMat = m_originalMat.clone();
        }
        return m_modifiedMat.get();
    }

    void setMat(cv::Mat mat)
    {
        m_modifiedMat = mat;
    }

    bool isModified() const
    {
        return m_modifiedMat.is_initialized();
    }

private:
    cv::Mat const&           m_originalMat;
    boost::optional<cv::Mat> m_modifiedMat;
};

#endif // BIOTRACKER3PROXYMAT_H
