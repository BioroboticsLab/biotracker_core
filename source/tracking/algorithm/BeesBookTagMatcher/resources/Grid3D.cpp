#include "Grid3D.h"

#include <numeric>

#include <source/tracking/serialization/types.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include "utility/CvHelper.h"

const Grid3D::coordinates3D_t Grid3D::_coordinates3D = Grid3D::generate_3D_base_coordinates();

const double Grid3D::INNER_RING_RADIUS  = 0.4;
const double Grid3D::MIDDLE_RING_RADIUS = 0.8;
const double Grid3D::OUTER_RING_RADIUS  = 1.0;
const double Grid3D::BULGE_FACTOR       = 0.4;
const double Grid3D::FOCAL_LENGTH       = 2.0;

Grid3D::Grid3D()
	: Grid3D(cv::Point2i(0, 0), 0., 0., 0., 0.)
{}

Grid3D::Grid3D(cv::Point2i center, double radius_px, double angle_z, double angle_y, double angle_x)
	: _center(center)
    , _radius(radius_px * FOCAL_LENGTH)
	, _angle_z(angle_z)
	, _angle_y(angle_y)
	, _angle_x(angle_x)
	, _coordinates2D(NUM_CELLS)
	, _transparency(0.5)
    , _bitsTouched(false)
{
	prepare_visualization_data();
}

Grid3D::~Grid3D() = default;

/**
 * precompute set of 3D points which will be transformed according to grid parameters
 */
Grid3D::coordinates3D_t Grid3D::generate_3D_base_coordinates() {

	typedef coordinates3D_t::value_type value_type;
	typedef coordinates3D_t::point_type point_type;

	coordinates3D_t result;

	// generate x,y coordinates
	for(size_t i = 0; i < POINTS_PER_RING; ++i)
	{
		// angle of a unit vector
		const value_type angle = i * 2.0 * CV_PI / static_cast<double>(POINTS_PER_RING);
		// unit vector
		const point_type p(
		  std::cos(angle),
		  std::sin(angle),
		  0.0
		);

		// scale unit vector to obtain three concentric rings in the plane (z = 0)
		result._inner_ring[i]  = p * INNER_RING_RADIUS;
		result._middle_ring[i] = p * MIDDLE_RING_RADIUS;
		result._outer_ring[i]  = p * OUTER_RING_RADIUS;
	}

	// span a line from one to the other side of the inner ring
	const double radiusInPoints = static_cast<double>(POINTS_PER_LINE / 2);
	for (size_t i = 0; i < POINTS_PER_LINE; ++i)
	{
		// distance of the point to center (sign is irrelevant in next line, so save the "abs()")
		const double y = (radiusInPoints - i) / radiusInPoints * INNER_RING_RADIUS;
		// the farther away, the deeper (away from the camera)
		const double z = - std::cos(BULGE_FACTOR * y);
		// save new coordinate
		result._inner_line[i] = cv::Point3d(0, y, z);
	}

	// generate z coordinates for the three rings
	{
		// all points on each ring have the same radius, thus should have the same z-value
		const value_type z_inner_ring  = - std::cos(BULGE_FACTOR * INNER_RING_RADIUS);
		const value_type z_middle_ring = - std::cos(BULGE_FACTOR * MIDDLE_RING_RADIUS);
		const value_type z_outer_ring  = - std::cos(BULGE_FACTOR * OUTER_RING_RADIUS);

		const double z_line_sum = std::accumulate(result._inner_line.begin(), result._inner_line.end(), 0.,
			[](double res, const cv::Point3d& val) { return (res + val.z); });

		// mean z of all points in the ring (discard the few points on the line)
		const value_type mean = (z_inner_ring + z_middle_ring + z_outer_ring + z_line_sum) /
		        (3.0 + POINTS_PER_LINE);

		// subtract mean, otherwise rotation will be eccentric
		for(size_t i = 0; i < POINTS_PER_RING; ++i)
		{
			result._inner_ring[i].z  = z_inner_ring  - mean;
			result._middle_ring[i].z = z_middle_ring - mean;
			result._outer_ring[i].z  = z_outer_ring  - mean;
		}
		for (size_t i = 0; i < POINTS_PER_LINE; ++i)
		{
			result._inner_line[i].z -= mean;
		}
	}

	return result;
}


/**
 * rotates and scales the base mesh according to given parameter set
 */
