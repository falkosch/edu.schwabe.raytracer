#include "raytracing_tests.h"
#include "raytracing/shading/spectral/blackbody.h"
#include "raytracing/shading/spectral/wavelengths.h"

#include <vectorization.h>

namespace raytracer::test
{
    using namespace raytracer::spectral;

    TEST_CLASS (BlackbodyTest)
    {
        public
        :
        TEST_METHOD(zeroTemperatureProducesZero)
        {
            const auto w = wavelengthsAt(500.0f);
            const auto b = blackbody(0.0f, w);
            Assert::AreEqual(0.0f, x1(b), L"lane 0 zero", LINE_INFO());
            Assert::AreEqual(0.0f, x4(b), L"lane 3 zero", LINE_INFO());
            Assert::AreEqual(0.0f, x8(b), L"lane 7 zero", LINE_INFO());
        }

        TEST_METHOD(allOutputsNonNegative)
        {
            const Float temps[] = {1000.0f, 2856.0f, 5778.0f, 10000.0f};
            for (const auto T : temps)
            {
                const auto w = wavelengthsAt(380.0f);
                const auto b = blackbody(T, w);
                const Float vals[] = {x1(b), x2(b), x3(b), x4(b), x5(b), x6(b), x7(b), x8(b)};
                for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
                {
                    Assert::IsTrue(vals[i] > 0.0f, L"positive output", LINE_INFO());
                }
            }
        }

        TEST_METHOD(wienDisplacementLaw)
        {
            // For T=5778K, peak ~502nm. Lanes near peak should be larger than wings.
            const auto w = wavelengthsAt(380.0f);
            const auto b = blackbody(5778.0f, w);

            const auto atWing380 = x1(b);
            const auto nearPeak = x3(b);
            const auto atRedWing = x7(b);

            Assert::IsTrue(nearPeak > atWing380, L"near peak > 380nm wing", LINE_INFO());
            Assert::IsTrue(nearPeak > atRedWing, L"near peak > red wing", LINE_INFO());
        }

        TEST_METHOD(hotterObjectPeaksAtShorterWavelength)
        {
            // At 450nm: hotter star should emit more than cooler star
            const auto w = wavelengthsAt(400.0f);
            const auto cool = blackbody(3000.0f, w);
            const auto hot = blackbody(10000.0f, w);

            // At shorter wavelengths, hot blackbody dominates
            const auto ratio450 = x2(hot) / x2(cool);
            // At longer wavelengths, ratio should be smaller
            const auto ratio700 = x7(hot) / x7(cool);

            Assert::IsTrue(ratio450 > ratio700, L"hot/cool ratio larger at short wavelengths", LINE_INFO());
        }

        TEST_METHOD(scalesWithTemperature)
        {
            const auto w = wavelengthsAt(500.0f);
            const auto cool = blackbody(3000.0f, w);
            const auto hot = blackbody(6000.0f, w);

            Float sumCool = 0.0f, sumHot = 0.0f;
            const Float coolVals[] = {
                x1(cool), x2(cool), x3(cool), x4(cool),
                x5(cool), x6(cool), x7(cool), x8(cool)
            };
            const Float hotVals[] = {
                x1(hot), x2(hot), x3(hot), x4(hot),
                x5(hot), x6(hot), x7(hot), x8(hot)
            };
            for (ASizeT i = 0; i < 8; ++i)
            {
                sumCool += coolVals[i];
                sumHot += hotVals[i];
            }

            Assert::IsTrue(sumHot > sumCool, L"hotter produces more total power", LINE_INFO());
        }

        TEST_METHOD(negativeTemperatureProducesZero)
        {
            const auto w = wavelengthsAt(500.0f);
            const auto b = blackbody(-100.0f, w);
            Assert::AreEqual(0.0f, x1(b), L"negative temp zero lane 0", LINE_INFO());
            Assert::AreEqual(0.0f, x4(b), L"negative temp zero lane 3", LINE_INFO());
            Assert::AreEqual(0.0f, x8(b), L"negative temp zero lane 7", LINE_INFO());
        }

        TEST_METHOD(veryHighTemperatureNoInfOrNaN)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto b = blackbody(50000.0f, w);
            const Float vals[] = {x1(b), x2(b), x3(b), x4(b), x5(b), x6(b), x7(b), x8(b)};
            for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
            {
                Assert::IsFalse(std::isnan(vals[i]), L"no NaN at extreme temperature", LINE_INFO());
                Assert::IsFalse(std::isinf(vals[i]), L"no Inf at extreme temperature", LINE_INFO());
                Assert::IsTrue(vals[i] > 0.0f, L"positive at extreme temperature", LINE_INFO());
            }
        }

        TEST_METHOD(veryLowTemperatureNoNaN)
        {
            const auto w = wavelengthsAt(500.0f);
            const auto b = blackbody(1.0f, w);
            const Float vals[] = {x1(b), x2(b), x3(b), x4(b), x5(b), x6(b), x7(b), x8(b)};
            for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
            {
                Assert::IsFalse(std::isnan(vals[i]), L"no NaN at 1K", LINE_INFO());
                Assert::IsTrue(vals[i] >= 0.0f, L"non-negative at 1K", LINE_INFO());
            }
        }

        TEST_METHOD(wrappedWavelengthsProduceValidOutput)
        {
            const auto w = wavelengthsAt(740.0f);
            const auto b = blackbody(5778.0f, w);
            const Float vals[] = {x1(b), x2(b), x3(b), x4(b), x5(b), x6(b), x7(b), x8(b)};
            for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
            {
                Assert::IsTrue(vals[i] > 0.0f, L"positive with wrapped wavelengths", LINE_INFO());
                Assert::IsFalse(std::isnan(vals[i]), L"no NaN with wrapped wavelengths", LINE_INFO());
            }
        }
    };
}
