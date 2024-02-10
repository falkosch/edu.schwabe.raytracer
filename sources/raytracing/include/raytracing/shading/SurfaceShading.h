#pragma once

#include <vectorization.h>

namespace raytracer {
  using namespace vectorization;

  struct SurfaceShading {
    Float4 diffusion;

    Float4 reflectance;

    Float4 specular;

    Float4 shininess;

    Float4 transmittance;

    Float4 refractionEta;

    Float4 emittance;

    SurfaceShading();

    SurfaceShading(
        const Float4 &diffusion, const Float4 &reflectance, const Float4 &specular, const Float4 &shininess,
        const Float4 &transmittanceIn, const Float4 &refractionEta, const Float4 &emittance
    );
  };
}
