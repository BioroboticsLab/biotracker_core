#include "FishTank.h"

#include <limits>

#include "core/fish/model/simple/SimpleFishAttributes.h"


FishTank::FishTank(SystemProperty &systemProperty) :
	_systemProperty(systemProperty)
{
	initFishes(_systemProperty.getValueOfParam<int>(FISHTANKPARAM::FISHTANK_FISH_AMOUNT));
}

FishTank::~FishTank(void)
{
}

void FishTank::initFishes(int fishNumber)
{
	// clear the fish list before initializing a new fish list.
	_fishes.clear();

	if(fishNumber == 0)
		fishNumber = _systemProperty.getValueOfParam<int>(FISHTANKPARAM::FISHTANK_FISH_AMOUNT);

	for(int id = 1; id <= fishNumber; id++)
	{
		Fish fish = Fish(id,FishModel(),History(_systemProperty.getValueOfParam<int>(FISHPARAM::FISH_HISTORY_MAX_SIZE)));
		_fishes.push_back(fish);
	}
}

std::vector<Fish> FishTank::fishes()
{ 
	QMutexLocker lock(&_fishMutex);
	return _fishes;
}

void FishTank::updateFishes(std::vector<Fish> fishes)
{
	QMutexLocker lock(&_fishMutex);
	// copies the fish list into this member fish list.
	_fishes = std::vector<Fish>(fishes);	
}

void FishTank::addFish(Fish fish)
{
	QMutexLocker lock(&_fishMutex);
	_fishes.push_back(fish);
}


bool FishTank::removeFish(Fish fish)
{
	QMutexLocker lock(&_fishMutex);
	int fishIndexToRemove = -1;
	for(int i = 0; i < _fishes.size(); i++)
	{
		if(_fishes.at(i).id() == fish.id())
		{
			fishIndexToRemove = i;
			break;
		}
	}

	if(fishIndexToRemove < 0)
	{
		return false;
	}

	_fishes.erase(_fishes.begin() + fishIndexToRemove);

	return true;
}


cv::Point2f FishTank::currentSwarmCenterCm()
{
	QMutexLocker lock(&_fishMutex);
	cv::Point2f positionMean(0.0f,0.0f);
	for(int i = 0; i < _fishes.size(); i++)
	{
		positionMean += _fishes.at(i).model().pose().position_cm();
	}

	return positionMean * (1.0 / float(_fishes.size()));
}

cv::Point FishTank::currentSwarmCenterPx()
{
	QMutexLocker lock(&_fishMutex);
	cv::Point positionMean(0.0f,0.0f);
	for(int i = 0; i < _fishes.size(); i++)
	{
		positionMean += _fishes.at(i).model().pose().position_px();
	}

	return positionMean * (1.0 / float(_fishes.size()));
}

void FishTank::swapFishId(int newId, cv::Point p)
{
	int fishIdToChange = getNearestFishIdToLocation(p);

	if(fishIdToChange < 0)
		return;

	// set the id for the one which was seleted.
	setFishOfIdWithId(fishIdToChange,newId);	
}


void FishTank::setFishOfIdWithId(int oldId, int newId)
{
	if(oldId == newId)
		return;

	//Fish oldFish = getFishById(oldId);
	//Fish newFish = getFishById(newId);

	QMutexLocker lock(&_fishMutex);

	for (int i = 0; i < _fishes.size(); i++)
	{
		if(_fishes.at(i).id() == oldId)
		{
			for (int j = 0; j < _fishes.size(); j++)
			{
				if(_fishes.at(j).id() == newId)
				{
					//_fishes.at(j).copyFrom(oldFish);
					_fishes.at(j).setId(oldId);
					break;
				}
			}
			//_fishes.at(i).copyFrom(newFish);
			_fishes.at(i).setId(newId);
			break;
		}
	}
}

int FishTank::getNearestFishIdToLocation(cv::Point point)
{	
	double dist = std::numeric_limits<double>::max();

	int id = -1;

	QMutexLocker lock(&_fishMutex);

	for (int i = 0; i < _fishes.size(); i++)
	{		
		double tmpDist = Fish::getDistancePx(_fishes.at(i),point);
		if(tmpDist < dist)
		{
			dist = tmpDist;
			id = _fishes.at(i).id();
		}		
	}
	return id;
}

Fish FishTank::getFishById(int id)
{	
	Fish fish;

	for (int i = 0; i < _fishes.size(); i++)
	{		
		if(_fishes.at(i).id() == id)
		{
			fish = _fishes.at(i);
			break;
		}
	}
	return fish;
}

void FishTank::toggleRobofish(cv::Point p)
{	
	int possibleIdOfRobofish = getNearestFishIdToLocation(p);

	QMutexLocker lock(&_fishMutex);

	for (int i = 0; i < _fishes.size(); i++)
	{
		if(_fishes.at(i).id() == possibleIdOfRobofish)
		{
			bool value = _fishes.at(i).model().getAttribute<bool>(SimpleFishAttributes_VARIABLES::BOOL::ROBOT);
			_fishes.at(i).model().setAttribute<bool>(SimpleFishAttributes_VARIABLES::BOOL::ROBOT,!value);
			break;
		}
	}
}