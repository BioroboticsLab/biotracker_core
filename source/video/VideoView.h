#ifndef VideoView_H
#define VideoView_H

#include <QGLWidget>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <QtOpenGL>
#include <iostream>
#include "source/helper/StringHelper.h"

class VideoView : public QGLWidget
{
	Q_OBJECT
public:
	VideoView(QWidget *parent = 0);	
	void showImage(cv::Mat img);
	void updateDisplay();
	void takeScreenshot(QString screenShotFilename);

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

	void initializeGL(); 
	void paintGL(); 
    void resizeGL(int width, int height);
private:
	GLuint _texture; 
    QVector<QVector2D> _vertices; 
    QVector<QVector2D> _texCoords;     
    cv::Mat _displayImage;

signals:
	//events for port mouse button 
	void mousePressEventL(QVector2D point);
	void mouseReleaseEventL(QVector2D Point);
	//events for starboard mouse button
	void mousePressEventR(QVector2D point);
	void mouseReleaseEventR(QVector2D point);
	//events for prow mouse button
	void mousePressEventM(QVector2D point);
	void mouseReleaseEventM(QVector2D point);


};
#endif // !VideoView_H
