#ifndef BeesBookTagMatcher_H
#define BeesBookTagMatcher_H

#include "cv.h"
#include "source/settings/settings.h"
#include "source/tracking/trackingalgorithm.h"

class BeesBookTagMatcher : public TrackingAlgorithm
{
	public:
		BeesBookTagMatcher	( Settings & settings );
		~BeesBookTagMatcher	( void );
		
		void track			( std::vector<TrackedObject>& objectList, ulong frameNumber, cv::Mat & frame );
		void paint			( cv::Mat& image );
		void reset			( );
};

#endif