#include "LandmarkTracker.h"
#include <QApplication>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "source/tracking/algorithm/algorithms.h"
#include "source/tracking/TrackingThread.h"


namespace {
    auto _ = Algorithms::Registry::getInstance().register_tracker_type<LandmarkTracker>("Landmark Tracker");
}

LandmarkTracker::LandmarkTracker(Settings & settings, QWidget *parent)
	: TrackingAlgorithm(settings, parent)
	, _glwidget(std::make_shared<GLWidget>())
	, _roiFrame(std::make_shared<QFrame>())
	, _selectorRecStart()
	, _selectorRecEnd()
	, _showSelectorRec(false)
	, _mouseMoved(false)
	, _cubeIsRoi(true)
	, _lockRect(false)
	, _movedRect(false)
		
{
	//KML
	std::cout<<"LANDMARK TRACKER"<<std::endl;
	initParamsWidget();

	//Buttons in Params & Tool-Widget
	QObject::connect(this->_roiButton, SIGNAL(clicked()), this, SLOT(setLabelRoi()));
	QObject::connect(this->_sampledRoiButton, SIGNAL(clicked()), this, SLOT(setLabelSampledRoi()));

	if (_trackedObjects.empty())
	{
		const size_t dummyID = 0;
		_trackedObjects.emplace_back(dummyID);
	}
	else if (_trackedObjects.size()>1)
	{
		std::cerr << "Unexpected number of trackedObjects: " << _trackedObjects.size() << std::endl;
	}
}


void LandmarkTracker::track(ulong /* frameNumber */, const cv::Mat & /* frame */)
{
	setRect();
}


void LandmarkTracker::paint	(ProxyPaintObject& p  , const View&)
{
	
auto &image = p.getmat();
	if(_showSelectorRec)
	{
		drawRectangle(image);
	}
	
}

//this will draw a basic rectangle onto the diplay image 
void LandmarkTracker::drawRectangle(cv::Mat image)
{
	cv::rectangle(image, _selectorRecStart, _selectorRecEnd, COLOR_RECTANGLE, 1, 8);
	
	if (_lockRect)
	{
		cv::putText(image, rectInfo(), pointOfText(), cv::FONT_HERSHEY_PLAIN, 0.8, COLOR_RECTANGLE, 1, 8, false);
	}
}

//Calculate the position of the text above the frame
cv::Point LandmarkTracker::pointOfText()
{
	if (_selectorRecStart.x < _selectorRecEnd.x){
		pot = _selectorRecStart;
	}
	else{
		pot = _selectorRecEnd;
	}
	pot.y -= 5;
	
	return pot;
}

//ID and Frame Number of Rectangle 
std::string LandmarkTracker::rectInfo()
{
	std::string fNr;

	auto ptr = _trackedObjects[0].maybeGet<RectObject>(getCurrentFrameNumber());
	if (ptr != nullptr){
		fNr = std::to_string(ptr->getRectFrameNr());
	}
	else fNr= "N.A.";

	std::string info = "FrameID: " + fNr;
	return info;
}

void LandmarkTracker::postLoad()
{
	_showSelectorRec = true;
	std::cout << "Loaded TrackingsObjects..." << std::endl;
	setRect();
}

void LandmarkTracker::setRect()
{
		auto rectPtr = _trackedObjects[0].maybeGet<RectObject>(getCurrentFrameNumber());
		
		if (rectPtr != nullptr){
			lockRect();
			_selectorRecStart = rectPtr->getRectStart();
			_selectorRecEnd = rectPtr->getRectEnd();

			img = getCurrentImageCopy();
			defineROI(img.get());
		}
		else{
			unlockRect();
			_glwidget->clearPixelCubes();
		}
		
		emit update();

}

void LandmarkTracker::addRect2TrackedObj()
{
	_frameNr = getCurrentFrameNumber();
	
	_rectObject = std::make_shared<RectObject>(_frameNr, _selectorRecStart, _selectorRecEnd);
	_trackedObjects.front().add(getCurrentFrameNumber(), _rectObject);

	emit update();
}

