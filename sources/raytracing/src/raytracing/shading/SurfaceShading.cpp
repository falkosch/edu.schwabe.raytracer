#include "raytracing/shading/SurfaceShading.h"
#include "../../stdafx.h"

namespace raytracer
{
    SurfaceShading::SurfaceShading()
        : diffusion(), reflectance(), specular(), roughness(), transmittance(), refractionEta(), emittance()
    {
    }

    SurfaceShading::SurfaceShading(
        const spectral::Spectrum& diffusion, const spectral::Spectrum& reflectance,
        const spectral::Spectrum& specular, const Float4& roughness,
        const spectral::Spectrum& transmittance, const Float4& refractionEta,
        const spectral::Spectrum& emittance
    )
        : diffusion(diffusion), reflectance(reflectance), specular(specular), roughness(roughness),
          transmittance(transmittance), refractionEta(refractionEta), emittance(emittance)
    {
    }
}
