#include "History.h"


History::History(int histMaxSize) :
	_histMaxSize(histMaxSize)
{
}


History::~History(void)
{
}

void History::add(FishModel fc)
{
	if(_histMaxSize <= 0)
		return;

	if(_histComponents.size() >= _histMaxSize)
	{
		_histComponents.pop_front();
	}

	_histComponents.push_back(fc);
}

double History::orientation()
{
	double orientation = 0;

	for (int i = 0; i < _histComponents.size() - 1 && _histComponents.size() > 1; i++)
	{
		FishModel fm1 = _histComponents.at(i);
		FishModel fm2 = _histComponents.at(i+1);

		cv::Point2f p1 = fm1.pose().position_cm();
		cv::Point2f p2 = fm2.pose().position_cm();

		orientation += std::atan2(p2.y-p1.y,p2.x-p1.x);
	}

	if(_histComponents.size() > 2)
		orientation /= _histComponents.size() - 1;

	orientation = orientation * 180.0 / CV_PI;

	return orientation;
}

std::vector<FishPose> History::poses()
{
	std::vector<FishPose> poses;

	for (int i = 0; i < _histComponents.size(); i++)
	{
		FishModel fm = _histComponents.at(i);

		poses.push_back(fm.pose());		
	}
	return poses;
}