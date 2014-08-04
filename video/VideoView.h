#ifndef VideoView_H
#define VideoView_H

#include <opencv2/opencv.hpp>
#include <QtWidgets/QGraphicsView>

using namespace cv;

/**
* QT widget class to draw video in
*/
class VideoView :
	
	public QGraphicsView
{
	Q_OBJECT

protected:
	QGraphicsScene *_graphicsScene;
	QGraphicsPixmapItem *_pixmapItem;

public:
	VideoView(QWidget *parent = 0);
	virtual ~VideoView();
	void showImage(const Mat& img);


};  
#endif // !VideoView_H
