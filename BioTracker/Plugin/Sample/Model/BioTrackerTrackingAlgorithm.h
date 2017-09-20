#ifndef BIOTRACKERTRACKINGALGORITHM_H
#define BIOTRACKERTRACKINGALGORITHM_H


#include "Interfaces/IModel/IModel.h"

#include "TrackerParameter.h"

#include <opencv2/opencv.hpp>
#include "Interfaces/IModel/IModelTrackingAlgorithm.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackedTrajectory.h"
#include <iostream>
#include <fstream>


namespace sampleTracker {
	class BioTrackerTrackingAlgorithm : public IModelTrackingAlgorithm
	{
		Q_OBJECT
	public:
		BioTrackerTrackingAlgorithm(IModel* parameter, IModel* trajectory/*QObject *parent = 0, ITrackedComponentFactory *factory = 0*/);

		int _imageX;
		int _imageY;

	Q_SIGNALS:
		void emitCvMatA(std::shared_ptr<cv::Mat> image, QString name);
		void emitDimensionUpdate(int x, int y);

		// ITrackingAlgorithm interface
		public Q_SLOTS:
		void doTracking(std::shared_ptr<cv::Mat> image, uint framenumber) override;

	private:

		TrackedTrajectory* _TrackedTrajectoryMajor;

		TrackerParameter* _TrackingParameter;

	};
}
#endif // BIOTRACKERTRACKINGALGORITHM_H
