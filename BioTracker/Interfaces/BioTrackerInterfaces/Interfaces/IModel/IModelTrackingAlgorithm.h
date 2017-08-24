#ifndef ITRACKINGALGORITHM_H
#define ITRACKINGALGORITHM_H

#include "IModel.h"
#include "opencv2/core/core.hpp"

class IModelTrackingAlgorithm :public IModel {
    Q_OBJECT
  public:
    IModelTrackingAlgorithm(QObject *parent = 0);

    virtual void doTracking(std::shared_ptr<cv::Mat> image, uint frameNumber) = 0;

signals:
    void emitTrackingDone();
	void emitChangeDisplayImage(QString str);


};

#endif // ITRACKINGALGORITHM_H
