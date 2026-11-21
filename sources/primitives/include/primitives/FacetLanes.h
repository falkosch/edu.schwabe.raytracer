#pragma once

#include <vectorization.h>

namespace primitives {
  using namespace vectorization;

  // Lane vector of the facet block test (FacetNormalsBlock, FacetBlockRay, FacetBlockTest): one lane per facet. Its
  // component count sets the facets per block.
  using FacetLanes = Float8;
}
