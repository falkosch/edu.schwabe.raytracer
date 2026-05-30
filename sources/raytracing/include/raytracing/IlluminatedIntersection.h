#pragma once

#include "shading/spectral/spectrum.h"

namespace raytracer
{
    using namespace vectorization;

    struct IlluminatedIntersection
    {
        Float4 depth;

        spectral::Spectrum color;

        IlluminatedIntersection(Float depth, const spectral::Spectrum& color);

        IlluminatedIntersection(const Float4& depth, const spectral::Spectrum& color);
    };
}
