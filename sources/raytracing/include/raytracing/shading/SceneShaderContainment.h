#pragma once

#include "../common/StatisticsCookie.h"

#include "PerLightShadowCache.h"
#include "Raytrace.h"

namespace raytracer
{
    using namespace vectorization;

    struct SceneShaderContainment
    {
        Raytrace incidentRay;
        Float4 F0;
        Float roughness;
        Float visibilityCutoff;
        PerLightShadowCache::ShadowCacheType* shadowCache;
        StatisticsCookie* statistics;

        SceneShaderContainment(PerLightShadowCache::ShadowCacheType& shadowCache, StatisticsCookie& statistics);
    };
}
