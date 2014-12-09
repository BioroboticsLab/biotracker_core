#ifndef Grid_H_
#define Grid_H_

#include <string>
#include <vector>

#include <boost/logic/tribool.hpp>

#include <cereal/access.hpp>
#include <cereal/cereal.hpp>

#include <opencv2/opencv.hpp>

#include <source/tracking/trackedObject/ObjectModel.h>

class Grid : public ObjectModel
{
public:
	//default constructor
	Grid();
	//constructor with 1 parameter
	Grid(const cv::Point2f& centerGrid, double Alpha, size_t objectId);
	//constructor with 7 parameters
	Grid(double Scale, const cv::Point2f& CenterGrid, double Alpha, double Theta,
	     double Phi, std::vector<boost::tribool> Id, size_t objectId);

	//destructor
	~Grid() {}

	//Object properties
	cv::Point2f centerGrid; // center of the grid

	cv::Size2f axesGrid;    // major and minor axes of the ellipse defined by the Grid
	cv::Size2f axesTag;     // major and minor axes of the ellipse defined by the Tag

	double angleGrid;       // the angle of the grid, is the angle where the bit cells start to be
							// drawn, it is calculated from phi and alpha
	double angleTag;        // this angle denotes the orientation of the ellipses (both ellipses
							// should have the same phi parameter) (measured from the y-axis)
	double scale;           // scale parameter for the tag, referenced to axisTag.
	double alpha;           // bee orientation, where 0 degrees is the x-axis and from there the
							// angle is measured counterclockwise
	double theta;           // this angle denotes the tilt of the grid
	double phi;             // this angle denotes the orientation of the ellipses (both ellipses
							// should have the same phi parameter) (measured from the y-axis)

	std::vector<boost::tribool> ID; // bit pattern of tag (false and true for black and white, indeterminate for unrecognizable)

	std::vector<std::vector<cv::Point>> cellsContours;

	std::vector<cv::Point2f> absPoints; // vector of points which are used by the user to define a
										// new grid (absolute coordinates, relative to the image)
	std::vector<cv::Point2f> relPoints; // vector of points which are used by the user to define a
										// new grid (coordinates relative to the center of the grid P0)
	std::vector<cv::Point2f> realCoord; // vector of points which are used by the user to define a
										// new grid (relative and exact coordinates for computing purposes)

	size_t objectId;

	//Object methods
	// function that updates the set of 3 points that define a grid from parameters (for displaying pursoses).
	void updatePoints();
	// function that updates the set of 3 points that define a grid from parameters (for arithmetic purposes).
	void updateVectors();
	// function that updates parameters when the tag is translated.
	void translation(const cv::Point& newCenter);
	// function that updates parameters when the tag orientation is modified.
	void orientation(const cv::Point& newP1);
	// function that updates the mayor axis and the minor axis using theta and scale as parameters.
	void updateAxes();
	// function that is called to set the binary ID
	void updateID(const cv::Point& newID, bool indeterminate);

	// function that determines coordinates for a point on an ellipse's contour (for arithmetic purposes).
	cv::Point2f ellipsePoint(const cv::Point2f& Center, const cv::Size2f& SemiAxes, double Phi, double T) const;

	// function that calculates the vector of parameters of the grid from points configuration.
	void updateParam();

	/** function that generates a vector of points for a specific cell from the grid
	 * @ param cell ID between [0,14]; which cell of the grid is to be rendered?
	 * @ param offset angle offset to draw inner half circles with different angles 1 offset = 30�
	 * @ return a vector with the contours of the cell
	 * @ 0-11 bit cells
	 * @ 12 white semicircle
	 * @ 13 black semicircle
	 * @ 14 diameter
	 * @ 15 tag perimeter
	 */
	std::vector<cv::Point> renderGridCell(unsigned short cell) const;

	// function that renders all the necessary cells in a Tag calling renderGridCell
	std::vector<std::vector<cv::Point>> renderFullTag();
	// function that draws a grid, active grids are printed in a different color.
	void drawFullTag(cv::Mat &img, int active);

	// function that obtains the cartesians coordinates from polar elements (for arithmetic purposes).
	cv::Point2f polar2rect(double radius, double angle) const;

	cv::Scalar tribool2scalar(const boost::tribool& tribool) const;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(CEREAL_NVP(centerGrid),
		  CEREAL_NVP(axesGrid),
		  CEREAL_NVP(axesTag),
		  CEREAL_NVP(angleGrid),
		  CEREAL_NVP(angleTag),
		  CEREAL_NVP(scale),
		  CEREAL_NVP(alpha),
		  CEREAL_NVP(theta),
		  CEREAL_NVP(phi),
		  CEREAL_NVP(ID),
		  CEREAL_NVP(absPoints),
		  CEREAL_NVP(relPoints),
		  CEREAL_NVP(realCoord),
		  CEREAL_NVP(objectId));
	}
};

#endif
