#include "raytracing/shading/brdf/apply_brdf.h"

namespace raytracer::brdf
{
    spectral::Spectrum applyBRDF(const BRDFParameters& brdf)
    {
        // Lambert's BRDF diffusion / pi: the ambient light, a radiance from all directions, brings pi times its value
        // as irradiance; the direct light is an irradiance. Under a specular lobe, coupled to what it leaves.
        const auto diffusion = brdf.surface.diffusion * brdf.diffuseCoupling;
        const auto ambient = diffusion * brdf.lighting.ambient;
        const auto diffuse = diffusion * brdf.lighting.diffuse * ReciprocalPi<spectral::SpectralVector>();
        const auto directLighting = ambient + diffuse + brdf.lighting.specular;

        // the reflected and transmitted light carry their Fresnel weights already
        const auto transmitted = brdf.lighting.transmitted * brdf.fractionTransmitted;

        return brdf.surface.emittance + directLighting + brdf.lighting.reflected + transmitted;
    }
}
