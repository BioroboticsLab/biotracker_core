#pragma once

#include <map>

#include <boost/filesystem/path.hpp>

#include <QObject>

#include "source/core/TrackingAlgorithm.h"
#include "source/util/stdext.h"
#include "source/util/singleton.h"

namespace BioTracker {
namespace Core {

typedef std::shared_ptr<TrackingAlgorithm>
    (*new_tracker_function_t) (Settings& settings, QWidget *parent);
typedef uint8_t TrackerType;

static const TrackerType NoTracking = 0;
// construct on first use idiom
TrackerType getNextId();

class Registry : public QObject, public Util::Singleton<Registry>
{
    Q_OBJECT
public:
    typedef std::map<const TrackerType, new_tracker_function_t> map_type_fun_t;
    typedef std::map<const TrackerType, const std::string> map_type_string_t;
    typedef std::map<const std::string, const TrackerType> map_string_type_t;

    /**
     * @brief Load and register a new tracker from a shared library
     * @param path location of library in filesystem
     */
    void load_tracker_library(const boost::filesystem::path &path);

    /**
     * @see register_tracker_type(std::string, new_tracker_function_t);
     * automagically creates f :)
     * @param name name of tracking alg
     */
    template<class TRACKER>
    bool register_tracker_type(std::string name);

    /**
     * creates a new tracker-instance
     * @return new instance
     */
    std::shared_ptr<TrackingAlgorithm> make_new_tracker(const TrackerType name, Settings& settings, QWidget *parent) const;

    const map_string_type_t &typeByString() const { return _typeByString; }
    const map_type_fun_t &trackerByType() const { return _trackerByType; }
    const map_type_string_t &stringByType() const { return _stringByType; }

signals:
    void newTracker(const TrackerType type);

private:
    friend class Singleton<Registry>;

    map_string_type_t _typeByString;
    map_type_string_t _stringByType;
    map_type_fun_t _trackerByType;

    Registry();

    /**
     * adds new tracker-type to this
     * @param name name of tracking alg
     * @param f function that creates a new instance
     * @return dummy value
     */
    bool register_tracker_type(std::string name, new_tracker_function_t f);
};

template<class TRACKER>
bool Registry::register_tracker_type(std::string name) {
    struct local_function {
        static std::shared_ptr<TrackingAlgorithm> f(Settings& settings, QWidget *parent) {
            return std::make_shared<TRACKER>(settings, parent);
        }
    };
    return this->register_tracker_type(std::move(name), local_function::f);
}

}
}
