#include "primitives/Ray.h"

using namespace vectorization;

namespace primitives {
  Ray::Ray() noexcept = default;

  Ray::Ray(const Float4 &origin) noexcept : origin(origin) {
  }

  Ray::Ray(const Float4 &origin, const Float4 &direction) noexcept
      : origin(origin), direction(direction), reciprocalDirection(zeroW(reciprocal(direction))) {
  }

  void Ray::setDirection(const Float4 &direction) noexcept {
    this->direction = direction;
    reciprocalDirection = zeroW(reciprocal(direction));
  }

  Ray replaceDirection(const Ray &ray, const Float4 &value) noexcept {
    return Ray(ray.origin, value);
  }

  Float4 point(const Ray &ray, const Float4 &value) noexcept {
    return ray.origin + ray.direction * value;
  }

  Float4 point(const Ray &ray, const Float t) noexcept {
    return point(ray, Float4(t));
  }
}
