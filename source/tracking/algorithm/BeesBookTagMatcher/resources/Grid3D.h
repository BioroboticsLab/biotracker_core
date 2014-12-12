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

	static const size_t NUM_MIDDLE_CELLS = 12;

	static const size_t INDEX_OUTER_WHITE_RING       = 0;
	static const size_t INDEX_INNER_WHITE_SEMICIRCLE = 1;
	static const size_t INDEX_INNER_BLACK_SEMICIRCLE = 2;
	static const size_t INDEX_MIDDLE_CELLS_BEGIN     = 3;
	static const size_t INDEX_MIDDLE_CELLS_END       = INDEX_MIDDLE_CELLS_BEGIN + NUM_MIDDLE_CELLS;

	static const size_t NUM_CELLS = INDEX_MIDDLE_CELLS_END;


	static const size_t POINTS_PER_MIDDLE_CELL = 4;
	static const size_t POINTS_PER_LINE = 51;
	static_assert(POINTS_PER_LINE % 2 != 0, "POINTS_PER_LINE must be odd");
	static const size_t POINTS_PER_RING = NUM_MIDDLE_CELLS * POINTS_PER_MIDDLE_CELL;

	static_assert(POINTS_PER_RING % 4 == 0 , "");

	static const double INNER_RING_RADIUS;
	static const double MIDDLE_RING_RADIUS;
	static const double OUTER_RING_RADIUS;
	static const double BULGE_FACTOR;

private:

	template<typename POINT>
	struct coordinates_t {

		typedef typename POINT::value_type              value_type;
		typedef POINT                                   point_type;
		typedef std::array<point_type, POINTS_PER_RING> container_type;

		std::array<container_type, 3> _rings;
		std::array<point_type, POINTS_PER_LINE> _inner_line;

		container_type &_inner_ring;
		container_type &_middle_ring;
		container_type &_outer_ring;

		coordinates_t() : _inner_ring(_rings[0]), _middle_ring(_rings[1]), _outer_ring(_rings[2]) {}
		coordinates_t(coordinates_t &&rhs) : _rings(std::move(rhs._rings)), _inner_line(std::move(rhs._inner_line)), _inner_ring(_rings[0]), _middle_ring(_rings[1]), _outer_ring(_rings[2]) {}
		coordinates_t(const coordinates_t&) = delete;
		coordinates_t& operator=(const coordinates_t&) = delete;
	};

	typedef coordinates_t<cv::Point3d> coordinates3D_t;
	typedef coordinates_t<cv::Point2i> coordinates2D_t;

	std::array<double, NUM_CELLS> z_coordinates;




	static coordinates3D_t generate_3D_base_coordinates();

	coordinates2D_t        generate_3D_coordinates_from_parameters_and_project_to_2D() const;



	/** Parameter Section
		Each of the following parameters define the tag uniquely. 
		The Grid3D class holds a set of 3D-points that make up a mesh of the bee tag. These points are
		transformed according to the given parameters and projected onto the camera plane to produce 2D coordinates
		for displaying the tag.
	*/
	cv::Point2i                 _center;  // center point of the grid (within image borders - unit: px)
	double                      _radius;  // radius of the tag (unit: px)
	double                      _angle_z; // the angle of the grid (unit: rad. points towards the head of the bee, positive is counter-clock)
	double                      _angle_y; // the rotation angle of the grid around y axis (rotates into z - space)
	double                      _angle_x; // the rotation angle of the grid around x axis (rotates into z - space)


	std::array<boost::tribool, NUM_CELLS> _ID;           // bit pattern of tag (false and true for black and white, indeterminate for unrecognizable)

	std::vector<std::vector<cv::Point>> _coordinates2D; // 2D coordinates of mesh (after perspective projection) (see opencv function drawContours)

	static const coordinates3D_t _coordinates3D;

public:
		explicit Grid3D(cv::Point2i center, double radius, double angle_z, double angle_y, double angle_x);
		virtual ~Grid3D() override;

	// updates the 2D contour vector coordinates2D
	void perspective_Projection();

	void prepare_visualization_data();

	// draws 2D projection of 3D-mesh on image
	void draw(cv::Mat &img, int active) const;

	void setXRotation(double angle);
	double getXRotation() const { return _angle_x; }

	void setYRotation(double angle);
	double getYRotation() const { return _angle_y; }

	void setZRotation(double angle);
	double getZRotation() const { return _angle_z; }

};

#endif
