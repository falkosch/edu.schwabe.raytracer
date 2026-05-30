#include "raytracing_tests.h"
#include "raytracing/shading/color/primaries.h"

namespace raytracer::test
{
    using namespace raytracer::color;

    static constexpr Float D65_x = 0.3127f;
    static constexpr Float D65_y = 0.3290f;
    static constexpr Float D50_x = 0.3457f;
    static constexpr Float D50_y = 0.3585f;

    TEST_CLASS(PrimariesTest)
    {
    public:
        TEST_METHOD(sRGBToXYZRoundTrip)
        {
            const auto rgb = Float4{0.5f, 0.3f, 0.8f, 0.0f};
            const auto xyz = sRGBLinearToXYZ() * rgb;
            const auto recovered = XYZToSRGBLinear() * xyz;

            Assert::AreEqual(x(rgb), x(recovered), 1e-5f, L"R round-trip", LINE_INFO());
            Assert::AreEqual(y(rgb), y(recovered), 1e-5f, L"G round-trip", LINE_INFO());
            Assert::AreEqual(z(rgb), z(recovered), 1e-5f, L"B round-trip", LINE_INFO());
        }

        TEST_METHOD(Rec2020ToXYZRoundTrip)
        {
            const auto rgb = Float4{0.6f, 0.1f, 0.9f, 0.0f};
            const auto xyz = Rec2020LinearToXYZ() * rgb;
            const auto recovered = XYZToRec2020Linear() * xyz;

            Assert::AreEqual(x(rgb), x(recovered), 1e-5f, L"R round-trip", LINE_INFO());
            Assert::AreEqual(y(rgb), y(recovered), 1e-5f, L"G round-trip", LINE_INFO());
            Assert::AreEqual(z(rgb), z(recovered), 1e-5f, L"B round-trip", LINE_INFO());
        }

        TEST_METHOD(sRGBWhitePointMapsToD65Luminance)
        {
            const auto white = Float4{1.0f, 1.0f, 1.0f, 0.0f};
            const auto xyz = sRGBLinearToXYZ() * white;

            Assert::AreEqual(1.0f, y(xyz), 1e-4f, L"Y luminance for white", LINE_INFO());
        }

        TEST_METHOD(chromaticAdaptationD65ToD50RoundTrip)
        {
            const auto d65ToD50 = chromaticAdaptation(D65_x, D65_y, D50_x, D50_y);
            const auto d50ToD65 = chromaticAdaptation(D50_x, D50_y, D65_x, D65_y);
            const auto roundTrip = d50ToD65 * d65ToD50;

            const auto testXYZ = Float4{0.5f, 0.6f, 0.7f, 0.0f};
            const auto result = roundTrip * testXYZ;

            Assert::AreEqual(x(testXYZ), x(result), 1e-4f, L"X idempotent", LINE_INFO());
            Assert::AreEqual(y(testXYZ), y(result), 1e-4f, L"Y idempotent", LINE_INFO());
            Assert::AreEqual(z(testXYZ), z(result), 1e-4f, L"Z idempotent", LINE_INFO());
        }

        TEST_METHOD(chromaticAdaptationIdentityWhenSameWhitePoint)
        {
            const auto identity = chromaticAdaptation(D65_x, D65_y, D65_x, D65_y);
            const auto testXYZ = Float4{0.3f, 0.5f, 0.8f, 0.0f};
            const auto result = identity * testXYZ;

            Assert::AreEqual(x(testXYZ), x(result), 1e-5f, L"X unchanged", LINE_INFO());
            Assert::AreEqual(y(testXYZ), y(result), 1e-5f, L"Y unchanged", LINE_INFO());
            Assert::AreEqual(z(testXYZ), z(result), 1e-5f, L"Z unchanged", LINE_INFO());
        }

        TEST_METHOD(wComponentUntouchedByColorSpaceTransform)
        {
            const auto rgb = Float4{0.5f, 0.3f, 0.8f, 0.0f};
            const auto xyz = sRGBLinearToXYZ() * rgb;

            Assert::AreEqual(0.0f, w(xyz), 1e-7f, L"W must stay zero", LINE_INFO());
        }
    };
}
