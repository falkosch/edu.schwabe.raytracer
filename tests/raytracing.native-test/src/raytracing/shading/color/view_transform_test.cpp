#include "raytracing_tests.h"
#include "raytracing/shading/color/view_transform.h"

namespace raytracer::test
{
    using namespace raytracer::color;

    TEST_CLASS(ViewTransformTest)
    {
    public:
        TEST_METHOD(srgbEncodeDecodeRoundTrip)
        {
            const auto linear = Float4{0.5f, 0.1f, 0.9f, 0.0f};
            const auto encoded = srgbEncode(linear);
            const auto decoded = srgbDecode(encoded);

            Assert::AreEqual(x(linear), x(decoded), 1e-6f, L"R round-trip", LINE_INFO());
            Assert::AreEqual(y(linear), y(decoded), 1e-6f, L"G round-trip", LINE_INFO());
            Assert::AreEqual(z(linear), z(decoded), 1e-6f, L"B round-trip", LINE_INFO());
        }

        TEST_METHOD(srgbEncodeBlackIsBlack)
        {
            const auto black = Zero<Float4>();
            const auto encoded = srgbEncode(black);

            Assert::AreEqual(0.0f, x(encoded), 1e-7f, L"black stays black", LINE_INFO());
        }

        TEST_METHOD(srgbEncodeWhiteIsWhite)
        {
            const auto white = Float4{1.0f, 1.0f, 1.0f, 0.0f};
            const auto encoded = srgbEncode(white);

            Assert::AreEqual(1.0f, x(encoded), 1e-5f, L"white stays white", LINE_INFO());
            Assert::AreEqual(1.0f, y(encoded), 1e-5f, L"white stays white", LINE_INFO());
            Assert::AreEqual(1.0f, z(encoded), 1e-5f, L"white stays white", LINE_INFO());
        }

        TEST_METHOD(srgbEncodedValueHigherThanLinear)
        {
            const auto linear = Float4{0.5f, 0.5f, 0.5f, 0.0f};
            const auto encoded = srgbEncode(linear);

            Assert::IsTrue(x(encoded) > 0.5f, L"sRGB encode raises mid-tones", LINE_INFO());
        }

        TEST_METHOD(srgbLinearSegmentBelowThreshold)
        {
            const auto linear = Float4{0.002f, 0.002f, 0.002f, 0.0f};
            const auto encoded = srgbEncode(linear);

            Assert::AreEqual(0.002f * 12.92f, x(encoded), 1e-6f, L"linear segment", LINE_INFO());
        }

        TEST_METHOD(srgbDecodeLinearSegment)
        {
            const auto encoded = Float4{0.04f, 0.04f, 0.04f, 0.0f};
            const auto decoded = srgbDecode(encoded);

            Assert::AreEqual(0.04f / 12.92f, x(decoded), 1e-6f, L"decode linear segment", LINE_INFO());
        }

        TEST_METHOD(srgbWComponentPreserved)
        {
            const auto v = Float4{0.5f, 0.5f, 0.5f, 42.0f};
            const auto encoded = srgbEncode(v);

            Assert::AreEqual(42.0f, w(encoded), 1e-7f, L"W preserved on encode", LINE_INFO());

            const auto decoded = srgbDecode(encoded);
            Assert::AreEqual(42.0f, w(decoded), 1e-7f, L"W preserved on decode", LINE_INFO());
        }

        TEST_METHOD(acesFilmicBlackIsBlack)
        {
            const auto black = Zero<Float4>();
            const auto mapped = acesFilmic(black);

            Assert::AreEqual(0.0f, x(mapped), 1e-5f, L"black maps to ~black", LINE_INFO());
        }

        TEST_METHOD(acesFilmicOutputInUnitRange)
        {
            const auto bright = Float4{10.0f, 5.0f, 2.0f, 0.0f};
            const auto mapped = acesFilmic(bright);

            Assert::IsTrue(x(mapped) >= 0.0f && x(mapped) <= 1.0f, L"R in [0,1]", LINE_INFO());
            Assert::IsTrue(y(mapped) >= 0.0f && y(mapped) <= 1.0f, L"G in [0,1]", LINE_INFO());
            Assert::IsTrue(z(mapped) >= 0.0f && z(mapped) <= 1.0f, L"B in [0,1]", LINE_INFO());
        }

        TEST_METHOD(acesFilmicMonotone)
        {
            const auto low = Float4{0.1f, 0.1f, 0.1f, 0.0f};
            const auto mid = Float4{1.0f, 1.0f, 1.0f, 0.0f};
            const auto high = Float4{5.0f, 5.0f, 5.0f, 0.0f};

            const auto mLow = acesFilmic(low);
            const auto mMid = acesFilmic(mid);
            const auto mHigh = acesFilmic(high);

            Assert::IsTrue(x(mLow) < x(mMid), L"monotone low < mid", LINE_INFO());
            Assert::IsTrue(x(mMid) < x(mHigh), L"monotone mid < high", LINE_INFO());
        }

        TEST_METHOD(acesFilmicClampsNegativeInput)
        {
            const auto negative = Float4{-1.0f, -0.5f, -2.0f, 0.0f};
            const auto mapped = acesFilmic(negative);

            Assert::IsTrue(x(mapped) >= 0.0f, L"negative clamped", LINE_INFO());
        }

        TEST_METHOD(agxDarkerInputMapsLower)
        {
            const auto dark = Float4{0.001f, 0.001f, 0.001f, 0.0f};
            const auto mid = Float4{0.5f, 0.5f, 0.5f, 0.0f};
            const auto mappedDark = agx(dark);
            const auto mappedMid = agx(mid);

            Assert::IsTrue(x(mappedDark) < x(mappedMid), L"darker input maps lower", LINE_INFO());
        }

        TEST_METHOD(agxOutputInUnitRange)
        {
            const auto bright = Float4{10.0f, 5.0f, 2.0f, 0.0f};
            const auto mapped = agx(bright);

            Assert::IsTrue(x(mapped) >= 0.0f && x(mapped) <= 1.0f, L"R in [0,1]", LINE_INFO());
            Assert::IsTrue(y(mapped) >= 0.0f && y(mapped) <= 1.0f, L"G in [0,1]", LINE_INFO());
            Assert::IsTrue(z(mapped) >= 0.0f && z(mapped) <= 1.0f, L"B in [0,1]", LINE_INFO());
        }

        TEST_METHOD(agxMonotone)
        {
            const auto low = Float4{0.1f, 0.1f, 0.1f, 0.0f};
            const auto mid = Float4{1.0f, 1.0f, 1.0f, 0.0f};
            const auto high = Float4{5.0f, 5.0f, 5.0f, 0.0f};

            const auto mLow = agx(low);
            const auto mMid = agx(mid);
            const auto mHigh = agx(high);

            Assert::IsTrue(x(mLow) < x(mMid), L"monotone low < mid", LINE_INFO());
            Assert::IsTrue(x(mMid) < x(mHigh), L"monotone mid < high", LINE_INFO());
        }

        TEST_METHOD(agxPreservesKeyGray)
        {
            const auto keyGray = Float4{0.18f, 0.18f, 0.18f, 0.0f};
            const auto mapped = agx(keyGray);

            Assert::AreEqual(x(mapped), y(mapped), 1e-5f, L"achromatic input stays achromatic", LINE_INFO());
            Assert::AreEqual(0.18f, x(mapped), 0.1f, L"key gray maps near 0.18", LINE_INFO());
        }

        TEST_METHOD(agxWhiteIsNearWhite)
        {
            const auto white = Float4{1.0f, 1.0f, 1.0f, 0.0f};
            const auto mapped = agx(white);

            Assert::IsTrue(x(mapped) > 0.4f, L"white maps above mid", LINE_INFO());
            Assert::IsTrue(y(mapped) > 0.4f, L"white maps above mid", LINE_INFO());
            Assert::IsTrue(z(mapped) > 0.4f, L"white maps above mid", LINE_INFO());
        }
    };
}
