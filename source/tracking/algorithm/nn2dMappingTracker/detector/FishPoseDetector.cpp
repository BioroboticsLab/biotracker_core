#include "FishPoseDetector.h"

#include "source/helper/CvHelper.h"

FishPoseDetector::FishPoseDetector(Rectification &rectification, Settings &settings) :
		IDetector<FishPose>(rectification,settings), _bd(rectification,settings)
{
}

FishPoseDetector::~FishPoseDetector(void)
{
}

std::vector<FishPose> FishPoseDetector::getPoses(cv::Mat image_mat)
{
	return convertBlobPosesToFishPoses(_bd.getPoses(image_mat));
}

std::vector<FishPose> FishPoseDetector::convertBlobPosesToFishPoses(std::vector<BlobPose> blobPoses)
{
	std::vector<FishPose> fishPoses;

	
	for (int i = 0; i < blobPoses.size(); i++)
	{
		//cv::Point2f pos_cm , cv::Point pos_px, float rad, float deg, float width, float height		

		fishPoses.push_back(
			FishPose(
				blobPoses.at(i).posCm(),
				blobPoses.at(i).posPx(), 
				CvHelper::degToRad(blobPoses.at(i).angleDegree()),
				blobPoses.at(i).angleDegree(),
				blobPoses.at(i).width(),
				blobPoses.at(i).height()
			)
		);
	}

	return fishPoses;
}