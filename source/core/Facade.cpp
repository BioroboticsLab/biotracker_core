#include "Facade.h"
#include <limits>

namespace BioTracker {
namespace Core {

Facade::Facade()
        : m_settings()
        , m_registry(Registry::getInstance())
        , m_trackingThread(m_settings)
{}

Facade::Facade(QOpenGLContext *context)
        : m_settings()
        , m_registry(Registry::getInstance())
        , m_trackingThread(m_settings, context)
{}

Facade::~Facade()
{
    m_trackingThread.terminate();
    m_trackingThread.wait();

}

void Facade::openVideo(const boost::filesystem::path &path)
{
    m_trackingThread.loadVideo(path);
}

void Facade::openImages(std::vector<boost::filesystem::path> paths)
{
    m_trackingThread.loadPictures(std::move(paths));
}

void Facade::openCamera(int device) {
	m_trackingThread.openCamera(device);
}

void Facade::play()
{
    m_trackingThread.enableVideoPause(false);
}

void Facade::pause()
{
    m_trackingThread.enableVideoPause(true);
}

void Facade::setFrame(const size_t frameNumber)
{
    m_trackingThread.setFrameNumber(frameNumber);
}

void Facade::setTargetFps(const double fps)
{
    m_trackingThread.setFps(fps);
}

double Facade::getTargetFps() const
{
    return std::numeric_limits<double>::infinity();
}

size_t Facade::getNumFrames() const
{
    return m_trackingThread.getVideoLength();
}

size_t Facade::getCurrentFrameNumber() const
{
    return m_trackingThread.getFrameNumber();
}

void Facade::setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm> trackingAlgorithm)
{
    m_trackingThread.setTrackingAlgorithm(std::move(trackingAlgorithm));
}

void Facade::mouseEvent(QMouseEvent *event)
{
    m_trackingThread.mouseEvent(event);
}

void Facade::keyboardEvent(QKeyEvent *event)
{
    m_trackingThread.keyboardEvent(event);
}

} // Core
} // BioTracker
