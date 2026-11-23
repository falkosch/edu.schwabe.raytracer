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

        // The ray's share of the pixel: the sample budgets at its hit (glossy rays, area light rays) shrink with it
        Float visibilityIndex;

        // The share of the pixel that rides on the ray's estimate: the visibility index, but not divided among the rays
        // a glossy reflection splits into, since each of them estimates the whole reflection. The cutoffs of dim rays
        // and lights read it, so that they don't depend on the number of glossy rays.
        Float importance;

        // Spectral data of the path's hero wavelength, shared by every ray of the path
        const SpectralContext* spectralContext;

        // Seeds the sampling along the ray (glossy reflections), derived from the pixel and its sub-sample
        UInt_32 seed;

        // The sample sequence at the ray's hit and the ray's block in it (sampling.h, stratifiedIndex): a pixel's
        // primary rays share one sequence and take one block each, so that its sub-samples spread their samples
        // jointly. Secondary rays sample on their own: their seed, block 0.
        UInt_32 strataSeed;
        UInt_32 stratum;

        // A glossy reflection ray: the emissive objects of area lights don't show in it
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
