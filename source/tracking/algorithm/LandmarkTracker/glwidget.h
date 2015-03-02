#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <map>
#include "LandmarkTracker.h"

class LandmarkTracker;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
	explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

	void updateCubes();

    void drawingAxes();
    void drawCube (float red, float green, float blue, float f = 25.5/2);
	void drawingCubes();

	float calc_f(float pixel_size);
	float cube_size(float div_factor, int pixel_count);
	
	
	/******************************/
	//VON TOOLWINDOW ÜBERNOMMEN

	void getRoiCalcMap();
	void info();

	cv::Vec3b values;
	cv::Mat roiMat;

	size_t rgbValue_max;
	size_t rgbMap_size;

	struct Vec3bCompare
	{
		bool operator()(const cv::Vec3b& lhs, const cv::Vec3b& rhs) const{
			//return lhs.val[0]<rhs.val[0] && lhs.val[1]<rhs.val[1] && lhs.val[2]<rhs.val[2];
			for (size_t i = 0; i <3; ++i){
				if (lhs.val[i] < rhs.val[i]) return true;
				if (lhs.val[i] > rhs.val[i])	return false;
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

	GLfloat rotX;
	GLfloat rotY;
	GLfloat rotZ;

	GLfloat moveUpDown;
	GLfloat moveLeftRight;

	QPoint lastPos;
	GLfloat zoomFactor;

	QSize viewport_size;

	//const ToolWindow * const parent_tw; //ToolWindow unnötig

	void updateCube();

	/******************************/
	//VON TOOLWINDOW ÜBERNOMMEN

	LandmarkTracker *tracker;

	void computeRgbMap(const cv::Mat &mat);
	void calcHistogram(const cv::Mat &mat);

	map_t rgbMap;

	cv::Mat roi;

signals:

public slots:

    //mouse click and move events
    void mouseMoveEvent		(QMouseEvent * e);
    void mousePressEvent	(QMouseEvent * e);
    void mouseReleaseEvent  (QMouseEvent * e);
    void wheelEvent(QWheelEvent *e);

	void keyPressEvent(QKeyEvent *e);


};

#endif // GLWIDGET_H
