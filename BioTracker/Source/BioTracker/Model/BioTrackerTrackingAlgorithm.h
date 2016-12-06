#ifndef BIOTRACKERTRACKINGALGORITHM_H
#define BIOTRACKERTRACKINGALGORITHM_H
#include "Interfaces/IModel/imodel.h"

#include "BioTracker3TextureObject.h"

#include "Interfaces/IModel/ITrackingAlgorithm.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackedTrajectory.h"

#include <boost/optional.hpp>

#include <opencv2/opencv.hpp>


class BioTrackerTrackingAlgorithm : public ITrackingAlgorithm
{
    Q_OBJECT
  public:
    BioTrackerTrackingAlgorithm(/*QObject *parent = 0, ITrackedComponentFactory *factory = 0*/);

Q_SIGNALS:
    void emitCvMatA(std::shared_ptr<cv::Mat> image, QString name);

    // ITrackingAlgorithm interface
  public Q_SLOTS:
    void doTracking(std::shared_ptr<cv::Mat> image, uint frameNumber) override;


private:
    TrackedElement *m_TrackedElement;

};

#endif // BIOTRACKERTRACKINGALGORITHM_H
