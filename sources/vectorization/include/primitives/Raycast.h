#pragma once

#include "Ray.h"

namespace primitives {
  using namespace vectorization;

  /*
   * Combines casting data about a ray
   */
  struct RayCast {
    Ray ray;

    // x: indicates backface culling wanted
    // y: indicates frontface culling wanted
    // z, w: reserved
    Float4::VectorBoolType cullingMask;

    Size2 originIds;

    Float maxDistance;

    RayCast() noexcept;

    explicit RayCast(
        const Ray &ray, const Float4::VectorBoolType &cullingMask, const Size2 &originIds, const Float maxDistance
    ) noexcept;
  };

  // Note: changes the maxDistance to the max-value of its value-type,
  // instead of taking over the original maxDistance.
  const RayCast toObjectSpace(const RayCast &r, const Float44 &transInvModelMatrix, const Float4 &osOrigin) noexcept;

  const RayCast toObjectSpace(const RayCast &r, const Float44 &inverseModelMatrix) noexcept;

  const Int cullingOrientation(const RayCast &r) noexcept;

  const Float4::VectorBoolType cullingOrientationToMask(const Int cullingOrientation) noexcept;

  const Float4::VectorBoolType backfaceCulledv(const RayCast &r) noexcept;

  const bool backfaceCulled(const RayCast &r) noexcept;

  const Float4::VectorBoolType frontfaceCulledv(const RayCast &r) noexcept;

  const bool frontfaceCulled(const RayCast &r) noexcept;

  const Float4::VectorBoolType notCulledv(const RayCast &r) noexcept;

  const bool notCulled(const RayCast &r) noexcept;

  const bool selfOcclusion(const RayCast &r, const Size2::ValueType toCheckId) noexcept;

  const bool
  selfOcclusion(const RayCast &r, const Size2::ValueType toCheckId, const Float t, const Float epsilon) noexcept;

  const bool selfOcclusion(const RayCast &r, const Size2::ValueType toCheckId, const Float t) noexcept;

  const bool outOfReach(const RayCast &r, const Float t) noexcept;

  const Float4::VectorBoolType outOfReach(const RayCast &r, const Float4 &t) noexcept;

  const Float4 farPoint(const RayCast &r) noexcept;
}
