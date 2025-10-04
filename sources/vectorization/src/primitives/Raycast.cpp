#include "primitives/RayCast.h"

#include <limits>

using namespace vectorization;

namespace primitives {
  RayCast::RayCast() noexcept : ray(), cullingMask(), originIds(), maxDistance(std::numeric_limits<Float>::max()) {
  }

  RayCast::RayCast(
      const Ray &rayIn, const Float4::VectorBoolType &cullingMaskIn, const Size2 &originIdsIn, const Float maxDistanceIn
  ) noexcept
      : ray(rayIn), cullingMask(cullingMaskIn), originIds(originIdsIn), maxDistance(maxDistanceIn) {
  }

  const RayCast toObjectSpace(const RayCast &r, const Float44 &transInvModelMatrix, const Float4 &osOrigin) noexcept {
    return RayCast(
        Ray(osOrigin, normalize3(r.ray.direction * transInvModelMatrix)), r.cullingMask, r.originIds,
        distance3(osOrigin, farPoint(r) * transInvModelMatrix)
    );
  }

  const RayCast toObjectSpace(const RayCast &r, const Float44 &transInvModelMatrix) noexcept {
    return toObjectSpace(r, transInvModelMatrix, r.ray.origin * transInvModelMatrix);
  }

  const Float4::VectorBoolType backfaceCulledv(const RayCast &r) noexcept {
    return xxxx(r.cullingMask);
  }

  const bool backfaceCulled(const RayCast &r) noexcept {
    return !!x(backfaceCulledv(r));
  }

  const Float4::VectorBoolType frontfaceCulledv(const RayCast &r) noexcept {
    return yyyy(r.cullingMask);
  }

  const bool frontfaceCulled(const RayCast &r) noexcept {
    return !!x(frontfaceCulledv(r));
  }

  const Float4::VectorBoolType notCulledv(const RayCast &r) noexcept {
    return ~(backfaceCulledv(r) ^ frontfaceCulledv(r));
  }

  const bool notCulled(const RayCast &r) noexcept {
    return !!x(notCulledv(r));
  }

  const Int cullingOrientation(const RayCast &r) noexcept {
    if (notCulled(r)) {
      return Zero<Int>();
    }
    return backfaceCulled(r) ? NegativeOne<Int>() : One<Int>();
  }

  const Float4::VectorBoolType cullingOrientationToMask(const Int cullingOrientation) noexcept {
    return convert<Float4::VectorBoolType>(
        cullingOrientation<Zero<Float4::BoolType>(), cullingOrientation> Zero<Float4::BoolType>()
    );
  }

  const bool selfOcclusion(const RayCast &r, const Size2::ValueType toCheckId) noexcept {
    return y(r.originIds) == toCheckId;
  }

  const bool
  selfOcclusion(const RayCast &r, const Size2::ValueType toCheckId, const Float t, const Float epsilon) noexcept {
    return selfOcclusion(r, toCheckId) & (t <= epsilon);
  }

  const bool selfOcclusion(const RayCast &r, const Size2::ValueType toCheckId, const Float t) noexcept {
    return selfOcclusion(r, toCheckId, t, Epsilon<Float>());
  }

  const bool outOfReach(const RayCast &r, const Float t) noexcept {
    return anyTrue(outOfReach(r, Float4(t)));
  }

  const Float4::VectorBoolType outOfReach(const RayCast &r, const Float4 &t) noexcept {
    auto rMaxDistanceV = Float4(r.maxDistance);
    // t == NaN or (r.maxDistance != NaN and t >= r.maxDistance)
    return isNaN(t) | andnot(isNaN(rMaxDistanceV), t >= rMaxDistanceV);
  }

  const Float4 farPoint(const RayCast &r) noexcept {
    return point(r.ray, r.maxDistance);
  }
}