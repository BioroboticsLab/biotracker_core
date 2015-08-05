#pragma once

#include <QObject>

#include "source/core/Facade.h"

namespace BioTracker {
namespace Core {

class BioTrackerApp : public QObject
{
    Q_OBJECT
public:
    BioTrackerApp();
    BioTrackerApp(QOpenGLContext *context);

protected slots:
    void unknownError(const std::exception &err) const;
    //void fileError(const fileException &err) const;

    void notify(const std::string &message, const MSGS::MTYPE type) const;
    void frameCalculated(const size_t frameNumber, const std::string &filename, const double currentFps) const;

protected:
    Facade m_facade;
};

} // Core
} // BioTracker
