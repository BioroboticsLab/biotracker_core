#ifndef SampleTracker_H
#define SampleTracker_H

#include "cv.h"
#include "source/settings/Settings.h"
#include "source/tracking/TrackingAlgorithm.h"


#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

class SampleTracker : public TrackingAlgorithm
{
	Q_OBJECT
private:
	cv::Point _selectorRecStart;
	cv::Point _selectorRecEnd;
	bool _showSelectorRec;
	bool _showOriginal;
	void drawRectangle(cv::Mat image);
	void forcePointIntoPicture(cv::Point & point, cv::Mat & image);
	std::vector<TrackedObject>		_trackedObjects;
	void initUI();
	//values for filtering a color (HSV format)
	int _lowH;
	int _highH;
	int _lowS; 
	int _highS;
	int _lowV;
	int _highV;
	cv::Mat _imgTracked;
	//gui elements to set those values
	QLineEdit *_lowHEdit;
	QLineEdit *_highHEdit;
	QLineEdit *_lowSEdit;
	QLineEdit *_highSEdit;
	QLineEdit *_lowVEdit;
	QLineEdit *_highVEdit;

	QPushButton *_colorBut;
	QPushButton *_modeBut;
public:
	SampleTracker	( Settings & settings, QWidget *parent );
	~SampleTracker	( void );




	void track ( ulong frameNumber,
		cv::Mat & frame ) override;
	void paint ( cv::Mat& image ) override;
	void reset ( ) override;
	QWidget* getParamsWidget () override;
	QWidget* getToolsWidget	() override;


	public slots:
		void changeFilterColor();
		void switchMode();
		//mouse click and move events
		void mouseMoveEvent		( QMouseEvent * e );
		void mousePressEvent	( QMouseEvent * e );
		void mouseReleaseEvent	( QMouseEvent * e );
		void mouseWheelEvent	( QWheelEvent * e );

};

#endif
