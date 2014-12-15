#include "Grid3D.h"

#include <numeric>

#include "utility/CvHelper.h"

const Grid3D::coordinates3D_t Grid3D::_coordinates3D = Grid3D::generate_3D_base_coordinates();

const double Grid3D::INNER_RING_RADIUS  = 0.4;
const double Grid3D::MIDDLE_RING_RADIUS = 0.8;
const double Grid3D::OUTER_RING_RADIUS  = 1.0;
const double Grid3D::BULGE_FACTOR       = 0.7;

Grid3D::Grid3D(cv::Point2i center, double radius, double angle_z, double angle_y, double angle_x)
	: _center(center)
	, _radius(radius)
	, _angle_z(angle_z)
	, _angle_y(angle_y)
	, _angle_x(angle_x)
{
	prepare_visualization_data();
}

Grid3D::~Grid3D() = default;

/** precompute set of 3D points which will be transformed according to grid parameters
* @return 
*
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
		double y = (radiusInPoints - i) / radiusInPoints * INNER_RING_RADIUS;
		// the farther away, the deeper (away from the camera)
		double z = - std::cos(BULGE_FACTOR * y);
		// save new coordinate
		result._inner_line[i] = cv::Point3d(0, y, z);
	}

	// generate z coordinates for the three rings
	{
		// all points on each ring have the same radius, thus should have the same z-value
		const value_type z_inner_ring	= - std::cos(BULGE_FACTOR * INNER_RING_RADIUS);
		const value_type z_middle_ring	= - std::cos(BULGE_FACTOR * MIDDLE_RING_RADIUS);
		const value_type z_outer_ring	= - std::cos(BULGE_FACTOR * OUTER_RING_RADIUS);
		
		// @Tobi: macht blödsinn, deswegen aus der Mittelwertberechnung entfernt
		//const double z_line_mean		= std::accumulate(result._inner_line.begin(), result._inner_line.end(), 0,
		//	[](double res, cv::Point3d val) { return res += val.z; }) / static_cast<double>(POINTS_PER_LINE); 

		// mean z of all points in the ring (discard the few points on the line)
		const value_type mean = (z_inner_ring + z_middle_ring + z_outer_ring) / 3.0;

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
 * @return
 */
Grid3D::coordinates2D_t Grid3D::generate_3D_coordinates_from_parameters_and_project_to_2D() const 
{
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

			result._rings[r][i] = cv::Point2i(round((p.x / (p.z + 2.0))  * _radius), round((p.y / (p.z + 2.0)) * _radius)) + _center;
		}
	}

	// iterate over points of inner ring
	for (size_t i = 0; i < POINTS_PER_LINE; ++i)
	{
		// rotate and scale point (aka vector)
		const cv::Point3d p = rotationMatrix * _coordinates3D._inner_line[i];

		result._inner_line[i] = cv::Point2i(round((p.x / (p.z + 2.0))  * _radius), round((p.y / (p.z + 2.0)) * _radius)) + _center;
	}

	// compute interaction features
	// grid center 
	// center of grid cells
	// head point, aka P1
	return result;
}


/**
 * updates the 2D contour vector coordinates2D
 */
