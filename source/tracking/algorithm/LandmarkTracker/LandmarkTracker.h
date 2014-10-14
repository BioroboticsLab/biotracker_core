#ifndef LandmarkTracker_H
#define LandmarkTracker_H

#include "cv.h"
#include "source/settings/Settings.h"
#include "source/tracking/TrackingAlgorithm.h"


class LandmarkTracker : public TrackingAlgorithm
{
private:
		cv::Point _selectorRecStart;
		cv::Point _selectorRecEnd;
		bool _showSelectorRec;
		
		//KML
		void drawRectangle(cv::Mat image);
		cv::Rect box;
		cv::Mat _image;
		bool _clonedImage;


	public:
		LandmarkTracker	( Settings & settings );
		~LandmarkTracker	( void );
		



		void track			( std::vector<TrackedObject>& objectList, ulong frameNumber, cv::Mat & frame );
		void paint			( cv::Mat& image );
		void reset			( );

		void defineROI( cv::Mat image );


	public slots:
		//mouse click and move events
		void mouseMoveEvent		( QMouseEvent * e );
		void mousePressEvent	( QMouseEvent * e );
		void mouseReleaseEvent	( QMouseEvent * e );
		void mouseWheelEvent	( QWheelEvent * e );
		
};

#endif