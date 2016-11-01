#ifndef BIOTRACKERTRACKINGALGORITHM_H
#define BIOTRACKERTRACKINGALGORITHM_H
#include "Interfaces/imodel.h"

#include "BioTracker3TextureObject.h"

#include "Interfaces/ITrackingAlgorithm.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackedObject.h"

#include <boost/optional.hpp>

#include <opencv2/opencv.hpp>


class BioTrackerTrackingAlgorithm : public ITrackingAlgorithm
{
    Q_OBJECT
  public:
    BioTrackerTrackingAlgorithm(QObject *parent = 0, ITrackedComponentFactory *factory = 0);

    void setTextureModel(IModel *model);    

    // ITrackingAlgorithm interface
  public Q_SLOTS:
    void doTracking(cv::Mat image) override;


private:
    BioTracker::Core::BioTracker3TextureObject *m_TextureObject;

    TrackedElement *m_TrackedElement;
};

#endif // BIOTRACKERTRACKINGALGORITHM_H
