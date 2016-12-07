#ifndef ITRACKINGALGORITHM_H
#define ITRACKINGALGORITHM_H

#include "IModel.h"
#include "opencv2/core/core.hpp"

//#include "Interfaces/IModel/ITrackedComponentFactory.h"
//#include "Interfaces/IModel/ITrackedComponent.h"

class IModelTrackingAlgorithm :public IModel {
    Q_OBJECT
  public:
    IModelTrackingAlgorithm(QObject *parent = 0);

    virtual void doTracking(std::shared_ptr<cv::Mat> image, uint frameNumber) = 0;

signals:
    void emitTrackingDone();

//    void setTrackedComponentFactory(ITrackedComponentFactory *factory);
//    void setTrackedComponent(ITrackedComponent *trackedComponent);

//    ITrackedComponentFactory *getTrackedComponentFactory();
//    ITrackedComponent *getTrackedComponent();

protected:
//    ITrackedComponentFactory *m_TrackedComponentFactory;
//    ITrackedComponent *m_TrackedComponent;
};

#endif // ITRACKINGALGORITHM_H
