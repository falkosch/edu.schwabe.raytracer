#include "raytracing/shading/Raytrace.h"
#include "../../stdafx.h"

#include "raytracing/shading/spectral/wavelengths.h"

namespace raytracer
{
    Raytrace::Raytrace()
        : rayCast(), originIntersection(), traceDepth(), visibilityIndex(),
          heroLambda(spectral::LAMBDA_MIN + spectral::LAMBDA_RANGE * Half<Float>())
    {
    }

    Raytrace::Raytrace(
        const RayCast& rayCast, const SceneIntersection* const originIntersection, const ASizeT traceDepth,
        const Float visibilityIndex, const Float heroLambda
    )
        : rayCast(rayCast), originIntersection(originIntersection), traceDepth(traceDepth),
          visibilityIndex(visibilityIndex), heroLambda(heroLambda)
    {
    }
}
