#include "Facade.h"
#include <limits>

namespace BioTracker {
namespace Core {

Facade::Facade()
    :m_settings()
    ,m_registry(Algorithms::Registry::getInstance())
    ,m_trackingThread(m_settings)
    ,m_status(TrackerStatus::NothingLoaded)
{

}

Facade::~Facade()
{
    m_trackingThread.terminate();
    m_trackingThread.wait();

}

void Facade::openVideo(const boost::filesystem::path &path)
{

}

void Facade::openImages(const std::vector<boost::filesystem::path> &paths)
{

}

void Facade::play()
{

}

void Facade::pause()
{

}

void Facade::setFrame(const size_t frameNumber)
{

}

void Facade::setTargetFps(const double fps)
{

}

double Facade::getTargetFps() const
{
    return std::numeric_limits<double>::infinity();
}

size_t Facade::getNumFrames() const
{
    return 0;
}

size_t Facade::getCurrentFrameNumber() const
{
    return 0;
}

void Facade::mouseEvent(QMouseEvent *event)
{

}

void Facade::keyboardEvent(QKeyEvent *event)
{

}

} // Core
} // BioTracker
