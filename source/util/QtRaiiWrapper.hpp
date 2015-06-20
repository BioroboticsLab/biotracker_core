#pragma once

namespace BioTracker {
namespace Util {

/**
 * source: http://stackoverflow.com/a/28125217
 */
template<typename UiClass, typename ParentClass> class QtRaiiWrapper : public UiClass
{
public:
    QtRaiiWrapper(ParentClass *parent)
    {
        this->setupUi(parent);
    }
};

}
}
