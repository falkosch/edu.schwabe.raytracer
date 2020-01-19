#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    struct SurfaceShading
    {
        Float4 diffusion;

        Float4 reflectance;

        Float4 specularity;

        Float4 shininess;

        Float4 absorbtionCoefficient;

        Float4 refractionEta;

        Float4 emittivity;

        SurfaceShading();

        SurfaceShading(
            const Float4 & diffusion,
            const Float4 & reflectance,
            const Float4 & specularity,
            const Float4 & shininess,
            const Float4 & absorbtionCoefficient,
            const Float4 & refractionEta,
            const Float4 & emittivity);
    };
}
