#include "AreaInfo.h"

#include "Rectification.h"

#include "settings/Settings.h"
#include "util/misc.h"
#include <qvector.h>
#include "AreaMemory.h"

using namespace AreaMemory;

AreaInfo::AreaInfo(QObject *parent) :
    IModelAreaDescriptor(parent)
{
    _parms = nullptr;
    _rect = std::make_shared<AreaInfoElement>();
    _rect->setShowNumbers(true);
    _rect->setAreaType(BiotrackerTypes::AreaType::RECT);
    _apperture = std::make_shared<AreaInfoElement>();
    _apperture->setAreaType(BiotrackerTypes::AreaType::APPERTURE);
    loadAreas();
    
}

QString AreaInfo::myType(){
    if (_apperture != nullptr) {
        return QString::number(_apperture->getType());
    }
    return "0";
}

void AreaInfo::reset(int w, int h) {
    _useEntireScreen = true;
    _rectInitialized = false;
    _vdimX = w;
    _vdimY = h;


    QVector<QString> vertices = _parms != nullptr ? getVertices(_parms->m_CurrentFilename) : QVector<QString>();

    if ((!vertices.empty() && vertices[1] == QString(DEFAULT_RECT) && vertices[2] == QString(DEFAULT_AREA)) || vertices.empty()) {
        std::vector<cv::Point> v{
            cv::Point(0,0),
            cv::Point(0,_vdimY),
            cv::Point(_vdimX,_vdimY),
            cv::Point(_vdimX,0)
        };
        _apperture->setVertices(v);
        _rect->setVertices(v);
    }
    else
    {
        std::vector<QPoint> pts = toQPointVector(vertices[0]);
        std::vector<QPoint> pts2 = toQPointVector(vertices[1]);
        _apperture->setVertices(QVecToCvVec(pts));
        _rect->setVertices(QVecToCvVec(pts2));
    }
}

void AreaInfo::loadAreas() {

    QVector<QString> pair = getVertices(_parms ? _parms->m_CurrentFilename : "");

    if (pair[1] == QString(DEFAULT_RECT)) {
        if (_parms) {
            std::shared_ptr<cv::Mat> m = _parms->m_CurrentFrame;
            reset(m->size().width, m->size().height);
        }
        else {
            reset(100, 100);
        }
    }


    std::vector<cv::Point> p = stringToCVPointVec(pair[0].toStdString());
    _rect->setVertices(p);

    p = stringToCVPointVec(pair[1].toStdString());;
    _apperture->setVertices(p);
}

void AreaInfo::rcvPlayerParameters(playerParameters* parameters)
{
    if (_parms == nullptr || _parms->m_CurrentFilename != parameters->m_CurrentFilename) {
        _rectInitialized = false;
    }
    _parms = parameters;
    std::shared_ptr<cv::Mat> m = parameters->m_CurrentFrame;
    if ((m->size().width != _vdimX || m->size().height != _vdimY) &&
        _useEntireScreen) {
        reset(m->size().width, m->size().height);
        loadAreas();
        updateRectification();
    }
}

void AreaInfo::updateRectification() {
    if (_parms) {
        if (!_rectInitialized) {
            QVector<QString> vertices = getVertices(_parms->m_CurrentFilename);
            std::vector<QPoint> pts = toQPointVector(vertices[0]);
            Rectification::instance().setArea(pts);
            Rectification::instance().setupRecitification(100, 100, _vdimX, _vdimY);
            _rectInitialized = true;
        }
        else {
            QVector<QString> vertices = getVertices(_parms->m_CurrentFilename);
            Rectification::instance().setArea(_rect->getQVertices());
            Rectification::instance().setupRecitification(100, 100, _vdimX, _vdimY);

            setVertices(_parms->m_CurrentFilename,
                QVector<QString>{
                cvPointsToString(_rect->getVertices()).c_str(),
                cvPointsToString(_apperture->getVertices()).c_str(),
                QString(myType()), QString(vertices[3])});  //TODO
        }
    }
    else {
        //TODO: What's to do here?
    }
}

void AreaInfo::setRectificationDimensions(double w, double h) {
	Rectification::instance().setDimension(w,h);
}

void AreaInfo::updateApperture() {

    if (_parms && _rectInitialized) {
            QVector<QString> vertices = getVertices(_parms->m_CurrentFilename);
            std::vector<cv::Point> p = _apperture->getVertices();

            setVertices(_parms->m_CurrentFilename,
                QVector<QString>{
                cvPointsToString(_rect->getVertices()).c_str(),
                cvPointsToString(p).c_str(),
                QString(myType()), QString(vertices[3]) });  //TODO
  
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
