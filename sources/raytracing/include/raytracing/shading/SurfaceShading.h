#pragma once

#include "../common/RGBS.h"

namespace raytracer {
  using namespace vectorization;

  struct SurfaceShading {
    RGBS diffusion;

    RGBS reflectance;

    RGBS specular;

    Float4 shininess;

    Float4 transmittance;

    Float4 refractionEta;

    RGBS emittance;

    SurfaceShading();

    SurfaceShading(
        const RGBS &diffusion, const RGBS &reflectance, const RGBS &specular, const Float4 &shininess,
        const Float4 &transmittanceIn, const Float4 &refractionEta, const RGBS &emittance
    );
  };
}
