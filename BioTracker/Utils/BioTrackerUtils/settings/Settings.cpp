#include "settings/Settings.h"

#include <boost/filesystem.hpp>

#include <QFile>
#include <QMessageBox>

#include "util/Exceptions.h"
#include "settings/Messages.h"


namespace boost {
	void throw_exception(std::exception const & e) {};
}

namespace BioTracker {
namespace Core {

const boost::property_tree::ptree Settings::getDefaultParams() {
    boost::property_tree::ptree pt;

    return pt;
}

}
}
