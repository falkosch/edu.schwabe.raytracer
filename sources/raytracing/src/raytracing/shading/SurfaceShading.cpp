#include "raytracing/shading/SurfaceShading.h"
#include "../../stdafx.h"

namespace raytracer {
  SurfaceShading::SurfaceShading()
      : diffusion(), reflectance(), specular(), shininess(), transmittance(), refractionEta(), emittance() {
  }

  SurfaceShading::SurfaceShading(
      const RGBS &diffusionIn, const RGBS &reflectanceIn, const RGBS &specularIn, const Float4 &shininessIn,
      const Float4 &transmittanceIn, const Float4 &refractionEtaIn, const RGBS &emittanceIn
  )
      : diffusion(diffusionIn), reflectance(reflectanceIn), specular(specularIn), shininess(shininessIn),
        transmittance(transmittanceIn), refractionEta(refractionEtaIn), emittance(emittanceIn) {
  }
}
