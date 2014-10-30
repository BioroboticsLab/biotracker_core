#ifndef Algorithms_H
#define Algorithms_H

#include <map>

#include <qwidget.h>

#include "source/settings/Settings.h"
#include "source/tracking/TrackingAlgorithm.h"
#include "source/tracking/algorithm/simpletracker/SimpleTracker.h"
#include "source/tracking/algorithm/BeesBookTagMatcher/BeesBookTagMatcher.h"
#include "source/tracking/algorithm/colorPatchTracker/colorPatchTracker.h"
#include "source/tracking/algorithm/fishparticle/ParticleFishTracker.h"
#include "source/tracking/algorithm/SampleTracker/SampleTracker.h"
#include "source/tracking/algorithm/LandmarkTracker/LandmarkTracker.h"

namespace Algorithms {
    enum Type
    {
        NoTracking,
        TBeesBookTagMatcher,
        TColorPatchTracker,
        TParticleFishTracker,
        TLandmarkTracker,
        TSampleTracker,
        TSimpleTracker
    };

    template<typename T>
    TrackingAlgorithm* createInstance(Settings& settings, std::string& serializationPathName, QWidget* parent)
    { return new T(settings, serializationPathName, parent); }

    typedef std::map<Type, TrackingAlgorithm*(*)(Settings&, std::string&, QWidget*)> map_type_t;
    static const map_type_t byType
    {
        {TBeesBookTagMatcher,  &createInstance<BeesBookTagMatcher>},
        {TColorPatchTracker,   &createInstance<ColorPatchTracker>},
        {TParticleFishTracker, &createInstance<ParticleFishTracker>},
        {TLandmarkTracker,     &createInstance<LandmarkTracker>},
        {TSampleTracker,       &createInstance<SampleTracker>},
        {TSimpleTracker,       &createInstance<SimpleTracker>}
    };

    typedef std::map<QString, Type> map_string_t;
    static const map_string_t byString
    {
        {"No tracking", NoTracking},
        {"Simple algorithm", TSimpleTracker},
        {"BeesBook tag matcher", TBeesBookTagMatcher},
        {"Color patch tag matcher", TColorPatchTracker},
        {"Fish Particle tracker", TParticleFishTracker},
        {"Sample tracker", TSampleTracker},
        {"Landmark tracker", TLandmarkTracker}
    };
}

#endif
