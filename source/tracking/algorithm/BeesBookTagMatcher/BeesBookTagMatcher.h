//Class used to generate ground truth data for BeesBook
// _newGrid			-- CTRL + RIGHT Click
// _modPosGrid		-- ALT + LEFT Button
// _modSizeGrid		-- SHIFT + LEFT Button
// _modAngleTag		-- CTRL + LEFT Button
// _modTiltGrid		-- SHIFT + RIGHT Button
// _modAngleGrid	-- ALT + RIGHT Button

#ifndef BeesBookTagMatcher_H
#define BeesBookTagMatcher_H

#include "cv.h"
#include "source/settings/Settings.h"
#include "source/tracking/TrackingAlgorithm.h"
#include "resources/myGrid.h"
#include "resources/myNewGrid.h"

class BeesBookTagMatcher : public TrackingAlgorithm
{
	private:
		
		cv::Point			_endGrid;
		
		myGrid				g; //active Grid
		myNewGrid			gNew; //active Grid
		myGrid				gtemp; //Grids already in vector

		std::vector<myGrid> _Grids;

		
		bool _newGrid; //ready to generate a new grid

		bool _activeGrid; //a new Grid has been set and can now be modified
		bool _activeTag; //a new Grid has been set and the Tag perimeter can now be modified

		bool _modPosGrid; //modify position of active Grid
		bool _modPosTag; //modify position of active Tag

		bool _modHeightInn; //modify height Inner circle and angleGrid
		bool _modWidthInn; //modify width Inner circle and angleGrid
		bool _modHeightMid; //modify height Middle circle and angleGrid
		bool _modWidthMid; //modify width Middle circle and angleGrid		
		bool _modHeightOut; //modify height Outer circle and angleTag
		bool _modWidthOut; //modify width Outer circle and angleTag

		cv::Point diff; //auxiliar variable
		cv::Point prevPosition; //auxiliar variable

		void drawGrid(cv::Mat image);
		double BeesBookTagMatcher::dist(cv::Point p1, cv::Point p2);

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