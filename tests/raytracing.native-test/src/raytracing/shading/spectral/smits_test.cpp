#include "raytracing_tests.h"
#include "raytracing/shading/spectral/smits.h"
#include "raytracing/shading/spectral/wavelengths.h"

#include <vectorization.h>

namespace raytracer::test
{
    using namespace raytracer::spectral;

    TEST_CLASS (SmitsTest)
    {
        public
        :
        TEST_METHOD(whiteProducesNearUniformSpectrum)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto s = rgbToSpectrum(Float4{1.0f, 1.0f, 1.0f, 0.0f}, w);
            const Float vals[] = {x1(s), x2(s), x3(s), x4(s), x5(s), x6(s), x7(s), x8(s)};
            for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
            {
                Assert::AreEqual(1.0f, vals[i], 0.1f, L"white spectrum near 1.0", LINE_INFO());
            }
        }

        TEST_METHOD(blackProducesZeroSpectrum)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto s = rgbToSpectrum(Float4{0.0f, 0.0f, 0.0f, 0.0f}, w);
            const Float vals[] = {x1(s), x2(s), x3(s), x4(s), x5(s), x6(s), x7(s), x8(s)};
            for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
            {
                Assert::AreEqual(0.0f, vals[i], 1e-6f, L"black spectrum is zero", LINE_INFO());
            }
        }

        TEST_METHOD(primaryRedHighInRedRegion)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto s = rgbToSpectrum(Float4{1.0f, 0.0f, 0.0f, 0.0f}, w);
            Assert::IsTrue(x7(s) > 0.5f, L"red high in red region", LINE_INFO());
            Assert::IsTrue(x3(s) < 0.1f, L"red low in blue-green region", LINE_INFO());
        }

        TEST_METHOD(primaryGreenHighInGreenRegion)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto s = rgbToSpectrum(Float4{0.0f, 1.0f, 0.0f, 0.0f}, w);
            Assert::IsTrue(x4(s) > 0.5f, L"green high in green region", LINE_INFO());
            Assert::IsTrue(x2(s) < 0.1f, L"green low in blue region", LINE_INFO());
        }

        TEST_METHOD(allOutputsNonNegative)
        {
            const auto w = wavelengthsAt(380.0f);
            const Float testValues[] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
            for (const Float r : testValues)
            {
                for (const Float g : testValues)
                {
                    for (const Float b : testValues)
                    {
                        const auto s = rgbToSpectrum(Float4{r, g, b, 0.0f}, w);
                        const Float vals[] = {x1(s), x2(s), x3(s), x4(s), x5(s), x6(s), x7(s)};
                        for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
                        {
                            Assert::IsTrue(vals[i] >= 0.0f, L"non-negative output", LINE_INFO());
                        }
                    }
                }
            }
        }

        TEST_METHOD(grayIsHalfOfWhite)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto white = rgbToSpectrum(Float4{1.0f, 1.0f, 1.0f, 0.0f}, w);
            const auto gray = rgbToSpectrum(Float4{0.5f, 0.5f, 0.5f, 0.0f}, w);
            Assert::AreEqual(x1(white) * 0.5f, x1(gray), 0.01f, L"gray = 0.5 * white", LINE_INFO());
            Assert::AreEqual(x4(white) * 0.5f, x4(gray), 0.01f, L"gray = 0.5 * white lane 3", LINE_INFO());
        }

        TEST_METHOD(yellowHighInRedAndGreenRegions)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto s = rgbToSpectrum(Float4{1.0f, 1.0f, 0.0f, 0.0f}, w);
            Assert::IsTrue(x5(s) > 0.5f, L"yellow high in orange region", LINE_INFO());
            Assert::IsTrue(x7(s) > 0.5f, L"yellow high in red region", LINE_INFO());
            Assert::IsTrue(x2(s) < 0.1f, L"yellow low in blue region", LINE_INFO());
        }

        TEST_METHOD(scalesLinearly)
        {
            const auto w = wavelengthsAt(405.0f);
            const auto half = rgbToSpectrum(Float4{0.5f, 0.0f, 0.0f, 0.0f}, w);
            const auto full = rgbToSpectrum(Float4{1.0f, 0.0f, 0.0f, 0.0f}, w);
            Assert::AreEqual(x4(full), 2.0f * x4(half), 0.01f, L"linear scaling", LINE_INFO());
        }

        TEST_METHOD(primaryBlueHighInBlueRegion)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto s = rgbToSpectrum(Float4{0.0f, 0.0f, 1.0f, 0.0f}, w);
            Assert::IsTrue(x2(s) > 0.5f, L"blue high in blue region", LINE_INFO());
            Assert::IsTrue(x7(s) < 0.1f, L"blue low in red region", LINE_INFO());
        }

        TEST_METHOD(cyanHighInBlueAndGreenRegions)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto s = rgbToSpectrum(Float4{0.0f, 1.0f, 1.0f, 0.0f}, w);
            Assert::IsTrue(x2(s) > 0.5f, L"cyan high in blue region", LINE_INFO());
            Assert::IsTrue(x4(s) > 0.5f, L"cyan high in green region", LINE_INFO());
        }

        TEST_METHOD(magentaHighInRedAndBlueRegions)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto s = rgbToSpectrum(Float4{1.0f, 0.0f, 1.0f, 0.0f}, w);
            Assert::IsTrue(x1(s) > 0.5f, L"magenta high at 380nm", LINE_INFO());
            Assert::IsTrue(x7(s) > 0.5f, L"magenta high in red region", LINE_INFO());
        }

        TEST_METHOD(wrappedWavelengthsProduceValidOutput)
        {
            const auto w = wavelengthsAt(740.0f);
            const auto s = rgbToSpectrum(Float4{0.5f, 0.5f, 0.5f, 0.0f}, w);
            const Float vals[] = {x1(s), x2(s), x3(s), x4(s), x5(s), x6(s), x7(s)};
            for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
            {
                Assert::IsTrue(vals[i] >= 0.0f, L"non-negative with wrapped wavelengths", LINE_INFO());
            }
        }
    };
}