void LandmarkTracker::lockRect()
{
	_lockRect = true;
	COLOR_RECTANGLE = cv::Scalar(0, 255, 0);
}

void LandmarkTracker::unlockRect()
{
	_lockRect = false;
	COLOR_RECTANGLE = cv::Scalar(0, 0, 255);
}

//Mat2QPixmap
QPixmap LandmarkTracker::mat2QPixmap(const cv::Mat &mat)
{
	cv::Mat rgb;
	QPixmap p;

	cvtColor(mat, rgb, CV_BGR2RGB);
	QImage dest(static_cast<const unsigned char*>(rgb.data), rgb.cols, rgb.rows, static_cast<int>(rgb.step), QImage::Format_RGB888);
	dest.bits();
	p.convertFromImage(dest);

	return p;
}

/*
*	Mouse and keyboard event handler
*/
void LandmarkTracker::mouseMoveEvent		( QMouseEvent * e )
{
    //const bool ctrlModifier = QApplication::keyboardModifiers().testFlag(Qt::ControlModifier);
	const bool _leftMouseHold = QApplication::mouseButtons().testFlag(Qt::LeftButton);
	const bool _rightMouseHold = QApplication::mouseButtons().testFlag(Qt::RightButton);

	_mouseMoved = true;

	cv::Point mousePosition(e->x(), e->y());

	if (_leftMouseHold)
	{
		_selectorRecEnd.x = e->x();
		_selectorRecEnd.y = e->y();		
		//draw rectangle!
		emit update();
		
	}

	if (_rightMouseHold /*&& ctrlModifier*/ && !_lockRect)
	{
		// position of mouse cursor
		if (!mousePosition.inside(cv::Rect(_rectangle)))
			forcePointIntoBorders(mousePosition, _rectangle);

		if (cv::Rect(_selectorRecStart, _selectorRecEnd).contains(cv::Point(e->x(),e->y())))
		{
			_mouseStop.x = e->x();
			_mouseStop.y = e->y();

			_selectorRecStart += _mouseStop - _mouseStart;
			_selectorRecEnd += _mouseStop - _mouseStart;

			_showSelectorRec = true;
			_movedRect = true;

			emit update();
		}
		
		_mouseStart = _mouseStop;
	}	
}

void LandmarkTracker::mousePressEvent		( QMouseEvent * e )
{
	// position of mouse cursor 
	cv::Point mousePosition(e->x(), e->y());

	// restrict mouse pointer to live inside image borders only
	if (!mousePosition.inside(cv::Rect(_rectangle)))
		forcePointIntoBorders(mousePosition, _rectangle);

	//check if left button is clicked
	if ( e->button() == Qt::LeftButton)
	{
		//Set rect color to red
		COLOR_RECTANGLE = cv::Scalar(0, 0, 255);
		//initialize coordinates for selection tool
		_selectorRecStart.x = e->x();
		_selectorRecStart.y = e->y();
		_selectorRecEnd.x = e->x();
		_selectorRecEnd.y = e->y();
		_showSelectorRec = true;
		_movedRect = false;
		_lockRect = false;
	}
	
	if (e->button() == Qt::RightButton)
	{
		_mouseStart.x = e->x();
		_mouseStart.y = e->y();
	}
}

void LandmarkTracker::mouseReleaseEvent	( QMouseEvent * e ) 
{	
	if (e->button() == Qt::LeftButton || e->button() == Qt::RightButton)
	{
			emit update();
			std::string note = "selected area from " + QString::number(_selectorRecStart.x).toStdString() + ":"+ QString::number(_selectorRecStart.y).toStdString()
				+ " to " +  QString::number(_selectorRecEnd.x).toStdString() + ":"+ QString::number(_selectorRecEnd.y).toStdString();
			emit notifyGUI(note,MSGS::NOTIFICATION);
		
		_rectangle = cv::Rect(_selectorRecStart, _selectorRecEnd);
		img = getCurrentImageCopy();
		if (img && _mouseMoved) {
			_mouseMoved = false;
			defineROI(img.get());
			//updateParamsWidget();
		
			
		}
	}
}

