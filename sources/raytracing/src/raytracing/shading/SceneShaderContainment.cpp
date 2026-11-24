#include "raytracing/shading/SceneShaderContainment.h"
#include "../../stdafx.h"

namespace raytracer
{
    SceneShaderContainment::SceneShaderContainment(
        PerLightShadowCache::ShadowCacheType& shadowCacheIn, StatisticsCookie& statisticsIn
    )
        : incidentRay(), fresnel(), roughness(), visibilityCutoff(), sphereLightSamples(16), glossyRays(),
          pointLightSamples(),
          shadowCache(&shadowCacheIn), statistics(&statisticsIn)
    {
    }
}
