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

bool Registry::registerTrackerType(std::string name, new_tracker_function_t f) {
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
        /*
        =======
        const QString str = QString::fromStdString(path.string());
        if (str.endsWith("zmq")) {
         Zmq::ZmqInfoFile info = Zmq::getInfo(path);

         Q_EMIT newZmqTracker(info);

        } else {
        >>>>>>> first steps for zmq implementa. Strings may be received by the tracker now
                 */
        QLibrary trackerLibrary(QString::fromStdString(path.string()));
        auto registerFunction = static_cast<RegisterFunction>
                                (trackerLibrary.resolve("registerTracker"));

        registerFunction();
    } else if (ext == "zmq") {
        std::cout << "LOAD ZMQ THINGY" << std::endl;
        Zmq::ZmqInfoFile info = Zmq::getInfo(path);
        Q_EMIT newZmqTracker(info);
    } else {

    }
}

std::shared_ptr<TrackingAlgorithm> Registry::makeNewTracker(
    const TrackerType type, Settings &settings, QWidget *parent) const {
    const auto &it = m_trackerByType.find(type);
    if (it != m_trackerByType.cend()) {
        std::shared_ptr<TrackingAlgorithm> tracker = (it->second)(settings, parent);
        tracker->setType(type);
        return tracker;
    } else {
        return nullptr;
    }
}

std::shared_ptr<TrackingAlgorithm> Registry::getTracker(Zmq::ZmqInfoFile &info,
        Settings &s, QWidget *p) const {
    auto tracker = std::make_shared<Zmq::ZmqTrackingAlgorithm>(info, s, p);
    return tracker;
}

TrackerType getNextId() {
    static TrackerType nextType = NoTracking + 1;

    return nextType++;
}

}
}
