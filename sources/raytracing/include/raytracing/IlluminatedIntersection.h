#pragma once

#include "shading/spectral/spectrum.h"

namespace raytracer
{
    using namespace vectorization;

    struct LightInfo;

    struct IlluminatedIntersection
    {
        Float4 depth;

        spectral::Spectrum color;

        // The area light whose emitter a ray of Raytrace::weighsEmitters hit (color is zero then), nullptr otherwise
        const LightInfo* emitterLight;

        IlluminatedIntersection(Float depth, const spectral::Spectrum& color);

        IlluminatedIntersection(const Float4& depth, const spectral::Spectrum& color);

        IlluminatedIntersection(Float depth, const spectral::Spectrum& color, const LightInfo* emitterLight);
    };
}
