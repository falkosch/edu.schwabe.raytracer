#include "raytracing/shading/beer_lambert_transmittance.h"

#include "raytracing/shading/spectral/spectrum.h"

namespace raytracer
{
    spectral::SpectralVector beerLambertTransmittance(
        const spectral::SpectralVector& distance, const spectral::SpectralVector& absorptionLengths
    ) noexcept
    {
        return spectral::spectralExp(-distance / absorptionLengths);
    }
}
