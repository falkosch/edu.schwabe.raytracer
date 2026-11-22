#include "raytracing_tests.h"

#include <raytracing/common/noise/NoiseGenerator.h>
#include <raytracing/scene/SceneObject.h>
#include <raytracing/shading/shaders/NoiseGeneratorMap.h>

#include <cmath>

namespace raytracer::test {
  TEST_CLASS(NoiseGeneratorMapTest) {
    // Returns `value` where x of the sample point lies in [low, high), else 0, so that a test picks the octaves by
    // their frequency: octave i samples at 2^i times the texture coordinates
    class BandNoiseGenerator final : public NoiseGenerator {
      Float low;
      Float high;
      Float value;

    public:
      BandNoiseGenerator(const Float lowIn, const Float highIn, const Float valueIn)
          : low(lowIn), high(highIn), value(valueIn) {
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
    };

    template <ASizeT Octaves>
    static Float4 sampleAt(const NoiseGenerator &generator, const Float u) {
      const NoiseGeneratorMap<Octaves> map{generator};
      const SceneObject object{"test"};
      FacetIntersection intersection{};
      intersection.texCoords = Float4{u, 0.0f, 0.0f, 1.0f};
      return map.sample(object, intersection);
    }

    // The map's output for a sum of weighted octaves
    static Float mapped(const Float sum) {
      return 0.5f * sum + 0.5f;
    }

  public:
    TEST_METHOD(oneOctaveMapsTheNoiseToZeroOne) {
      const BandNoiseGenerator generator{-1000.0f, 1000.0f, 0.4f};

      const auto sample = sampleAt<1>(generator, 0.3f);

      Assert::AreEqual(mapped(std::sin(0.4f)), x(sample), 1e-6f, L"x", LINE_INFO());
      Assert::AreEqual(mapped(std::sin(0.4f)), y(sample), 1e-6f, L"y", LINE_INFO());
      Assert::AreEqual(mapped(std::sin(0.4f)), z(sample), 1e-6f, L"z", LINE_INFO());
      Assert::AreEqual(1.0f, w(sample), 1e-6f, L"w", LINE_INFO());
    }

    TEST_METHOD(coarsestOctaveHasTheFullWeight) {
      // Only the base frequency (x = 0.3) lies in the band; octave 1 samples at 0.6
      const BandNoiseGenerator generator{0.0f, 0.5f, 0.2f};

      const auto sample = sampleAt<8>(generator, 0.3f);

      Assert::AreEqual(mapped(std::sin(0.2f)), x(sample), 1e-6f, L"weight 1", LINE_INFO());
    }

    TEST_METHOD(finestOctaveHasTheSmallestWeight) {
      // Only octave 7 (x = 0.3 * 128 = 38.4) lies in the band; octave 6 samples at 19.2
      const BandNoiseGenerator generator{30.0f, 50.0f, 0.2f};

      const auto sample = sampleAt<8>(generator, 0.3f);

      Assert::AreEqual(mapped(std::sin(0.2f) / 128.0f), x(sample), 1e-6f, L"weight 2^-7", LINE_INFO());
    }

    TEST_METHOD(octaveWeightsHalveWithEachOctave) {
      // Octaves 1 and 2 (x = 0.6 and 1.2) lie in the band
      const BandNoiseGenerator generator{0.5f, 1.5f, 0.2f};

      const auto sample = sampleAt<8>(generator, 0.3f);

      Assert::AreEqual(mapped(0.75f * std::sin(0.2f)), x(sample), 1e-6f, L"weights 1/2 + 1/4", LINE_INFO());
    }
  };
}
