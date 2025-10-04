#pragma once

#include "NoiseGenerator.h"

namespace raytracer {
  using namespace vectorization;

  class PerlinNoiseGenerator : public NoiseGenerator {
    static constexpr ASizeT DefaultPermutationsCount = 256;

    Float4 *gradients1;
    Float4 *gradients2;
    Float4 *gradients3;
    Float4 *gradients4;
    Int *permutations;

    ASizeT permutationsCount;
    Int iPermutationsCount, iPermutationsCountS1;
    Float fPermutationsCount;

    void setup(const Float &v, Int4 &b, Float4 &r) const;

    static Float4 randomGradient(const Int4 &scale, const Int4 &offset, const Float4 &normalization);

    static Float4 splineCurve(const Float4 &t);

  public:
    PerlinNoiseGenerator(UInt_64 seed);

    ~PerlinNoiseGenerator() override {
    }

    Float noise(Float v) const override;
    Float noise(const Float4 &v) const override;
    Float noise2(const Float4 &v) const override;
    Float noise3(const Float4 &v) const override;
    Float noise4(const Float4 &v) const override;
  };
}
