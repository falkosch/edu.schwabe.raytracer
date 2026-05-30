#include "raytracing_tests.h"
#include "raytracing/shading/spectral/spectrum.h"
#include "raytracing/shading/spectral/wavelengths.h"

#include <vectorization.h>

namespace raytracer::test
{
    using namespace raytracer::spectral;

    TEST_CLASS (SpectrumTest)
    {
        public
        :
        TEST_METHOD(zeroFactoryAllLanesZero)
        {
            const auto s = Spectrum::zero();
            Assert::AreEqual(0.0f, x1(s.data), L"lane 0", LINE_INFO());
            Assert::AreEqual(0.0f, x4(s.data), L"lane 3", LINE_INFO());
            Assert::AreEqual(0.0f, x7(s.data), L"lane 6", LINE_INFO());
        }

        TEST_METHOD(constantFactoryAllLanesEqual)
        {
            const auto s = Spectrum::constant(1.0f);
            Assert::AreEqual(1.0f, x1(s.data), L"lane 0", LINE_INFO());
            Assert::AreEqual(1.0f, x5(s.data), L"lane 4", LINE_INFO());
            Assert::AreEqual(1.0f, x7(s.data), L"lane 6", LINE_INFO());
        }

        TEST_METHOD(additionElementwise)
        {
            const auto a = Spectrum::constant(2.0f);
            const auto b = Spectrum::constant(3.0f);
            const auto c = a + b;

            Assert::AreEqual(5.0f, x1(c.data), L"lane 0", LINE_INFO());
            Assert::AreEqual(5.0f, x7(c.data), L"lane 6", LINE_INFO());
        }

        TEST_METHOD(subtractionElementwise)
        {
            const auto a = Spectrum::constant(5.0f);
            const auto b = Spectrum::constant(2.0f);
            const auto c = a - b;

            Assert::AreEqual(3.0f, x1(c.data), L"lane 0", LINE_INFO());
            Assert::AreEqual(3.0f, x7(c.data), L"lane 6", LINE_INFO());
        }

        TEST_METHOD(multiplicationElementwise)
        {
            const auto a = Spectrum::constant(3.0f);
            const auto b = Spectrum::constant(4.0f);
            const auto c = a * b;

            Assert::AreEqual(12.0f, x1(c.data), L"lane 0", LINE_INFO());
            Assert::AreEqual(12.0f, x7(c.data), L"lane 6", LINE_INFO());
        }

        TEST_METHOD(scalarMultiplication)
        {
            const auto a = Spectrum::constant(2.0f);
            const auto b = a * 3.0f;
            const auto c = 3.0f * a;

            Assert::AreEqual(6.0f, x1(b.data), L"spectrum * scalar", LINE_INFO());
            Assert::AreEqual(6.0f, x1(c.data), L"scalar * spectrum", LINE_INFO());
        }

        TEST_METHOD(compoundAddAssign)
        {
            auto a = Spectrum::constant(1.0f);
            const auto b = Spectrum::constant(2.0f);
            a += b;

            Assert::AreEqual(3.0f, x1(a.data), L"lane 0", LINE_INFO());
            Assert::AreEqual(3.0f, x7(a.data), L"lane 6", LINE_INFO());
        }

        TEST_METHOD(compoundMulAssign)
        {
            auto a = Spectrum::constant(3.0f);
            const auto b = Spectrum::constant(4.0f);
            a *= b;

            Assert::AreEqual(12.0f, x1(a.data), L"lane 0", LINE_INFO());
        }

        TEST_METHOD(compoundScalarMulAssign)
        {
            auto a = Spectrum::constant(3.0f);
            a *= 2.0f;

            Assert::AreEqual(6.0f, x1(a.data), L"lane 0", LINE_INFO());
        }

        TEST_METHOD(monochromaticHitsHeroBin)
        {
            const auto s = monochromatic(500.0f, 500.0f, 1.0f);
            Assert::AreEqual(1.0f, x1(s.data), L"lane 0: hero wavelength matches", LINE_INFO());
            Assert::AreEqual(0.0f, x4(s.data), L"lane 3: different wavelength", LINE_INFO());
        }

        TEST_METHOD(monochromaticZeroOutsideRange)
        {
            const auto s = monochromatic(380.0f, 380.0f, 5.0f);
            Assert::AreEqual(5.0f, x1(s.data), L"lane 0: 380 matches", LINE_INFO());
            Assert::AreEqual(0.0f, x2(s.data), L"lane 1: other wavelength", LINE_INFO());
        }

        TEST_METHOD(distributivityHolds)
        {
            const auto a = Spectrum::constant(2.0f);

            const auto lhs = a * 5.0f;
            const auto rhs = a * 2.0f + a * 3.0f;

            Assert::AreEqual(x1(lhs.data), x1(rhs.data), 1e-5f, L"distributivity lane 0", LINE_INFO());
            Assert::AreEqual(x7(lhs.data), x7(rhs.data), 1e-5f, L"distributivity lane 6", LINE_INFO());
        }

        TEST_METHOD(additionCommutative)
        {
            const auto a = Spectrum::constant(2.0f);
            const auto b = Spectrum::constant(3.0f);
            const auto ab = a + b;
            const auto ba = b + a;
            Assert::AreEqual(x1(ab.data), x1(ba.data), 1e-6f, L"a+b == b+a lane 0", LINE_INFO());
            Assert::AreEqual(x7(ab.data), x7(ba.data), 1e-6f, L"a+b == b+a lane 6", LINE_INFO());
        }

        TEST_METHOD(multiplicationCommutative)
        {
            const auto a = Spectrum::constant(2.0f);
            const auto b = Spectrum::constant(3.0f);
            const auto ab = a * b;
            const auto ba = b * a;
            Assert::AreEqual(x1(ab.data), x1(ba.data), 1e-6f, L"a*b == b*a lane 0", LINE_INFO());
            Assert::AreEqual(x7(ab.data), x7(ba.data), 1e-6f, L"a*b == b*a lane 6", LINE_INFO());
        }

        TEST_METHOD(zeroIsAdditiveIdentity)
        {
            const auto a = Spectrum::constant(7.0f);
            const auto z = Spectrum::zero();
            const auto result = a + z;
            Assert::AreEqual(7.0f, x1(result.data), 1e-6f, L"a+0 == a lane 0", LINE_INFO());
            Assert::AreEqual(7.0f, x7(result.data), 1e-6f, L"a+0 == a lane 6", LINE_INFO());
        }

        TEST_METHOD(oneIsMultiplicativeIdentity)
        {
            const auto a = Spectrum::constant(7.0f);
            const auto result = a * 1.0f;
            Assert::AreEqual(7.0f, x1(result.data), 1e-6f, L"a*1 == a lane 0", LINE_INFO());
            Assert::AreEqual(7.0f, x7(result.data), 1e-6f, L"a*1 == a lane 6", LINE_INFO());
        }

        TEST_METHOD(zeroScalarAnnihilates)
        {
            const auto a = Spectrum::constant(7.0f);
            const auto result = a * 0.0f;
            Assert::AreEqual(0.0f, x1(result.data), L"a*0 == 0 lane 0", LINE_INFO());
        }

        TEST_METHOD(monochromaticWrappedWavelength)
        {
            const auto s = monochromatic(740.0f, 740.0f, 2.0f);
            Assert::AreEqual(2.0f, x1(s.data), L"lane 0: hero matches", LINE_INFO());
            Assert::AreEqual(0.0f, x4(s.data), L"lane 3: different wavelength", LINE_INFO());
        }

        TEST_METHOD(monochromaticBinEdgeBehavior)
        {
            const Float halfBin = DELTA_LAMBDA * Half<Float>();
            const auto inside = monochromatic(500.0f, 500.0f + halfBin - 1.0f, 1.0f);
            Assert::AreEqual(1.0f, x1(inside.data), L"within halfBin of hero", LINE_INFO());

            const auto outside = monochromatic(500.0f, 500.0f + halfBin + 1.0f, 1.0f);
            Assert::AreEqual(0.0f, x1(outside.data), L"outside halfBin of hero", LINE_INFO());
        }

        // Lane 7 used to carry heroLambda metadata; it now holds a real spectral sample.
        // Verify it participates in arithmetic like every other lane.
        TEST_METHOD(lane7ParticipatesInAddition)
        {
            const auto a = Spectrum::constant(2.0f);
            const auto b = Spectrum::constant(3.0f);
            const auto c = a + b;
            Assert::AreEqual(5.0f, x8(c.data), L"lane 7 sums normally", LINE_INFO());
        }

        TEST_METHOD(lane7ParticipatesInMultiplication)
        {
            const auto a = Spectrum::constant(2.0f);
            const auto b = Spectrum::constant(3.0f);
            const auto c = a * b;
            Assert::AreEqual(6.0f, x8(c.data), L"lane 7 multiplies normally", LINE_INFO());
        }

        TEST_METHOD(lane7ParticipatesInUnaryNegation)
        {
            const auto a = Spectrum::constant(4.0f);
            const auto neg = -a;
            Assert::AreEqual(-4.0f, x8(neg.data), L"lane 7 negates normally", LINE_INFO());
        }

        TEST_METHOD(lane7ParticipatesInScalarMul)
        {
            const auto a = Spectrum::constant(2.0f);
            const auto b = a * 3.0f;
            Assert::AreEqual(6.0f, x8(b.data), L"lane 7 scales normally", LINE_INFO());
        }

        TEST_METHOD(spectralMaxConsidersLane7)
        {
            // Spectrum where lane 7 is the maximum among 8 lanes.
            auto s = Spectrum::zero();
            s.data[7] = 10.0f;
            Assert::AreEqual(10.0f, spectralMax(s), L"max sees lane 7", LINE_INFO());
        }

        TEST_METHOD(monochromaticPlacesEnergyInLane7AtMatchingWavelength)
        {
            // wavelengthsAt(380) lane 7 = 380 + 7*Δλ = 730nm. Targeting 730 lights lane 7.
            const auto s = monochromatic(380.0f, 380.0f + DELTA_LAMBDA * 7.0f, 1.0f);
            Assert::AreEqual(1.0f, x8(s.data), L"lane 7 receives intensity", LINE_INFO());
            Assert::AreEqual(0.0f, x1(s.data), L"lane 0 stays zero", LINE_INFO());
        }
    };
}
