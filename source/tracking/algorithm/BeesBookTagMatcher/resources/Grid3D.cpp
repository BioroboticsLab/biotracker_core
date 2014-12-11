#include "Grid3D.h"

#include <cmath> // std::sin, std::cos

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
	_rotationMatrix = calculateRotMatrix();
	prepare_visualization_data();
}

Grid3D::~Grid3D() = default;

Grid3D::coordinates3D_t Grid3D::generate_3D_base_coordinates() {

	typedef coordinates3D_t::value_type value_type;
	typedef coordinates3D_t::point_type point_type;

	coordinates3D_t result;

	// generate x,y coordinates
	for(size_t i = 0; i < POINTS_PER_RING; ++i)
	{
		const value_type angle = i * 2.0 * CV_PI / static_cast<double>(POINTS_PER_RING);
		const point_type p(
		  std::cos(angle),
		  std::sin(angle),
		  0.0
		);
		result._inner_ring[i]  = p * INNER_RING_RADIUS;
		result._middle_ring[i] = p * MIDDLE_RING_RADIUS;
		result._outer_ring[i]  = p * OUTER_RING_RADIUS;
	}

	// generate z coordinates
	{
		const value_type z_inner_ring  = std::cos(BULGE_FACTOR * INNER_RING_RADIUS);
		const value_type z_middle_ring = std::cos(BULGE_FACTOR * MIDDLE_RING_RADIUS);
		const value_type z_outer_ring  = std::cos(BULGE_FACTOR * OUTER_RING_RADIUS);

		// Schwerpunkt bestimmen, damit Tag um Mittelachse rotiert
		const value_type mean = (z_inner_ring + z_middle_ring + z_outer_ring) / 3.0;
		for(size_t i = 0; i < POINTS_PER_RING; ++i)
		{
			result._inner_ring[i].z  = z_inner_ring  - mean;
			result._middle_ring[i].z = z_middle_ring - mean;
			result._outer_ring[i].z  = z_outer_ring  - mean;

			result._inner_ring[i]	*= 1.0 / sqrt(1 + result._inner_ring[i].z * result._inner_ring[i].z);
			result._middle_ring[i]	*= 1.0 / sqrt(1 + result._middle_ring[i].z * result._middle_ring[i].z);
			result._outer_ring[i]	*= 1.0 / sqrt(1 + result._outer_ring[i].z * result._outer_ring[i].z);
		}
	}

	return result;
}

// rotates and scales the base mesh according to given parameter set
Grid3D::coordinates2D_t Grid3D::generate_3D_coordinates_from_parameters_and_project_to_2D() const 
{
	// output variable
	coordinates2D_t result;

	// iterate over all rings
	for (size_t r = 0; r < _coordinates3D._rings.size(); ++r) 
	{
		// iterate over all points in ring
		for (size_t i = 0; i < _coordinates3D._rings[r].size(); ++i) 
		{
			// rotate and scale point (aka vector)
			const cv::Point3d p = ( _rotationMatrix * _coordinates3D._rings[r][i] );

			result._rings[r][i] = cv::Point2i(round(_radius* p.x / (p.z + 4)), round(_radius*p.y / (p.z + +4))) + _center;
		}
	}
	return result;
}

// updates the 2D contour vector coordinates2D
void Grid3D::prepare_visualization_data()
{
	// weird: if resize and reserve are called after rings_2d is generated, the values in the array are changed (to something weird)
	_coordinates2D.resize(1);
	_coordinates2D[INDEX_OUTER_WHITE_RING].reserve(POINTS_PER_RING);

	const auto rings_2d = generate_3D_coordinates_from_parameters_and_project_to_2D();

	_coordinates2D[INDEX_OUTER_WHITE_RING].insert(_coordinates2D[INDEX_OUTER_WHITE_RING].begin(), rings_2d._outer_ring.cbegin(), rings_2d._outer_ring.cend());
	

/*
	_coordinates2D.resize(NUM_CELLS);

	{
		auto &vec = _coordinates2D[INDEX_OUTER_WHITE_RING];

		vec.clear();
		vec.reserve(2 * (POINTS_PER_RING + 1));

		vec.insert(vec.end(), rings_2d._outer_ring.cbegin(), rings_2d._outer_ring.cend());
		vec.push_back(rings_2d._outer_ring[0]);
		vec.push_back(rings_2d._middle_ring[0]);
		vec.insert(vec.end(), rings_2d._middle_ring.crbegin(), rings_2d._middle_ring.crend());

		
	}
	{
		auto &vec = _coordinates2D[INDEX_INNER_WHITE_SEMICIRCLE];

		vec.clear();
		vec.reserve(POINTS_PER_RING / 2 + 1);

		const size_t index_270_deg_begin = POINTS_PER_RING * 3 / 4;
		const size_t index_90_deg_end  = POINTS_PER_RING * 1 / 4 + 1;

		vec.insert(vec.end(), rings_2d._inner_ring.cbegin() + index_270_deg_begin, rings_2d._inner_ring.cend());
		vec.insert(vec.end(), rings_2d._inner_ring.cbegin(), rings_2d._inner_ring.cbegin() + index_90_deg_end);
	}
	{
		auto &vec = _coordinates2D[INDEX_INNER_BLACK_SEMICIRCLE];

		vec.clear();
		vec.reserve(POINTS_PER_RING / 2 + 1);

		const size_t index_90_deg_begin = POINTS_PER_RING * 1 / 4;
		const size_t index_270_deg_end  = POINTS_PER_RING * 3 / 4 + 1;

		vec.insert(vec.end(), rings_2d._inner_ring.cbegin() + index_90_deg_begin, rings_2d._inner_ring.cbegin() + index_270_deg_end);
	}
	{
		for (size_t i = 0; i < NUM_MIDDLE_CELLS; ++i)
		{
			auto &vec = _coordinates2D[INDEX_MIDDLE_CELLS_BEGIN + i];

			vec.clear();
			vec.reserve(2 * (POINTS_PER_MIDDLE_CELL + 1));

			const size_t index_begin = POINTS_PER_MIDDLE_CELL * i;
			const size_t index_end  =  POINTS_PER_MIDDLE_CELL * (i + 1);
			const size_t index_rbegin = POINTS_PER_RING - index_end;
			const size_t index_rend  =  POINTS_PER_RING - index_begin;
			const size_t index_end_elem  =  index_end < POINTS_PER_RING ? index_end + 1 : 0;

			vec.insert(vec.end(), rings_2d._middle_ring.cbegin() + index_begin, rings_2d._middle_ring.cbegin() + index_end);
			vec.push_back(rings_2d._middle_ring[index_end_elem]);

			vec.push_back(rings_2d._inner_ring[index_end_elem]);
			vec.insert(vec.end(), rings_2d._inner_ring.rbegin() + index_rbegin, rings_2d._inner_ring.rbegin() + index_rend);

		}
	}*/




	//for (size_t i = 0; i < coordinates3D[0].size(); i++)
	{
		//coordinates2D[0] = coordinates3D[0].at(i)
		//coordinates2D[1] = 
		//coordinates2D[2] = 

	}
	
}


