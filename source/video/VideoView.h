#ifndef VideoView_H
#define VideoView_H

#include <QGLWidget>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <QtOpenGL>
#include <iostream>
#include "source/helper/StringHelper.h"
#include "source/tracking/TrackingAlgorithm.h"

class VideoView : public QGLWidget
{
	Q_OBJECT
public:
	VideoView(QWidget *parent = 0);	
	void showImage(cv::Mat img);
	void updateDisplay();
	void takeScreenshot(QString screenShotFilename);
	void fitToWindow();

protected:
	void initializeGL(); 
	void paintGL(); 
	void resizeGL(int width, int height);
	void mouseMoveEvent( QMouseEvent * e );
	void mousePressEvent( QMouseEvent * e );
	void mouseReleaseEvent( QMouseEvent * e );
	void wheelEvent( QWheelEvent * e );

private:
	GLuint _texture; 
	QVector<QVector2D> _vertices; 
	QVector<QVector2D> _texCoords;     
	cv::Mat _displayImage;
	TrackingAlgorithm* _tracker;
	bool _isPanZoomMode;

	/* Modified by user input. Initially 1.0 */
	float _zoomFactor;
	float _panX;
	float _panY;
	bool _isPanning;
	int _lastMPos[2];

	public slots:
		void setTrackingAlgorithm(TrackingAlgorithm &trackingAlgorithm);
		void setPanZoomMode (bool isPanZoom);

signals:
		//events for port mouse button 
		void moveEvent			( QMouseEvent * e );
		void pressEvent			( QMouseEvent * e );
		void releaseEvent		( QMouseEvent * e );	
		void mouseWheelEvent	( QWheelEvent * e );
		/**
		* send a message to the GUI.
		*/
		void notifyGUI(std::string message, MSGS::MTYPE type = MSGS::MTYPE::NOTIFICATION);

};
#endif // !VideoView_H
