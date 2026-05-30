#include "raytracing_tests.h"
#include "raytracing/shading/spectral/cmf.h"
#include "raytracing/shading/spectral/wavelengths.h"

#include <vectorization.h>

namespace raytracer::test
{
    using namespace raytracer::spectral;

    TEST_CLASS (CmfTest)
    {
        public
        :
        TEST_METHOD(yBarPeakAt555nm)
        {
            const auto wavelengths = wavelengthsAt(555.0f);
            const auto cmf = cmfAt(wavelengths);
            Assert::AreEqual(1.0f, x1(cmf.yBar), 0.01f, L"yBar peak at 555nm", LINE_INFO());
        }

        TEST_METHOD(cmfNearZeroAtBoundaries)
        {
            const auto wavelengths = wavelengthsAt(380.0f);
            const auto cmf = cmfAt(wavelengths);
            Assert::IsTrue(x1(cmf.xBar) < 0.01f, L"xBar near zero at 380nm", LINE_INFO());
            Assert::IsTrue(x1(cmf.yBar) < 0.001f, L"yBar near zero at 380nm", LINE_INFO());
            Assert::IsTrue(x1(cmf.zBar) < 0.01f, L"zBar near zero at 380nm", LINE_INFO());
        }

        TEST_METHOD(xBarHasTwoPeaks)
        {
            const auto w1 = wavelengthsAt(440.0f);
            const auto cmf1 = cmfAt(w1);
            Assert::IsTrue(x1(cmf1.xBar) > 0.3f, L"xBar first peak > 0.3 at 440nm", LINE_INFO());

            const auto w2 = wavelengthsAt(600.0f);
            const auto cmf2 = cmfAt(w2);
            Assert::IsTrue(x1(cmf2.xBar) > 1.0f, L"xBar second peak > 1.0 at 600nm", LINE_INFO());
        }

        TEST_METHOD(zBarPeakInBlueRegion)
        {
            const auto wavelengths = wavelengthsAt(445.0f);
            const auto cmf = cmfAt(wavelengths);
            Assert::IsTrue(x1(cmf.zBar) > 1.7f, L"zBar peak > 1.7 at 445nm", LINE_INFO());
        }

        TEST_METHOD(allValuesNonNegative)
        {
            for (Float hero = 380.0f; hero < 780.0f; hero += 50.0f)
            {
                const auto w = wavelengthsAt(hero);
                const auto cmf = cmfAt(w);
                const Float xvals[] = {
                    x1(cmf.xBar), x2(cmf.xBar), x3(cmf.xBar), x4(cmf.xBar),
                    x5(cmf.xBar), x6(cmf.xBar), x7(cmf.xBar), x8(cmf.xBar)
                };
                const Float yvals[] = {
                    x1(cmf.yBar), x2(cmf.yBar), x3(cmf.yBar), x4(cmf.yBar),
                    x5(cmf.yBar), x6(cmf.yBar), x7(cmf.yBar), x8(cmf.yBar)
                };
                const Float zvals[] = {
                    x1(cmf.zBar), x2(cmf.zBar), x3(cmf.zBar), x4(cmf.zBar),
                    x5(cmf.zBar), x6(cmf.zBar), x7(cmf.zBar), x8(cmf.zBar)
                };
                for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
                {
                    Assert::IsTrue(xvals[i] >= 0.0f, L"xBar non-negative", LINE_INFO());
                    Assert::IsTrue(yvals[i] >= 0.0f, L"yBar non-negative", LINE_INFO());
                    Assert::IsTrue(zvals[i] >= 0.0f, L"zBar non-negative", LINE_INFO());
                }
            }
        }

        TEST_METHOD(interpolationBetweenTableEntries)
        {
            const auto wavelengths = wavelengthsAt(382.5f);
            const auto cmf = cmfAt(wavelengths);
            const Float expected = 0.001368f * 0.5f + 0.002236f * 0.5f;
            Assert::AreEqual(expected, x1(cmf.xBar), 1e-5f, L"interpolation at 382.5nm", LINE_INFO());
        }
    };
}
