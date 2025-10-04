#include "primitives/RayCast.h"

#include <limits>

using namespace vectorization;

namespace primitives {
  RayCast::RayCast() noexcept : ray(), cullingMask(), originIds(), maxDistance(std::numeric_limits<Float>::max()) {
  }

  RayCast::RayCast(
      const Ray &rayIn, const Float4::VectorBoolType &cullingMaskIn, const Size2 &originIdsIn, Float maxDistanceIn
  ) noexcept
      : ray(rayIn), cullingMask(cullingMaskIn), originIds(originIdsIn), maxDistance(maxDistanceIn) {
  }

  RayCast toObjectSpace(const RayCast &r, const Float44 &transInvModelMatrix, const Float4 &osOrigin) noexcept {
    return RayCast(
        Ray(osOrigin, normalize3(r.ray.direction * transInvModelMatrix)), r.cullingMask, r.originIds,
        distance3(osOrigin, farPoint(r) * transInvModelMatrix)
    );
  }

  RayCast toObjectSpace(const RayCast &r, const Float44 &transInvModelMatrix) noexcept {
    return toObjectSpace(r, transInvModelMatrix, r.ray.origin * transInvModelMatrix);
  }

  Float4::VectorBoolType backfaceCulledv(const RayCast &r) noexcept {
    return xxxx(r.cullingMask);
  }

  bool backfaceCulled(const RayCast &r) noexcept {
    return !!x(backfaceCulledv(r));
  }

  Float4::VectorBoolType frontfaceCulledv(const RayCast &r) noexcept {
    return yyyy(r.cullingMask);
  }

  bool frontfaceCulled(const RayCast &r) noexcept {
    return !!x(frontfaceCulledv(r));
  }

  Float4::VectorBoolType notCulledv(const RayCast &r) noexcept {
    return ~(backfaceCulledv(r) ^ frontfaceCulledv(r));
  }

  bool notCulled(const RayCast &r) noexcept {
    return !!x(notCulledv(r));
  }

  Int cullingOrientation(const RayCast &r) noexcept {
    if (notCulled(r)) {
      return Zero<Int>();
    }
    return backfaceCulled(r) ? NegativeOne<Int>() : One<Int>();
  }

  Float4::VectorBoolType cullingOrientationToMask(Int cullingOrientation) noexcept {
    return convert<Float4::VectorBoolType>(
        cullingOrientation<Zero<Float4::BoolType>(), cullingOrientation> Zero<Float4::BoolType>()
    );
  }

  bool selfOcclusion(const RayCast &r, Size2::ValueType toCheckId) noexcept {
    return y(r.originIds) == toCheckId;
  }

  bool selfOcclusion(const RayCast &r, Size2::ValueType toCheckId, Float t, Float epsilon) noexcept {
    return selfOcclusion(r, toCheckId) & (t <= epsilon);
  }

  bool selfOcclusion(const RayCast &r, Size2::ValueType toCheckId, Float t) noexcept {
    return selfOcclusion(r, toCheckId, t, Epsilon<Float>());
  }

  bool outOfReach(const RayCast &r, Float t) noexcept {
    return anyTrue(outOfReach(r, Float4(t)));
  }

  Float4::VectorBoolType outOfReach(const RayCast &r, const Float4 &t) noexcept {
    auto rMaxDistanceV = Float4(r.maxDistance);
    // t == NaN or (r.maxDistance != NaN and t >= r.maxDistance)
    return isNaN(t) | andnot(isNaN(rMaxDistanceV), t >= rMaxDistanceV);
  }

  Float4 farPoint(const RayCast &r) noexcept {
    return point(r.ray, r.maxDistance);
  }
}
