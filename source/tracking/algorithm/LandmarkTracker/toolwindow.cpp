#include "toolwindow.h"
#include <iostream>
#include "LandmarkTracker.h"


ToolWindow::ToolWindow(LandmarkTracker *parentTracker, QWidget *parent)
    : QDialog(parent)
    , tracker(parentTracker)
{
    ui.setupUi(this);

    QObject::connect(ui.pushButton, SIGNAL(  clicked() ), this, SLOT(emitClose()));
	setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    
	//std::cout<<"DOING STUFF HERE!!!!"<<std::endl;
}

void ToolWindow::initToolWindow()
{

	roiMat = tracker->getSelectedRoi();

	std::cout<<"First ROI loaded..."<<std::endl;
    ui.roiOne->setPixmap(Mat2QPixmap(roiMat).scaled(ui.roiOne->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
 
    getRGBValues(roiMat);
}

QPixmap ToolWindow::Mat2QPixmap(const cv::Mat &mat)
{
    cv::Mat rgb;
    QPixmap p;
	
    cvtColor(mat, rgb, CV_BGR2RGB);
    p.convertFromImage(QImage(static_cast<const unsigned char*>(rgb.data), rgb.cols, rgb.rows, QImage::Format_RGB888));

    std::cout<<"Pixmap set..."<<std::endl;

    return p;
}

//Ausgabe für Vector
std::ostream &operator<<(std::ostream &os, const cv::Vec3b &v)
{
	return os<<"("<<static_cast<unsigned>(v.val[0])<<", "<<static_cast<unsigned>(v.val[1])<<", "<<static_cast<unsigned>(v.val[2])<<")";
}

void ToolWindow::getRGBValues(const cv::Mat &mat)
{

    cv::Mat image = mat;

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
			rgbMap[image.at<cv::Vec3b>(i, j)]++;
        }
    }

	std::cout<<"rgbMap cv::Size: "<<rgbMap.size()<<std::endl;
	
	/*for(const auto &v:rgbMap){
		std::cout<<"Vector: "<<v.first<< "| "<<v.second<<std::endl;
	}*/
   
    std::cout <<"RGB VALUES COMPUTED!"<<std::endl;

}

void ToolWindow::emitClose()
{
	this->close();
	tracker->setToolPtr();
}



