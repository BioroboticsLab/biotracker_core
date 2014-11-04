#include "TrackingAlgorithm.h"

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>

TrackingAlgorithm::TrackingAlgorithm(Settings &settings, std::string &serializationPath, QWidget *parent)
    : _settings(settings)
    , _parent(parent)
    , _serializationPathName(serializationPath)
{}

TrackingAlgorithm::~TrackingAlgorithm()
{
    std::cout << "Storing in " << _serializationPathName << std::endl;
    std::ofstream ostream(_serializationPathName, std::ios::binary);
    cereal::XMLOutputArchive archive(ostream);
    archive(_trackedObjects);
}

void TrackingAlgorithm::track(ulong /* frameNumber */, cv::Mat &/*frame*/)
{}

void TrackingAlgorithm::loadObjects(std::vector<TrackedObject>&& objects )
{	_trackedObjects = std::move(objects);  }
void TrackingAlgorithm::mouseMoveEvent		( QMouseEvent * )	{}
void TrackingAlgorithm::mousePressEvent		( QMouseEvent * )	{}
void TrackingAlgorithm::mouseReleaseEvent	( QMouseEvent * )	{}
void TrackingAlgorithm::mouseWheelEvent		( QWheelEvent * )	{}
QWidget* TrackingAlgorithm::getToolsWidget	()	{ return  new QWidget(_parent,0); }
QWidget* TrackingAlgorithm::getParamsWidget	()	{ return  new QWidget(_parent,0); }
