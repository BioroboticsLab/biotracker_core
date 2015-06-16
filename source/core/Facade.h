#pragma once

#include <memory>

#include <boost/filesystem.hpp>

#include <QKeyEvent>
#include <QMouseEvent>

#include "source/core/TrackingThread.h"
#include "source/core/ImageStream.h"
#include "source/core/Registry.h"
#include "source/core/settings/Settings.h"

namespace BioTracker {
namespace Core {

class Facade {
public:
    Settings& getSettings() const;
    Algorithms::Registry& getRegistry() const;

    void openVideo(const boost::filesystem::path &path);
    void openImages(const std::vector<boost::filesystem::path> &paths);

    void play();
    void pause();
    void setFrame(const size_t frameNumber);
    void setTargetFps(const double fps);

    double getTargetFps() const;
    size_t getNumFrames() const;

    void mouseEvent(QMouseEvent *event);
    void keyboardEvent(QKeyEvent *event);

signals:
    void unknownError(const std::exception &err);
    //void fileError(const fileException &err);

    void notify(const std::string &message, const MSGS::MTYPE type);
    void frameCalculated(const size_t frameNumber, const std::string &filename, const double currentFps);

private:
    Settings m_settings;
    Algorithms::Registry m_registry;
    TrackingThread m_trackingThread;
    //std::unique_ptr<ImageStream> m_imageStream;
};

} // Core
} // BioTracker
