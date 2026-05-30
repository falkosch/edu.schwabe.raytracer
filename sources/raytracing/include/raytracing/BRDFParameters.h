#pragma once

#include "geometry/SceneIntersection.h"

#include "shading/LightShading.h"
#include "shading/SurfaceShading.h"
#include "shading/spectral/wavelengths.h"

namespace raytracer
{
    using namespace vectorization;

    struct BRDFParameters
    {
        // Ray-scene intersection point
        SceneIntersection intersection;

        // Material properties at intersection
        SurfaceShading surface;

        // Lighting contributions at intersection
        LightShading lighting;

        // Direction of transmitted ray
        Float4 transmissionDirection;

        // Fresnel reflectance coefficient per wavelength [0,1]
        spectral::SpectralVector reflectanceCoefficient;

        // Beer-Lambert absorption weight per wavelength
        spectral::SpectralVector fractionTransmitted;

        // Distance from ray origin to intersection
        Float viewDistance;

        BRDFParameters();
    };
}
