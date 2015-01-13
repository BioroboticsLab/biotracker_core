#include "toolwindow.h"
#include <iostream>
#include "LandmarkTracker.h"


ToolWindow::ToolWindow(LandmarkTracker *parentTracker, QWidget *parent) :
	QDialog(parent), tracker(parentTracker),rgbValue_max(1),
    ui(new Ui::ToolWindow)
{

	ui->setupUi(this);

	QObject::connect(ui->pushButton, SIGNAL(  clicked() ), this, SLOT(emitClose()));
	setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    
	//std::cout<<"DOING STUFF HERE!!!!"<<std::endl;
   
}

ToolWindow::~ToolWindow()
{
    //delete ui;
}

void ToolWindow::initToolWindow()
{
	//selected in GUI
	//roiMat = tracker->getSelectedRoi();

	//hardcoded picture
	roiMat = imread("C:\\Users\\adam\\Downloads\\RGB_PIXEL_2.bmp");
	//roiMat = imread("C:\\Users\\adam\\Downloads\\APM_2_5_MOTORS_QUAD_enc.jpg");

	std::cout<<"First ROI loaded..."<<std::endl;
	ui->roiOne->setPixmap(Mat2QPixmap(roiMat).scaled(ui->roiOne->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
 
    getRGBValues(roiMat);
}

QPixmap ToolWindow::Mat2QPixmap(const Mat &mat)
{
    Mat rgb;
    QPixmap p;
	
	cvtColor(mat, rgb, CV_BGR2RGB);
	QImage dest((const uchar*) rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
	dest.bits();
	p.convertFromImage(dest);

    return p;
}

//Ausgabe für Vector
std::ostream &operator<<(std::ostream &os, const Vec3b &v)
{
	return os<<"("<<static_cast<unsigned>(v.val[0])<<", "<<static_cast<unsigned>(v.val[1])<<", "<<static_cast<unsigned>(v.val[2])<<")";
}

void ToolWindow::getRGBValues(const Mat &mat)
{

    Mat image = mat;

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
			rgbMap[image.at<Vec3b>(i, j)]++;
			if(rgbMap[image.at<Vec3b>(i, j)] > rgbValue_max){
				rgbValue_max = rgbMap[image.at<Vec3b>(i, j)];
			}
        }
    }

	std::cout<<"rgbMap Size: "<<rgbMap.size()<<"| Max: "<<rgbValue_max<<std::endl;
	
	//Alle Einträge in der rgbMap - RGB Wert und Häufigkeit (sollte bei großen Bilder auskommentiert werden!)
	for(const auto &v:rgbMap){
		std::cout<<"Vector: "<<v.first<< "| #:"<<v.second<<std::endl;
	}
   
	std::cout <<"RGB VALUES COMPUTED!"<<std::endl;

}

void ToolWindow::calcHistogram(const Mat &mat)
{
	//read 2 images for histogram comparing
	
	Mat imgA;
	Mat imgA_mask;
	 
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

	Mat patch_HSV;
	MatND HistA, HistB;

	//cal histogram & normalization

	cvtColor(imgA, patch_HSV, CV_BGR2HSV);

	calcHist( &patch_HSV, 1, channels, Mat(), //imgA_mask, //MaskForHisto, // // do use mask
		HistA, 1, histSize, ranges,
		true, // the histogram is uniform
		false );
	normalize(HistA, HistA,  0, 255, CV_MINMAX);

	//Mat for drawing
	Mat histimg = Mat::zeros(240, 320, CV_8UC3); //size of Matrix
	histimg = Scalar::all(0);
	int binW = histimg.cols / hbins;
	Mat buf(1, hbins, CV_8UC3);
	
	//Set RGB color
	for( int i = 0; i <  hbins; i++ ){
		buf.at< Vec3b>(i) = Vec3b(saturate_cast< uchar>(i*180./hbins), 255, 255);
	}
	cvtColor(buf, buf, CV_HSV2BGR);
	
	//drawing routine
	for( int i = 0; i <  hbins; i++ ){
		int val = saturate_cast< int>(HistA.at< float>(i)*histimg.rows/255);
		rectangle( histimg, Point(i*binW,histimg.rows), Point((i+1)*binW,histimg.rows - 240), Scalar(buf.at< Vec3b>(i)), -1, 8 );
		int r,g,b;
		b =  buf.at< Vec3b>(i)[0];
		g =  buf.at< Vec3b>(i)[1];
		r =  buf.at< Vec3b>(i)[2];

	 //show bin and RGB value
	 printf("[%d] r=%d, g=%d, b=%d , bins = %d \n",i , r, g, b, val);
	}
	 
	imshow( "Histogram", histimg );

	waitKey(0);
}

void ToolWindow::on_histoButton_clicked()
{
	calcHistogram(roiMat);
	std::cout <<"HISTOGRAMM!"<<std::endl;
}

void ToolWindow::emitClose()
{
	this->close();
	tracker->setToolPtr();
}



