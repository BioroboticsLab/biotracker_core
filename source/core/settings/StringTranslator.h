#pragma once

#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

#include "ParamNames.h"

namespace StringTranslator {

template<typename external_type>
struct BiMapContainer {
    typedef boost::bimap<external_type, std::string> bm_type;
    typename bm_type::relation bm_relation;
    static const bm_type value;
};

// enables conversion from/to an arbitrary type and an std::string by using
// a boost::bimap. Requires a template specialization of BiMapContainer with
// external_type.
template <typename external_type>
struct StringTranslator {
    typedef std::string internal_type;
    typename BiMapContainer<external_type>::bm_type bm_type;

    const boost::bimap<external_type, std::string> bimap =
        BiMapContainer<external_type>::value;

    boost::optional<external_type> get_value(const internal_type &internal) {
        return boost::optional<external_type>(bimap.right.at(internal));
    }

    boost::optional<internal_type> put_value(const external_type &external) {
        return boost::optional<internal_type>(bimap.left.at(external));
    }
};

}

namespace boost {
namespace property_tree {

// template specialization for EnumTranslator for GUIPARAM::MediaType
// this allows boost::property_tree to store a string representation of
// MediaType in the settings file
template<>
struct translator_between<std::string, GUIPARAM::MediaType> {
    typedef StringTranslator::StringTranslator<GUIPARAM::MediaType> type;
};

}
}
