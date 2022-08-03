#include "AreaInfo.h"

#include "Rectification.h"

#include "Utility/misc.h"
#include <qvector.h>
#include "AreaMemory.h"

using namespace AreaMemory;
using namespace BioTrackerUtilsMisc;

AreaInfo::AreaInfo(QObject* parent)
: IModelAreaDescriptor(parent)
{
    _parms = nullptr;
    _areaInfoCache =
        static_cast<IControllerCfg*>(parent)->getConfig()->AreaDefinitions;
    _rect = std::make_shared<AreaInfoElement>();
    _rect->setShowNumbers(true);
    _rect->setAreaType(BiotrackerTypes::AreaType::RECT);
    _apperture = std::make_shared<AreaInfoElement>();
    _apperture->setAreaType(BiotrackerTypes::AreaType::APPERTURE);
    _useEntireScreen = true;
    loadAreas();
    updateRectification();
}

QString AreaInfo::myType()
{
    if (_apperture != nullptr) {
        return QString::number(_apperture->getType());
    }
    return "0";
}

void AreaInfo::reset(int w, int h)
{
    _useEntireScreen = true;
    _rectInitialized = false;
    _vdimX           = w;
    _vdimY           = h;

    QVector<QString> vertices = _parms != nullptr
                                    ? getVertices(_parms->m_CurrentFilename,
                                                  _areaInfoCache)
                                    : QVector<QString>();

    if (vertices == DEFAULT_PAIR // couldn't find entry
        || vertices.empty())     // biotracker just started
    {
        std::vector<cv::Point> v{cv::Point(0, 0),
                                 cv::Point(0, _vdimY),
                                 cv::Point(_vdimX, _vdimY),
                                 cv::Point(_vdimX, 0)};
        _apperture->setVertices(v);
        _rect->setVertices(v);
    } else {
        std::vector<QPoint> pts  = toQPointVector(vertices[0]);
        std::vector<QPoint> pts2 = toQPointVector(vertices[1]);
        _apperture->setVertices(QVecToCvVec(pts2));
        _rect->setVertices(QVecToCvVec(pts));
    }
}

void AreaInfo::loadAreas()
{

    QVector<QString> pair = getVertices(_parms ? _parms->m_CurrentFilename
                                               : "",
                                        _areaInfoCache,
                                        true);

    if (pair[1] == QString(DEFAULT_RECT)) {
        if (_parms) {
            auto m = _parms->m_CurrentFrame;
            reset(m->size().width, m->size().height);
        } else {
            reset(100, 100);
        }
    }

    std::vector<cv::Point> p = stringToCVPointVec(pair[0].toStdString());
    _rect->setVertices(p);

    p = stringToCVPointVec(pair[1].toStdString());
    ;
    _apperture->setVertices(p);
}

void AreaInfo::rcvPlayerParameters(
    std::shared_ptr<const playerParameters> parameters)
{
    if (_parms == nullptr ||
        _parms->m_CurrentFilename != parameters->m_CurrentFilename) {
        _rectInitialized = false;
    }
    if (!parameters->m_CurrentFrame) {
        return;
    }

    _parms = parameters;
    auto m = *parameters->m_CurrentFrame;
    if ((m.size().width != _vdimX || m.size().height != _vdimY) &&
        _useEntireScreen) { // TODO: _useEntireScreen? if we do not update
                            // stuff in the very beginning, tracking
                            // rectification will break
        reset(m.size().width, m.size().height);
        loadAreas();
        updateRectification();
    }
}

void AreaInfo::updateRectification()
{
    if (_parms) {
        if (!_rectInitialized) {
            QVector<QString> vertices = getVertices(_parms->m_CurrentFilename,
                                                    _areaInfoCache);
            std::vector<QPoint> pts   = toQPointVector(vertices[0]);
            Rectification::instance().setArea(pts);
            Rectification::instance().setupRecitification(100,
                                                          100,
                                                          _vdimX,
                                                          _vdimY);
            _rectInitialized = true;
        } else {
            QVector<QString> vertices = getVertices(_parms->m_CurrentFilename,
                                                    _areaInfoCache);
            Rectification::instance().setArea(_rect->getQVertices());
            Rectification::instance().setupRecitification(100,
                                                          100,
                                                          _vdimX,
                                                          _vdimY);

            setVertices(
                _parms->m_CurrentFilename,
                QVector<QString>{
                    cvPointsToString(_rect->getVertices()).c_str(),
                    cvPointsToString(_apperture->getVertices()).c_str(),
                    QString(myType()),
                    QString(vertices[3])},
                _areaInfoCache);

            setVertices(
                "Last",
                QVector<QString>{
                    cvPointsToString(_rect->getVertices()).c_str(),
                    cvPointsToString(_apperture->getVertices()).c_str(),
                    QString(myType()),
                    QString(vertices[3])},
                _areaInfoCache);
        }
    } else {
        // TODO: What's to do here?
    }
}

void AreaInfo::setRectificationDimensions(double w, double h)
{
    Rectification::instance().setDimension(w, h);
}

void AreaInfo::updateApperture()
{

    if (_parms && _rectInitialized) {
        QVector<QString> vertices = getVertices(_parms->m_CurrentFilename,
                                                _areaInfoCache);
        std::vector<cv::Point> p  = _apperture->getVertices();

        setVertices(
            _parms->m_CurrentFilename,
            QVector<QString>{cvPointsToString(_rect->getVertices()).c_str(),
                             cvPointsToString(p).c_str(),
                             QString(myType()),
                             QString(vertices[3])},
            _areaInfoCache); // TODO
    }
}

bool AreaInfo::inTrackingArea(cv::Point2f point_cm)
{
    return _apperture->insideElement(point_cm);
}

cv::Point2f AreaInfo::pxToCm(cv::Point point_px)
{
    return Rectification::instance().pxToCm(point_px);
}

cv::Point2f AreaInfo::cmToPx(cv::Point2f point_cm)
{
    return Rectification::instance().cmToPx(point_cm);
}
