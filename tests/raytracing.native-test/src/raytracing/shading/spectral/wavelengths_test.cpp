#include "raytracing_tests.h"
#include "raytracing/shading/spectral/wavelengths.h"

#include <vectorization.h>

namespace raytracer::test
{
    using namespace raytracer::spectral;

    TEST_CLASS (WavelengthsTest)
    {
        public
        :
        TEST_METHOD(wrapToVisibleInRange)
        {
            Assert::AreEqual(500.0f, wrapToVisible(500.0f), L"in-range unchanged", LINE_INFO());
            Assert::AreEqual(380.0f, wrapToVisible(380.0f), L"lower bound", LINE_INFO());
            Assert::AreEqual(779.0f, wrapToVisible(779.0f), L"just below upper bound", LINE_INFO());
        }

        TEST_METHOD(wrapToVisibleWrapsUpperBound)
        {
            Assert::AreEqual(380.0f, wrapToVisible(780.0f), 1e-5f, L"780 wraps to 380", LINE_INFO());
        }

        TEST_METHOD(wrapToVisibleWrapsAboveRange)
        {
            Assert::AreEqual(430.0f, wrapToVisible(830.0f), 1e-5f, L"830 wraps to 430", LINE_INFO());
        }

        TEST_METHOD(wavelengthsAtBase)
        {
            const auto w = wavelengthsAt(380.0f);
            const Float tol = 1e-3f;
            Assert::AreEqual(380.0f, x1(w), tol, L"lane 0", LINE_INFO());
            Assert::AreEqual(380.0f + DELTA_LAMBDA, x2(w), tol, L"lane 1", LINE_INFO());
            Assert::AreEqual(380.0f + DELTA_LAMBDA * 2, x3(w), tol, L"lane 2", LINE_INFO());
            Assert::AreEqual(380.0f + DELTA_LAMBDA * 3, x4(w), tol, L"lane 3", LINE_INFO());
            Assert::AreEqual(380.0f + DELTA_LAMBDA * 4, x5(w), tol, L"lane 4", LINE_INFO());
            Assert::AreEqual(380.0f + DELTA_LAMBDA * 5, x6(w), tol, L"lane 5", LINE_INFO());
            Assert::AreEqual(380.0f + DELTA_LAMBDA * 6, x7(w), tol, L"lane 6", LINE_INFO());
            Assert::AreEqual(380.0f + DELTA_LAMBDA * 7, x8(w), tol, L"lane 7", LINE_INFO());
        }

        TEST_METHOD(wavelengthsAtOffset)
        {
            const auto w = wavelengthsAt(500.0f);
            const Float tol = 1e-3f;
            Assert::AreEqual(500.0f, x1(w), tol, L"lane 0", LINE_INFO());
            Assert::AreEqual(wrapToVisible(500.0f + DELTA_LAMBDA), x2(w), tol, L"lane 1", LINE_INFO());
            Assert::AreEqual(wrapToVisible(500.0f + DELTA_LAMBDA * 2), x3(w), tol, L"lane 2", LINE_INFO());
            Assert::AreEqual(wrapToVisible(500.0f + DELTA_LAMBDA * 3), x4(w), tol, L"lane 3", LINE_INFO());
            Assert::AreEqual(wrapToVisible(500.0f + DELTA_LAMBDA * 4), x5(w), tol, L"lane 4", LINE_INFO());
            Assert::AreEqual(wrapToVisible(500.0f + DELTA_LAMBDA * 5), x6(w), tol, L"lane 5", LINE_INFO());
            Assert::AreEqual(wrapToVisible(500.0f + DELTA_LAMBDA * 6), x7(w), tol, L"lane 6", LINE_INFO());
            Assert::AreEqual(wrapToVisible(500.0f + DELTA_LAMBDA * 7), x8(w), tol, L"lane 7", LINE_INFO());
        }

        TEST_METHOD(wavelengthsAtWrapsCorrectly)
        {
            const auto w = wavelengthsAt(740.0f);
            const Float tol = 1e-3f;
            Assert::AreEqual(740.0f, x1(w), tol, L"lane 0: 740", LINE_INFO());
            Assert::AreEqual(wrapToVisible(740.0f + DELTA_LAMBDA), x2(w), tol, L"lane 1 wraps", LINE_INFO());
            Assert::AreEqual(wrapToVisible(740.0f + DELTA_LAMBDA * 2), x3(w), tol, L"lane 2 wraps", LINE_INFO());
            Assert::AreEqual(wrapToVisible(740.0f + DELTA_LAMBDA * 6), x7(w), tol, L"lane 6 wraps", LINE_INFO());
            Assert::AreEqual(wrapToVisible(740.0f + DELTA_LAMBDA * 7), x8(w), tol, L"lane 7 wraps", LINE_INFO());
        }

        TEST_METHOD(allSpectralLanesInVisibleRange)
        {
            const Float testLambdas[] = {380.0f, 429.99f, 500.0f, 730.0f, 779.0f};
            for (const auto lambda : testLambdas)
            {
                const auto w = wavelengthsAt(lambda);
                const Float lanes[] = {x1(w), x2(w), x3(w), x4(w), x5(w), x6(w), x7(w), x8(w)};
                for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
                {
                    Assert::IsTrue(lanes[i] >= 380.0f, L"lane >= 380", LINE_INFO());
                    Assert::IsTrue(lanes[i] < 780.0f, L"lane < 780", LINE_INFO());
                }
            }
        }

        TEST_METHOD(spectralSamplesIs8)
        {
            Assert::AreEqual(ASizeT{8}, SPECTRAL_SAMPLES, L"8 spectral samples", LINE_INFO());
        }
    };
}
