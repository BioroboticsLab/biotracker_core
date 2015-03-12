#include "toolwindow.h"
#include <iostream>
#include "LandmarkTracker.h"
#include "glwidget.h"


ToolWindow::ToolWindow(LandmarkTracker *parentTracker, QWidget *parent) 
	: QDialog(parent)
	, tracker(parentTracker)
	, rgbValue_max(1)
	, ui()
{
    ui.setupUi(this);
	//ui.setupUi(_toolWidget.get());

    QObject::connect(ui.pushButton, SIGNAL(  clicked() ), this, SLOT(emitClose()));
	setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    
	//std::cout<<"DOING STUFF HERE!!!!"<<std::endl;
}

void ToolWindow::updateToolWindow()
{
	initToolWindow();
}

void ToolWindow::initToolWindow()
{
	//selected in GUI
	//roiMat = tracker->getSelectedRoi();

	//imshow("roiMat", roiMat); //show ROI in new window
	
	//hardcoded picture
	roiMat = cv::imread("C:\\Users\\adam\\Downloads\\RGB_9PIXEL_3SAME.bmp");
	//roiMat = cv::imread("C:\\Users\\adam\\Downloads\\RGB_3PIXEL.bmp");
	//roiMat = cv::imread("C:\\Users\\adam\\Downloads\\APM_2_5_MOTORS_QUAD_enc.jpg");

	std::cout<<"First ROI loaded..."<<std::endl;
    ui.roiOne->setPixmap(Mat2QPixmap(roiMat).scaled(ui.roiOne->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
	
 
//    getRGBValues(roiMat);
}

QPixmap ToolWindow::Mat2QPixmap(const cv::Mat &mat)
{
    cv::Mat rgb;
    QPixmap p;
	
	cvtColor(mat, rgb, CV_BGR2RGB);
	QImage dest(static_cast<const unsigned char*>(rgb.data), rgb.cols, rgb.rows, static_cast<int>(rgb.step), QImage::Format_RGB888);
	dest.bits();
	p.convertFromImage(dest);

    return p;
}


void ToolWindow::calcHistogram(const cv::Mat &mat)
{
	//read 2 images for histogram comparing
	
	cv::Mat imgA;
	cv::Mat imgA_mask;
	 
	imgA = mat;
	cvtColor(imgA, imgA_mask, CV_BGR2GRAY); 
	 
	imshow("img1", imgA);
	imshow("img1_mask", imgA_mask);

	//variables preparing
	
	int hbins = 30;
	int channels[] = {0};
	int histSize[] = {hbins};
	float hranges[] = {0, 180};
	const float* ranges[] = {hranges};

	cv::Mat patch_HSV;
	cv::MatND HistA, HistB;

	//cal histogram & normalization

	cvtColor(imgA, patch_HSV, CV_BGR2HSV);

	calcHist( &patch_HSV, 1, channels, cv::Mat(), //imgA_mask, //MaskForHisto, // // do use mask
		HistA, 1, histSize, ranges,
		true, // the histogram is uniform
		false );
	normalize(HistA, HistA,  0, 255, CV_MINMAX);

	//Mat for drawing
	cv::Mat histimg = cv::Mat::zeros(240, 320, CV_8UC3); //size of Matrix
	histimg = cv::Scalar::all(0);
	int binW = histimg.cols / hbins;
	cv::Mat buf(1, hbins, CV_8UC3);
	
	//Set RGB color
	for( int i = 0; i <  hbins; i++ ){
		buf.at< cv::Vec3b>(i) = cv::Vec3b(cv::saturate_cast<uchar>(i*180. / hbins), 255, 255);
	}
	cvtColor(buf, buf, CV_HSV2BGR);
	
	//drawing routine
	for( int i = 0; i <  hbins; i++ ){
		int val = cv::saturate_cast< int>(HistA.at< float>(i)*histimg.rows / 255);
		rectangle(histimg, cv::Point(i*binW, histimg.rows), cv::Point((i + 1)*binW, histimg.rows - 240), cv::Scalar(buf.at<cv::Vec3b>(i)), -1, 8);
		int r,g,b;
		b = buf.at<cv::Vec3b>(i)[0];
		g = buf.at<cv::Vec3b>(i)[1];
		r = buf.at<cv::Vec3b>(i)[2];

	 //show bin and RGB value
	 printf("[%d] r=%d, g=%d, b=%d , bins = %d \n",i , r, g, b, val);
	}
	 
	imshow( "Histogram", histimg );

	cv::waitKey(0);
}

void ToolWindow::on_histoButton_clicked()
{
	calcHistogram(roiMat);
	std::cout <<"HISTOGRAMM!"<<std::endl;
}

void ToolWindow::emitClose()
{
	this->close();
	//tracker->setToolPtr();
}



