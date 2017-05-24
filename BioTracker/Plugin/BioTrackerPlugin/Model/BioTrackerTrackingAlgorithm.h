#ifndef BIOTRACKERTRACKINGALGORITHM_H
#define BIOTRACKERTRACKINGALGORITHM_H


#include "Interfaces/IModel/IModel.h"

#include "TrackerParameter.h"

#include <opencv2/opencv.hpp>
#include "Interfaces/IModel/IModelTrackingAlgorithm.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackedTrajectory.h"
#include "Model/TrackingAlgorithm/imageProcessor/detector/blob/cvBlob/BlobsDetector.h"
#include "Model/TrackingAlgorithm/imageProcessor/preprocessor/ImagePreProcessor.h"
#include "Model/TrackingAlgorithm/NN2dMapper.h"


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

    TrackedTrajectory* _TrackedTrajectoryMajor;

	TrackerParameter* _TrackingParameter;

	ImagePreProcessor _ipp;
	BlobsDetector _bd;
	std::shared_ptr<NN2dMapper> _nn2d;

	// background subtraction
	cv::Ptr<cv::BackgroundSubtractorMOG2> _pMOG;

	int _noFish;

	std::ofstream _ofs;
};

#endif // BIOTRACKERTRACKINGALGORITHM_H
