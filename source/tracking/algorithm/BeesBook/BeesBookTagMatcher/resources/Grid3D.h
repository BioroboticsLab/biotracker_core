#ifndef Grid3D_H_
#define Grid3D_H_

#include <vector>                  // std::vector
#include <array>                   // std::array
#include <opencv2/opencv.hpp>      // cv::Mat, cv::Point3_
#include <boost/logic/tribool.hpp> // boost::tribool

#include <source/tracking/serialization/ObjectModel.h>

class Grid3D : public ObjectModel
{
public:
	/******************************************
	 *                                        *
	 *              constants                 *
	 *                                        *
	 ******************************************/

	// number of cells around the center semicircles
	static const size_t NUM_MIDDLE_CELLS = 12;

	// indices in polygon vector
	static const size_t INDEX_OUTER_WHITE_RING       = 0; 
	static const size_t INDEX_INNER_WHITE_SEMICIRCLE = 1;
	static const size_t INDEX_INNER_BLACK_SEMICIRCLE = 2;
	static const size_t INDEX_MIDDLE_CELLS_BEGIN     = 3;
	static const size_t INDEX_MIDDLE_CELLS_END       = INDEX_MIDDLE_CELLS_BEGIN + NUM_MIDDLE_CELLS;

	// total number of cells (non-coding and coding)
	static const size_t NUM_CELLS = INDEX_MIDDLE_CELLS_END;

	//
	static const size_t POINTS_PER_MIDDLE_CELL = 2;
	static const size_t POINTS_PER_LINE = 3;
	static_assert(POINTS_PER_LINE % 2 != 0, "POINTS_PER_LINE must be odd");
	static const size_t POINTS_PER_RING = NUM_MIDDLE_CELLS * POINTS_PER_MIDDLE_CELL;

	static_assert(POINTS_PER_RING % 4 == 0 , "POINTS_PER_RING = NUM_MIDDLE_CELLS * POINTS_PER_MIDDLE_CELL must be a multiple of 4");

	static const double INNER_RING_RADIUS;
	static const double MIDDLE_RING_RADIUS;
	static const double OUTER_RING_RADIUS;
	static const double BULGE_FACTOR;

	// focal length
	static const double FOCAL_LENGTH;

	typedef std::array<boost::tribool, NUM_MIDDLE_CELLS> idarray_t;

    /******************************************
	 *                                        *
	 *            public function             *
	 *                                        *
	 ******************************************/

	//default constructor, required for serialization
	explicit Grid3D();
	explicit Grid3D(cv::Point2i center, double radius, double angle_z, double angle_y, double angle_x);
	virtual ~Grid3D() override;

	/**
	 * draws 2D projection of 3D-mesh on image
	 */
	void	draw(cv::Mat &img, const bool isActive) const;

	void	setXRotation(double angle);
	double	getXRotation() const { return _angle_x; }

	void	setYRotation(double angle);
	double	getYRotation() const { return _angle_y; }

	void	setZRotation(double angle);
	double	getZRotation() const { return _angle_z; }

	void	zRotateTowardsPointInPlane(cv::Point p);
	void	xyRotateIntoPlane(float angle_y, float angle_x);

	void	setCenter(cv::Point c);
	cv::Point getCenter() const { return _center; }

	int		getKeyPointIndex(cv::Point p) const;

	void	toggleIdBit(size_t cell_id, bool indeterminate);
	cv::Scalar tribool2Color(const boost::logic::tribool &tribool) const;

	void	toggleTransparency();

	double	getPixelRadius() const { return _radius / FOCAL_LENGTH; }

	double	getWorldRadius() const { return _radius; }
	void	setWorldRadius(const double radius);

	boost::tribool hasBeenBitToggled() const { return _bitsTouched; }
	void setBeenBitToggled(const boost::logic::tribool::value_t toggled) { _bitsTouched.value = toggled; }

	bool    isSettable() const { return _isSettable; }
	void    setSettable(const bool settable) { _isSettable = settable; }

	idarray_t const& getIdArray() const { return _ID; }

	float getTransparency() const { return _transparency; }

	cv::Rect getBoundingBox() const;

private:
	/******************************************
	 *                                        *
	 *          types & typedefs              *
	 *                                        *
	 ******************************************/

	enum RingIndex {
		INNER_RING = 0,
		MIDDLE_RING,
		OUTER_RING
	};

