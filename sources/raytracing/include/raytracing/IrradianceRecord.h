#pragma once

#include "shading/spectral/spectrum.h"

namespace raytracer
{
    using namespace vectorization;

    // A record of the irradiance cache (IrradianceCache): the irradiance that diffuse rays gathered at a point (as
    // Raytracer::traceDiffuseReflection adds it to the diffuse lighting, at the wavelengths of the record's spectral
    // context), the point's normal, and the record's radius R: the harmonic mean distance of the rays' hits (at least
    // a pixel or so), which the neighbours' radii clamp. A radius of 0 is no record. maxRadius caps the radius the
    // record is valid by (a span of pixels: it keeps the records of a coarse grid local), but not the radius the
    // neighbours see: it says nothing about the distance of the surroundings. context is the index of the cache's
    // spectral context whose wavelengths the irradiance is at.
    struct IrradianceRecord
    {
        Float4 position;
        Float4 normal;
        spectral::Spectrum irradiance;
        Float radius;
        Float maxRadius;
        ASizeT context;

        IrradianceRecord();

        // The radius the record is valid by: R, at most maxRadius
        Float validRadius() const noexcept;
    };
}
