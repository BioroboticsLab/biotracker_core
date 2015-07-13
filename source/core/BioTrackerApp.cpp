#include "BioTrackerApp.h"

namespace BioTracker {
namespace Core {

BioTrackerApp::BioTrackerApp()
{

}

BioTrackerApp::BioTrackerApp(Util::QOpenGLContextWrapper *context)
    :   m_facade(context)
{
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

