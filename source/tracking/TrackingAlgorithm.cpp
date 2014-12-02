#include "TrackingAlgorithm.h"

#include <fstream>

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>

TrackingAlgorithm::TrackingAlgorithm(Settings &settings, std::string &serializationPath, QWidget *parent)
    : QObject(parent)
    , _settings(settings)
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
std::shared_ptr<QWidget> TrackingAlgorithm::getToolsWidget()	{ return std::make_shared<QWidget>(); }
std::shared_ptr<QWidget> TrackingAlgorithm::getParamsWidget()	{ return std::make_shared<QWidget>(); }
