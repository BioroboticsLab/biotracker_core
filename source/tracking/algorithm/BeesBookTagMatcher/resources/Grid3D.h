#ifndef Grid3D_H_
#define Grid3D_H_

#include <vector>                  // std::vector
#include <array>                   // std::array
#include <opencv2/opencv.hpp>      // cv::Mat, cv::Point3_
#include <boost/logic/tribool.hpp> // boost::tribool

#include <cereal/access.hpp>
#include <cereal/cereal.hpp>

#include <source/tracking/serialization/ObjectModel.h>

class Grid3D : public ObjectModel
{
public:
	static const size_t NUM_CELLS = 12;
	static const size_t POINTS_PER_CELL = 4;
	static const size_t POINTS_PER_RING = NUM_CELLS * POINTS_PER_CELL;

	static const double INNER_RING_RADIUS;
	static const double MIDDLE_RING_RADIUS;
	static const double OUTER_RING_RADIUS;
	static const double bulge_factor;

	/**
	 * [ 0, 12)  : CELL
	 *  12       : White semicircle
	 *  13       : Black semicircle
	 *  14       : outer white circle
	 */

private:

	struct coordinates3D_t {

		typedef double value_type;
		typedef cv::Point3_<value_type> point_type;
		typedef std::array<point_type, POINTS_PER_RING> container_type;

		container_type _inner_ring;
		container_type _middle_ring;
		container_type _outer_ring;
	};

	static const coordinates3D_t _coordinates3D;


	static coordinates3D_t generate_coordinates3D();

	
	/** Parameter Section
		Each of the following parameters define the tag uniquely. 
		The Grid3D class holds a set of 3D-points that make up a mesh of the bee tag. These points are
		transformed according to the given parameters and projected onto the camera plane to produce 2D coordinates
		for displaying the tag.
	*/
	cv::Point2f                 _center;       // center point of the grid (within image borders - unit: px)
	double                      _radius;       // radius of the tag (unit: px)
	double                      _orientation;  // the angle of the grid (unit: rad. points towards the head of the bee, positive is counter-clock)
	double                      _pitchAxis;          // the axis around which the tag is rotated in space
	double                      _pitchAngle;        // the pitch angle of the grid (unit: rad. only positive, if pitched the other way, phi is rotated 180�)

	std::array<boost::tribool, NUM_CELLS> _ID;           // bit pattern of tag (false and true for black and white, indeterminate for unrecognizable)


	/**
		from a given set of parameters we can compute the 3D - points(and their 2D - projection)
	*/
	//std::vector<std::vector<cv::Point3f>>	_coordinates3D;	// 3D coordinates of mesh, aka. contours (see opencv function drawContours)
	std::vector<std::vector<cv::Point>>		_coordinates2D;	// 2D coordinates of mesh (after perspective projection)

	cv::Mat _3d_to_2d_transformation;

	cv::Mat _rotationMatrix;
	cv::Mat calculateRotMatrix();


public:
		explicit Grid3D();
		~Grid3D();

	// precomputes mesh database (this is being rotated and scaled, based on the parameters above)
	void initializeBaseCoordinates();

	// updates the 2D contour vector coordinates2D
	void doPerspectiveProjection();

	// draws 2D projection of 3D-mesh on image
	void draw(cv::Mat &img, int active);

};

#endif
