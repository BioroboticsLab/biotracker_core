#ifndef BIOTRACKERTRACKINGALGORITHM_H
#define BIOTRACKERTRACKINGALGORITHM_H
#include "Interfaces/imodel.h"

#include "BioTracker3ProxyMat.h"
#include "serialization/BioTracker3TrackedObject.h"

#include <boost/optional.hpp>

#include <opencv2/opencv.hpp>


class BioTrackerTrackingAlgorithm {

  public:
    BioTrackerTrackingAlgorithm();

  public Q_SLOTS:
    void doTracking(const cv::Mat &frame, size_t frameNumber);

  Q_SIGNALS:
    void emitTrackingDone(BioTracker3TrackedObject trackedObject);
};

#endif // BIOTRACKERTRACKINGALGORITHM_H
