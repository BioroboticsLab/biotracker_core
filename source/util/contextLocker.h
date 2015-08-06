#include "source/core/BioTrackerApp.h"
#include "source/core/TrackingThread.h"

namespace BioTracker {
namespace Util {

class ContextLocker
{
public:
    ContextLocker(Core::BioTrackerApp& facade);
    ~ContextLocker();

private:
    Core::BioTrackerApp &m_facade;
};

}   // namespace Util
}   // namespace BioTracker

