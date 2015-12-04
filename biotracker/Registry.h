#pragma once

#include <map>

#include <boost/filesystem/path.hpp>

#include <QObject>

#include "TrackingAlgorithm.h"
#include "util/stdext.h"
#include "util/singleton.h"
#include "ZmqTrackingAlgorithm.h"

namespace BioTracker {
namespace Core {

typedef std::shared_ptr<TrackingAlgorithm>
(*new_tracker_function_t)(Settings &settings, QWidget *parent);
typedef uint8_t TrackerType;

static const TrackerType NoTracking = 0;
// construct on first use idiom
TrackerType getNextId();

struct NewTrackerFactory {
    virtual std::shared_ptr<TrackingAlgorithm> operator()(Settings &settings,
            QWidget *parent) const = 0;
    virtual ~NewTrackerFactory() {}
};

class Registry : public QObject, public Util::Singleton<Registry> {
    Q_OBJECT
  public:
    typedef std::map<const TrackerType, std::shared_ptr<NewTrackerFactory>>
            map_type_fun_t;
    typedef std::map<const TrackerType, const std::string> map_type_string_t;
    typedef std::map<const std::string, const TrackerType> map_string_type_t;

    /**
     * @brief Load and register a new tracker from a shared library
     * @param path location of library in filesystem
     */
    void loadTrackerLibrary(const boost::filesystem::path &path);

    /**
     * @see register_tracker_type(std::string, new_tracker_function_t);
     * automagically creates f :)
     * @param name name of tracking alg
     */
    template<class TRACKER>
    bool registerTrackerType(std::string name);

    bool registerZmqTracker(Zmq::ZmqInfoFile trackerInfo);

    /**
     * creates a new tracker-instance
     * @return new instance
     */
    std::shared_ptr<TrackingAlgorithm> makeNewTracker(const TrackerType name,
            Settings &settings, QWidget *parent) const;


    std::shared_ptr<TrackingAlgorithm> getTracker(Zmq::ZmqInfoFile &info,
            Settings &s, QWidget *p) const;


    const map_string_type_t &getTypeByString() const {
        return m_typeByString;
    }
    const map_type_fun_t &getTrackerByType() const {
        return m_trackerByType;
    }
    const map_type_string_t &getStringByType() const {
        return m_stringByType;
    }

  Q_SIGNALS:
    void newTracker(const TrackerType type);

  private:
    friend class Singleton<Registry>;

    map_string_type_t m_typeByString;
    map_type_string_t m_stringByType;
    map_type_fun_t m_trackerByType;

    std::map<std::string, Zmq::ZmqInfoFile> m_zmqRegistry;

    Registry();

    /**
     * adds new tracker-type to this
     * @param name name of tracking alg
     * @param f function that creates a new instance
     * @return dummy value
     */
    bool registerTrackerType(std::string name,
                             std::shared_ptr<NewTrackerFactory> f);
};

template<class TRACKER>
bool Registry::registerTrackerType(std::string name) {
    struct NewCppTrackerFactory : NewTrackerFactory {
        virtual std::shared_ptr<TrackingAlgorithm> operator()(Settings &settings,
                QWidget *parent) const override {
            return std::make_shared<TRACKER>(settings, parent);
        }
    };
    return this->registerTrackerType(std::move(name),
                                     std::make_shared<NewCppTrackerFactory>());
}

}
}
