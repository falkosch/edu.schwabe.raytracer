#pragma once

#include "../common/StatisticsCookie.h"

#include "PerLightShadowCache.h"
#include "Raytrace.h"
#include "brdf/SpecularFresnel.h"
#include "spectral/wavelengths.h"

namespace raytracer
{
    using namespace vectorization;

    struct SceneShaderContainment
    {
        Raytrace incidentRay;
        // The Fresnel reflectance of the intersection's specular lobe, for the highlights
        brdf::SpecularFresnel fresnel;
        Float roughness;
        Float visibilityCutoff;
        // the most shadow rays to an area light (RaytraceBaseParameters::sphereLightSamples)
        ASizeT sphereLightSamples;
        // the glossy reflection rays of the intersection, against which area lights weigh their highlights (0: none)
        ASizeT glossyRays;
        // the most point lights whose shadow rays the intersection traces (0: all)
        ASizeT pointLightSamples;
        PerLightShadowCache::ShadowCacheType* shadowCache;
        StatisticsCookie* statistics;

        SceneShaderContainment(PerLightShadowCache::ShadowCacheType& shadowCache, StatisticsCookie& statistics);
    };
}
