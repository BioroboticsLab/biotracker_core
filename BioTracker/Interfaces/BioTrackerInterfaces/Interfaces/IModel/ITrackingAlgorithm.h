#ifndef ITRACKINGALGORITHM_H
#define ITRACKINGALGORITHM_H

#include "imodel.h"
#include "opencv2/core/core.hpp"

//#include "Interfaces/IModel/ITrackedComponentFactory.h"
//#include "Interfaces/IModel/ITrackedComponent.h"

class ITrackingAlgorithm :public IModel {
    Q_OBJECT
  public:
    ITrackingAlgorithm(QObject *parent = 0);

    virtual void doTracking(std::shared_ptr<cv::Mat> image) = 0;

//    void setTrackedComponentFactory(ITrackedComponentFactory *factory);
//    void setTrackedComponent(ITrackedComponent *trackedComponent);

//    ITrackedComponentFactory *getTrackedComponentFactory();
//    ITrackedComponent *getTrackedComponent();

protected:
//    ITrackedComponentFactory *m_TrackedComponentFactory;
//    ITrackedComponent *m_TrackedComponent;
};

#endif // ITRACKINGALGORITHM_H