void Grid3D::draw(cv::Mat &img, int) const
{
	cv::drawContours(img, _coordinates2D, INDEX_OUTER_WHITE_RING,        cv::Scalar(255, 255, 255), CV_FILLED);

	cv::drawContours(img, _coordinates2D, INDEX_INNER_WHITE_SEMICIRCLE,  cv::Scalar(255, 255, 255), CV_FILLED);
	cv::drawContours(img, _coordinates2D, INDEX_INNER_BLACK_SEMICIRCLE,  cv::Scalar(  0,   0,   0), CV_FILLED);

	cv::drawContours(img, _coordinates2D, INDEX_MIDDLE_CELLS_BEGIN +  0, cv::Scalar(255,   0,   0), CV_FILLED);
	cv::drawContours(img, _coordinates2D, INDEX_MIDDLE_CELLS_BEGIN +  1, cv::Scalar(0,   255,   0), CV_FILLED);
	cv::drawContours(img, _coordinates2D, INDEX_MIDDLE_CELLS_BEGIN +  2, cv::Scalar(0,     0, 255), CV_FILLED);

	cv::drawContours(img, _coordinates2D, INDEX_MIDDLE_CELLS_BEGIN +  3, cv::Scalar(255,   0,   0), CV_FILLED);
	cv::drawContours(img, _coordinates2D, INDEX_MIDDLE_CELLS_BEGIN +  4, cv::Scalar(0,   255,   0), CV_FILLED);
	cv::drawContours(img, _coordinates2D, INDEX_MIDDLE_CELLS_BEGIN +  5, cv::Scalar(0,     0, 255), CV_FILLED);

	cv::drawContours(img, _coordinates2D, INDEX_MIDDLE_CELLS_BEGIN +  6, cv::Scalar(255, 255, 255), CV_FILLED);
	cv::drawContours(img, _coordinates2D, INDEX_MIDDLE_CELLS_BEGIN +  7, cv::Scalar(0,     0,   0), CV_FILLED);
	cv::drawContours(img, _coordinates2D, INDEX_MIDDLE_CELLS_BEGIN +  8, cv::Scalar(255, 255, 255), CV_FILLED);

	cv::drawContours(img, _coordinates2D, INDEX_MIDDLE_CELLS_BEGIN +  9, cv::Scalar(0,     0,   0), CV_FILLED);
	cv::drawContours(img, _coordinates2D, INDEX_MIDDLE_CELLS_BEGIN + 10, cv::Scalar(255, 255, 255), CV_FILLED);
	cv::drawContours(img, _coordinates2D, INDEX_MIDDLE_CELLS_BEGIN + 11, cv::Scalar(0,     0,   0), CV_FILLED);

}

cv::Matx<double, 3, 3> Grid3D::calculateRotMatrix() const
{
	using std::cos;
	using std::sin;

	// rotation angles:
	double a = _angle_z;	// angle to rotate around z axis
	double b = _angle_y;		// angle to rotate around y axis
	double c = _angle_x;		// angle to rotate around x axis
	
	cv::Matx<double, 3, 3> result(
			cos(a)*cos(b),  cos(a)*sin(b)*sin(c) - sin(a)*cos(c),   cos(a)*sin(b)*cos(c) + sin(a)*sin(c),
			sin(a)*cos(b),  sin(a)*sin(b)*sin(c) + cos(a)*cos(c),   sin(a)*sin(b)*cos(c) - cos(a)*sin(c),
			- sin(b),       cos(b)*sin(c),                          cos(b)*cos(c)
	);

	return result;
}
