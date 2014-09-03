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
	void initializeGL(); 
	void paintGL(); 
    void resizeGL(int width, int height);
	void mouseMoveEvent( QMouseEvent * e );
	void mousePressEvent( QMouseEvent * e );
	void mouseReleaseEvent( QMouseEvent * e );

private:
	GLuint _texture; 
    QVector<QVector2D> _vertices; 
    QVector<QVector2D> _texCoords;     
    cv::Mat _displayImage;

signals:
	//events for port mouse button 
	void moveEvent( QMouseEvent * e );
	void pressEvent( QMouseEvent * e );
	void releaseEvent( QMouseEvent * e );

};
#endif // !VideoView_H
