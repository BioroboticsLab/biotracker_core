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
#include "source/tracking/algorithm/BeesBookTagMatcher/resources/myGrid.h"
#include "source/tracking/algorithm/BeesBookTagMatcher/resources/myNewGrid.h"

class BeesBookTagMatcher : public TrackingAlgorithm
{
	private:
				
		myNewGrid			g;			//active Grid
		myNewGrid			gtemp;		//Grids already in vector

		std::vector<myNewGrid> _Grids;	//vector of already set grids
		
		bool _newGrid;					//ready to generate a new grid

		bool _activeDragGrid;			//a new Grid has been set and can now be draged
		bool _activeGrid;				//a new Grid has been set and can now be modified
		bool _activeTag;				//a new Grid has been set and the Tag perimeter can now be modified

		bool _modPosGrid;				//modify position of active Grid
		bool _modPosTag;				//modify position of active Tag

		//------ NEW FLAGS
		bool _ready;					//Ready for a new tag --ctrl + Right Click--
		bool _activePoints;				//a new set of points is being configured
		bool _setP0;					//Set P0 --Left Click and drag--
		bool _setP1;					//Set P1 --Left Click and drag--
		bool _setP2;					//Set P2 --Left Click and drag--
		bool _setP3;					//Set P3 --Left Click and drag--
		bool _setP4;					//Set P4 --Left Click and drag--
		bool _setP5;					//Set P5 --Left Click and drag--
		bool _setP6;					//Set P6 --Left Click and drag--
		bool _setP7;					//Set P7 --Left Click and drag--
		bool _setP8;					//Set P8 --Left Click and drag--

		cv::Point diff; //auxiliar variable
		cv::Point prevPosition; //auxiliar variable

		// FUNCTIONS
		void drawGrid(cv::Mat image); //function that draws a grid calling an instance of MyNewGrid
		void drawDragGrid(cv::Mat image); //function that draws a grid that can be draged and rotated calling an instance of MyNewGrid
		void drawPoints(cv::Mat image); //function that draws the points while being edited
		double dist(cv::Point p1, cv::Point p2); //function that calculates the distance between two points
		
	public:
		BeesBookTagMatcher	( Settings & settings, QWidget *parent );
		~BeesBookTagMatcher	( void );
		

		void track			( ulong frameNumber, cv::Mat & frame );
		void paint			( cv::Mat& image );
		void reset			( );


	public slots:
		//mouse click and move events
		void mouseMoveEvent		( QMouseEvent * e );
		void mousePressEvent	( QMouseEvent * e );
		void mouseReleaseEvent	( QMouseEvent * e );
		void mouseWheelEvent	( QWheelEvent * e );

};

#endif