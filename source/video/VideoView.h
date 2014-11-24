#ifndef VideoView_H
#define VideoView_H

#include <QGLWidget>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <QtOpenGL>
#include <iostream>
#include "source/tracking/TrackingAlgorithm.h"
#include <memory>
#include <chrono>

class VideoView : public QGLWidget
{
	Q_OBJECT
public:
	VideoView(QWidget *parent = nullptr);	
	void showImage(cv::Mat img);
	void updateDisplay();
	void takeScreenshot(QString screenShotFilename);
	using QGLWidget::moveEvent;

protected:
	void initializeGL(); 
	void paintGL(); 
	void resizeGL(int width, int height);
	QPoint unprojectScreenPos(QPoint mouseCoord);
	void mouseMoveEvent( QMouseEvent * e );
	void mousePressEvent( QMouseEvent * e );
	void mouseReleaseEvent( QMouseEvent * e );
	void wheelEvent( QWheelEvent * e );

private:
	GLuint _texture; 
	QVector<QVector2D> _vertices; 
	QVector<QVector2D> _texCoords;     
	cv::Mat _displayImage;
    std::shared_ptr<TrackingAlgorithm> _tracker;
	bool _isPanZoomMode;
	int _currentWidth;
	int _currentHeight;

	/* Modified by user input. Initially 1.0 */
	float _zoomFactor;
	/* ratio of window size to picture size */
	float _screenPicRatio;

	float _panX;
	float _panY;
	bool _isPanning;
	int _lastMPos[2];
	std::chrono::system_clock::time_point _lastZoomedTime;
	QPoint _lastZoomedPoint;

	public slots:
        void setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm> trackingAlgorithm);
		void setPanZoomMode (bool isPanZoom);
		cv::Mat getCurrentScreen();
		void fitToWindow();

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