void Grid3D::prepare_visualization_data()
{
	// apply rotations and scaling (the basic parameters)
	const auto points_2d = generate_3D_coordinates_from_parameters_and_project_to_2D();

	_coordinates2D.resize(NUM_CELLS);

	// outer ring
	{
		auto &vec = _coordinates2D[INDEX_OUTER_WHITE_RING];

		vec.clear();
		vec.reserve( POINTS_PER_RING );
		/*vec.reserve(2 * (POINTS_PER_RING + 1));*/

		vec.insert(vec.end(), points_2d._outer_ring.cbegin(), points_2d._outer_ring.cend());
	/*	vec.push_back(points_2d._outer_ring[0]);
		vec.push_back(points_2d._middle_ring[0]);
		vec.insert(vec.end(), points_2d._middle_ring.crbegin(), points_2d._middle_ring.crend());*/
	}


	// inner ring: white half
	{
		auto &vec = _coordinates2D[INDEX_INNER_WHITE_SEMICIRCLE];

		vec.clear();
		/*vec.reserve(POINTS_PER_RING / 2 + 1 + POINTS_PER_LINE);*/
		vec.reserve(POINTS_PER_RING);

		const size_t index_270_deg_begin = POINTS_PER_RING * 3 / 4;
		const size_t index_90_deg_end  = POINTS_PER_RING * 1 / 4 + 1;

		vec.insert(vec.end(), points_2d._inner_ring.cbegin() + index_270_deg_begin, points_2d._inner_ring.cend());
		vec.insert(vec.end(), points_2d._inner_ring.cbegin(), points_2d._inner_ring.cbegin() + index_90_deg_end);
		/*vec.insert(vec.end(), points_2d._inner_line.cbegin(), points_2d._inner_line.cend());*/

		// ToDO: the center line is still being drawn: because it's in the vector or because the params are wrong in the drawing function?
	}

	// black semicircle plus curved center line
	{
		auto &vec = _coordinates2D[INDEX_INNER_BLACK_SEMICIRCLE];

		vec.clear();
		vec.reserve(POINTS_PER_RING / 2 + 1 + POINTS_PER_LINE);

		const size_t index_90_deg_begin = POINTS_PER_RING * 1 / 4;
		const size_t index_270_deg_end  = POINTS_PER_RING * 3 / 4 + 1;

		vec.insert(vec.end(), points_2d._inner_ring.cbegin() + index_90_deg_begin, points_2d._inner_ring.cbegin() + index_270_deg_end);
		vec.insert(vec.end(), points_2d._inner_line.crbegin(), points_2d._inner_line.crend());
	}

	{
		for (size_t i = 0; i < NUM_MIDDLE_CELLS; ++i)
		{
			auto &vec = _coordinates2D[INDEX_MIDDLE_CELLS_BEGIN + i];

			vec.clear();
			vec.reserve(2 * (POINTS_PER_MIDDLE_CELL + 1));

			const size_t index_begin	= POINTS_PER_MIDDLE_CELL * i;
			const size_t index_end		= POINTS_PER_MIDDLE_CELL * (i + 1);
			const size_t index_rbegin	= POINTS_PER_RING - index_end;
			const size_t index_rend		= POINTS_PER_RING - index_begin;
			const size_t index_end_elem	= index_end < POINTS_PER_RING ? index_end : 0;

			vec.insert(vec.end(), points_2d._middle_ring.cbegin() + index_begin, points_2d._middle_ring.cbegin() + index_end);
			
			vec.push_back(points_2d._middle_ring[index_end_elem]);
			vec.push_back(points_2d._inner_ring[index_end_elem]);

			//vec.insert(vec.end(), points_2d._inner_ring.rbegin() + index_rbegin, points_2d._inner_ring.rbegin() + index_rend);*/
		}
	}

}


void Grid3D::draw(cv::Mat &img, int) const
{

	size_t i = INDEX_MIDDLE_CELLS_BEGIN;
	const cv::Scalar color(255, 255, 255);
	for (; i < INDEX_MIDDLE_CELLS_BEGIN + NUM_MIDDLE_CELLS; ++i)
	{
		CvHelper::drawContoursOpen(img, _coordinates2D, i, color);
	}
	CvHelper::drawContoursOpen(img, _coordinates2D, INDEX_OUTER_WHITE_RING, cv::Scalar(255, 255, 255));
	CvHelper::drawContoursOpen(img, _coordinates2D, INDEX_INNER_WHITE_SEMICIRCLE, cv::Scalar(255, 255, 255));
	CvHelper::drawContoursOpen(img, _coordinates2D, INDEX_INNER_BLACK_SEMICIRCLE, cv::Scalar(0, 0, 0));


	

	/*for(; i < INDEX_MIDDLE_CELLS_END; ++i) {
		const cv::Scalar bgr(i & 1 ? 255 : 0,   i & 1 ? 255 : 0,   i & 1 ? 255 : 0);
		cv::drawContours(img, _coordinates2D, i, bgr);
	}*/
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

cv::Point2i Grid3D::center()
{
	return _center;
}

//
//double Grid3D::radius()
//{
//	return _radius;
//}
//
//
//double Grid3D::angle_z()
//{
//	return _angle_z;
//}
//
//double Grid3D::angle_y()
//{
//	return _angle_y;
//}
//double Grid3D::angle_x()
//{
//	return _angle_x;
//}