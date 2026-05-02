#pragma once

#include "../common/RGBS.h"

namespace raytracer {
  using namespace vectorization;

  struct LightShading {
    RGBS ambient;
    RGBS diffuse;
    RGBS specular;
    RGBS reflected;
    RGBS transmitted;

    LightShading();

    explicit LightShading(const RGBS &ambient);
  };
}
