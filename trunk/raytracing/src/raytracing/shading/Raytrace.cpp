#include "raytracing/shading/Raytrace.h"
#include "../../stdafx.h"

namespace raytracer
{
    Raytrace::Raytrace()
        :
        raycast(),
        originIntersection(),
        traceDepth(),
        visibilityIndex() { }

    Raytrace::Raytrace(
        const Raycast & raycastIn,
        const SceneIntersection * const originIntersectionIn,
        const ASizeT traceDepthIn,
        const Float visibilityIndexIn)
        :
        raycast(raycastIn),
        originIntersection(originIntersectionIn),
        traceDepth(traceDepthIn),
        visibilityIndex(visibilityIndexIn) { }
}