#include "SimpleTracker.h"

#include <opencv.hpp>

class BgSub : public cv::BackgroundSubtractorMOG2 {
public:
	BgSub() {
		bShadowDetection = true;
		nShadowDetection = 0;
		fTau = 0.90;
	}
};

const float SimpleTracker::MAX_TRACK_DISTANCE_PER_FRAME = 10;

const float SimpleTracker::MAX_TRACK_DISTANCE = 50;

const int SimpleTracker::CANDIDATE_SCORE_THRESHOLD = 50;

SimpleTracker::SimpleTracker(Settings& settings) : TrackingAlgorithm(settings), _bg_subtractor(BgSub())
{
}


SimpleTracker::~SimpleTracker(void)
{
}

cv::Mat SimpleTracker::track(std::vector<TrackedObject> objectList, cv::Mat frame) {
	try {
		std::vector<std::vector<cv::Point> > contours;
		cv::Mat foreground;
		cv::Mat background;
		_bg_subtractor.operator ()(frame, foreground);
		_bg_subtractor.getBackgroundImage(background);

		cv::erode(foreground, foreground, cv::Mat());

		cv::dilate(foreground, foreground, cv::Mat());

		cv::findContours(foreground, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		std::vector<std::vector<cv::Point> > contours_poly(contours.size());
		std::vector<cv::Point2f> center(contours.size());
		float radius_dummy;
		for(int i = 0; i < contours.size(); i++) {
			cv::minEnclosingCircle(contours[i], center[i], radius_dummy);
		}
		std::vector<cv::Point2f> contourCenters(center.begin(), center.end());
		// Now we know the centers of all the detected contours in the picture (center)

		// TRACKING
		// (1) Find the next contour belonging to each tracked fish (oldest fish first)
		for (TrackedFish& trackedFish : _tracked_fish) {
			unsigned age = trackedFish.age_of_last_known_position();
			float maxRange = min(age * MAX_TRACK_DISTANCE_PER_FRAME, MAX_TRACK_DISTANCE);
			cv::Point2f currentPosition = trackedFish.last_known_position();

			cv::Point2f minDistanceContour;
			float minDistance = MAX_TRACK_DISTANCE + 1;
			for (const cv::Point2f& contour : contourCenters) {
				cv::Point distPoint = contour - currentPosition;
				float distance = cv::sqrt((distPoint.x * distPoint.x) + (distPoint.y * distPoint.y));
				if (distance < minDistance) {
					minDistance = distance;
					minDistanceContour = contour;
				}
			}
			if (minDistance < maxRange) {
				trackedFish.setNextPosition(minDistanceContour);
				contourCenters.erase(std::find(contourCenters.begin(), contourCenters.end(), minDistanceContour));
			} else {
				trackedFish.setNextPositionUnknown();
			}
		}

		// (2) Try to find contours belonging to fish candidates, promoting to TrackedFish as appropriate
		std::vector<FishCandidate> candidates_to_promote;
		std::vector<FishCandidate> candidates_to_drop;
		for (FishCandidate& candidate : _fish_candidates) {
			cv::Point2f currentPosition = candidate.last_known_position();
			float minDistance = MAX_TRACK_DISTANCE + 1;
			cv::Point2f minDistanceContour;
			for (const cv::Point2f& contour : contourCenters) {
				cv::Point2f distPoint = contour - currentPosition;
				float distance = cv::sqrt((distPoint.x * distPoint.x) + (distPoint.y * distPoint.y));
				if (distance < minDistance) {
					minDistance = distance;
					minDistanceContour = contour;
				}
			}

			if (minDistance < MAX_TRACK_DISTANCE_PER_FRAME) {
				candidate.setNextPosition(minDistanceContour);
				candidate.increaseScore();
				contourCenters.erase(std::find(contourCenters.begin(), contourCenters.end(), minDistanceContour));
				if (candidate.score() > CANDIDATE_SCORE_THRESHOLD) {
					candidates_to_promote.push_back(candidate);
				}
			} else {
				candidate.decreaseScore();
				candidate.setNextPositionUnknown();
				if (candidate.score() < 0) {
					candidates_to_drop.push_back(candidate);
				}
			}
		}

		// (2.5) Drop/Promote candidates.
		for (FishCandidate& promoted : candidates_to_promote) {
			TrackedFish newFish(_tracked_fish.size() + 1, History());
			newFish.setNextPosition(promoted.last_known_position());
			_tracked_fish.push_back(newFish);
			_fish_candidates.erase(std::find(_fish_candidates.begin(), _fish_candidates.end(), promoted));
		}
		candidates_to_promote.clear();
		for (FishCandidate& dropped : candidates_to_drop) {
			_fish_candidates.erase(std::find(_fish_candidates.begin(), _fish_candidates.end(), dropped));
		}
		candidates_to_drop.clear();

		// (3) Create new candidates for unmatched contours
		for (cv::Point2f& contour : contourCenters) {
			FishCandidate newCandidate;
			newCandidate.setNextPosition(contour);
			_fish_candidates.push_back(newCandidate);
		}

		// END OF TRACKING

		// Draw the result
		cv::Mat canvas = frame;

		cv::RNG rng(12345);
		for (TrackedFish& fish : _tracked_fish) {
			//cv::Scalar color1(0, 0, 255);
			cv::Scalar color_rand(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
			//cv::drawContours(canvas, contours, i, color1, 2, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
			cv::circle(canvas, fish.last_known_position(), 2, color_rand, -1, 8, 0 );
		}

		for (FishCandidate& candidate : _fish_candidates) {
			cv::Scalar color(255, 0, 0);
			cv::circle(canvas, candidate.last_known_position(), 2, color, -1, 8, 0 );
		}

		return canvas;

	} catch(const cv::Exception& e) {
		return cv::Mat();
	}
}

void SimpleTracker::reset() {
	_bg_subtractor = BgSub();
	// TODO	clear other structures
}
