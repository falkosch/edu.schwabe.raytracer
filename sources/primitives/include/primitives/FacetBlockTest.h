#pragma once

#include "FacetLanes.h"

namespace primitives {
  using namespace vectorization;

  // Result of the facet block test (intersectionsHavel): the Havel determinants of every lane and the lanes that hit.
  struct FacetBlockTest {
    FacetLanes determinant, detT, detU, detV;
    // bit i: lane i is a candidate, isn't culled and passes the three sign tests of the Havel test
    unsigned hits;
  };

  // { u, v, distance, distance } of a lane, as nearestIntersectionHavel returns it
  Float4 texCoordsAndDistance(const FacetBlockTest &test, int lane) noexcept;
}
