#pragma once

#include "common/RGBS.h"

namespace raytracer {
  using namespace vectorization;

  struct IlluminatedIntersection {
    Float4 depth;

    RGBS color;

    IlluminatedIntersection(Float depth, const RGBS &color);

    IlluminatedIntersection(const Float4 &depth, const RGBS &color);
  };
}
