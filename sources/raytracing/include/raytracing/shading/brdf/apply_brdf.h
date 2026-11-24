#pragma once

#include "raytracing/BRDFParameters.h"

#include "raytracing/shading/spectral/spectrum.h"

namespace raytracer::brdf
{
    // The radiance a hit sends back along the incident ray: its emittance, plus the diffuse reflection (Lambert's
    // BRDF diffusion / pi: the ambient radiance times the diffusion, the direct irradiance times diffusion / pi), plus
    // the specular highlights, plus the reflected light and the transmitted light (times the Beer-Lambert fraction),
    // both already weighed by the specular lobe's Fresnel reflectance (brdf::SpecularFresnel): the reflection by it,
    // the transmission by what it leaves
    spectral::Spectrum applyBRDF(const BRDFParameters& brdf);
}
