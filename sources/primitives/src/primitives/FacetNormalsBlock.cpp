#include "primitives/FacetNormalsBlock.h"

using namespace vectorization;

namespace primitives {
  namespace {
    constexpr unsigned AllFacetLanes{(1u << FacetLanes::SIZE) - 1u};

    // Lane-wise dot products as explicit FMA chains, so no compiler setting can fuse or reorder them differently.
    // They round differently from dotv / dot3v (_mm_dp_ps, no FMA) in the last bits.
    FacetLanes dot3Lanes(const std::array<FacetLanes, 4> &a, const std::array<FacetLanes, 4> &b) noexcept {
      return multiplyAdd(a[2], b[2], multiplyAdd(a[1], b[1], a[0] * b[0]));
    }

    FacetLanes dotLanes(const std::array<FacetLanes, 4> &a, const std::array<FacetLanes, 4> &b) noexcept {
      return multiplyAdd(a[3], b[3], dot3Lanes(a, b));
    }
  }

  FacetBlockTest intersectionsHavel(
      const FacetNormalsBlock &block, const FacetBlockRay &ray, const FacetLanes &maxDistance,
      const unsigned candidates, const bool cullBack, const bool cullFront
  ) noexcept {
    const auto determinant = dot3Lanes(block.v0, ray.direction);
    const auto frontfaced = static_cast<unsigned>(isNegative(determinant));
    // masks instead of selects, so the compiler doesn't branch
    const auto culledLanes = (~frontfaced & AllFacetLanes & (Zero<unsigned>() - static_cast<unsigned>(cullBack)))
                             | (frontfaced & (Zero<unsigned>() - static_cast<unsigned>(cullFront)));

    const auto detT = -dotLanes(block.v0, ray.origin);
    const auto checkT = multiplySub(determinant, maxDistance, detT);

    // intersection point scaled by the determinant
    auto point = std::array<FacetLanes, 4>();
    for (auto c = Zero<ASizeT>(); c < point.size(); ++c) {
      point[c] = multiplyAdd(ray.origin[c], determinant, ray.direction[c] * detT);
    }
    const auto detU = dotLanes(point, block.v1);
    const auto checkU = determinant - detU;
    const auto detV = dotLanes(point, block.v2);
    const auto checkV = determinant - (detU + detV);

    const auto rejected =
        static_cast<unsigned>(isNegative(detT ^ checkT) | isNegative(detU ^ checkU) | isNegative(detV ^ checkV));
    return FacetBlockTest{determinant, detT, detU, detV, candidates & ~culledLanes & ~rejected};
  }
}
