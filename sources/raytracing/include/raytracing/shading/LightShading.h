#pragma once

#include "spectral/spectrum.h"

namespace raytracer
{
    using namespace vectorization;

    struct LightShading
    {
        spectral::Spectrum ambient;
        spectral::Spectrum diffuse;
        spectral::Spectrum specular;
        spectral::Spectrum reflected;
        spectral::Spectrum transmitted;

        LightShading();

        explicit LightShading(const spectral::Spectrum& ambient);
    };
}
