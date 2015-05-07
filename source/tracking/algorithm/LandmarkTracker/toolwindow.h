#ifndef TOOLWINDOW_H
#define TOOLWINDOW_H

#include <QDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <map>

#include "source/ui_toolwindow.h"

class LandmarkTracker;


namespace Ui {
class ToolWindow;
}

class ToolWindow : public QDialog
{
    Q_OBJECT


public slots:
		
	void emitClose();

public:
	explicit ToolWindow(LandmarkTracker *parentTracker, QWidget *parent = nullptr);

    void initToolWindow();

    cv::Vec3b values;
    cv::Mat roiMat;

	struct Vec3bCompare
	{
		bool operator()(const cv::Vec3b& lhs, const cv::Vec3b& rhs) const{
			//return lhs.val[0]<rhs.val[0] && lhs.val[1]<rhs.val[1] && lhs.val[2]<rhs.val[2];
			for(size_t i = 0; i <3; ++i){
				if(lhs.val[i] < rhs.val[i]) return true;
				if(lhs.val[i] > rhs.val[i])	return false;
			}
			return false;
		}
	};
	
	typedef std::map<const cv::Vec3b, size_t, Vec3bCompare> map_t;

	const map_t &getRGBMap() const
	{
		return rgbMap;
	}


private:
    Ui::ToolWindow ui;

	LandmarkTracker *tracker;

    QPixmap Mat2QPixmap(const cv::Mat& mat);
    void getRGBValues(const cv::Mat &mat);
   
	map_t rgbMap;

	cv::Mat roi;
};

#endif // TOOLWINDOW_H
