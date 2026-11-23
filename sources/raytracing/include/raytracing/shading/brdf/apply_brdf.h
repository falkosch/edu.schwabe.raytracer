#pragma once

#include "raytracing/BRDFParameters.h"

#include "raytracing/shading/spectral/spectrum.h"

namespace raytracer::brdf
{
    // The radiance a hit sends back along the incident ray: its emittance, plus the ambient and direct diffuse light
    // times the diffusion, plus the specular highlights, plus the reflected (times the reflectance) and transmitted
    // (times the Beer-Lambert fraction) light mixed by the Fresnel reflectance coefficient
    spectral::Spectrum applyBRDF(const BRDFParameters& brdf);
}
