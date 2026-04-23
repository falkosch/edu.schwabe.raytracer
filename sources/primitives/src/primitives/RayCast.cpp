#include "primitives/RayCast.h"

#include <limits>

using namespace vectorization;

namespace primitives {
  RayCast::RayCast() noexcept : maxDistance(std::numeric_limits<Float>::max()) {
  }

  RayCast::RayCast(
      const Ray &ray, const Float4::VectorBoolType &cullingMask, const Size2 &originIds, const Float maxDistance
  ) noexcept
      : ray(ray), cullingMask(cullingMask), originIds(originIds), maxDistance(maxDistance) {
  }

  RayCast
  toObjectSpace(const RayCast &rayCast, const Float44 &transInvModelMatrix, const Float4 &objectSpaceOrigin) noexcept {
    return RayCast(
        Ray(objectSpaceOrigin, normalize3(rayCast.ray.direction * transInvModelMatrix)), rayCast.cullingMask,
        rayCast.originIds, distance3(objectSpaceOrigin, farPoint(rayCast) * transInvModelMatrix)
    );
  }

  RayCast toObjectSpace(const RayCast &rayCast, const Float44 &inverseModelMatrix) noexcept {
    return toObjectSpace(rayCast, inverseModelMatrix, rayCast.ray.origin * inverseModelMatrix);
  }

  Float4::VectorBoolType backfaceCulledv(const RayCast &rayCast) noexcept {
    return xxxx(rayCast.cullingMask);
  }

  bool backfaceCulled(const RayCast &rayCast) noexcept {
    return !!x(backfaceCulledv(rayCast));
  }

  Float4::VectorBoolType frontfaceCulledv(const RayCast &rayCast) noexcept {
    return yyyy(rayCast.cullingMask);
  }

  bool frontfaceCulled(const RayCast &rayCast) noexcept {
    return !!x(frontfaceCulledv(rayCast));
  }

  Float4::VectorBoolType notCulledv(const RayCast &rayCast) noexcept {
    return ~(backfaceCulledv(rayCast) ^ frontfaceCulledv(rayCast));
  }

  bool notCulled(const RayCast &rayCast) noexcept {
    return !!x(notCulledv(rayCast));
  }

  Int cullingOrientation(const RayCast &rayCast) noexcept {
    if (notCulled(rayCast)) {
      return Zero<Int>();
    }
    return backfaceCulled(rayCast) ? NegativeOne<Int>() : One<Int>();
  }

  Float4::VectorBoolType cullingOrientationToMask(const Int cullingOrientation) noexcept {
    return convert<Float4::VectorBoolType>(
        cullingOrientation<Zero<Float4::BoolType>(), cullingOrientation> Zero<Float4::BoolType>()
    );
  }

  bool selfOcclusion(const RayCast &rayCast, const Size2::ValueType toCheckId) noexcept {
    return y(rayCast.originIds) == toCheckId;
  }

  bool
  selfOcclusion(const RayCast &rayCast, const Size2::ValueType toCheckId, const Float t, const Float epsilon) noexcept {
    const auto tooNear = t < epsilon;
    return selfOcclusion(rayCast, toCheckId) & tooNear;
  }

  bool selfOcclusion(const RayCast &rayCast, const Size2::ValueType toCheckId, const Float t) noexcept {
    return selfOcclusion(rayCast, toCheckId, t, Epsilon<Float>());
  }

  bool outOfReach(const RayCast &rayCast, const Float t) noexcept {
    return anyTrue(outOfReach(rayCast, Float4(t)));
  }

  Float4::VectorBoolType outOfReach(const RayCast &rayCast, const Float4 &t) noexcept {
    const auto rMaxDistanceV = Float4(rayCast.maxDistance);
    return isNaN(t) | andnot(isNaN(rMaxDistanceV), t >= rMaxDistanceV);
  }

  Float4 farPoint(const RayCast &rayCast) noexcept {
    return point(rayCast.ray, rayCast.maxDistance);
  }
}
