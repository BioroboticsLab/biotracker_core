#ifndef Algorithms_H
#define Algorithms_H

#include <map>

#include "source/tracking/TrackingAlgorithm.h"
#include "source/utility/stdext.h"
#include "source/utility/singleton.h"

namespace Algorithms {
typedef std::shared_ptr<TrackingAlgorithm> (*new_tracker_function_t) (Settings& settings, std::string& serializationPath, QWidget *parent);
typedef uint8_t Type;

static const Type NoTracking = 0;
// construct on first use idiom
Type getNextId();

class Registry : public Singleton<Registry> {

public:
    typedef std::map<const Type, new_tracker_function_t> map_type_t;
    typedef std::map<const std::string, const Type> map_string_t;

private:
    friend class Singleton<Registry>;
	Registry();
	map_string_t _typeByString;
	map_type_t _trackerByType;

public:
    /**
     * @see register_tracker_type(std::string, new_tracker_function_t);
     * automagically creates f :)
     * @param name : name of tracking alg
     * @return
     */
    template<class TRACKER>
    bool register_tracker_type(std::string name);

    /**
     * creates a new tracker-instance
     * @param name
     * @param tracker_arg : forwarded to tracking algs constructor
     * @return : new instance
     */
    std::shared_ptr<TrackingAlgorithm> make_new_tracker(const Type name, Settings& settings, std::string& serializationPath, QWidget *parent) const;

    map_string_t const& typeByString() const { return _typeByString; }
    map_type_t const& trackerByType() const { return _trackerByType; }

private:
    /**
     * adds new tracker-type to this
     * @param name : name of tracking alg
     * @param f : function that creates a new instance
     * @return dummy value
     * @throws
     */
    bool register_tracker_type(std::string name, new_tracker_function_t f);
};

template<class TRACKER>
bool Registry::register_tracker_type(std::string name) {
    struct local_function {
        static std::shared_ptr<TrackingAlgorithm> f(Settings& settings, std::string& serializationPath, QWidget *parent) {
            return std::make_shared<TRACKER>(settings, serializationPath, parent);
        }
    };
    return this->register_tracker_type(std::move(name), local_function::f);
}

}

#endif
