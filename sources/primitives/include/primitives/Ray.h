#pragma once

#include <vectorization.h>

namespace primitives {
  using namespace vectorization;

  /**
   * Ray origin and direction, with a precomputed reciprocal direction
   */
  struct Ray {
    Float4 origin, direction, reciprocalDirection;

    Ray() noexcept;
    explicit Ray(const Float4 &origin) noexcept;
    explicit Ray(const Float4 &origin, const Float4 &direction) noexcept;

    void setDirection(const Float4 &direction) noexcept;
  };

  Ray replaceDirection(const Ray &ray, const Float4 &value) noexcept;

  Float4 point(const Ray &ray, Float t) noexcept;
  Float4 point(const Ray &ray, const Float4 &value) noexcept;
}
