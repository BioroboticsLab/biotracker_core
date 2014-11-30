//Class used to generate ground truth data for BeesBook
// _newGrid			-- CTRL + RIGHT Click

#ifndef BeesBookTagMatcher_H
#define BeesBookTagMatcher_H

#include "cv.h"
#include "source/tracking/TrackingAlgorithm.h"
#include "source/tracking/algorithm/BeesBookTagMatcher/resources/myGrid.h"
#include "source/tracking/algorithm/BeesBookTagMatcher/resources/myNewGrid.h"
#include <math.h>
#define _USE_MATH_DEFINES

class BeesBookTagMatcher : public TrackingAlgorithm
{
	private:

		//////////////// TEST FLAGS!!!! ---------------------
		bool _testNewTag;			//RCM first click set a new tag with defined parameters
		
				
		myNewGrid			g;				//active Grid
		myNewGrid			gtemp;			//auxiliar instance for drwaing grids already in vector

		std::vector<myNewGrid> _Grids;		//vector of ready grids
		
		//------ NEW FLAGS
		bool _ready;						//Ready for a new tag --Ctrl + LCM--
		bool _setTag;						//a new set of points is being configured
		
		bool _activeTag;					//a new Tag has been set and can now be modified
		
		bool _setP0;						//Set P0 --Left Click and drag--
		bool _setP1;						//Set P1 --Left Click and drag--
		bool _setP2;						//Set P2 --Left Click and drag--
		bool _setTheta;						//activated with shift + LCM for rotation in 3D

		std::vector<cv::Point> orient;		//auxiliar variable for drawing the orientation while setting the Tag
		cv::Point diff;						//auxiliar variable
		cv::Point prevPosition;				//auxiliar variable

		// FUNCTIONS		
		void drawSetTags(cv::Mat image);										//function that draws the Tags set so far calling instances of MyNewGrid.
		void drawOrientation(cv::Mat image, std::vector<cv::Point> orient);		//function that draws the orientation vector while being set.		
		void drawActiveTag(cv::Mat image);										//function that draws an active tag calling an instance of MyNewGrid
		void setTag(cv::Point location);										//function called while setting the tag (it initializes orient vector)
		void cancelTag(cv::Point2f location);														//function that cancels the active tag and activates the previous one.
		bool selectPoint(cv::Point location);									//function that checks if one of the set Points is selected, returns true when one of the points is selected
		///////////////
		void setTheta(cv::Point location);										//function that allows P1 and P2 to be modified to calculate the tag's angle in space.
		/////////////
		void selectTag(cv::Point location);										//function that checks if one of the already set Tags is selected.
		double dist(cv::Point p1, cv::Point p2);								//function that calculates the distance between two points
		
	public:
        BeesBookTagMatcher	( Settings &settings, std::string &serializationPathName, QWidget *parent );
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
