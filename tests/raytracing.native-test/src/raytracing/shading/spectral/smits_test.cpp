#include "raytracing_tests.h"
#include "raytracing/shading/color/primaries.h"
#include "raytracing/shading/spectral/conversion.h"
#include "raytracing/shading/spectral/illuminants.h"
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

        TEST_METHOD(basisOverloadMatchesWavelengthOverload)
        {
            const auto w = wavelengthsAt(437.0f);
            const auto basis = smitsBasisAt(w);
            const Float4 colors[] = {
                Float4{0.2f, 0.5f, 0.9f, 0.0f}, Float4{0.9f, 0.1f, 0.4f, 0.0f}, Float4{0.3f, 0.8f, 0.1f, 0.0f}
            };
            for (const auto& rgb : colors)
            {
                const auto fromBasis = rgbToSpectrum(rgb, basis);
                const auto fromWavelengths = rgbToSpectrum(rgb, w);
                const Float a[] = {
                    x1(fromBasis), x2(fromBasis), x3(fromBasis), x4(fromBasis),
                    x5(fromBasis), x6(fromBasis), x7(fromBasis), x8(fromBasis)
                };
                const Float b[] = {
                    x1(fromWavelengths), x2(fromWavelengths), x3(fromWavelengths), x4(fromWavelengths),
                    x5(fromWavelengths), x6(fromWavelengths), x7(fromWavelengths), x8(fromWavelengths)
                };
                for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
                {
                    // Same arithmetic along two call paths; allow for differently contracted or folded FMAs.
                    Assert::AreEqual(b[i], a[i], 1e-6f, L"same spectrum from precomputed basis", LINE_INFO());
                }
            }
        }

        TEST_METHOD(wavelengthsBeyondBasisRangeUseLastSample)
        {
            // The basis spectra end at 720nm: lane 8 of hero 380 (730nm) and lane 7 of hero 420 (720nm) agree.
            const auto beyond = rgbToSpectrum(Float4{1.0f, 0.2f, 0.0f, 0.0f}, wavelengthsAt(380.0f));
            const auto atEnd = rgbToSpectrum(Float4{1.0f, 0.2f, 0.0f, 0.0f}, wavelengthsAt(420.0f));
            Assert::AreEqual(x7(atEnd), x8(beyond), 1e-6f, L"clamped to the last basis sample", LINE_INFO());
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
                        const Float vals[] = {x1(s), x2(s), x3(s), x4(s), x5(s), x6(s), x7(s), x8(s)};
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

        TEST_METHOD(reflectancesStayWithinZeroAndOne)
        {
            // every RGB reflectance of a grid, and one beyond white (an HDR texture's), at hero wavelengths across the
            // visible range
            const Float values[] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 1.5f};
            for (auto hero = 380.0f; hero < 780.0f; hero += 12.5f)
            {
                const auto basis = smitsBasisAt(wavelengthsAt(hero));
                for (const Float r : values)
                {
                    for (const Float g : values)
                    {
                        for (const Float b : values)
                        {
                            const auto s = rgbToReflectance(Float4{r, g, b, 0.0f}, basis);
                            Assert::IsTrue(allTrue(s >= Zero<Float8>()), L"at least 0", LINE_INFO());
                            Assert::IsTrue(allTrue(s <= One<Float8>()), L"at most 1", LINE_INFO());
                        }
                    }
                }
            }
        }

        TEST_METHOD(whiteReflectanceIsAboutOne)
        {
            // Smits' white upsamples to 1.06 at every wavelength; as a reflectance it is 1 at its brightest
            auto brightest = 0.0f;
            for (auto hero = 380.0f; hero < 780.0f; hero += 12.5f)
            {
                const auto s = rgbToReflectance(Float4{1.0f, 1.0f, 1.0f, 0.0f}, smitsBasisAt(wavelengthsAt(hero)));
                Assert::IsTrue(allTrue(s >= Float8(0.995f)), L"about 1", LINE_INFO());
                brightest = max(brightest, spectralMax(s));
            }
            Assert::AreEqual(1.0f, brightest, 1e-4f, L"1 at its brightest", LINE_INFO());
            const auto light = rgbToSpectrum(Float4{1.0f, 1.0f, 1.0f, 0.0f}, wavelengthsAt(500.0f));
            Assert::AreEqual(1.06f, x1(light), 0.005f, L"a light's white as before", LINE_INFO());
        }

        TEST_METHOD(whiteReflectanceUnderD65LooksWhite)
        {
            // the display white: a white surface under D65 (the white of sRGB) displays as equal RGB channels, at the
            // illuminant's luminance
            auto xyz = Zero<Float4>();
            auto illuminantXYZ = Zero<Float4>();
            for (auto hero = 380.0f; hero < 430.0f; hero += 1.0f)
            {
                const auto wavelengths = wavelengthsAt(hero);
                const auto context = makeXYZContext(hero);
                const auto d65 = illuminantAt(Illuminant::D65, wavelengths);
                const auto white = rgbToReflectance(Float4{1.0f, 1.0f, 1.0f, 0.0f}, smitsBasisAt(wavelengths));
                xyz += spectrumToXYZ(Spectrum{white * d65}, context);
                illuminantXYZ += spectrumToXYZ(Spectrum{d65}, context);
            }
            const auto rgb = color::XYZToSRGBLinear() * xyz;
            Assert::AreEqual(x(rgb), y(rgb), 0.01f * y(rgb), L"red as green", LINE_INFO());
            Assert::AreEqual(z(rgb), y(rgb), 0.01f * y(rgb), L"blue as green", LINE_INFO());
            Assert::AreEqual(y(illuminantXYZ), y(xyz), 0.005f * y(illuminantXYZ), L"all of its light", LINE_INFO());
        }

        TEST_METHOD(wrappedWavelengthsProduceValidOutput)
        {
            const auto w = wavelengthsAt(740.0f);
            const auto s = rgbToSpectrum(Float4{0.5f, 0.5f, 0.5f, 0.0f}, w);
            const Float vals[] = {x1(s), x2(s), x3(s), x4(s), x5(s), x6(s), x7(s), x8(s)};
            for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
            {
                Assert::IsTrue(vals[i] >= 0.0f, L"non-negative with wrapped wavelengths", LINE_INFO());
            }
        }
    };
}
