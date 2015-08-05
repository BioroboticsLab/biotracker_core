#include "BioTrackerApp.h"

namespace BioTracker {
namespace Core {

BioTrackerApp::BioTrackerApp()
{}

BioTrackerApp::BioTrackerApp(QOpenGLContext *context)
    : m_facade(context)
{
    // m_facade = new Facade(context);
}

void BioTrackerApp::unknownError(const std::exception &err) const
{

}

void BioTrackerApp::notify(const std::string &message, const MSGS::MTYPE type) const
{

}

void BioTrackerApp::frameCalculated(const size_t frameNumber, const std::string &filename, const double currentFps) const
{

}


}
}

