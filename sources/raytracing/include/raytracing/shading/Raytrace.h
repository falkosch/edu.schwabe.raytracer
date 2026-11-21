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

        Raytrace();

        explicit Raytrace(
            const RayCast& rayCast, const SceneIntersection* originIntersection, ASizeT traceDepth,
            Float visibilityIndex,
            const SpectralContext* spectralContext
        );
    };
}
