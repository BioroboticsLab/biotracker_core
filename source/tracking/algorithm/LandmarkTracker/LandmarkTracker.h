#ifndef LandmarkTracker_H
#define LandmarkTracker_H

#include <memory>
#include <string>
#include "glwidget.h"
#include "RectObject.h"
#include "cv.h"
#include "source/tracking/TrackingAlgorithm.h"

#include <QLabel>
#include <QFormLayout>
#include <QPushButton>

class LandmarkTracker : public TrackingAlgorithm
{
	Q_OBJECT

private:
	
	//boost::optional<size_t>	_frameNr;
	//boost::optional<size_t> _rectId;
	size_t	_frameNr;
	size_t _rectId;
	
	cv::Rect _rectangle;

	cv::Point _selectorRecStart;
	cv::Point _selectorRecEnd;
	bool _showSelectorRec;

	cv::Point _mouseStart;
	cv::Point _mouseStop;

	//KML
	cv::Point pointOfText();
	cv::Point pot;
	
	std::string rectInfo();

	void drawRectangle(cv::Mat image);
	void setRectangle(cv::Mat image, cv::Scalar color);

	void lockRect();
	void unlockRect();
	
	cv::Rect box;
	cv::Mat selectedRoi;
	cv::Mat sampledRoi;

	boost::optional<cv::Mat> img;
	bool _lockRect;

	QPixmap _pixmap;

	QPixmap mat2QPixmap(const cv::Mat& mat);

	void mouseMoveEvent		( QMouseEvent * e ) override;
	void mousePressEvent	( QMouseEvent * e ) override;
	void mouseReleaseEvent	( QMouseEvent * e ) override;
	void keyPressEvent(QKeyEvent *e) override;

	virtual std::set<Qt::Key> const& grabbedKeys() const override;

	bool _movedRect;

	bool _mouseMoved;
	bool _cubeIsRoi;

	
	std::shared_ptr<GLWidget> _glwidget;
	std::shared_ptr<QFrame> _roiFrame;

	std::shared_ptr<RectObject>	_rectObject;

	//ParamsWidget GUI Elements

	QLabel *_roiInParams;
	QLabel *_sampledRoiInParams;
	
	QPushButton *_roiButton;
	QPushButton *_sampledRoiButton;

	void initParamsWidget();
	void updateParamsWidget();

	void addRect2TrackedObj();

	void setRect();

public:

	cv::Scalar COLOR_RECTANGLE = cv::Scalar(0, 0, 255);

	LandmarkTracker	( Settings & settings, QWidget *parent );
	~LandmarkTracker	( void ) {}

	void track(ulong /* frameNumber */, cv::Mat & /* frame */) override;
	void paint			( cv::Mat& image, View const& view = OriginalView ) override;
	void reset			( ) override {}
	void postLoad() override;

	std::shared_ptr<QWidget> getToolsWidget() override { return std::static_pointer_cast<QWidget>(_glwidget); }
	std::shared_ptr<QWidget> getParamsWidget() override { return std::static_pointer_cast<QWidget>(_roiFrame); }

	//KML
	void defineROI( cv::Mat image );
	void startTool();

	cv::Mat getSelectedRoi();

	void samplingROI();

public slots:
	
	void setCubeRoi();
	void setCubeSampledRoi();

};

#endif
