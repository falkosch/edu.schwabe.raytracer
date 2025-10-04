#pragma once

#include <vectorization.h>

namespace raytracer {
  using namespace vectorization;

  class NoiseGenerator {
  public:
    virtual ~NoiseGenerator() {
    }

    virtual Float noise(Float v) const = 0;
    virtual Float noise(const Float4 &v) const = 0;
    virtual Float noise2(const Float4 &v) const = 0;
    virtual Float noise3(const Float4 &v) const = 0;
    virtual Float noise4(const Float4 &v) const = 0;
  };
}
