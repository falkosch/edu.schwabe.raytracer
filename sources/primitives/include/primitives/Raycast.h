#pragma once

#include "Ray.h"

namespace primitives {
  using namespace vectorization;

  /**
   * Ray with culling orientation, origin ids, and a max distance
   */
  struct RayCast {
    Ray ray;

    /**
     * @p .x indicates backface culling wanted
     * @p .y indicates front-face culling wanted
     * @p .z reserved
     * @p .w reserved
     */
    Float4::VectorBoolType cullingMask;

    Size2 originIds;

    Float maxDistance;

    RayCast() noexcept;

    explicit RayCast(
        const Ray &ray, const Float4::VectorBoolType &cullingMask, const Size2 &originIds, Float maxDistance
    ) noexcept;
  };

  // Note: changes the maxDistance to the max-value of its value-type,
  // instead of taking over the original maxDistance.
  RayCast toObjectSpace(const RayCast &r, const Float44 &transInvModelMatrix, const Float4 &osOrigin) noexcept;

  RayCast toObjectSpace(const RayCast &r, const Float44 &inverseModelMatrix) noexcept;

  Float4::VectorBoolType backfaceCulledv(const RayCast &r) noexcept;

  bool backfaceCulled(const RayCast &r) noexcept;

  Float4::VectorBoolType frontfaceCulledv(const RayCast &r) noexcept;

  bool frontfaceCulled(const RayCast &r) noexcept;

  Float4::VectorBoolType notCulledv(const RayCast &r) noexcept;

  bool notCulled(const RayCast &r) noexcept;

  Int cullingOrientation(const RayCast &r) noexcept;

  Float4::VectorBoolType cullingOrientationToMask(Int cullingOrientation) noexcept;

  bool selfOcclusion(const RayCast &r, Size2::ValueType toCheckId) noexcept;

  bool selfOcclusion(const RayCast &r, Size2::ValueType toCheckId, Float t, Float epsilon) noexcept;

  bool selfOcclusion(const RayCast &r, Size2::ValueType toCheckId, Float t) noexcept;

  bool outOfReach(const RayCast &r, Float t) noexcept;

  Float4::VectorBoolType outOfReach(const RayCast &r, const Float4 &t) noexcept;

  Float4 farPoint(const RayCast &r) noexcept;
}
