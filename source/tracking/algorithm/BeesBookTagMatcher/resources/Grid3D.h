#ifndef Grid3D_H_
#define Grid3D_H_

#include <vector>
#include <boost/logic/tribool.hpp>
#include <cereal/access.hpp>
#include <cereal/cereal.hpp>
#include <opencv2/opencv.hpp>
#include <source/tracking/serialization/ObjectModel.h>

class Grid3D : public ObjectModel
{
	/** Parameter Section
		Each of the following parameters define the tag uniquely. 
		The Grid3D class holds a set of 3D-points that make up a mesh of the bee tag. These points are
		transformed according to the given parameters and projected onto the camera plane to produce 2D coordinates
		for displaying the tag.
	*/
	cv::Point2f					_center;			// center point of the grid (within image borders - unit: px)
	double						_radius;			// radius of the tag (unit: px)
	double						_orientation;	// the angle of the grid (unit: rad. points towards the head of the bee, positive is counter-clock)
	double						_pitch;          // the pitch angle of the grid (unit: rad. only positive, if pitched the other way, phi is rotated 180°)
	double						_phi;            // the axis around which the tag is rotated in space
	std::vector<boost::tribool> _ID;				// bit pattern of tag (false and true for black and white, indeterminate for unrecognizable)


	/**
		from a given set of parameters we can compute the 3D - points(and their 2D - projection)
	*/
	std::vector<std::vector<cv::Point3f>>	_coordinates3D;	// 3D coordinates of mesh, aka. contours (see opencv function drawContours)
	std::vector<std::vector<cv::Point>>		_coordinates2D;	// 2D coordinates of mesh (after perspective projection)


public:
				Grid3D();
				~Grid3D();
	// precomputes mesh database (this is being rotated and scaled, based on the parameters above)
	void		initializeBaseCoordinates	();							
	
	// updates the 2D contour vector coordinates2D
	void		doPerspectiveProjection		();

	// draws 2D projection of 3D-mesh on image
	void		draw						(cv::Mat &img, int active);	

};

#endif