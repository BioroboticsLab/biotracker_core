#include "SampleTracker.h"
#include <QApplication>
#include <QIntValidator>
#include <QPushButton>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

SampleTracker::SampleTracker(Settings & settings, std::string &serializationPathName, QWidget *parent )
    : TrackingAlgorithm( settings, serializationPathName, parent )
{
	_showSelectorRec = false;
	_showOriginal = false;
	initUI();
	_lowH = 0;
	_highH= 255;
	_lowS = 0; 
	_highS = 255;
	_lowV = 0;
	_highV = 255;

}


SampleTracker::~SampleTracker(void)
{
}

void SampleTracker::track		( ulong, cv::Mat& imgOriginal )
{
	//dont do nothing if we ain't got an image
	if(imgOriginal.empty())
		return;
	using namespace cv;

	Mat imgHSV;

	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV


	inRange(imgHSV, Scalar(_lowH, _lowS, _lowV), Scalar(_highH, _highS, _highV), _imgTracked); //Threshold the image

	//morphological opening (remove small objects from the foreground)
	erode(_imgTracked, _imgTracked, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	dilate( _imgTracked, _imgTracked, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	//morphological closing (fill small holes in the foreground)
	dilate( _imgTracked, _imgTracked, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	erode(_imgTracked, _imgTracked, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

}

void SampleTracker::paint		( cv::Mat& image )
{
	if(!_imgTracked.empty() && !_showOriginal)
		//since gui is expecting a BGR image we have to convert our grayscale image
		cv::cvtColor(_imgTracked, image, CV_GRAY2BGR);		
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

	cv::Mat color(roi.size(), CV_8UC3, cv::Scalar(255, 255, 0)); 
	double alpha = 0.3;
	cv::addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi); 
	//easier but without transparent effect:
	//cv::rectangle( image,
	//          _selectorRecStart,
	//          _selectorRecEnd,
	//          cv::Scalar( 255, 255, 0 ),           
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
		std::string note = "shift + left button press on: x=" 
			+ QString::number(x).toStdString() + " y=" + QString::number(y).toStdString();
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
			std::string note = "selected area from " + QString::number(_selectorRecStart.x).toStdString() + ":"+ 
				QString::number(_selectorRecStart.y).toStdString()
				+ " to " +  QString::number(_selectorRecEnd.x).toStdString() + ":"+ 
				QString::number(_selectorRecEnd.y).toStdString();
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
	layout->addRow("H-low",  _lowHEdit);
	layout->addRow("H-high", _highHEdit);
	layout->addRow("S-low",  _lowSEdit);
	layout->addRow("S-high", _highSEdit);
	layout->addRow("V-low",  _lowVEdit);
	layout->addRow("V-high", _highVEdit);
	paramsFrame->setLayout(layout);
	return paramsFrame;
}

QWidget* SampleTracker::getToolsWidget	()
{
	QFrame *toolsFrame = new QFrame(_parent);
	QFormLayout *layout = new QFormLayout;	
	layout->addRow(_colorBut);
	layout->addRow(_modeBut);
	toolsFrame->setLayout(layout);
	return toolsFrame;
}

void SampleTracker::initUI()
{
	_lowHEdit  = new QLineEdit(_parent);
	_highHEdit = new QLineEdit(_parent);
	_lowSEdit  = new QLineEdit(_parent);
	_highSEdit = new QLineEdit(_parent);
	_lowVEdit  = new QLineEdit(_parent);
	_highVEdit = new QLineEdit(_parent);

	_colorBut = new QPushButton("change color!", _parent);
	_modeBut = new QPushButton("show Original!", _parent);
	_lowHEdit->setValidator( new QIntValidator(0, 255, _parent) );
	_highHEdit->setValidator( new QIntValidator(0, 255, _parent) );
	_lowSEdit->setValidator( new QIntValidator(0, 255, _parent) );
	_highSEdit->setValidator( new QIntValidator(0, 255, _parent) );
	_lowVEdit->setValidator( new QIntValidator(0, 255, _parent) );
	_highVEdit->setValidator( new QIntValidator(0, 255, _parent) );

	_lowH = 0;		_lowHEdit->setText("0");
	_highH= 255;	_highHEdit->setText("255");
	_lowS = 0;		_lowSEdit->setText("0");
	_highS = 255;	_highSEdit->setText("255");
	_lowV = 0;		_lowVEdit->setText("0");
	_highV = 255;	_highVEdit->setText("255");
	QObject::connect(this->_colorBut, SIGNAL(clicked()), this, SLOT(changeFilterColor()));
	QObject::connect(this->_modeBut, SIGNAL(clicked()), this, SLOT(switchMode()));
}

void SampleTracker::changeFilterColor()
{
	_lowH  =	_lowHEdit->text().toInt();
	_highH =	_highHEdit->text().toInt();
	_lowS  =	_lowSEdit->text().toInt();
	_highS =	_highSEdit->text().toInt();
	_lowV  =	_lowVEdit->text().toInt();
	_highV =	_highVEdit->text().toInt();	
}

void SampleTracker::switchMode()
{
	_showOriginal = !_showOriginal;
	if (_showOriginal)
		_modeBut->setText("show Filter!");
	else
		_modeBut->setText("show Original!");
	emit update();
}