void LandmarkTracker::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_M:
		if (!_lockRect){
			lockRect();
			addRect2TrackedObj();
		}
		break;

	default:
		_glwidget->keyPressEvent(e);
	}	
}

std::set<Qt::Key> const& LandmarkTracker::grabbedKeys() const
{
	static const std::set<Qt::Key> keys{ Qt::Key_M,
										};
	return keys;
}

//Initialise Parameters Widget
void LandmarkTracker::initParamsWidget()
{
	_roiFrame->setFixedHeight(300);

	QFormLayout *layout = new QFormLayout(_roiFrame.get());

	//ROI Label
	_roiInParams = new QLabel(_roiFrame.get());
	_sampledRoiInParams = new QLabel(_roiFrame.get());

	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(_roiInParams->sizePolicy().hasHeightForWidth());

	_roiInParams->setSizePolicy(sizePolicy);
	_roiInParams->setFocusPolicy(Qt::NoFocus);
	_roiInParams->setScaledContents(true);

	layout->setWidget(0, QFormLayout::LabelRole, _roiInParams);

	//ROI Button
	_roiButton = new QPushButton("ROI", _roiFrame.get());
	_roiButton->setVisible(false);

	QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy1.setHorizontalStretch(0);
	sizePolicy1.setVerticalStretch(0);
	sizePolicy1.setHeightForWidth(_roiButton->sizePolicy().hasHeightForWidth());
	_roiButton->setSizePolicy(sizePolicy1);
	layout->setWidget(1, QFormLayout::LabelRole, _roiButton);

	//Sampled ROI Label
	sizePolicy.setHeightForWidth(_sampledRoiInParams->sizePolicy().hasHeightForWidth());

	_sampledRoiInParams->setSizePolicy(sizePolicy);
	_sampledRoiInParams->setFocusPolicy(Qt::NoFocus);
	_sampledRoiInParams->setScaledContents(true);

	layout->setWidget(0, QFormLayout::FieldRole, _sampledRoiInParams);

	//Sampled ROI Button
	_sampledRoiButton = new QPushButton("Sampled ROI", _roiFrame.get());
	_sampledRoiButton->setVisible(false);

	QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy2.setHorizontalStretch(0);
	sizePolicy2.setVerticalStretch(0);
	sizePolicy2.setHeightForWidth(_sampledRoiButton->sizePolicy().hasHeightForWidth());
	_sampledRoiButton->setSizePolicy(sizePolicy2);
	layout->setWidget(1, QFormLayout::FieldRole, _sampledRoiButton);
	
	_roiFrame->setLayout(layout);
}

