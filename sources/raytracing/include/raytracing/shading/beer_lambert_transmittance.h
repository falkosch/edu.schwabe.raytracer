#pragma once

#include "raytracing/shading/spectral/wavelengths.h"

namespace raytracer
{
    // Beer-Lambert law: the fraction of the light that passes the distance through a medium, per wavelength.
    // absorptionLengths is the material's transmittance channel, the distance over which the light falls to 1/e:
    // the longer, the clearer.
    spectral::SpectralVector beerLambertTransmittance(
        const spectral::SpectralVector& distance, const spectral::SpectralVector& absorptionLengths
    ) noexcept;
}
