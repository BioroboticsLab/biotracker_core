#include "Grid3D.h"


Grid3D::Grid3D()
{
	coordinates3D.resize(3);
	coordinates2D.resize(3);
	initializeBaseCoordinates();
}


Grid3D::~Grid3D()
{
}

void Grid3D::initializeBaseCoordinates()
{
	// number of points for rings --> N-1 segments
	const short N = 25;

	// 0 .. 2*pi in radians
	std::valarray<double> a(N);
	// fill angle array
	for (size_t i = 0; i < N; i++)
	{
		a[i] = (double) i * 2.0 * CV_PI / (N - 1);
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

		coordinates3D[0].push_back( cv::Point3f(X0[i], Y0[i], cos(0.7 * sqrt(X0[i] * X0[i] + Y0[i] * Y0[i]))) );
		coordinates3D[1].push_back( cv::Point3f(X1[i], Y1[i], cos(0.7 * sqrt(X1[i] * X1[i] + Y1[i] * Y1[i]))) );
		coordinates3D[2].push_back( cv::Point3f(X2[i], Y2[i], cos(0.7 * sqrt(X2[i] * X2[i] + Y2[i] * Y2[i]))) );
	}

	// Schwerpunkt bestimmen, damit Tag um Mittelachse rotiert
	// subtract mean(Z) 

	// scale to "right" size ( -> (x,y,z) * scale )
	// scale is width of tag in pixels
}


// updates the 2D contour vector coordinates2D
void Grid3D::doPerspectiveProjection()
{
	for (size_t i = 0; i < coordinates3D[0].size(); i++)
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