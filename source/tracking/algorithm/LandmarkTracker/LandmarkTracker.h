#ifndef LandmarkTracker_H
#define LandmarkTracker_H

#include "cv.h"
#include "source/settings/Settings.h"
#include "source/tracking/TrackingAlgorithm.h"

#include "toolwindow.h"


class LandmarkTracker : public TrackingAlgorithm
{
private:
		cv::Point _selectorRecStart;
		cv::Point _selectorRecEnd;
		bool _showSelectorRec;
		
		//KML
		void drawRectangle(cv::Mat image);
		cv::Rect box;
		cv::Mat selectedRoi;
		ToolWindow *toolWindow;



	public:
        LandmarkTracker	( Settings & settings, std::string& serializationPathName, QWidget *parent );
		~LandmarkTracker	( void );
		



		void track			(  ulong frameNumber, cv::Mat & frame );
		void paint			( cv::Mat& image );
		void reset			( );

		//KML
		void defineROI( cv::Mat image );
		void startTool();

		cv::Mat getSelectedRoi();

		void setToolPtr();


	public slots:
		//mouse click and move events
		void mouseMoveEvent		( QMouseEvent * e );
		void mousePressEvent	( QMouseEvent * e );
		void mouseReleaseEvent	( QMouseEvent * e );
		void mouseWheelEvent	( QWheelEvent * e );
		
};

#endif
