#ifndef LandmarkTracker_H
#define LandmarkTracker_H

#include "cv.h"
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

	void mouseMoveEvent		( QMouseEvent * e ) override;
	void mousePressEvent	( QMouseEvent * e ) override;
	void mouseReleaseEvent	( QMouseEvent * e ) override;

public:
	LandmarkTracker	( Settings & settings, QWidget *parent );
	~LandmarkTracker	( void ) {}

	void track			( ulong /* frameNumber */, cv::Mat & /* frame */ ) override {}
	void paint			( cv::Mat& image ) override;
	void reset			( ) override {}

	//KML
	void defineROI( cv::Mat image );
	void startTool();

	cv::Mat getSelectedRoi();

	void setToolPtr();
};

#endif
