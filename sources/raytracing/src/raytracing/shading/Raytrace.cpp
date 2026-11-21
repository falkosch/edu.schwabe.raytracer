#include "raytracing/shading/Raytrace.h"
#include "../../stdafx.h"

namespace raytracer
{
    Raytrace::Raytrace()
        : rayCast(), originIntersection(), traceDepth(), visibilityIndex(), spectralContext()
    {
    }

    Raytrace::Raytrace(
        const RayCast& rayCast, const SceneIntersection* const originIntersection, const ASizeT traceDepth,
        const Float visibilityIndex, const SpectralContext* const spectralContext
    )
        : rayCast(rayCast), originIntersection(originIntersection), traceDepth(traceDepth),
          visibilityIndex(visibilityIndex), spectralContext(spectralContext)
    {
    }
}
