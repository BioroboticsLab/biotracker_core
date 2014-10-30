#include "Fish.h"

#include "source/helper/CvHelper.h"

Fish::Fish(int id, FishModel model, History history) :
	_id(id),
	_model(model),
	_history(history)
{
}

Fish::~Fish(void)
{
}

void Fish::update(FishModel fishModel)
{	
	_model = fishModel;
	_history.add(fishModel);
	//_model.setPose(fishModel.pose());
	//_model.setAttributes(fishModel.getAttributes());
}


double Fish::getOrientation()
{	
	return _history.orientation();
}

void Fish::copyFrom(Fish fish)
{
	_id = fish.id();
	
	/*_model.setAttribute<bool>(
		SimpleFishAttributes_VARIABLES::BOOL::ROBOT,
		fish.model().getAttribute<bool>(SimpleFishAttributes_VARIABLES::BOOL::ROBOT)
		);		

	_history = fish.history();*/
}

Fish Fish::getFishCopyInListById(std::vector<Fish> fishList, int id)
{
	for (int i = 0; i < fishList.size(); i++)
	{
		Fish fish = fishList.at(i);
		if(fish.id() == id)
			return Fish(fish);
	}

	return Fish();
}

double Fish::getDistanceCm(Fish fishA, Fish fishB)
{ 
	return CvHelper::getDistance(fishA.model().pose().position_cm(),fishB.model().pose().position_cm());
}

double Fish::getDistancePx(Fish fishA, Fish fishB)
{ 
	return CvHelper::getDistance(fishA. model().pose().position_px(),fishB.model().pose().position_px());
}

double Fish::getDistanceCm(Fish fishA, cv::Point2f point2f)
{ 
	return CvHelper::getDistance(fishA.model().pose().position_cm(), point2f);
}

double Fish::getDistancePx(Fish fishA, cv::Point point)
{ 
	return CvHelper::getDistance(fishA. model().pose().position_px(), point);
}