Grid3D::coordinates2D_t Grid3D::generate_3D_coordinates_from_parameters_and_project_to_2D() 
{
	_interactionPoints.clear();

	// output variable
	coordinates2D_t result;

	const auto rotationMatrix = CvHelper::rotationMatrix(_angle_z, _angle_y, _angle_x);

	// iterate over all rings
	for (size_t r = 0; r < _coordinates3D._rings.size(); ++r) 
	{
		// iterate over all points in ring
		for (size_t i = 0; i < _coordinates3D._rings[r].size(); ++i) 
		{
			// rotate and scale point (aka vector)
			const cv::Point3d p = rotationMatrix * _coordinates3D._rings[r][i];

            // project onto image plane
            result._rings[r][i] = cv::Point2i(round((p.x / (p.z + FOCAL_LENGTH))  * _radius), round((p.y / (p.z + FOCAL_LENGTH)) * _radius));

			if (r == 1) // inner ring
				if ( (i % POINTS_PER_MIDDLE_CELL) == POINTS_PER_MIDDLE_CELL / 2 )
					_interactionPoints.push_back(0.5*(result._rings[r][i] + result._rings[r - 1][i]));
		}
	}

	// iterate over points of inner ring
	for (size_t i = 0; i < POINTS_PER_LINE; ++i)
	{
		// rotate point (aka vector)
		const cv::Point3d p = rotationMatrix * _coordinates3D._inner_line[i];
        
        // project onto image plane
        const cv::Point   p2(round((p.x / (p.z + FOCAL_LENGTH))  * _radius), round((p.y / (p.z + FOCAL_LENGTH)) * _radius));

		result._inner_line[i] = p2;

		// if center point reached: save point to interaction-points-list
		if (i == POINTS_PER_LINE / 2)
		{
			_interactionPoints.push_back( p2 );
		}

	}

    // the last interaction point is P1
	_interactionPoints.push_back(result._outer_ring[0]);

	return result;
}


/**
* performs all computations required to draw the tag 
*
* has to be called whenever a parameter of the grid is changed (except _center)
* i.e. in the constructor and in most of the setters (except setCenter ...)
*/
void Grid3D::prepare_visualization_data()
{
	// apply rotations and scaling (the basic parameters)
	const auto points_2d = generate_3D_coordinates_from_parameters_and_project_to_2D();

	// outer ring
	{
		auto &vec = _coordinates2D[INDEX_OUTER_WHITE_RING];
		vec.clear();

		vec.insert(vec.end(), points_2d._outer_ring.cbegin(), points_2d._outer_ring.cend());
		vec.push_back(points_2d._outer_ring[0]); // add first point to close circle
	}


	// inner ring: white half
	{
		auto &vec = _coordinates2D[INDEX_INNER_WHITE_SEMICIRCLE];
		vec.clear();

		const size_t index_270_deg_begin = POINTS_PER_RING * 3 / 4;
		const size_t index_90_deg_end    = POINTS_PER_RING * 1 / 4 + 1;

		vec.insert(vec.end(), points_2d._inner_ring.cbegin() + index_270_deg_begin, points_2d._inner_ring.cend());
		vec.insert(vec.end(), points_2d._inner_ring.cbegin(), points_2d._inner_ring.cbegin() + index_90_deg_end);
	}

	// black semicircle plus curved center line
	{
		auto &vec = _coordinates2D[INDEX_INNER_BLACK_SEMICIRCLE];
		vec.clear();

		const size_t index_90_deg_begin = POINTS_PER_RING * 1 / 4;
		const size_t index_270_deg_end  = POINTS_PER_RING * 3 / 4 + 1;

		vec.insert(vec.end(), points_2d._inner_ring.cbegin() + index_90_deg_begin, points_2d._inner_ring.cbegin() + index_270_deg_end);
		vec.insert(vec.end(), points_2d._inner_line.crbegin(), points_2d._inner_line.crend());
	}

	// cells
	{
		for (size_t i = 0; i < NUM_MIDDLE_CELLS; ++i)
		{
			auto &vec = _coordinates2D[INDEX_MIDDLE_CELLS_BEGIN + i];
			vec.clear();

			const size_t index_begin    = POINTS_PER_MIDDLE_CELL * i;
			const size_t index_end      = POINTS_PER_MIDDLE_CELL * (i + 1);
			const size_t index_end_elem = index_end < POINTS_PER_RING ? index_end : 0;

			vec.insert(vec.end(), points_2d._middle_ring.cbegin() + index_begin, points_2d._middle_ring.cbegin() + index_end);

			vec.push_back(points_2d._middle_ring[index_end_elem]);
			vec.push_back(points_2d._inner_ring[index_end_elem]);
		}
	}



}


/**
 * draws the tag at position center in image dst
 *
 * @param img dst
 * @param center center of the tag
 */
