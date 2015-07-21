#include "source/core/Facade.h"
#include "source/core/TrackingThread.h"

namespace BioTracker {
namespace Util {

class ContextLocker
{
public:
    ContextLocker(Core::Facade& facade);
    ~ContextLocker();

private:
    Core::Facade &m_facade;
};

}   // namespace Util
}   // namespace BioTracker

