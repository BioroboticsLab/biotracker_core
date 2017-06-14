#include "FishPose.h"
#include "helper/CvHelper.h"

FishPose::FishPose(cv::Point2f pos_cm , cv::Point pos_px, float rad, float deg, float width, float height, float score) :	
	_position_cm(pos_cm),
	_position_px(pos_px),
	_radAngle(rad),
	_degAngle(deg),
	_width(width),
	_height(height),
	_score(score)
{
	assert(_degAngle >= -360.0f && _degAngle <= 360.0f);
	time(&_timev);
}


std::string FishPose::toString(bool rectified)
{
	std::ostringstream out;
	if(rectified == false)
		//out << _position_px.x << "," << _position_px.y << "," << _degAngle << "," << _radAngle;
		out << _position_px.x << ";" << _position_px.y << ";" << _degAngle << ";" << _radAngle;
	else
		//out << _position_cm.x << "," << _position_cm.y << "," << _degAngle << "," << _radAngle;
		out << _position_cm.x << ";" << _position_cm.y << ";" << _degAngle << ";" << _radAngle;
	return out.str();
}

float FishPose::calculateProbabilityOfIdentity(const FishPose &first, const FishPose &second, float angleImportance)
{
	const float distance = CvHelper::getDistance(first.position_cm(), second.position_cm());
	
	// we are not sure about the direction of the angle, so just use the closer one
	const float absAngleDifference = std::min(
		std::abs(CvHelper::angleDifference(first.orientation_rad(), second.orientation_rad())), 
		std::abs(CvHelper::angleDifference(first.orientation_rad() + CV_PI, second.orientation_rad())));

	auto normalDistributionPdf = [](double sigma, double distance)
	{
		return std::exp(-(distance * distance) / (2.0 * sigma * sigma));
	};

	
	auto adjustForFrameSpeed = [](const float &sigma)
	{
		//return (sigma / 1000.0f) * static_cast<float>(FishTrackerThread::instance()->getRealTimePerFrameMs());
		//TODO: IMPORTANT?
		// FishTrackerThread::instance()->getRealTimePerFrameMs() should be 1000 / FPS. Put true fps here!
		return (sigma / 1000.0f) * static_cast<float>(1000.0 / 30.0);

	};

	// 0.5cm per millisecond sounds good as a ~66% estimate - that is about 18 km/h
	// the factors are a hand-optimized scaling of the distribution's dropoff
	//TODO IMPORTANT This metric >!HEAVILY!< relies on the speed of the entity! 
	// If we want to publish this, there should be a generally decent working solution. Hardcoding a value for guppies is not good!
	const float distanceSigma = adjustForFrameSpeed(100.0f * 5.f); //This was 0.5
	const float distanceIdentity = normalDistributionPdf(distanceSigma, distance);
	
	const float angleSigma = adjustForFrameSpeed(10.0f * CV_PI / 2.0f);
	const float angleIdentity = normalDistributionPdf(angleSigma, absAngleDifference);
	/*std::cout << "distance: \t" << int(100.0f * distanceIdentity) << "\t\t\t angle: " << int(100.0f * angleIdentity) << std::endl;
	std::cout << "\t\t^- " << distance << "\t\t\t^-" << (180.0f * absAngleDifference / CV_PI) << "(" << first.orientation_deg() << " - " << second.orientation_deg() << ")" << std::endl;
	std::cout << "\tSPFs:\t" << FishTrackerThread::instance()->getRealTimePerFrameMs() << std::endl;*/
	return (1.0f - angleImportance) * distanceIdentity + angleImportance * angleIdentity;
}
