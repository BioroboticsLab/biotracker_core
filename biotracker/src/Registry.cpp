#include "Registry.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>

#include <QLibrary>

#include "Exceptions.h"

namespace BioTracker {
namespace Core {

Registry::Registry() {
    m_typeByString.insert(std::make_pair("No Tracking", NoTracking));
    m_trackerByType.insert(std::make_pair(NoTracking, nullptr));
    m_stringByType.insert(std::make_pair(NoTracking, "No Tracking"));
}

bool Registry::registerTrackerType(std::string name,
                                   std::shared_ptr<NewTrackerFactory> f) {
    if (m_typeByString.find(name) != m_typeByString.end()) {
        throw std::invalid_argument("Tracker with same name already registered");
    }
    const TrackerType type = getNextId();
    m_typeByString.emplace(name, type);
    m_stringByType.emplace(type, name);
    m_trackerByType.emplace(type, f);

    Q_EMIT newTracker(type);

    return true;
}

void Registry::loadTrackerLibrary(const boost::filesystem::path &path) {
    typedef void (*RegisterFunction)();

    if (!boost::filesystem::exists(path)) {
        throw file_not_found("Could not find file " + path.string());
    }

    std::vector<std::string> parts;
    boost::split(parts, path.string(), boost::is_any_of("."));

    /* expect filename to be of form: name.tracker.ext */
    if (parts.size() < 3) {
        throw std::invalid_argument("Filename must obey pattern $name.tracker.$ext");
    }

    const std::string ext(parts[parts.size() - 1]);
    if (ext == "so" || ext == "dylib" || ext == "dll") {
        QLibrary trackerLibrary(QString::fromStdString(path.string()));
        auto registerFunction = static_cast<RegisterFunction>
                                (trackerLibrary.resolve("registerTracker"));

        registerFunction();
    } else if (ext == "zmq") {
        registerZmqTracker(Zmq::getInfo(path));
    } else {

    }
}

std::shared_ptr<TrackingAlgorithm> Registry::makeNewTracker(
    const TrackerType type, Settings &settings) const {
    const auto &it = m_trackerByType.find(type);
    if (it != m_trackerByType.cend()) {
        std::shared_ptr<NewTrackerFactory> factory = it->second;
        std::shared_ptr<TrackingAlgorithm> tracker = (*factory)(settings);
        tracker->setType(type);
        return tracker;
    } else {
        return nullptr;
    }
}

std::shared_ptr<TrackingAlgorithm> Registry::getTracker(Zmq::ZmqInfoFile &info,
        Settings &s) const {
    auto tracker = std::make_shared<Zmq::ZmqTrackingAlgorithm>(info, s);
    return tracker;
}


TrackerType getNextId() {
    static TrackerType nextType = NoTracking + 1;

    return nextType++;
}

bool BioTracker::Core::Registry::registerZmqTracker(Zmq::ZmqInfoFile
        trackerInfo) {
    struct NewZmqTrackerFactory : NewTrackerFactory {
        NewZmqTrackerFactory(Zmq::ZmqInfoFile trackerInfo) : m_trackerInfo(
                trackerInfo) {}

        virtual std::shared_ptr<TrackingAlgorithm> operator()(Settings &settings)
        const override {
            return std::make_shared<Zmq::ZmqTrackingAlgorithm>(m_trackerInfo, settings);
        }

        Zmq::ZmqInfoFile m_trackerInfo;
    };
    return this->registerTrackerType(trackerInfo.m_name.toStdString(),
                                     std::make_shared<NewZmqTrackerFactory>(trackerInfo));
}

}
}
