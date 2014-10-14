#include "SampleTracker.h"
#include "source/helper/StringHelper.h"
#include <QApplication>
#include <QIntValidator>
#include <QPushButton>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

SampleTracker::SampleTracker( Settings & settings, QWidget *parent ) : TrackingAlgorithm( settings, parent )
{
	_showSelectorRec = false;
	_selectorRecStart = cv::Point();
	_selectorRecEnd = cv::Point();
	initUI();

}


SampleTracker::~SampleTracker(void)
{
}

void SampleTracker::track		( ulong, cv::Mat& imgOriginal )
{
	//dont do shit if we aint got an image
	if(imgOriginal.empty())
		return;
	using namespace cv;
	int iLowH = 110;
	int iHighH = 130;

	int iLowS = 0; 
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;
	Mat imgHSV;

	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

	Mat imgThresholded;

	inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

	//morphological opening (remove small objects from the foreground)
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	//morphological closing (fill small holes in the foreground)
	dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	namedWindow( "Thresholded Image", WINDOW_AUTOSIZE );
	imshow("Thresholded Image", imgThresholded); //show the thresholded image
	cv::waitKey(10);

}

void SampleTracker::paint		( cv::Mat& image )
{
	//if(_imgTracked.rows > 0)
	//	image = _imgTracked;
	if(_showSelectorRec)
	{
		drawRectangle(image);
	}
}
void SampleTracker::reset		(){}

void SampleTracker::forcePointIntoPicture(cv::Point & point, cv::Mat & image)
{
	//check if any coordinates outside picture
	//and if so change them!
	if ( point.x < 0 )
		point.x = 0;
	if ( point.x > image.cols )
		point.x = image.cols;
	if ( point.y < 0 )
		point.y = 0;
	if ( point.y > image.rows )
		point.y = image.rows;
}

//this will draw a basic rectangle onto the diplay image 
void SampleTracker::drawRectangle(cv::Mat image)
{
	forcePointIntoPicture(_selectorRecStart,image);
	forcePointIntoPicture(_selectorRecEnd,image);

	int width	= _selectorRecEnd.x - _selectorRecStart.x;
	int height	= _selectorRecEnd.y - _selectorRecStart.y;
	int x,y;
	//check into which direction rectangle is stretched
	if		( width < 0 && height > 0 )
	{		x = _selectorRecEnd.x;		y = _selectorRecStart.y;	}
	else if ( width > 0 && height < 0 )
	{		x = _selectorRecStart.x;		y = _selectorRecEnd.y;	}
	else if ( width < 0 && height < 0 )
	{		x = _selectorRecEnd.x;		y = _selectorRecEnd.y;	}
	else
	{		x = _selectorRecStart.x;		y = _selectorRecStart.y;	}

	cv::Mat roi = image(cv::Rect(x, y, abs(width), abs(height)));	

	cv::Mat color(roi.size(), CV_8UC3, cv::Scalar(_blue, _green, _red)); 
	double alpha = 0.3;
	cv::addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi); 
	//easier but without transparent effect:
	//cv::rectangle( image,
	//          _selectorRecStart,
	//          _selectorRecEnd,
	//          cv::Scalar( _blue, _green, _red ),           
	//          4 );
}

void SampleTracker::mouseMoveEvent		( QMouseEvent * e )
{
	if(_showSelectorRec)
	{			
		_selectorRecEnd.x = e->x();
		_selectorRecEnd.y = e->y();
		//draw rectangle!
		emit update();
	}
}
void SampleTracker::mousePressEvent		( QMouseEvent * e )
{
	//check if left button is clicked
	if ( e->button() == Qt::LeftButton)
	{
		int x = e->x(); int y = e->y();
		std::string note = "shift + left button press on: x=" + StringHelper::iToSS(x) + " y=" + StringHelper::iToSS(y);
		//initialize coordinates for selection tool
		_selectorRecStart.x = e->x();
		_selectorRecStart.y = e->y();
		_selectorRecEnd.x = e->x();
		_selectorRecEnd.y = e->y();
		_showSelectorRec = true;
	}
}
void SampleTracker::mouseReleaseEvent	( QMouseEvent * e )
{	
	if ( e->button() == Qt::LeftButton)
	{
		if(_showSelectorRec)
		{
			_selectorRecEnd.x = e->x();
			_selectorRecEnd.y = e->y();
			_showSelectorRec = false;
			//next draw will delete rectangle!
			emit update();
			std::string note = "selected area from " + StringHelper::iToSS(_selectorRecStart.x) + ":"+ StringHelper::iToSS(_selectorRecStart.y)
				+ " to " +  StringHelper::iToSS(_selectorRecEnd.x) + ":"+ StringHelper::iToSS(_selectorRecEnd.y);
			emit notifyGUI(note,MSGS::NOTIFICATION);
		}
	}
}

void SampleTracker::mouseWheelEvent ( QWheelEvent *) {}

QWidget* SampleTracker::getParamsWidget	()
{
	QFrame *paramsFrame = new QFrame(_parent);
	QFormLayout *layout = new QFormLayout;
	initUI();
	layout->addRow("R", _redEdit);
	layout->addRow("G", _greenEdit);
	layout->addRow("B", _blueEdit);
	paramsFrame->setLayout(layout);
	return paramsFrame;
}

QWidget* SampleTracker::getToolsWidget	()
{
	QFrame *toolsFrame = new QFrame(_parent);
	QFormLayout *layout = new QFormLayout;	
	layout->addRow(_colorBut);
	toolsFrame->setLayout(layout);
	return toolsFrame;
}

void SampleTracker::initUI()
{
	_redEdit = new QLineEdit(_parent);
	_greenEdit = new QLineEdit(_parent);
	_blueEdit = new QLineEdit(_parent);
	_colorBut = new QPushButton("change color!", _parent);
	_redEdit->setValidator( new QIntValidator(0, 255, _parent) );

	_greenEdit->setValidator( new QIntValidator(0, 255, _parent) );
	_blueEdit->setValidator( new QIntValidator(0, 255, _parent) );
	_red = 0; _redEdit->setText("0");
	_green = 255; _greenEdit->setText("255");
	_blue = 255; _blueEdit->setText("255");
	QObject::connect(this->_colorBut, SIGNAL(clicked()), this, SLOT(changeRectangleColor()));
}

void SampleTracker::changeRectangleColor()
{
	_red = _redEdit->text().toInt();
	_green = _greenEdit->text().toInt();
	_blue = _blueEdit->text().toInt();
	notifyGUI("box color changed", MSGS::NOTIFICATION);
}