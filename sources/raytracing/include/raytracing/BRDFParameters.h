#pragma once

#include "geometry/SceneIntersection.h"

#include "shading/LightShading.h"
#include "shading/SurfaceShading.h"
#include "shading/brdf/SpecularFresnel.h"
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

        // The Fresnel reflectance of the specular lobe (its weight included), for highlights and reflections alike
        brdf::SpecularFresnel fresnel;

        // The specular lobe's reflectance towards the viewer at the smooth surface per wavelength [0,1]: the mirror
        // reflection's weight, and 1 minus the transmission's
        spectral::SpectralVector reflectanceCoefficient;

        // Beer-Lambert absorption weight per wavelength
        spectral::SpectralVector fractionTransmitted;

        // Distance from ray origin to intersection
        Float viewDistance;

        BRDFParameters();
    };
}
