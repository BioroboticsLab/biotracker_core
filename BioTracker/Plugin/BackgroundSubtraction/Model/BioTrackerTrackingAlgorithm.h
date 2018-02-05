#ifndef BIOTRACKERTRACKINGALGORITHM_H
#define BIOTRACKERTRACKINGALGORITHM_H


#include "Interfaces/IModel/IModel.h"

#include "TrackerParameter.h"

#include <opencv2/opencv.hpp>
#include "Interfaces/IModel/IModelTrackingAlgorithm.h"
#include "Interfaces/IModel/IModelDataExporter.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackedTrajectory.h"
#include "Model/TrackingAlgorithm/imageProcessor/detector/blob/cvBlob/BlobsDetector.h"
#include "Model/TrackingAlgorithm/imageProcessor/preprocessor/ImagePreProcessor.h"
#include "Model/TrackingAlgorithm/NN2dMapper.h"
#include "Interfaces/IModel/IModelAreaDescriptor.h"
#include <iostream>

#include "Model/Network/TcpListener.h"

class BioTrackerTrackingAlgorithm : public IModelTrackingAlgorithm
{
    Q_OBJECT
public:
    BioTrackerTrackingAlgorithm(IModel* parameter, IModel* trajectory);
	~BioTrackerTrackingAlgorithm();

Q_SIGNALS:
    void emitCvMatA(std::shared_ptr<cv::Mat> image, QString name);
	void emitDimensionUpdate(int x, int y);
	void emitTrackingDone(uint framenumber);

    // ITrackingAlgorithm interface
public Q_SLOTS:
	void doTracking(std::shared_ptr<cv::Mat> image, uint framenumber) override;
	void receiveAreaDescriptorUpdate(IModelAreaDescriptor *areaDescr);
    void receiveParametersChanged();

private:
	void refreshPolygon();
    void sendSelectedImage(std::map<std::string, std::shared_ptr<cv::Mat>>* images);

	std::vector<FishPose> getLastPositionsAsPose();

    TrackedTrajectory* _TrackedTrajectoryMajor;
	TrackerParameter* _TrackingParameter;
	IModelAreaDescriptor* _AreaInfo;

	TcpListener* _listener;

	ImagePreProcessor _ipp;
	BlobsDetector _bd;
	std::shared_ptr<NN2dMapper> _nn2d;

	// background subtraction
	cv::Ptr<cv::BackgroundSubtractorMOG2> _pMOG;

	int _noFish;

	//std::ofstream _ofs;

	int _imageX;
	int _imageY;

    std::shared_ptr<cv::Mat> _lastImage;
    uint _lastFramenumber;
};

#endif // BIOTRACKERTRACKINGALGORITHM_H
