#ifndef SampleTracker_H
#define SampleTracker_H

#include "cv.h"
#include "source/settings/Settings.h"
#include "source/tracking/TrackingAlgorithm.h"

class SampleTracker : public TrackingAlgorithm
{
	private:
		cv::Point _selectorRecStart;
		cv::Point _selectorRecEnd;
		bool _showSelectorRec;
		void drawRectangle(cv::Mat image);

	public:
		SampleTracker	( Settings & settings );
		~SampleTracker	( void );
		



		void track			( std::vector<TrackedObject>& objectList, ulong frameNumber, cv::Mat & frame );
		void paint			( cv::Mat& image );
		void reset			( );


	public slots:
		//mouse click and move events
		void mouseMoveEvent		( QMouseEvent * e );
		void mousePressEvent	( QMouseEvent * e );
		void mouseReleaseEvent	( QMouseEvent * e );
		
};

#endif