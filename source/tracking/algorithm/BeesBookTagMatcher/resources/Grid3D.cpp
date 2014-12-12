#include "Grid3D.h"

#include <cmath> // std::sin, std::cos
#include "utility/CvHelper.h"

const Grid3D::coordinates3D_t Grid3D::_coordinates3D = Grid3D::generate_coordinates3D();

const double Grid3D::INNER_RING_RADIUS  = 0.4;
const double Grid3D::MIDDLE_RING_RADIUS = 0.8;
const double Grid3D::OUTER_RING_RADIUS  = 1.0;
const double Grid3D::BULGE_FACTOR       = 0.7;

Grid3D::Grid3D(cv::Point2i center, double radius, double orientation, double pitchAxis, double pitchAngle)
	: _center(center)
	, _radius(radius)
	, _orientation(orientation)
	, _pitchAxis(pitchAxis)
	, _pitchAngle(pitchAngle)
{
	doPerspectiveProjection();
}

Grid3D::~Grid3D() = default;

Grid3D::coordinates3D_t Grid3D::generate_coordinates3D() {

	typedef coordinates3D_t::value_type value_type;
	typedef coordinates3D_t::point_type point_type;

	coordinates3D_t result;

	// generate x,y coordiantes
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
		}
	}

	return result;
}

Grid3D::coordinates2D_t Grid3D::generate_coordinates2D() const {

	const auto rotationMatrix = CvHelper::rotationMatrix(_orientation, _pitchAxis, _pitchAngle);
	coordinates2D_t result;
	for (size_t r = 0; r < _coordinates3D._rings.size(); ++r) {
		for (size_t i = 0; i < _coordinates3D._rings[r].size(); ++i) {
			const cv::Point3d p = rotationMatrix * _coordinates3D._rings[r][i] * _radius;
			result._rings[r][i] = cv::Point2i(p.x, p.y) + _center;
		}
	}
	return result;
}

// updates the 2D contour vector coordinates2D
void Grid3D::doPerspectiveProjection()
{
	const auto rings_2d = generate_coordinates2D();

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
	}

}


void Grid3D::draw(cv::Mat &img, int) const
{
	cv::drawContours(img, _coordinates2D, INDEX_OUTER_WHITE_RING,        cv::Scalar(255, 255, 255), CV_FILLED);

	cv::drawContours(img, _coordinates2D, INDEX_INNER_WHITE_SEMICIRCLE,  cv::Scalar(255, 255, 255), CV_FILLED);
	cv::drawContours(img, _coordinates2D, INDEX_INNER_BLACK_SEMICIRCLE,  cv::Scalar(  0,   0,   0), CV_FILLED);


	size_t i = INDEX_MIDDLE_CELLS_BEGIN;
	for(; i < (INDEX_MIDDLE_CELLS_BEGIN + INDEX_MIDDLE_CELLS_END) / 2; ++i) {
		const cv::Scalar bgr(i % 3 == 0 ? 255 : 0,   i % 3 == 1 ? 255 : 0,   i % 3 == 2 ? 255 : 0);
		cv::drawContours(img, _coordinates2D, i, bgr, CV_FILLED);
	}

	for(; i < INDEX_MIDDLE_CELLS_END; ++i) {
		const cv::Scalar bgr(i & 1 ? 255 : 0,   i & 1 ? 255 : 0,   i & 1 ? 255 : 0);
		cv::drawContours(img, _coordinates2D, i, bgr, CV_FILLED);
	}


}
