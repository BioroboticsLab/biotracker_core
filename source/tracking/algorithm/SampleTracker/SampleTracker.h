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
		void drawRectangle(cv::Mat image);
		void forcePointIntoPicture(cv::Point & point, cv::Mat & image);
		void initUI();
		int _red;
		int _green;
		int _blue;

		QLineEdit *_redEdit;
		QLineEdit *_greenEdit;
		QLineEdit *_blueEdit;
		QPushButton *_colorBut;
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
		void changeRectangleColor();
		//mouse click and move events
		void mouseMoveEvent		( QMouseEvent * e );
		void mousePressEvent	( QMouseEvent * e );
		void mouseReleaseEvent	( QMouseEvent * e );
		void mouseWheelEvent	( QWheelEvent * e );
				
};

#endif
