#include "BioTrackerTrackingAlgorithm.h"
#include <future>
#include "TrackedComponents/TrackedComponentFactory.h"
#include <chrono>

#include "settings/Settings.h"

BioTrackerTrackingAlgorithm::BioTrackerTrackingAlgorithm(IModel *parameter, IModel *trajectory) 
{
	_TrackingParameter = (TrackerParameter*)parameter;
	_TrackedTrajectoryMajor = (TrackedTrajectory*)trajectory;
	BioTracker::Core::Settings *set = _TrackingParameter->getSettings();

    _lastImage = nullptr;
    _lastFramenumber = -1;
}


void BioTrackerTrackingAlgorithm::receiveAreaDescriptorUpdate(IModelAreaDescriptor *areaDescr) {
	_AreaInfo = areaDescr;
}

BioTrackerTrackingAlgorithm::~BioTrackerTrackingAlgorithm()
{
}

void BioTrackerTrackingAlgorithm::receiveParametersChanged() {
    if (_lastFramenumber >= 0 && _lastImage && !_lastImage->empty()) {
        doTracking(_lastImage, _lastFramenumber);
    }
}


void clampPosition(std::vector<cv::Point2f> &pos, int w, int h) {
    // When points are outside the image boarders they cannot be rescued anymore
    // This function clamps them to be inside the image again which is actually not
    // very nice. See: https://github.com/BioroboticsLab/biotracker_lucasKanade/issues/8
    for (cv::Point2f &p : pos) {
        if (p.x < 0) {
            p.x = 0;
        }
        else if (p.x > w) {
            p.x = w - 1;
        }
        if (p.y < 0) {
            p.y = 0;
        }
        else if (p.y > h) {
            p.y = h - 1;
        }
    }
}

std::vector<cv::Point2f> getPoints(TrackedTrajectory* trackedTrajectoryMajor, int frameNo) {
   
    std::vector<cv::Point2f> points;
    int trajNumber = 0;
    for (int i = 0; i < trackedTrajectoryMajor->size(); i++) {
        TrackedTrajectory *t = dynamic_cast<TrackedTrajectory *>(trackedTrajectoryMajor->getChild(i));
        if (t && t->getValid() && !t->getFixed()) {
            IModelTrackedPoint* p = dynamic_cast<IModelTrackedPoint*>(t->getChild(frameNo));
            if (p != nullptr) {
                points.push_back(cv::Point2f(p->getX(), p->getY()));
            }
            else {
                points.push_back(cv::Point2f(100, 100));
            }

            trajNumber++;
        }
    }
    return points;
}

void setPoints(TrackedTrajectory* trackedTrajectoryMajor, int frameNo, std::vector<cv::Point2f> points) {

    int trajNumber = 0;
    for (int i = 0; i < trackedTrajectoryMajor->size(); i++) {
        TrackedTrajectory *t = dynamic_cast<TrackedTrajectory *>(trackedTrajectoryMajor->getChild(i));
        if (t && t->getValid() && !points.empty() && !t->getFixed()) {
            cv::Point2f p = points[0];
            points.erase(points.begin());
            TrackedElement *e = new TrackedElement(t, "n.a.", t->getId());
            e->setPoint(p);
            t->add(e, frameNo);

            trajNumber++;
        }
    }
}


void BioTrackerTrackingAlgorithm::doTracking(std::shared_ptr<cv::Mat> p_image, uint framenumber)
{

	//dont do nothing if we ain't got an image
	if (p_image->empty()) {
		return;
	}

	if (_imageX != p_image->size().width || _imageY != p_image->size().height) {
		_imageX = p_image->size().width;
		_imageY = p_image->size().height;
		Q_EMIT emitDimensionUpdate(_imageX, _imageY);
	}

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    int noFish = _TrackedTrajectoryMajor->validCount();
    cv::Mat gray;
    cv::cvtColor(*p_image, gray, cv::COLOR_BGR2GRAY);
    int wndSizeInt = _TrackingParameter->getWndSize();
    cv::Size wndSize = cv::Size(wndSizeInt, wndSizeInt);
    cv::TermCriteria termcrit(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 20, 0.03);

	//if current frame is first frame return
	if (framenumber == 0) {
		Q_EMIT emitTrackingDone(framenumber);
		return;
	}

    std::vector<cv::Point2f> prevPts = getPoints(_TrackedTrajectoryMajor, framenumber-1);

    //if (!currentPointsOnlyActive.empty()) {
    if (_lastImage != nullptr && ! _lastImage->empty() && !prevPts.empty()){
        std::vector<float> err;

        // calculate pyramids:
        const size_t maxLevel = 10;
        std::vector<cv::Mat> prevPyr;
        cv::buildOpticalFlowPyramid(*_lastImage, prevPyr, wndSize, maxLevel);

        std::vector<cv::Mat> pyr;
        cv::buildOpticalFlowPyramid(*p_image, pyr, wndSize, maxLevel);

        std::vector<cv::Point2f> newPoints;
        std::vector<uchar> status;
        cv::calcOpticalFlowPyrLK(
            prevPyr, /* prev */
            pyr, /* next */
            prevPts,	/* prevPts */
            newPoints, /* nextPts */
            status,	/* status */
            err	/* err */
            , wndSize,	/* winSize */
            maxLevel, /* maxLevel */
            termcrit,	/* criteria */
            0, /* flags */
            0.001 /* minEigThreshold */
        );

        clampPosition(newPoints, p_image->size().width, p_image->size().height);
        setPoints(_TrackedTrajectoryMajor, framenumber, newPoints);
    }


    Q_EMIT emitCvMatA(p_image, QString("Original"));
    Q_EMIT emitChangeDisplayImage("Original");


	Q_EMIT emitTrackingDone(framenumber);

    _lastImage = p_image;
    _lastFramenumber = framenumber;
}

