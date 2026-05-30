#pragma once

#include "spectral/spectrum.h"

namespace raytracer
{
    using namespace vectorization;

    struct SurfaceShading
    {
        spectral::Spectrum diffusion;

        spectral::Spectrum reflectance;

        spectral::Spectrum specular;

        Float4 roughness;

        spectral::Spectrum transmittance;

        Float4 refractionEta;

        spectral::Spectrum emittance;

        SurfaceShading();

        SurfaceShading(
            const spectral::Spectrum& diffusion, const spectral::Spectrum& reflectance,
            const spectral::Spectrum& specular, const Float4& roughness,
            const spectral::Spectrum& transmittance, const Float4& refractionEta,
            const spectral::Spectrum& emittance
        );
    };
}
