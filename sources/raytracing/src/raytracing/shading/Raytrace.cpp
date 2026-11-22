#include "raytracing/shading/Raytrace.h"
#include "../../stdafx.h"

namespace raytracer
{
    Raytrace::Raytrace()
        : rayCast(), originIntersection(), traceDepth(), visibilityIndex(), importance(), spectralContext(), seed(),
          strataSeed(),
          stratum(), glossy()
    {
    }

    Raytrace::Raytrace(
        const RayCast& rayCast, const SceneIntersection* const originIntersection, const ASizeT traceDepth,
        const Float visibilityIndex, const SpectralContext* const spectralContext, const UInt_32 seed
    )
        : rayCast(rayCast), originIntersection(originIntersection), traceDepth(traceDepth),
          visibilityIndex(visibilityIndex), importance(visibilityIndex), spectralContext(spectralContext), seed(seed),
          strataSeed(seed), stratum(),
          glossy()
    {
    }
}
