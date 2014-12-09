#ifndef VideoView_H
#define VideoView_H

#include <memory>
#include <chrono>

#include <QGLWidget>
#include <QtOpenGL>

#include <opencv2/opencv.hpp>

#include "source/settings/Messages.h"

class TrackingAlgorithm;

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
	void keyPressEvent(QKeyEvent *e) override;
	void mouseMoveEvent(QMouseEvent * e) override;
	void mousePressEvent(QMouseEvent * e) override;
	void mouseReleaseEvent(QMouseEvent * e) override;
	void wheelEvent(QWheelEvent * e) override;

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
	std::chrono::system_clock::time_point _lastPannedTime;
	std::chrono::system_clock::time_point _lastZoomedTime;
	QPoint _lastZoomedPoint;

public slots:
	void setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm> trackingAlgorithm);
	void setPanZoomMode (bool isPanZoom);
	cv::Mat getCurrentScreen();
	void fitToWindow();

signals:
	/**
	* send a message to the GUI.
	*/
	void notifyGUI(std::string message, MSGS::MTYPE type = MSGS::MTYPE::NOTIFICATION);
};
#endif // !VideoView_H
