#ifndef BIOTRACKERTRACKINGALGORITHM_H
#define BIOTRACKERTRACKINGALGORITHM_H


#include "Interfaces/IModel/IModel.h"

#include "Interfaces/IModel/IModelTrackingAlgorithm.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackedTrajectory.h"
#include "TrackerParameter.h"

#include <opencv2/opencv.hpp>
#include "imageProcessor/detector/blob/cvBlob/BlobsDetector.h"
#include "imageProcessor/preprocessor/ImagePreProcessor.h"
#include "fish/Fish.h"
#include "fish/NN2dMapper.h"


class BioTrackerTrackingAlgorithm : public IModelTrackingAlgorithm
{
    Q_OBJECT
  public:
    BioTrackerTrackingAlgorithm(IModel* parameter, IModel* trajectory/*QObject *parent = 0, ITrackedComponentFactory *factory = 0*/);

Q_SIGNALS:
    void emitCvMatA(std::shared_ptr<cv::Mat> image, QString name);

    // ITrackingAlgorithm interface
  public Q_SLOTS:
    void doTracking(std::shared_ptr<cv::Mat> image, uint framenumber) override;


private:
	std::vector<FishPose> getLastPositionsAsPose();

    TrackedTrajectory *m_TrackedTrajectoryMajor;

	TrackerParameter* m_TrackingParameter; 

	ImagePreProcessor _ipp;
	BlobsDetector _bd;
	std::vector<Fish> m_fishList;
	NN2dMapper *_nn2d;

	int m_frameCount;

	// background subtraction
	cv::Ptr<cv::BackgroundSubtractorMOG2> _pMOG;
};

#endif // BIOTRACKERTRACKINGALGORITHM_H
