#include "raytracing_tests.h"
#include "raytracing/shading/spectral/illuminants.h"
#include "raytracing/shading/spectral/wavelengths.h"

#include <vectorization.h>

namespace raytracer::test
{
    using namespace raytracer::spectral;

    TEST_CLASS (IlluminantsTest)
    {
        public
        :
        TEST_METHOD(d65AllPositive)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto spd = illuminantAt(Illuminant::D65, w);
            const Float vals[] = {x1(spd), x2(spd), x3(spd), x4(spd), x5(spd), x6(spd), x7(spd), x8(spd)};
            for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
            {
                Assert::IsTrue(vals[i] > 0.0f, L"D65 positive across visible", LINE_INFO());
            }
        }

        TEST_METHOD(d65NormalizedTo100At560nm)
        {
            const auto w = wavelengthsAt(560.0f);
            const auto spd = illuminantAt(Illuminant::D65, w);
            Assert::AreEqual(100.0f, x1(spd), 0.01f, L"D65 normalized to 100 at 560nm", LINE_INFO());
        }

        TEST_METHOD(d65PeakInBlueRegion)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto spd = illuminantAt(Illuminant::D65, w);
            Assert::IsTrue(x3(spd) > x7(spd), L"D65 blue region brighter than red", LINE_INFO());
        }

        TEST_METHOD(d65InterpolatesBetweenTableEntries)
        {
            const auto w = wavelengthsAt(382.5f);
            const auto spd = illuminantAt(Illuminant::D65, w);
            const Float expected = 49.9755f * 0.5f + 52.3118f * 0.5f;
            Assert::AreEqual(expected, x1(spd), 0.01f, L"D65 interpolation at 382.5nm", LINE_INFO());
        }

        TEST_METHOD(illuminantAAllPositive)
        {
            const auto w = wavelengthsAt(400.0f);
            const auto spd = illuminantAt(Illuminant::A, w);
            const Float vals[] = {x1(spd), x2(spd), x3(spd), x4(spd), x5(spd), x6(spd), x7(spd), x8(spd)};
            for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
            {
                Assert::IsTrue(vals[i] > 0.0f, L"illuminant A positive", LINE_INFO());
            }
        }

        TEST_METHOD(illuminantARisesTowardRed)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto spd = illuminantAt(Illuminant::A, w);
            Assert::IsTrue(x7(spd) > x1(spd), L"A is warmer: red region > 380nm", LINE_INFO());
            Assert::IsTrue(x7(spd) > x3(spd), L"A is warmer: red region > blue region", LINE_INFO());
        }

        TEST_METHOD(illuminantANormalizedTo100At560nm)
        {
            const auto w = wavelengthsAt(560.0f);
            const auto spd = illuminantAt(Illuminant::A, w);
            Assert::AreEqual(100.0f, x1(spd), 0.5f, L"A normalized to ~100 at 560nm", LINE_INFO());
        }

        TEST_METHOD(d65WrappedWavelengths)
        {
            const auto w = wavelengthsAt(740.0f);
            const auto spd = illuminantAt(Illuminant::D65, w);
            Assert::IsTrue(x1(spd) > 0.0f, L"D65 at 740nm positive", LINE_INFO());
            Assert::IsTrue(x2(spd) > 0.0f, L"D65 at wrapped wavelength positive", LINE_INFO());
        }

        TEST_METHOD(d65BoundaryWavelengths)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto spd = illuminantAt(Illuminant::D65, w);
            Assert::AreEqual(49.9755f, x1(spd), 0.01f, L"D65 exact at 380nm", LINE_INFO());
        }
    };
}