void LandmarkTracker::updateParamsWidget()
{
	std::cout << "Update Parameters" << std::endl;
	
	_roiButton->setVisible(true);
	_sampledRoiButton->setVisible(true);
	
	_pixmap = mat2QPixmap(selectedRoi);

	_roiInParams->setPixmap(_pixmap.scaled(_roiFrame->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
	_roiInParams->show();

	_pixmap = mat2QPixmap(sampledRoi);
	_sampledRoiInParams->setPixmap(_pixmap.scaled(_roiFrame->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
	_sampledRoiInParams->show();
}

void LandmarkTracker::defineROI	(cv::Mat image)
{
	//Define ROI
	box.width	= abs(_selectorRecStart.x - _selectorRecEnd.x);
	box.height	= abs(_selectorRecStart.y - _selectorRecEnd.y);
	box.x	= std::min(_selectorRecStart.x , _selectorRecEnd.x);
	box.y	= std::min(_selectorRecStart.y , _selectorRecEnd.y);

	std::cout <<"Defined ROI" << std::endl;

	//Make an image out of just the selected ROI
	if(box.width > 0 && box.height > 0){

		cv::Mat roi(image, box);
		selectedRoi = roi.clone();
		_glwidget->roiMat = selectedRoi;
		startTool();
		//samplingROI(); //start Sampling (Quantization - kmeans)
	}	
}

void LandmarkTracker::forcePointIntoBorders(cv::Point & point, cv::Rect const & borders)
{
	std::cout << "Force into borders..." << std::endl;

	if (point.x < borders.x)
		point.x = borders.x;
	else if (point.x >= (borders.x + borders.width))
		point.x = (borders.x + borders.width - 1);

	if (point.y < borders.y)
		point.y = borders.y;
	else if (point.y >= (borders.y + borders.height))
		point.y = (borders.y + borders.height - 1);
}

void LandmarkTracker::startTool()
{
	std::cout << "Starting CalcMap" << std::endl;
	_glwidget->getRoiCalcMap();

	//SPEICHERT QGLWIDGET IN PNG
	//_glwidget->grabFrameBuffer().save("C:\\Users\\adam\\Downloads\\opengl_test.png");
}

cv::Mat LandmarkTracker::getSelectedRoi() //unbenutzt bis jetzt
{
	return selectedRoi;
}

void LandmarkTracker::setLabelRoi()
{
	if (!_cubeIsRoi)
	{
		_glwidget->roiMat = selectedRoi;
		startTool();
		_cubeIsRoi = true;
	}
}

void LandmarkTracker::setLabelSampledRoi()
{
	if (_cubeIsRoi)
	{
		_glwidget->roiMat = sampledRoi;
		startTool();
		_cubeIsRoi = false;
	}
}

/*
void LandmarkTracker::samplingROI()
{
#pragma warning ( disable : 4267 )
	cv::Mat src = selectedRoi;

	//step 1 : map the src to the samples
	cv::Mat samples(src.total(), 3, CV_32F);
	auto samples_ptr = samples.ptr<float>(0);
	for (int row = 0; row != src.rows; ++row){
		auto src_begin = selectedRoi.ptr<uchar>(row);
		auto src_end = src_begin + src.cols * selectedRoi.channels();
		//auto samples_ptr = samples.ptr<float>(row * src.cols);
		while (src_begin != src_end){
			samples_ptr[0] = src_begin[0];
			samples_ptr[1] = src_begin[1];
			samples_ptr[2] = src_begin[2];
			samples_ptr += 3; src_begin += 3;
		}
	}

	//step 2 : apply kmeans to find labels and centers
	int clusterCount = 5;
	cv::Mat labels;
	int attempts = 2;
	cv::Mat centers;
	cv::kmeans(samples, clusterCount, labels, cv::TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10, 0.01), attempts, cv::KMEANS_PP_CENTERS, centers);

	 //step 3 : map the centers to the output
    cv::Mat new_image(src.size(), src.type());
    for( int row = 0; row != src.rows; ++row){
        auto new_image_begin = new_image.ptr<uchar>(row);
        auto new_image_end = new_image_begin + new_image.cols * 3;
        auto labels_ptr = labels.ptr<int>(row * src.cols);

        while(new_image_begin != new_image_end){
            int const cluster_idx = *labels_ptr;
            auto centers_ptr = centers.ptr<float>(cluster_idx);
            new_image_begin[0] = centers_ptr[0];
            new_image_begin[1] = centers_ptr[1];
            new_image_begin[2] = centers_ptr[2];
            new_image_begin += 3; ++labels_ptr;
        }
    }

	sampledRoi = new_image;

    cv::Mat binary;    
    cv::Canny(new_image, binary, 30, 90);

   // cv::imshow("original", src);
   // cv::imshow("binary", binary);
   //cv::imshow( "clustered image", new_image );

	//updateParamsWidget();

}
*/
