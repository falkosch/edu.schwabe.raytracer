#pragma once

#include "../common/StatisticsCookie.h"

#include "PerLightShadowCache.h"
#include "Raytrace.h"
#include "spectral/wavelengths.h"

namespace raytracer
{
    using namespace vectorization;

    struct SceneShaderContainment
    {
        Raytrace incidentRay;
        // Fresnel reflectance at normal incidence, per wavelength [0,1]
        spectral::SpectralVector F0;
        Float roughness;
        Float visibilityCutoff;
        PerLightShadowCache::ShadowCacheType* shadowCache;
        StatisticsCookie* statistics;

        SceneShaderContainment(PerLightShadowCache::ShadowCacheType& shadowCache, StatisticsCookie& statistics);
    };
}
