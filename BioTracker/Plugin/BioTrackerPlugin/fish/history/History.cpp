#include "History.h"
#include <QMutex>

QMutex histLenMutex;

History::History() :
    _histMaxSize(0),
    _nextInsertedPosition(0),
    _histComponents(std::deque<FishPose>()) {
}

History::History(int histMaxSize) :
    _histMaxSize(histMaxSize),
    _nextInsertedPosition(0),
    _histComponents(std::deque<FishPose>()) {
}

History::History(const History& history) :
    _histMaxSize(history._histMaxSize),
    _nextInsertedPosition(history._nextInsertedPosition),
    _histComponents(history._histComponents)
{}

History::~History(void)
{}

void History::add(FishPose fishState) {
    if (getHistoryLength() <= 0)
        return;

    if (_histComponents.size() > getHistoryLength()) {
        int diff = _histComponents.size() - getHistoryLength();
        while (diff-- > 0) {
            _histComponents.pop_front();
        }

        _histComponents.resize(getHistoryLength());
    }

    if (_histComponents.size() >= getHistoryLength()) {
        _histComponents.pop_front();
        _histComponents.push_back(fishState);
    } else {
        _histComponents.push_back(fishState);
    }


}

float History::getCurrentSpeed(int smoothingWindow, cv::Point2f(FishPose::*positionFunction)() const) const {
    // don't try to estimate a speed when NO smoothing is possible
    if (_histComponents.size() < 4) {
        return std::numeric_limits<float>::quiet_NaN();
    }
    // Can only smooth over max. the frames we know.
    // Also, never use the first invalid "pose".
    if (smoothingWindow > _histComponents.size()) smoothingWindow = _histComponents.size() - 1;

    auto calculateSpeed = [](const cv::Point2f & current, const cv::Point2f & previous) {
        const cv::Point2f derivation = current - previous;
        const float speed = std::sqrt(derivation.x * derivation.x + derivation.y * derivation.y);
        assert(std::isfinite(speed));
        return speed;
    };

    const float totalPoints = static_cast<float>(smoothingWindow);
    float totalSpeed = 0.0f;
    for (auto iter = _histComponents.rbegin(); iter != _histComponents.rend(); ++iter) {
        assert((iter + 1) != _histComponents.rend());
        const FishPose& currentPose = *iter;
        const FishPose& nextPose = *(iter + 1);
        const float currentSpeed = calculateSpeed((currentPose.*positionFunction)(), (nextPose.*positionFunction)());
        totalSpeed += currentSpeed;

        if (--smoothingWindow == 0) break;
    }
    assert(smoothingWindow == 0);

    // the current speed is the (non-weighted!) average over the last frames
    const float currentSpeed = totalSpeed / totalPoints;
    assert(std::isfinite(currentSpeed));
    return currentSpeed;
}

float History::estimateOrientationRad(float* confidence) const {
    // can't give estimate if not enough poses available
    if (_histComponents.size() < 3) return std::numeric_limits<float>::quiet_NaN();

    std::deque<FishPose>::const_reverse_iterator iter = _histComponents.rbegin();
    cv::Point2f nextPoint = iter->position_cm();
    cv::Point2f positionDerivative(0.0f, 0.0f);

    // weights the last poses with falloff^k * pose[end - k] until falloff^k < falloffMargin
    int posesUsed = 0;
    float currentWeight = 1.0f;
    float weightSum = 0.0f;
    const float falloff = 0.9f;
    const float falloffMargin = 0.4f;

    while (++iter != _histComponents.rend()) {
        const cv::Point2f& currentPoint = iter->position_cm();
        const cv::Point2f oneStepDerivative = nextPoint - currentPoint;

        positionDerivative += currentWeight * oneStepDerivative;
        weightSum += currentWeight;

        currentWeight *= falloff;
        if (currentWeight < falloffMargin) break;

        nextPoint = currentPoint;
        ++posesUsed;
    }
    // calculate average (weighted) movement of the fish
    if (weightSum != 0.0f) {
        positionDerivative.x /= weightSum;
        positionDerivative.y /= weightSum;
    }
    // use the euclidian distance in cm
    const float distance = std::sqrt(std::pow(positionDerivative.x, 2.0f) + std::pow(positionDerivative.y, 2.0f));
    // Calculate cm/s.
    //TODO FishTrackerThread::instance()->getRealTimePerFrameMs() IS: 1000/FPS
    const float distanceNormalized = 1000.0f * distance / static_cast<float>(1000.0 / 30.0);
    const float confidenceDistanceMinCm = 2.0f;
    const float confidenceDistanceMaxCm = 6.0f;
    // if we have either nearly no data or are very unsure (left movement offsets right movement f.e.), just return nothing
    if (distanceNormalized < confidenceDistanceMinCm)
        return std::numeric_limits<float>::quiet_NaN();
    *confidence = std::min(distanceNormalized / confidenceDistanceMaxCm, 1.0f);

    // negative y coordinate to offset open cv coordinate system
    return std::atan2(-positionDerivative.y, positionDerivative.x);
}

int History::getHistoryLength() const {
    QMutexLocker locker(&histLenMutex);
    return _histMaxSize;
}
int History::getPoseCount() const {
    return static_cast<int>(_histComponents.size());
}

void History::updateLength(int newLength) {
    //TODO need to fix the history length issue

    QMutexLocker locker(&histLenMutex);
    if (newLength < 0) return;

    // sets the minimal length of the history cue
    //int minLength = newLength > _histMaxSize ? _histMaxSize : newLength;

    // set new history queue length
    _histMaxSize = newLength;
    _histComponents.resize(_histMaxSize);
    /*int newDiff = _histComponents.size() - newLength;

    if (newDiff > 0)
    {
        while (newDiff-- >= 0) {
            _histComponents.pop_front();
        }
    }*/
}


//void History::updateLength(int newLength)
//{
//	if(newLength < 0) return;
//
//
//	std::vector<FishPose> newHistComponents(newLength);
//
//	int minLength = newLength > _histMaxSize ? _histMaxSize : newLength;
//
//	int nextInsertedPosition = 0;
//
//	// copy all data from all vector to a new vector
//	for(int i = 0; i < minLength; i++)
//	{
//		nextInsertedPosition = (_nextInsertedPosition + i) % _histMaxSize;
//		newHistComponents.push_back(_histComponents.at(nextInsertedPosition));
//	}
//
//	// restore the old attributes
//	_nextInsertedPosition = minLength;
//	_histMaxSize = newLength;
//	_histComponents = newHistComponents;
//}

std::deque<FishPose> History::poses() {
    return _histComponents;
}
