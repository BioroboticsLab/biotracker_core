#ifndef VideoView_H
#define VideoView_H

#include <QGLWidget>
#include <cv.h>

class VideoView : public QGLWidget
{
	Q_OBJECT
public:
	VideoView(QWidget *parent = 0);	
	void showImage(cv::Mat img);
	void updateDisplay();

protected:
	 void initializeGL(); 
	 void paintGL(); 
     void resizeGL(int width, int height);
private:
	GLuint texture; 
    QVector<QVector2D> vertices; 
    QVector<QVector2D> texCoords;     
    cv::Mat displayImage;

};
#endif // !VideoView_H
