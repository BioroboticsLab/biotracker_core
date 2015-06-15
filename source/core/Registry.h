#ifndef Algorithms_H
#define Algorithms_H

#include <map>

#include "source/core/TrackingAlgorithm.h"
#include "source/util/stdext.h"
#include "source/util/singleton.h"

namespace Algorithms {
typedef std::shared_ptr<TrackingAlgorithm> (*new_tracker_function_t) (Settings& settings, QWidget *parent);
typedef uint8_t Type;

static const Type NoTracking = 0;
// construct on first use idiom
Type getNextId();

class Registry : public Singleton<Registry> {

public:
    typedef std::map<const Type, new_tracker_function_t> map_type_fun_t;
    typedef std::map<const Type, const std::string> map_type_string_t;
    typedef std::map<const std::string, const Type> map_string_type_t;

private:
    friend class Singleton<Registry>;
	Registry();
	map_string_type_t _typeByString;
	map_type_string_t _stringByType;
	map_type_fun_t _trackerByType;

public:
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
    std::shared_ptr<TrackingAlgorithm> make_new_tracker(const Type name, Settings& settings, QWidget *parent) const;

    map_string_type_t const& typeByString() const { return _typeByString; }
    map_type_fun_t const& trackerByType() const { return _trackerByType; }
    map_type_string_t const& stringByType() const { return _stringByType; }

private:
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

#endif
