#include "primitives/Ray.h"

using namespace vectorization;

namespace primitives {
  Ray::Ray() noexcept : origin(), direction(), reciprocalDirection() {
  }

  Ray::Ray(const Float4 &originIn) noexcept : origin(originIn), direction(), reciprocalDirection() {
  }

  Ray::Ray(const Float4 &originIn, const Float4 &directionIn) noexcept
      : origin(originIn), direction(directionIn), reciprocalDirection(zeroW(reciprocal(directionIn))) {
  }

  void Ray::setDirection(const Float4 &directionIn) noexcept {
    direction = directionIn;
    reciprocalDirection = zeroW(reciprocal(directionIn));
  }

  Ray replaceDirection(const Ray &r, const Float4 &value) noexcept {
    return Ray(r.origin, value);
  }

  Float4 point(const Ray &r, const Float4 &v) noexcept {
    return r.origin + r.direction * v;
  }

  Float4 point(const Ray &r, const Float t) noexcept {
    return point(r, Float4(t));
  }
}
