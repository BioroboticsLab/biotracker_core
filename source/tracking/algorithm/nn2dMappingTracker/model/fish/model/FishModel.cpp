#include "FishModel.h"


FishModel::FishModel(void) :
	_ifa(new SimpleFishAttributes())
{

}

FishModel::FishModel(FishPose pose, IFishAttributes* fishAttribute) :
	_pose(pose),
	_ifa(fishAttribute)
{
}



FishModel::~FishModel(void)
{
	//delete _ifa;
}