void Grid3D::draw(cv::Mat &img, const cv::Point &center, const bool isActive) const {

	static const cv::Scalar white(255, 255, 255);
	static const cv::Scalar black(0, 0, 0);
	static const cv::Scalar red(0, 0, 255);
	static const cv::Scalar yellow(0, 255, 255);

	const cv::Scalar &outerColor = isActive ? yellow : white;

	for (size_t i = INDEX_MIDDLE_CELLS_BEGIN; i < INDEX_MIDDLE_CELLS_BEGIN + NUM_MIDDLE_CELLS; ++i)
	{
		CvHelper::drawPolyline(img, _coordinates2D, i, white, false, center);
	}
	CvHelper::drawPolyline(img, _coordinates2D, INDEX_OUTER_WHITE_RING,       outerColor, false, center);
	CvHelper::drawPolyline(img, _coordinates2D, INDEX_INNER_WHITE_SEMICIRCLE, white,      false, center);
	CvHelper::drawPolyline(img, _coordinates2D, INDEX_INNER_BLACK_SEMICIRCLE, black,      false, center);

	for (size_t i = 0; i < _interactionPoints.size() - 1; ++i)
	{
		cv::Scalar color = tribool2Color(_ID[i]);
		cv::circle(img, _interactionPoints[i] + center, 1, color);
	}
	cv::circle(img, _interactionPoints.back() + center, 1, red);

}

void Grid3D::draw(cv::Mat &img, const bool isActive) const
{
	const int radius = static_cast<int>(std::ceil(this->getPixelRadius()));
	const cv::Point subimage_origin( std::max(       0, _center.x - radius), std::max(       0, _center.y - radius) );
	const cv::Point subimage_end   ( std::min(img.cols, _center.x + radius), std::min(img.rows, _center.y + radius) );

	// draw only if subimage has a valid size (i.e. width & height > 0)
	if (subimage_origin.x < subimage_end.x && subimage_origin.y < subimage_end.y)
	{
		const cv::Point subimage_center( std::min(radius, _center.x), std::min(radius, _center.y) );

		cv::Mat subimage      = img( cv::Rect(subimage_origin, subimage_end) );
		cv::Mat subimage_copy = subimage.clone();

		draw(subimage_copy, subimage_center, isActive);

		cv::addWeighted(subimage_copy, _transparency, subimage, 1.0 - _transparency, 0.0, subimage);
	}
}

void Grid3D::setXRotation(double angle)
{
	_angle_x = angle;
	prepare_visualization_data();
}

void Grid3D::setYRotation(double angle)
{
	_angle_y = angle;
	prepare_visualization_data();
}

void Grid3D::setZRotation(double angle)
{
	_angle_z = angle;
	prepare_visualization_data();
}

void Grid3D::setCenter(cv::Point c)
{
	_center = c;
}

int Grid3D::getKeyPointIndex(cv::Point p) const
{
	for (size_t i = 0; i < _interactionPoints.size(); ++i)
	{
		if (cv::norm(_center + _interactionPoints[i] - p) < (_radius / 10) )
			return i;
	}
	return -1;
}

void Grid3D::toggleIdBit(size_t cell_id, bool indeterminate)
{ 
    _bitsTouched = true;

	if (_ID[cell_id].value == boost::logic::tribool::value_t::indeterminate_value)
		_ID[cell_id] = true;

	_ID[cell_id] = indeterminate ? boost::logic::indeterminate : !_ID[cell_id]; 
}

cv::Scalar Grid3D::tribool2Color(const boost::logic::tribool &tribool) const
{
	int value;
	switch (tribool.value) {
	case boost::logic::tribool::value_t::true_value:
		value = 255;
		break;
	case boost::logic::tribool::value_t::indeterminate_value:
		value = 0.5 * 255;
		break;
	case boost::logic::tribool::value_t::false_value:
		value = 0;
		break;
	default:
		assert(false);
		value = 0.;
		break;
	}

	return cv::Scalar(value, value, value);
}

void Grid3D::zRotateTowardsPointInPlane(cv::Point p)
{
    // still seems to flutter when heavily rotated ... hmmm ..

	// vector of grid center to mouse pointer
    cv::Point d_p = (p - _center);
    
    // angular bisection of current orientation
    double d_a = fmod( _angle_z - atan2(d_p.y, d_p.x), 2*CV_PI );
    d_a = (d_a > CV_PI)     ? d_a - CV_PI: d_a;
    d_a = (d_a < -CV_PI)    ? d_a + CV_PI: d_a;

    // current rotation axis
    cv::Point2f axis0(_angle_x, _angle_y);

    // new rotation axis
    cv::Point2f axis(cos(-d_a) * _angle_x + sin(-d_a) * _angle_y, -sin(-d_a) * _angle_x + cos(-d_a) * _angle_y);
    
    if (axis0.dot(axis) < 0)
        axis = -axis;

    _angle_x = axis.x;
    _angle_y = axis.y;
    _angle_z = atan2(d_p.y, d_p.x);


    /*cv::Point d = (p - _center);
    _angle_z = atan2(d.y, d.x);*/
    prepare_visualization_data();
}

void Grid3D::xyRotateIntoPlane(float angle_y, float angle_x)
{
	_angle_x = angle_x;
	_angle_y = angle_y;
	prepare_visualization_data();
}

void Grid3D::toggleTransparency()
{
	_transparency = std::abs(_transparency - 0.6);
}

void Grid3D::setWorldRadius(const double radius)
{
	_radius = radius;
	prepare_visualization_data();
}

CEREAL_REGISTER_TYPE(Grid3D)
