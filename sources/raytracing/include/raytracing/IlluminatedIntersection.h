#pragma once

#include <vectorization.h>

namespace raytracer {
  using namespace vectorization;

  struct IlluminatedIntersection {
    Float4 depth;

    Float4 color;

    IlluminatedIntersection(Float depth, const Float4 &color);

    IlluminatedIntersection(const Float4 &depth, const Float4 &color);
  };
}
