#include "ContoursDetector.h"
#include "helper/CvHelper.h"

std::vector<ContourPose> ContoursDetector::findBlobs(const cv::Mat& binImage, const cv::Mat& oriImage)
{
	std::vector<ContourPose> contourPoses;	

	return contourPoses;
}

std::vector<ContourPose> ContoursDetector::findContours(const cv::Mat& binImage, const cv::Mat& oriImage)
{
	return findBlobs(binImage,oriImage);
}

void ContoursDetector::createMask(std::vector<cv::Point> points)
{}
//	if (_xRes == 0 || _yRes == 0)
//		return;
//
//	mask = cvCreateImage(cvSize(_xRes, _yRes), IPL_DEPTH_8U,1);//erstellt schwarzweiﬂ bild 
//
//	//folgender Block setzt die Pixel schwarz
//	for( int y=0; y<mask->height; y++ ) { 
//		uchar* ptr = (uchar*) ( mask->imageData + y * mask->widthStep ); 
//		for( int x=0; x<mask->width; x++ ) { 
//			ptr[x+2] = 0; 
//		}
//	}
//		
//	CvPoint maskPoints[4];
//
//	for(int z=0;z<4;z++)
//	{
//		maskPoints[z] = points[z];
//	}
//
//	//Array im Array von Polygonen. Hier Grˆﬂe eins, da nur eins erzeugt wird
//	CvPoint* contours[1]={
//		maskPoints,
//	};
//
//		//Anzahl der Punkte der Polygone. Anzahl wie oben
//	int contours_n[1]={
//		4,
//	};
//
//	//opencv Funktion zum erstellen und ausf¸llen eines polygons in weiss
//	cvFillPoly(mask, contours, contours_n, 1, cvScalar(255));
//
//	//Visualisierung zum Debuggen
//	//cvNamedWindow("MyWindow",CV_WINDOW_AUTOSIZE);
//	//cvShowImage("MyWindow", mask);
//	//cvWaitKey(0);
//	//cvReleaseImage(&mask);
//	//cvDestroyWindow("MyWindow");
//}


std::vector<ContourPose> ContoursDetector::getPoses(cv::Mat& image_mat, cv::Mat& image_ori)
{	
	return findContours(image_mat,image_ori);
}

void ContoursDetector::setDouble(std::string spec_param, double value)
{	
	std::cout << "ContoursDetector::Warning - Parameter: " << spec_param << " not found!" << std::endl;
}