#include "raytracing/shading/brdf/apply_brdf.h"

namespace raytracer::brdf
{
    spectral::Spectrum applyBRDF(const BRDFParameters& brdf)
    {
        // Lambert's BRDF diffusion / pi: the ambient light, a radiance from all directions, brings pi times its value
        // as irradiance; the direct light is an irradiance
        const auto ambient = brdf.surface.diffusion * brdf.lighting.ambient;
        const auto diffuse = brdf.surface.diffusion * brdf.lighting.diffuse
            * ReciprocalPi<spectral::SpectralVector>();
        const auto directLighting = ambient + diffuse + brdf.lighting.specular;

        const auto reflection = brdf.surface.reflectance * brdf.lighting.reflected;
        const auto transmitted = brdf.lighting.transmitted * brdf.fractionTransmitted;

        return brdf.surface.emittance + directLighting
            + spectral::spectralMix(transmitted, reflection, brdf.reflectanceCoefficient);
    }
}
