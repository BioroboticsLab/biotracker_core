#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef ELLIPSE_H_
#define ELLIPSE_H_

using namespace cv;
using namespace std;

namespace decoder {

class Ellipse {
public:
	// number of edge pixels supporting this ellipse
	int vote;
	// this ellipse' center pixel
	Point2i cen;
	// tuple of axis' length (major_axes_length, minor_axes_length)
	Size axis;
	// ellipse' orientation in degrees
	double angle;
	/**
	 * subimage of the cannyEdgeMap fpr the bounding box. transformed, so tht ellipse is now a circle with no angle.
	 */
	Mat transformedImage;
	Mat binarizedImage;



	double getAngle() const;
	void setAngle(double angle);
	Size getAxis() const;
	void setAxis(Size axis);
	Point2i getCen() const;
	void setCen(Point2i cen);
	int getVote() const;
	void setVote( int vote);

	Ellipse();

	Ellipse( int vote, Point2i center, Size axis_length, double angle);

	bool operator<(const Ellipse & elli2) const;

	const Mat& getTransformedImage() const {
		return transformedImage;
	}

	void setTransformedImage(const Mat& transformedImage) {
		this->transformedImage = transformedImage;
	}
};

}
#endif /* ELLIPSE_H_ */
