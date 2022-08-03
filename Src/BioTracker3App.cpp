#include "BioTracker3App.h"

BioTracker3App::BioTracker3App(QObject* parent)
: QObject(parent)
{
}

void BioTracker3App::runBioTracker()
{
    m_BioTrackerContext->createApplication();
}

void BioTracker3App::setBioTrackerContext(IBioTrackerContext* context)
{
    m_BioTrackerContext = context;
}

IBioTrackerContext* BioTracker3App::getBioTrackerContext()
{
    return m_BioTrackerContext;
}
