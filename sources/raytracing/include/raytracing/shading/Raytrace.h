#pragma once

#include "../geometry/SceneIntersection.h"

#include <primitives.h>

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    struct SpectralContext;

    /**
   * Adds visibility properties to a RayCast
   */
    struct Raytrace
    {
        RayCast rayCast;

        const SceneIntersection* originIntersection;

        ASizeT traceDepth;

        Float visibilityIndex;

        // Spectral data of the path's hero wavelength, shared by every ray of the path
        const SpectralContext* spectralContext;

        // Seeds the sampling along the ray (glossy reflections), derived from the pixel and its sub-sample
        UInt_32 seed;

        // A glossy reflection ray: the emissive spheres of sphere lights don't show in it
        // (ObjectShader::isLightEmitter)
        bool glossy;

        Raytrace();

        explicit Raytrace(
            const RayCast& rayCast, const SceneIntersection* originIntersection, ASizeT traceDepth,
            Float visibilityIndex,
            const SpectralContext* spectralContext, UInt_32 seed = Zero<UInt_32>()
        );
    };
}