	template<typename POINT>
	struct coordinates_t 
	{
		typedef typename POINT::value_type              value_type;
		typedef POINT                                   point_type;
		typedef std::array<point_type, POINTS_PER_RING> container_type;

		std::array<container_type, 3> _rings;
		std::array<point_type, POINTS_PER_LINE> _inner_line;

		container_type &_inner_ring;
		container_type &_middle_ring;
		container_type &_outer_ring;

        // default constructor with member initialization
		coordinates_t() : _inner_ring(_rings[INNER_RING]), _middle_ring(_rings[MIDDLE_RING]), _outer_ring(_rings[OUTER_RING]) {}
		
        // move constructor, && : r-value reference 
		coordinates_t(coordinates_t &&rhs) : _rings(std::move(rhs._rings)), _inner_line(std::move(rhs._inner_line)), _inner_ring(_rings[INNER_RING]), _middle_ring(_rings[MIDDLE_RING]), _outer_ring(_rings[OUTER_RING]) {}
		
        // delete copy constructor and assignment operator
        // -> make struct non-copyable
        coordinates_t(const coordinates_t&) = delete;
		coordinates_t& operator=(const coordinates_t&) = delete;
	};

	typedef coordinates_t<cv::Point3d> coordinates3D_t;
	typedef coordinates_t<cv::Point2i> coordinates2D_t;

	/******************************************
	 *                                        *
	 *          private functions             *
	 *                                        *
	 ******************************************/

	static coordinates3D_t generate_3D_base_coordinates();

	coordinates2D_t        generate_3D_coordinates_from_parameters_and_project_to_2D();
	void                   prepare_visualization_data();
	void                   draw(cv::Mat &img, const cv::Point& center, const bool isActive) const;

	/***************************************************************************
	 * Parameter Section
	 *
	 * Each of the following parameters define the tag uniquely.
	 * The Grid3D class holds a set of 3D-points that make up a mesh of the
	 * bee tag. These points are transformed according to the given parameters
	 * and projected onto the camera plane to produce 2D coordinates for
	 * displaying the tag.
	 *
	 ***************************************************************************/

	cv::Point2i                         _center;            // center point of the grid (within image borders - unit: px)
	double                              _radius;            // radius of the tag (unit: px)
	double                              _angle_z;           // the angle of the grid (unit: rad. points towards the head of the bee, positive is counter-clock)
	double                              _angle_y;           // the rotation angle of the grid around y axis (rotates into z - space)
	double                              _angle_x;           // the rotation angle of the grid around x axis (rotates into z - space)
	idarray_t                           _ID;                // bit pattern of tag (false and true for black and white, indeterminate for unrecognizable)
	std::vector<std::vector<cv::Point>> _coordinates2D;     // 2D coordinates of mesh (after perspective projection) (see opencv function drawContours)
	std::vector<cv::Point>              _interactionPoints; // 2D coordinates of interaction points (center of grid, grid cell centers, etc)
	static const coordinates3D_t        _coordinates3D;     // underlying 3D coordinates of grid mesh
	float                               _transparency;      // weight in drawing mixture
	boost::tribool                      _bitsTouched;       // if at least one bit was set, this is true, after copy & paste indeterminate
	bool                                _isSettable;        // if tag can be recognized by a human
	cv::Rect                            _boundingBox;       // bounding box of the projected 2d points


	// generate serialization functions
	friend class cereal::access;
	template <class Archive>
	void save(Archive& ar) const
	{
		ar(CEREAL_NVP(_center),
		   CEREAL_NVP(_radius),
		   CEREAL_NVP(_angle_z),
		   CEREAL_NVP(_angle_y),
		   CEREAL_NVP(_angle_x),
		   CEREAL_NVP(_ID),
		   CEREAL_NVP(_bitsTouched),
		   CEREAL_NVP(_isSettable));
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(CEREAL_NVP(_center),
		   CEREAL_NVP(_radius),
		   CEREAL_NVP(_angle_z),
		   CEREAL_NVP(_angle_y),
		   CEREAL_NVP(_angle_x),
		   CEREAL_NVP(_ID),
		   CEREAL_NVP(_bitsTouched),
		   CEREAL_NVP(_isSettable));

		prepare_visualization_data();
	}
};

#endif
