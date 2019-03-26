#include "raytracing/shading/SurfaceShading.h"
#include "../../stdafx.h"

namespace raytracer
{

    SurfaceShading::SurfaceShading()
        :
        diffusion(),
        reflectance(),
        specularity(),
        shininess(),
        absorbtionCoefficient(),
        refractionEta(),
        emittivity()
    { }

    SurfaceShading::SurfaceShading(
        const Float4 & diffusionIn,
        const Float4 & reflectanceIn,
        const Float4 & specularityIn,
        const Float4 & shininessIn,
        const Float4 & absorbtionCoefficientIn,
        const Float4 & refractionEtaIn,
        const Float4 & emittivityIn)
        :
        diffusion(diffusionIn),
        reflectance(reflectanceIn),
        specularity(specularityIn),
        shininess(shininessIn),
        absorbtionCoefficient(absorbtionCoefficientIn),
        refractionEta(refractionEtaIn),
        emittivity(emittivityIn)
    { }

}
