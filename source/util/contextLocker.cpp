#include "contextLocker.h"

namespace BioTracker {
namespace Util {

ContextLocker::ContextLocker(Core::Facade &facade)
    : m_facade(facade)
{
    m_facade.getTrackingThread().requestContext();
    m_facade.getTrackingThread().getContextNotCurrent().Lock();

}

ContextLocker::~ContextLocker()
{
    //m_facade.getOpenGLContext()->moveToThread(&m_facade.getTrackingThread());
    m_facade.getTrackingThread().getContextNotCurrent().Unlock();
}

}   // namespace Util
}   // namespace BioTracker
