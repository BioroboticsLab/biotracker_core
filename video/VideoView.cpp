#include <QGraphicsPixmapItem>
#include <video/VideoView.h>

VideoView::VideoView(QWidget *parent) :
	QGraphicsView(parent),	
	_graphicsScene(new QGraphicsScene(this)),
	_pixmapItem(_graphicsScene->addPixmap(QPixmap()))
{
	setParent(parent);
}

VideoView::~VideoView() 
{
	delete _graphicsScene;
}

void VideoView::showImage(const cv::Mat& img)
{		
	const QImage displayImg = QImage((const uchar*)img.data, img.cols, img.rows, QImage::Format_RGB888).rgbSwapped();
	_pixmapItem->setPixmap(QPixmap::fromImage(displayImg));
	setScene(_graphicsScene);
	show();
}