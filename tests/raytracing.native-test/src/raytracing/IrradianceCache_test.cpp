#include "raytracing_tests.h"

#include <raytracing/IrradianceCache.h>
#include <raytracing/scene/Scene.h>
#include <raytracing/shading/SpectralContext.h>
#include <raytracing/shading/spectral/wavelengths.h>

#include <vector>

namespace raytracer::test {
  TEST_CLASS(IrradianceCacheTest) {
    static constexpr Float Accuracy = 0.2f;

    // Two spectral contexts for the cache's records
    struct Contexts {
      Scene scene{};
      std::vector<SpectralContext, AlignedAllocator<SpectralContext>> contexts;

      Contexts() {
        contexts.push_back(scene.createSpectralContext(spectral::LAMBDA_MIN + 0.25f * spectral::LAMBDA_RANGE));
        contexts.push_back(scene.createSpectralContext(spectral::LAMBDA_MIN + 0.75f * spectral::LAMBDA_RANGE));
      }
    };

    // A record on the floor (normal +y) at the point, of radius 1 and the irradiance in every wavelength
    static IrradianceRecord floorRecord(const Float4 &position, const Float irradiance) {
      auto record = IrradianceRecord();
      record.position = position;
      record.normal = Float4(0.0f, 1.0f, 0.0f, 0.0f);
      record.irradiance = spectral::Spectrum{spectral::SpectralVector(irradiance)};
      record.radius = 1.0f;
      return record;
    }

    static const Float4 &up() {
      static const auto normal = Float4(0.0f, 1.0f, 0.0f, 0.0f);
      return normal;
    }

    // The irradiance interpolated in context 0 (averaged over the wavelengths), -1 where no record is valid
    static Float irradianceAt(const IrradianceCache &cache, const Float4 &position, const Float4 &normal) {
      auto irradiance = spectral::Spectrum::zero();
      if (!(cache.interpolate(0, position, normal, irradiance) > 0.0f)) {
        return -1.0f;
      }
      return horizontalSum(irradiance.data) / 8.0f;
    }

    // A cache of the two contexts with the records in context 0, one level
    static IrradianceCache cacheOf(const Contexts &contexts, const std::vector<IrradianceRecord> &records) {
      auto cache = IrradianceCache(contexts.contexts.data(), 2, 1, Accuracy);
      cache.addLevel({records, {}});
      return cache;
    }

  public:
    TEST_METHOD(aRecordGivesItsIrradianceAtItsPoint) {
      const auto contexts = Contexts();
      const auto cache = cacheOf(contexts, {floorRecord(Float4(0.0f, 0.0f, 0.0f, 1.0f), 3.0f)});
      Assert::AreEqual(3.0f, irradianceAt(cache, Float4(0.0f, 0.0f, 0.0f, 1.0f), up()), 1e-6f, L"at", LINE_INFO());
      Assert::AreEqual(3.0f, irradianceAt(cache, Float4(0.1f, 0.0f, 0.05f, 1.0f), up()), 1e-6f, L"near", LINE_INFO());
    }

    TEST_METHOD(aRecordIsValidWithinItsAccuracy) {
      const auto contexts = Contexts();
      const auto cache = cacheOf(contexts, {floorRecord(Float4(0.0f, 0.0f, 0.0f, 1.0f), 3.0f)});
      // |x - x_i| / R below a = 0.2 along the floor, not beyond
      Assert::AreEqual(3.0f, irradianceAt(cache, Float4(0.19f, 0.0f, 0.0f, 1.0f), up()), 1e-6f, L"in", LINE_INFO());
      Assert::AreEqual(-1.0f, irradianceAt(cache, Float4(0.21f, 0.0f, 0.0f, 1.0f), up()), L"out", LINE_INFO());
      // 0.5 sqrt(1 - cos) below a: tilted by 30 degrees (0.183) valid, by 40 degrees (0.242) not, by 20 degrees
      // (0.123) with the distance (0.1) not
      const auto tilted = [](const Float degrees) {
        const auto angle = degrees * Pi<Float>() / 180.0f;
        return Float4(std::sin(angle), std::cos(angle), 0.0f, 0.0f);
      };
      Assert::AreEqual(3.0f, irradianceAt(cache, Float4(0.0f, 0.0f, 0.0f, 1.0f), tilted(30.0f)), 1e-6f,
                       L"tilted", LINE_INFO());
      Assert::AreEqual(-1.0f, irradianceAt(cache, Float4(0.0f, 0.0f, 0.0f, 1.0f), tilted(40.0f)),
                       L"tilted too far", LINE_INFO());
      Assert::AreEqual(-1.0f, irradianceAt(cache, Float4(0.1f, 0.0f, 0.0f, 1.0f), tilted(20.0f)),
                       L"tilted and off", LINE_INFO());
    }

    TEST_METHOD(aRecordInFrontOfThePointDoesntApply) {
      // the point lies 0.1 below the record's floor: the record is in front of it
      const auto contexts = Contexts();
      const auto cache = cacheOf(contexts, {floorRecord(Float4(0.0f, 0.0f, 0.0f, 1.0f), 3.0f)});
      Assert::AreEqual(-1.0f, irradianceAt(cache, Float4(0.0f, -0.1f, 0.0f, 1.0f), up()), L"below", LINE_INFO());
      Assert::AreEqual(3.0f, irradianceAt(cache, Float4(0.0f, 0.1f, 0.0f, 1.0f), up()), 1e-6f, L"above",
                       LINE_INFO());
    }

    TEST_METHOD(theNearerRecordWeighsMore) {
      const auto contexts = Contexts();
      const auto cache = cacheOf(
          contexts,
          {floorRecord(Float4(0.0f, 0.0f, 0.0f, 1.0f), 1.0f), floorRecord(Float4(0.2f, 0.0f, 0.0f, 1.0f), 3.0f)}
      );
      const auto middle = irradianceAt(cache, Float4(0.1f, 0.0f, 0.0f, 1.0f), up());
      const auto nearFirst = irradianceAt(cache, Float4(0.05f, 0.0f, 0.0f, 1.0f), up());
      Assert::AreEqual(2.0f, middle, 1e-5f, L"middle", LINE_INFO());
      Assert::IsTrue(nearFirst > 1.0f && nearFirst < middle, L"nearer the first", LINE_INFO());
    }

    TEST_METHOD(eachContextHasItsOwnRecords) {
      const auto contexts = Contexts();
      const auto cache = cacheOf(contexts, {floorRecord(Float4(0.0f, 0.0f, 0.0f, 1.0f), 3.0f)});
      auto irradiance = spectral::Spectrum::zero();
      Assert::AreEqual(0.0f, cache.interpolate(1, Float4(0.0f, 0.0f, 0.0f, 1.0f), up(), irradiance), L"context 1",
                       LINE_INFO());
      Assert::AreEqual(One<ASizeT>(), cache.contextIndex(&contexts.contexts[1]), L"index", LINE_INFO());
      const auto other = contexts.scene.createSpectralContext(spectral::LAMBDA_MIN);
      Assert::AreEqual(Two<ASizeT>(), cache.contextIndex(&other), L"no records", LINE_INFO());
      Assert::AreEqual(One<ASizeT>(), cache.size(), L"size", LINE_INFO());
    }

    TEST_METHOD(levelsAddUp) {
      // a later level's record next to an earlier level's: both found where both are valid
      const auto contexts = Contexts();
      auto cache = IrradianceCache(contexts.contexts.data(), 2, 1, Accuracy);
      cache.addLevel({{floorRecord(Float4(0.0f, 0.0f, 0.0f, 1.0f), 1.0f)}, {}});
      cache.addLevel({{floorRecord(Float4(0.1f, 0.0f, 0.0f, 1.0f), 5.0f)}, {}});
      Assert::AreEqual(3.0f, irradianceAt(cache, Float4(0.05f, 0.0f, 0.0f, 1.0f), up()), 1e-5f, L"both",
                       LINE_INFO());
      Assert::AreEqual(1.0f, irradianceAt(cache, Float4(-0.15f, 0.0f, 0.0f, 1.0f), up()), 1e-6f, L"the first only",
                       LINE_INFO());
    }

    TEST_METHOD(neighboursClampTheRadius) {
      // a record far too large for its neighbourhood (R 10, the small one 0.5 away has R 0.1) reaches only its
      // neighbour's radius plus their distance: R 0.6, valid up to 0.12
      const auto contexts = Contexts();
      auto cache = IrradianceCache(contexts.contexts.data(), 2, 1, Accuracy);
      auto large = floorRecord(Float4(0.0f, 0.0f, 0.0f, 1.0f), 1.0f);
      large.radius = 10.0f;
      auto small = floorRecord(Float4(0.5f, 0.0f, 0.0f, 1.0f), 5.0f);
      small.radius = 0.1f;
      cache.addLevel({{large}, {}});
      Assert::AreEqual(1.0f, irradianceAt(cache, Float4(-0.5f, 0.0f, 0.0f, 1.0f), up()), 1e-6f, L"alone",
                       LINE_INFO());
      cache.addLevel({{small}, {}});
      Assert::AreEqual(1.0f, irradianceAt(cache, Float4(-0.11f, 0.0f, 0.0f, 1.0f), up()), 1e-6f, L"within",
                       LINE_INFO());
      Assert::AreEqual(-1.0f, irradianceAt(cache, Float4(-0.13f, 0.0f, 0.0f, 1.0f), up()), L"beyond", LINE_INFO());
      Assert::AreEqual(5.0f, irradianceAt(cache, Float4(0.51f, 0.0f, 0.0f, 1.0f), up()), 1e-6f, L"the small one",
                       LINE_INFO());
    }

    TEST_METHOD(theCapBoundsOnlyItsOwnRecord) {
      // a record of R 10 next to a later one of R 10 capped at 0.1 (a fine grid's): the capped one is valid up to
      // 0.02 only, and the first keeps its validity up to 2 (clamped by the cap, it would reach only 0.12)
      const auto contexts = Contexts();
      auto cache = IrradianceCache(contexts.contexts.data(), 2, 1, Accuracy);
      auto large = floorRecord(Float4(0.0f, 0.0f, 0.0f, 1.0f), 1.0f);
      large.radius = 10.0f;
      auto capped = floorRecord(Float4(0.5f, 0.0f, 0.0f, 1.0f), 5.0f);
      capped.radius = 10.0f;
      capped.maxRadius = 0.1f;
      cache.addLevel({{large}, {}});
      cache.addLevel({{capped}, {}});
      Assert::AreEqual(0.1f, capped.validRadius(), L"valid radius", LINE_INFO());
      Assert::AreEqual(1.0f, irradianceAt(cache, Float4(-1.0f, 0.0f, 0.0f, 1.0f), up()), 1e-6f, L"the first",
                       LINE_INFO());
      Assert::AreEqual(1.0f, irradianceAt(cache, Float4(0.53f, 0.0f, 0.0f, 1.0f), up()), 1e-6f,
                       L"beyond the capped one", LINE_INFO());
      Assert::IsTrue(irradianceAt(cache, Float4(0.505f, 0.0f, 0.0f, 1.0f), up()) > 1.5f, L"near the capped one",
                     LINE_INFO());
    }

    TEST_METHOD(coverageIsTheWeightSumTimesTheAccuracy) {
      // one record at half its validity radius covers fully, nearer more, farther less
      const auto contexts = Contexts();
      const auto cache = cacheOf(contexts, {floorRecord(Float4(0.0f, 0.0f, 0.0f, 1.0f), 3.0f)});
      auto irradiance = spectral::Spectrum::zero();
      Assert::AreEqual(1.0f, cache.interpolate(0, Float4(0.1f, 0.0f, 0.0f, 1.0f), up(), irradiance), 1e-5f,
                       L"half", LINE_INFO());
      Assert::AreEqual(1.0f / 3.0f, cache.interpolate(0, Float4(0.15f, 0.0f, 0.0f, 1.0f), up(), irradiance), 1e-5f,
                       L"three quarters", LINE_INFO());
    }

    TEST_METHOD(aSetsContextsReadEachOthersRecordsAtTheirWavelengths) {
      // one set of two contexts half a wavelength step apart; a record of the first context with a ramp over the
      // wavelengths, one of the second with 2 everywhere
      const auto scene = Scene();
      auto contexts = std::vector<SpectralContext, AlignedAllocator<SpectralContext>>();
      contexts.push_back(scene.createSpectralContext(spectral::LAMBDA_MIN + 0.25f * spectral::DELTA_LAMBDA));
      contexts.push_back(scene.createSpectralContext(spectral::LAMBDA_MIN + 0.75f * spectral::DELTA_LAMBDA));
      auto cache = IrradianceCache(contexts.data(), 2, 2, Accuracy);
      Assert::AreEqual(One<ASizeT>(), cache.getSetCount(), L"sets", LINE_INFO());
      auto ramp = floorRecord(Float4(0.0f, 0.0f, 0.0f, 1.0f), 0.0f);
      ramp.irradiance = spectral::Spectrum{Float8(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f)};
      auto flat = floorRecord(Float4(0.3f, 0.0f, 0.0f, 1.0f), 2.0f);
      flat.context = 1;
      cache.addLevel({{ramp, flat}});

      auto irradiance = spectral::Spectrum::zero();
      cache.interpolate(0, Float4(0.0f, 0.0f, 0.0f, 1.0f), up(), irradiance);
      Assert::AreEqual(1.0f, x2(irradiance.data), 1e-5f, L"its own context", LINE_INFO());
      cache.interpolate(1, Float4(0.0f, 0.0f, 0.0f, 1.0f), up(), irradiance);
      Assert::AreEqual(1.5f, x2(irradiance.data), 1e-5f, L"half a step up", LINE_INFO());
      Assert::AreEqual(7.0f, x8(irradiance.data), 1e-5f, L"the last lane holds", LINE_INFO());
      // halfway between them, equally weighted
      cache.interpolate(0, Float4(0.15f, 0.0f, 0.0f, 1.0f), up(), irradiance);
      Assert::AreEqual(1.0f, x1(irradiance.data), 1e-5f, L"mixed", LINE_INFO());
      Assert::AreEqual(4.5f, x8(irradiance.data), 1e-5f, L"mixed, last lane", LINE_INFO());
    }
  };
}
