#include "settings/StringTranslator.h"

namespace {
typedef GuiParam::MediaType EnumType;
}

namespace StringTranslator {
template<>
const BiMapContainer<EnumType>::bm_type BiMapContainer<EnumType>::value =
    boost::assign::list_of<BiMapContainer<EnumType>::bm_type::relation>
    (EnumType::NoMedia, "NoMedia")
    (EnumType::Images, "Images")
    (EnumType::Video, "Video");
}
