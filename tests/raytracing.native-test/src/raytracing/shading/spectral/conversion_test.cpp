#include "raytracing_tests.h"
#include "raytracing/shading/spectral/conversion.h"
#include "raytracing/shading/spectral/wavelengths.h"

#include <vectorization.h>

namespace raytracer::test
{
    using namespace raytracer::spectral;

    TEST_CLASS (ConversionTest)
    {
        public
        :
        TEST_METHOD(monochromaticAt555nmProducesExpectedXYZ)
        {
            const auto s = monochromatic(555.0f, 555.0f, 1.0f);
            const auto xyz = spectrumToXYZ(s, makeXYZContext(555.0f));

            Assert::IsTrue(x(xyz) > 0.1f, L"X from 555nm mono nonzero", LINE_INFO());
            Assert::IsTrue(y(xyz) > x(xyz), L"Y > X at 555nm (yBar peak)", LINE_INFO());
            Assert::IsTrue(z(xyz) < 0.01f, L"Z near zero at 555nm", LINE_INFO());
        }

        TEST_METHOD(zeroSpectrumProducesBlackXYZ)
        {
            const auto s = Spectrum::zero();
            const auto xyz = spectrumToXYZ(s, makeXYZContext(500.0f));

            Assert::AreEqual(0.0f, x(xyz), L"X is zero", LINE_INFO());
            Assert::AreEqual(0.0f, y(xyz), L"Y is zero", LINE_INFO());
            Assert::AreEqual(0.0f, z(xyz), L"Z is zero", LINE_INFO());
        }

        TEST_METHOD(xyzToDisplayRGBD65WhitePoint)
        {
            // D65 white point in XYZ
            const auto d65xyz = Float4{0.95047f, 1.0f, 1.08883f, 0.0f};
            const auto rgb = xyzToDisplayRGB(d65xyz);

            Assert::AreEqual(1.0f, x(rgb.value), 0.01f, L"R for D65 white", LINE_INFO());
            Assert::AreEqual(1.0f, y(rgb.value), 0.01f, L"G for D65 white", LINE_INFO());
            Assert::AreEqual(1.0f, z(rgb.value), 0.01f, L"B for D65 white", LINE_INFO());
        }

        TEST_METHOD(spectrumLinearityInXYZ)
        {
            const auto ctx = makeXYZContext(500.0f);
            const auto a = Spectrum::constant(1.0f);
            const auto b = Spectrum::constant(2.0f);

            const auto xyzA = spectrumToXYZ(a, ctx);
            const auto xyzB = spectrumToXYZ(b, ctx);

            Assert::AreEqual(2.0f * x(xyzA), x(xyzB), 0.01f, L"X linearity", LINE_INFO());
            Assert::AreEqual(2.0f * y(xyzA), y(xyzB), 0.01f, L"Y linearity", LINE_INFO());
            Assert::AreEqual(2.0f * z(xyzA), z(xyzB), 0.01f, L"Z linearity", LINE_INFO());
        }

        TEST_METHOD(wComponentZeroInXYZOutput)
        {
            const auto s = Spectrum::constant(1.0f);
            const auto xyz = spectrumToXYZ(s, makeXYZContext(500.0f));
            Assert::AreEqual(0.0f, w(xyz), L"W component zero", LINE_INFO());
        }

        // Lane 7 used to be heroLambda metadata. Now it's a real spectral sample, so
        // energy in lane 7 must contribute to the XYZ integration.
        TEST_METHOD(lane7ContributesToXYZ)
        {
            // wavelengthsAt(380) lane 7 = 730nm — yBar(730nm) is small but nonzero.
            // Put a spike of energy in lane 7 only.
            auto s = Spectrum::zero();
            s.data[7] = 100.0f;
            const auto xyz = spectrumToXYZ(s, makeXYZContext(380.0f));
            Assert::IsTrue(x(xyz) > 0.0f, L"X gets contribution from lane 7 (730nm)", LINE_INFO());
            Assert::IsTrue(y(xyz) > 0.0f, L"Y gets contribution from lane 7 (730nm)", LINE_INFO());
        }

        TEST_METHOD(uniformSpectrumYieldsApproxNeutralXYZ)
        {
            // Spectrum::constant(1) covers all 8 lanes equally. After XYZ integration and
            // sRGB conversion this should be near the equal-energy white point (E illuminant,
            // CIE x=y=1/3), which after the renderer's XYZ->sRGB matrix and AgX view transform
            // is roughly perceptually neutral. We assert the linear xyz directly.
            const auto s = Spectrum::constant(1.0f);
            const auto xyz = spectrumToXYZ(s, makeXYZContext(spectral::LAMBDA_MIN));
            Assert::AreEqual(x(xyz), y(xyz), 0.05f, L"X ~ Y for equal-energy", LINE_INFO());
            Assert::AreEqual(y(xyz), z(xyz), 0.20f, L"Y ~ Z for equal-energy", LINE_INFO());
        }
    };
}
