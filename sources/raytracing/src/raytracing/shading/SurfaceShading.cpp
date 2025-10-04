#include "raytracing/shading/SurfaceShading.h"
#include "../../stdafx.h"

namespace raytracer {
  SurfaceShading::SurfaceShading()
      : diffusion(), reflectance(), specular(), shininess(), absorptionCoefficient(), refractionEta(), emittance() {
  }

  SurfaceShading::SurfaceShading(
      const Float4 &diffusionIn, const Float4 &reflectanceIn, const Float4 &specularIn, const Float4 &shininessIn,
      const Float4 &absorptionCoefficientIn, const Float4 &refractionEtaIn, const Float4 &emittanceIn
  )
      : diffusion(diffusionIn), reflectance(reflectanceIn), specular(specularIn), shininess(shininessIn),
        absorptionCoefficient(absorptionCoefficientIn), refractionEta(refractionEtaIn), emittance(emittanceIn) {
  }
}
