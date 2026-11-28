#include "raytracing_tests.h"

#include <raytracing/common/noise/NoiseGenerator.h>
#include <raytracing/scene/SceneObject.h>
#include <raytracing/shading/shaders/NoiseGeneratorMap.h>

#include <algorithm>
#include <cmath>

namespace raytracer::test {
  TEST_CLASS(NoiseGeneratorMapTest) {
    // Returns `value` where x of the sample point lies in [low, high), else 0, so that a test picks the octaves by
    // their frequency: octave i samples at 2^i times the texture coordinates
    class BandNoiseGenerator final : public NoiseGenerator {
      Float low;
      Float high;
      Float value;
      Float standardDeviation;

    public:
      BandNoiseGenerator(const Float lowIn, const Float highIn, const Float valueIn, const Float deviationIn = 1.0f)
          : low(lowIn), high(highIn), value(valueIn), standardDeviation(deviationIn) {
      }

      Float noise(Float) const override {
        return 0.0f;
      }

      Float noise(const Float4 &) const override {
        return 0.0f;
      }

      Float noise2(const Float4 &) const override {
        return 0.0f;
      }

      Float noise3(const Float4 &v) const override {
        return x(v) >= low && x(v) < high ? value : 0.0f;
      }

      Float noise4(const Float4 &) const override {
        return 0.0f;
      }

      Float deviation() const override {
        return standardDeviation;
      }
    };

    template <ASizeT Octaves>
    static Float4 sampleAt(const NoiseGenerator &generator, const Float u) {
      const NoiseGeneratorMap<Octaves> map{generator};
      const SceneObject object{"test"};
      FacetIntersection intersection{};
      intersection.texCoords = Float4{u, 0.0f, 0.0f, 1.0f};
      return map.sample(object, intersection);
    }

    // The map's output for a sum of weighted octaves of a generator of deviation 1: around the middle grey by the
    // contrast per the octaves' summed deviation
    template <ASizeT Octaves>
    static Float mapped(const Float sum) {
      using Map = NoiseGeneratorMap<Octaves>;
      auto squaredWeights = 0.0f;
      auto weight = 1.0f;
      for (ASizeT i = 0; i < Octaves; ++i) {
        squaredWeights += weight * weight;
        weight *= Map::Gain;
      }
      return std::clamp(0.5f + Map::Contrast * sum / std::sqrt(squaredWeights), 0.0f, 1.0f);
    }

  public:
    TEST_METHOD(oneOctaveMapsTheNoiseAroundTheMiddleGrey) {
      const BandNoiseGenerator generator{-1000.0f, 1000.0f, 0.4f};

      const auto sample = sampleAt<1>(generator, 0.3f);

      Assert::AreEqual(0.5f + NoiseGeneratorMap<1>::Contrast * 0.4f, x(sample), 1e-5f, L"x", LINE_INFO());
      Assert::AreEqual(x(sample), y(sample), 1e-6f, L"y", LINE_INFO());
      Assert::AreEqual(x(sample), z(sample), 1e-6f, L"z", LINE_INFO());
      Assert::AreEqual(1.0f, w(sample), 1e-6f, L"w", LINE_INFO());
    }

    TEST_METHOD(theGeneratorsDeviationSetsTheContrast) {
      // half the deviation: the same value reaches twice as far from the middle grey
      const BandNoiseGenerator generator{-1000.0f, 1000.0f, 0.4f, 0.5f};

      const auto sample = sampleAt<1>(generator, 0.3f);

      Assert::AreEqual(0.5f + NoiseGeneratorMap<1>::Contrast * 0.8f, x(sample), 1e-5f, L"twice as far", LINE_INFO());
    }

    TEST_METHOD(clampsToZeroOne) {
      const BandNoiseGenerator bright{-1000.0f, 1000.0f, 10.0f};
      const BandNoiseGenerator dark{-1000.0f, 1000.0f, -10.0f};

      Assert::AreEqual(1.0f, x(sampleAt<4>(bright, 0.3f)), 1e-6f, L"white", LINE_INFO());
      Assert::AreEqual(0.0f, x(sampleAt<4>(dark, 0.3f)), 1e-6f, L"black", LINE_INFO());
    }

    TEST_METHOD(coarsestOctaveHasTheFullWeight) {
      // Only the base frequency (x = 0.3) lies in the band; octave 1 samples at 0.6
      const BandNoiseGenerator generator{0.0f, 0.5f, 0.2f};

      const auto sample = sampleAt<8>(generator, 0.3f);

      Assert::AreEqual(mapped<8>(0.2f), x(sample), 1e-5f, L"weight 1", LINE_INFO());
    }

    TEST_METHOD(finestOctaveHasTheSmallestWeight) {
      // Only octave 7 (x = 0.3 * 128 = 38.4) lies in the band; octave 6 samples at 19.2
      const BandNoiseGenerator generator{30.0f, 50.0f, 0.2f};

      const auto sample = sampleAt<8>(generator, 0.3f);

      const auto gain = NoiseGeneratorMap<8>::Gain;
      Assert::AreEqual(mapped<8>(0.2f * std::pow(gain, 7.0f)), x(sample), 1e-5f, L"weight Gain^7", LINE_INFO());
    }

    TEST_METHOD(octaveWeightsFallByTheGain) {
      // Octaves 1 and 2 (x = 0.6 and 1.2) lie in the band
      const BandNoiseGenerator generator{0.5f, 1.5f, 0.2f};

      const auto sample = sampleAt<8>(generator, 0.3f);

      const auto gain = NoiseGeneratorMap<8>::Gain;
      Assert::AreEqual(mapped<8>(0.2f * (gain + gain * gain)), x(sample), 1e-5f, L"weights Gain + Gain^2", LINE_INFO());
    }

    TEST_METHOD(theSumDividesByTheOctavesSummedDeviation) {
      // the same value in every octave, as fully correlated octaves: the octaves' summed deviation keeps a sum of
      // independent octaves at the contrast of one, so correlated ones reach further, by their weights' sum per root
      const BandNoiseGenerator generator{-1000.0f, 1000.0f, 0.1f};

      const auto one = x(sampleAt<1>(generator, 0.3f)) - 0.5f;
      const auto eight = x(sampleAt<8>(generator, 0.3f)) - 0.5f;

      auto sum = 0.0f, squares = 0.0f, weight = 1.0f;
      for (int i = 0; i < 8; ++i) {
        sum += weight;
        squares += weight * weight;
        weight *= NoiseGeneratorMap<8>::Gain;
      }
      Assert::AreEqual(one * sum / std::sqrt(squares), eight, 1e-5f, L"correlated octaves", LINE_INFO());
    }
  };
}
