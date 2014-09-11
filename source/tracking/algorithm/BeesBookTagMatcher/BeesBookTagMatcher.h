#ifndef BeesBookTagMatcher_H
#define BeesBookTagMatcher_H

#include "cv.h"
#include "source/settings/Settings.h"
#include "source/tracking/TrackingAlgorithm.h"
#include "resources/Grid.h"
#include "resources/myGrid.h"

class BeesBookTagMatcher : public TrackingAlgorithm
{
	private:
		cv::Point	_centerGrid;
		cv::Point	_endGrid;
		int			_axis1;
		int			_axis2;
		double		_tilt;
		double		_angle;
		cv::Scalar	_color;

		std::vector<myGrid> _Grids;

		bool _newGrid; //set new Grid and push last one in the vector
		bool _activeGrid; //a new Grid has been set and can now be modified		
		bool _modPosGrid; //modify position of active Grid
		bool _modSizeGrid; //modify size of active Grid		
		bool _modAngleXY; //modify angle of the ellipse in XY
		bool _modTiltGrid; //modify ratio axis1/axis2

		cv::Point diff; //auxiliar variable
		cv::Point prevPosition; //auxiliar variable

		void drawGrid(cv::Mat image);

	public:
		BeesBookTagMatcher	( Settings & settings );
		~BeesBookTagMatcher	( void );
		



		void track			( std::vector<TrackedObject>& objectList, ulong frameNumber, cv::Mat & frame );
		void paint			( cv::Mat& image );
		void reset			( );


	public slots:
		//mouse click and move events
		void mouseMoveEvent		( QMouseEvent * e );
		void mousePressEvent	( QMouseEvent * e );
		void mouseReleaseEvent	( QMouseEvent * e );
		
};

#endif