#pragma once

#include "FacetBlockRay.h"
#include "FacetBlockTest.h"
#include "FacetLanes.h"

#include <array>

namespace primitives {
  using namespace vectorization;

  // Plane normals (see nearestIntersectionHavel) of FacetLanes::SIZE facets, component-wise: lane i of v0[c] is
  // component c of v0 of facet i.
  struct alignas(64) FacetNormalsBlock {
    std::array<FacetLanes, 4> v0, v1, v2;
  };

  /**
   * nearestIntersectionHavel for the facets of a block at once: the same steps lane-wise. Its dot products are
   * explicit FMA chains, so the results can differ from nearestIntersectionHavel's in the last bits.
   * @param candidates bit i set: lane i is tested
   * @param cullBack culls lanes whose facet faces away from the ray (backfaceCulled)
   * @param cullFront culls lanes whose facet faces the ray (frontfaceCulled)
   * @return the determinants of all lanes; its hits are the candidates that aren't culled and pass the Havel test
   * against maxDistance
   */
  FacetBlockTest intersectionsHavel(
      const FacetNormalsBlock &block, const FacetBlockRay &ray, const FacetLanes &maxDistance, unsigned candidates,
      bool cullBack, bool cullFront
  ) noexcept;
}
