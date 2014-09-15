#ifndef ColorPatchTracker_H
#define ColorPatchTracker_H

#include "cv.h"
#include "source/settings/Settings.h"
#include "source/tracking/TrackingAlgorithm.h"

class ColorPatchTracker : public TrackingAlgorithm
{
	private:
		cv::Mat imgMask;
	public:
		ColorPatchTracker();
		ColorPatchTracker(Settings& settings);
		~ColorPatchTracker(void);
		void track(std::vector<TrackedObject> & objectList, ulong frameNumber, cv::Mat & frame);
		void paint			( cv::Mat& image );
		void reset			( );

	public slots:
		//mouse click and move events
		void mouseMoveEvent		( QMouseEvent * e );
		void mousePressEvent	( QMouseEvent * e );
		void mouseReleaseEvent	( QMouseEvent * e );
};

#endif
