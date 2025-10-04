#pragma once

#include "NoiseGenerator.h"

namespace raytracer {
  using namespace vectorization;

  class SimplexNoiseGenerator : public NoiseGenerator {
  public:
    SimplexNoiseGenerator(unsigned int seed);

    ~SimplexNoiseGenerator() override {
    }

    Float noise(Float v) const override;
    Float noise(const Float4 &v) const override;
    Float noise2(const Float4 &v) const override;
    Float noise3(const Float4 &v) const override;
    Float noise4(const Float4 &v) const override;

  protected:
    static constexpr ASizeT N = 8, B = 256;

    Int simplexGrid[N];

    Int simplex(const Int3 &ijk, Int B) const;

    Float K(Int a, const Float3 &ijk, const Float3 &uvw, Float3 &A) const;

    Int shuffle(const Int3 &ijk) const;
  };
}
