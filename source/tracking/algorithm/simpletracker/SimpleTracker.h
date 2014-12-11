#ifndef SimpleTracker_H
#define SimpleTracker_H

#include <opencv2/opencv.hpp>
#include <QMutex>

#include "source/tracking/TrackingAlgorithm.h"
#include "TrackedFish.h"
#include "FishCandidate.h"

class SimpleTracker : public TrackingAlgorithm
{
public:
	static const float MAX_TRACK_DISTANCE_PER_FRAME;
	static const float MAX_TRACK_DISTANCE;
	static const int   CANDIDATE_SCORE_THRESHOLD;
	static const int   MAX_NUMBER_OF_TRACKED_OBJECTS;

	SimpleTracker(Settings & settings, QWidget *parent);

	void track (ulong frameNumber, cv::Mat& frame) override;
	void reset () override;
	void paint (cv::Mat& image) override;

private:
	cv::BackgroundSubtractorMOG2 _bg_subtractor;
	std::vector<FishCandidate>   _fish_candidates;

	QMutex  lastFrameLock;
	cv::Mat lastFrame;
};

#endif
