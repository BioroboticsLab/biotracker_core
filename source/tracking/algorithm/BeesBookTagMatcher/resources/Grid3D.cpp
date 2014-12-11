#include "Grid3D.h"

#include <cmath> // std::sin, std::cos

const Grid3D::coordinates3D_t Grid3D::_coordinates3D = Grid3D::generate_coordinates3D();

const double Grid3D::INNER_RING_RADIUS  = 0.4;
const double Grid3D::MIDDLE_RING_RADIUS = 0.8;
const double Grid3D::OUTER_RING_RADIUS  = 1.0;
const double Grid3D::bulge_factor       = 0.7;

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
		const value_type z_inner_ring  = std::cos(bulge_factor * INNER_RING_RADIUS);
		const value_type z_middle_ring = std::cos(bulge_factor * MIDDLE_RING_RADIUS);
		const value_type z_outer_ring  = std::cos(bulge_factor * OUTER_RING_RADIUS);
		for(size_t i = 0; i < POINTS_PER_RING; ++i)
		{
			result._inner_ring[i].z  = z_inner_ring;
			result._middle_ring[i].z = z_middle_ring;
			result._outer_ring[i].z  = z_outer_ring;
		}
	}

	return result;
}



Grid3D::Grid3D()
{

}


Grid3D::~Grid3D() = default;

void Grid3D::initializeBaseCoordinates()
{
	// number of points for rings --> N-1 segments
	const short N = 25;

	// 0 .. 2*pi in radians
	std::valarray<double> a(N);
	// fill angle array
	for (size_t i = 0; i < N; i++)
	{
		//a[i] = (double) i * 2.0 * CV_PI / (N - 1);
	}

	// the outer ring (the tag border)
	std::valarray<double> X0 = std::cos(a);
	std::valarray<double> Y0 = std::sin(a);

	// middle ring (the grid border)
	std::valarray<double> X1 = 0.8 * X0;
	std::valarray<double> Y1 = 0.8 * Y0;

	// inner ring (the grid center circle)
	std::valarray<double> X2 = 0.4 * X0;
	std::valarray<double> Y2 = 0.4 * Y0;

	std::valarray<double> X(3 * N);
	std::valarray<double> Y(3 * N);
	std::valarray<double> Z(3 * N);

	// fill X and Y with the mesh parts
	for (size_t i = 0; i < N; i++)
	{
		/*
		X[		i]	= X0[i];
		X[N	+	i]	= X1[i];
		X[2*N + i]	= X2[i];

		Y[		i]	= Y0[i];
		Y[N +	i]	= Y1[i];
		Y[2*N + i]	= Y2[i];

		Z[		i]	= cos(0.7 * sqrt(X0[i] * X0[i] + Y0[i] * Y0[i]));
		Z[N +	i]	= cos(0.7 * sqrt(X1[i] * X1[i] + Y1[i] * Y1[i]));
		Z[2*N +	i]	= cos(0.7 * sqrt(X2[i] * X2[i] + Y2[i] * Y2[i]));
		*/

//		coordinates3D[0].push_back( cv::Point3f(X0[i], Y0[i], cos(0.7 * sqrt(X0[i] * X0[i] + Y0[i] * Y0[i]))) );
//		coordinates3D[1].push_back( cv::Point3f(X1[i], Y1[i], cos(0.7 * sqrt(X1[i] * X1[i] + Y1[i] * Y1[i]))) );
//		coordinates3D[2].push_back( cv::Point3f(X2[i], Y2[i], cos(0.7 * sqrt(X2[i] * X2[i] + Y2[i] * Y2[i]))) );
	}

	// Schwerpunkt bestimmen, damit Tag um Mittelachse rotiert
	// subtract mean(Z) 

	// scale to "right" size ( -> (x,y,z) * scale )
	// scale is width of tag in pixels
}


// updates the 2D contour vector coordinates2D
void Grid3D::doPerspectiveProjection()
{
	//for (size_t i = 0; i < coordinates3D[0].size(); i++)
	{
		//coordinates2D[0] = coordinates3D[0].at(i)
		//coordinates2D[1] = 
		//coordinates2D[2] = 

	}
	
}

void Grid3D::draw(cv::Mat &img, int active)
{
	//cv::drawContours(img, coordin)
}

cv::Mat Grid3D::calculateRotMatrix()
{
	using std::cos;
	using std::sin;

	// rotation angles:
	double a = _orientation;	// angle to rotate around z axis
	double b = _pitchAxis;		// angle to rotate around y axis
	double c = _pitchAngle;		// angle to rotate around x axis
	
	// create rotation matrix
	double rotationMatrixData[9] =
	{	cos(a)*cos(b),	cos(a)*sin(b)*sin(c) - sin(a)*cos(c),	cos(a)*sin(b)*cos(c) + sin(a)*sin(c),
		sin(a)*cos(b),	sin(a)*sin(b)*sin(c) + cos(a)*cos(c),	sin(a)*sin(b)*cos(c) - cos(a)*sin(c)
		- sin(b),		cos(b)*sin(c),							cos(b)*cos(c)		};

	return cv::Mat(3, 3, CV_64F, rotationMatrixData);
}