#pragma once

#include <raytracing.h>

namespace raytracerui {
  using namespace vectorization;

  // Placement of a scene object, applied in this order: scale, rotation in degrees about x, y and z, translation.
  struct Placement {
    Float3 scale{1.f, 1.f, 1.f};
    Float3 rotation{0.f, 0.f, 0.f};
    Float3 translation{0.f, 0.f, 0.f};
  };
}
