#ifndef LandmarkTracker_H
#define LandmarkTracker_H


#include <memory>

#include "cv.h"
#include "source/tracking/TrackingAlgorithm.h"

#include "toolwindow.h"
#include "glwidget.h"

class LandmarkTracker : public TrackingAlgorithm
{
private:

	cv::Rect _rectangle;

	cv::Point _selectorRecStart;
	cv::Point _selectorRecEnd;
	bool _showSelectorRec;

	cv::Point _mouseStart;
	cv::Point _mouseStop;

	//KML
	void drawRectangle(cv::Mat image);
	cv::Rect box;
	cv::Mat selectedRoi;

	//ToolWindow *toolWindow;
	//GLWidget *cube;

	void mouseMoveEvent		( QMouseEvent * e ) override;
	void mousePressEvent	( QMouseEvent * e ) override;
	void mouseReleaseEvent	( QMouseEvent * e ) override;

	std::shared_ptr<GLWidget> _glwidget;

public:
	LandmarkTracker	( Settings & settings, QWidget *parent );
	~LandmarkTracker	( void ) {}

	void track			( ulong /* frameNumber */, cv::Mat & /* frame */ ) override {}
	void paint			( cv::Mat& image, View const& view = OriginalView ) override;
	void reset			( ) override {}

	//std::shared_ptr<QWidget> getToolsWidget() override { return std::static_pointer_cast<QWidget>(_glwidget); }


	//KML
	void defineROI( cv::Mat image );
	void startTool();

	cv::Mat getSelectedRoi();

	void setToolPtr();

};

#endif
