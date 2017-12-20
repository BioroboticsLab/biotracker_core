#include "AreaInfo.h"

#include "Rectification.h"

#include "settings/Settings.h"
#include "util/misc.h"

#define DEFAULT_RECT        "15,15;15,105;105,105;105,15"

AreaInfo::AreaInfo(QObject *parent) :
	IModelAreaDescriptor(parent)
{

	_rect = std::make_shared<AreaInfoElement>();
	_apperture = std::make_shared<AreaInfoElement>();

	std::vector<cv::Point> p = Rectification::instance().getDefaultArena();
	_rect->setVertices(p);
    _rect->setShowNumbers(true);
    _rect->setAreaType(BiotrackerTypes::AreaType::RECT);


	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	std::string arena = _settings->getValueOrDefault<std::string>(AREADESCRIPTOR::CN_APPERTURE, DEFAULT_RECT);
	p = stringToCVPointVec(arena);
    if (arena == DEFAULT_RECT) {
        _useEntireScreen = true;
    }

	_apperture->setVertices(p);
    _apperture->setAreaType(BiotrackerTypes::AreaType::APPERTURE);
}

void AreaInfo::rcvPlayerParameters(playerParameters* parameters)
{
    std::shared_ptr<cv::Mat> m = parameters->m_CurrentFrame;
    if ((m->size().width != _vdimX || m->size().height != _vdimY) &&
        _useEntireScreen) {
        _vdimX = m->size().width;
        _vdimY = m->size().height;

        std::vector<cv::Point> v{
            cv::Point(0,0),
            cv::Point(0,_vdimY),
            cv::Point(_vdimX,_vdimY),
            cv::Point(_vdimX,0)
        };
        _apperture->setVertices(v);
        _rect->setVertices(v);
        updateRectification();
    }
}

void AreaInfo::updateRectification() {

	Rectification::instance().setArea(_rect->getQVertices());
	Rectification::instance().setupRecitification(100, 100, _vdimX, _vdimY);
}

void AreaInfo::setRectificationDimensions(double w, double h) {
	Rectification::instance().setDimension(w,h);
}

void AreaInfo::updateApperture() {
    BioTracker::Core::Settings *settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
    bool darea = settings->getValueOrDefault<bool>(AREADESCRIPTOR::DISP_AREA, false);
    if (!_useEntireScreen) {
        std::vector<cv::Point> p = _apperture->getVertices();
        settings->setParam(AREADESCRIPTOR::CN_APPERTURE, cvPointsToString(p).c_str());
    }
}

bool AreaInfo::inTrackingArea(cv::Point2f point_cm) {
	return _apperture->insideElement(point_cm);
}

cv::Point2f AreaInfo::pxToCm(cv::Point point_px) {
	return Rectification::instance().pxToCm(point_px);
}

cv::Point2f AreaInfo::cmToPx(cv::Point2f point_cm) {
	return Rectification::instance().cmToPx(point_cm);
}
