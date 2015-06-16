#include "Facade.h"
#include <limits>

namespace BioTracker {
namespace Core {

// === CTOR



// === PUBLIC

Facade::Facade()
    :m_settings()
    ,m_registry(Algorithms::Registry::getInstance())
    ,m_trackingThread(m_settings)
{

}

Settings &Facade::getSettings()
{
    return m_settings;
}

Algorithms::Registry &Facade::getRegistry()
{
    return m_registry;
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

void Facade::mouseEvent(QMouseEvent *event)
{

}

void Facade::keyboardEvent(QKeyEvent *event)
{

}

} // Core
} // BioTracker
