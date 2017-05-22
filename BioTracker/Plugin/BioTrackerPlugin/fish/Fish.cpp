#include "Fish.h"

#include "helper/CvHelper.h"
#include <QMutex>

QMutex histMutex;

Fish::Fish(int id, bool robot, bool active, bool marked, const FishPose &currentPose, const History &history) :
_id(id),
_robot(robot),
_active(active),
_marked(marked),
_currentPose(currentPose),
_history(history)
{
	_lastConfidentOrientationRad = std::numeric_limits<float>::quiet_NaN();
}

Fish::Fish(const Fish& fish) : Fish(fish._id, fish._robot, fish._active, fish._marked, fish._currentPose, fish._history)
{
	_lastConfidentOrientationRad = fish._lastConfidentOrientationRad;
}

Fish::Fish(const Fish& fish, const FishPose &currentPose, const History &history) : Fish(fish._id, fish._robot, fish._active, fish._marked, currentPose, history)
{
	_lastConfidentOrientationRad = fish._lastConfidentOrientationRad;
}

Fish::Fish() : Fish(-1, false, true, false, FishPose(), History())
{
}


Fish::~Fish(void)
{
}

void Fish::update(FishPose fishPose)
{	
	_history.add(fishPose);
}

void Fish::updateHistoryLength(int length)
{
	_history.updateLength(length);
}

void Fish::copyFrom(Fish& fish)
{
	_id = fish.id();
	_robot = fish.isRobot();
	_active = fish.isActive();
	_marked = fish.isMarked();
	_currentPose = fish.currentPose();
	_history = fish.history();
}

std::shared_ptr<FishPose> Fish::estimateNextPose() const
{
	// can't estimate next position?
	if (_history.getPoseCount() < 4) return nullptr;
	
	const FishPose &currentPose = this->currentPose();
	const float currentAngle = currentPose.orientation_rad();
	const int smoothingWindow = 3;
	const float currentSpeedCm = _history.getCurrentSpeed(smoothingWindow, &FishPose::position_cm);
	const float currentSpeedPx = _history.getCurrentSpeed(smoothingWindow, &FishPose::position_pxf);

	// safety!
	if (!std::isfinite(currentAngle) || !std::isfinite(currentSpeedCm)) { return nullptr; }

	const cv::Point2f nextPositionCm = currentPose.position_cm()
		+ cv::Point2f(currentSpeedCm * std::cos(currentAngle), -currentSpeedCm * std::sin(currentAngle));
	assert(std::isfinite(nextPositionCm.x) && std::isfinite(nextPositionCm.y));
	const cv::Point2i nextPositionPx = currentPose.position_px()
		+ cv::Point2i(static_cast<int>(currentSpeedPx * std::cos(currentAngle)), static_cast<int>(-currentSpeedPx * std::sin(currentAngle)));

	return std::make_shared<FishPose>(nextPositionCm, nextPositionPx, currentAngle, currentAngle * 180.0f / static_cast<float>(CV_PI), currentPose.width(), currentPose.height());
}

FishPose Fish::getPoseForMapping() const
{
	// try the estimated position first
	std::shared_ptr<FishPose> estimated = estimateNextPose();
	// ok? then use this!
	if (estimated.get())
	{
		assert(std::isfinite(estimated->position_cm().x));
		assert(std::isfinite(estimated->orientation_rad()));
		return *estimated;
	}
	// otherwise, just use the current pose
	return currentPose();
}


std::vector<Fish>::iterator Fish::getFishPositionInListById(std::vector<Fish>& fishList, int id)
{
	return std::find_if(fishList.begin(), fishList.end(), [&](const Fish &fish) { return fish.id() == id; });
}

double Fish::getDistanceCm(Fish& fishA, Fish& fishB)
{ 
	return CvHelper::getDistance(fishA.currentPose().position_cm(),fishB.currentPose().position_cm());
}

double Fish::getDistanceCmDegree(Fish& fishA, Fish& fishB)
{
	return CvHelper::getDistance(
		fishA.currentPose().position_cm().x,
		fishA.currentPose().position_cm().y,
		fishA.currentPose().orientation_rad(),
		fishB.currentPose().position_cm().x,
		fishB.currentPose().position_cm().y,
		fishB.currentPose().orientation_rad());
}

double Fish::getDistancePx(Fish& fishA, Fish& fishB)
{ 
	return CvHelper::getDistance(fishA.currentPose().position_px(),fishB.currentPose().position_px());
}

double Fish::getDistanceCm(Fish& fishA, cv::Point2f& point2f)
{ 
	return CvHelper::getDistance(fishA.currentPose().position_cm(), point2f);
}

double Fish::getDistancePx(Fish& fishA, cv::Point& point)
{ 
	return CvHelper::getDistance(fishA.currentPose().position_px(), point);
}

void Fish::setInt(std::string spec_param, int value){

	if(spec_param.compare(FISHATTRIBUTES::HISTORYLENGTH) == 0) {
		this->updateHistoryLength(value);
		return;
	}

	std::cout << "Fish::setInt -> Warning - Parameter: " << spec_param << " not found!" << std::endl;
}

void Fish::setBool(std::string spec_param, bool value){	

	if(spec_param.compare(FISHATTRIBUTES::ROBOT) == 0) {
		this->setRobot(value);
		return;
	} else if(spec_param.compare(FISHATTRIBUTES::ACTIVE) == 0) {
		this->setActive(value);
		return;
	} else if (spec_param.compare(FISHATTRIBUTES::MARKED) == 0) {
		this->setMarked(value);
		return;
	}

	std::cout << "Fish::setBool -> Warning - Parameter: " << spec_param << " not found!" << std::endl;	
}

void Fish::setFloat(std::string spec_param, float value){
	std::cout << "Fish::setFloat -> Warning - Parameter: " << spec_param << " not found!" << std::endl;
}

void Fish::setDouble(std::string spec_param, double value){
	std::cout << "Fish::setDouble -> Warning - Parameter: " << spec_param << " not found!" << std::endl;
}

History Fish::history() const
{ 
	QMutexLocker locker(&histMutex);
	return _history;
}

void Fish::setHistory(History history)
{ 
	QMutexLocker locker(&histMutex);
	_history = history;
}