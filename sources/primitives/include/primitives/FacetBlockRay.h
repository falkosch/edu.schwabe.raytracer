#pragma once

#include "FacetLanes.h"
#include "Ray.h"

#include <array>

namespace primitives {
  using namespace vectorization;

  // The ray's components, each broadcast to all lanes, for the facet block test.
  struct FacetBlockRay {
    std::array<FacetLanes, 4> origin, direction;

    explicit FacetBlockRay(const Ray &ray) noexcept;
  };
}
