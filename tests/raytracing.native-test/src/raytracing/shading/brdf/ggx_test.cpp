#include "raytracing_tests.h"
#include "raytracing/shading/brdf/ggx.h"

#include <cmath>
#include <random>

namespace raytracer::test
{
    using namespace raytracer::brdf;

    static constexpr Float PI = 3.14159265358979323846f;

    TEST_CLASS(GGXTest)
    {
    public:
        TEST_METHOD(ggxDNormalizesAtNoHOne)
        {
            const auto d = ggxD(1.0f, 0.5f);
            const auto expected = 1.0f / (PI * 0.25f);
            Assert::AreEqual(expected, d, 1e-5f, L"D at NoH=1", LINE_INFO());
        }

        TEST_METHOD(ggxDPeakDecreasesWithRoughness)
        {
            const auto dSharp = ggxD(1.0f, 0.1f);
            const auto dRough = ggxD(1.0f, 0.9f);
            Assert::IsTrue(dSharp > dRough, L"sharper peak at low alpha", LINE_INFO());
        }

        TEST_METHOD(ggxDNonNegative)
        {
            for (Float NoH = 0.0f; NoH <= 1.0f; NoH += 0.1f)
            {
                const auto d = ggxD(NoH, 0.5f);
                Assert::IsTrue(d >= 0.0f, L"D always non-negative", LINE_INFO());
            }
        }

        TEST_METHOD(ggxGNonNegative)
        {
            for (Float a = 0.1f; a <= 1.0f; a += 0.1f)
            {
                const auto g = ggxG(0.5f, 0.5f, a);
                Assert::IsTrue(g >= 0.0f, L"G always non-negative", LINE_INFO());
            }
        }

        TEST_METHOD(ggxGMaxAtGrazingIsBounded)
        {
            const auto g = ggxG(1.0f, 1.0f, 0.5f);
            Assert::IsTrue(g <= 1.0f + 1e-5f, L"G bounded by 1", LINE_INFO());
        }

        TEST_METHOD(ggxGZeroWhenLBelowHorizon)
        {
            const auto g = ggxG(0.5f, 0.0f, 0.5f);
            Assert::AreEqual(0.0f, g, 1e-7f, L"G=0 when NoL=0", LINE_INFO());
        }

        TEST_METHOD(evaluateGGXNonNegative)
        {
            const auto N = Float4{0.0f, 1.0f, 0.0f, 0.0f};
            const auto V = normalize3(Float4{0.0f, 1.0f, 0.3f, 0.0f});
            const auto L = normalize3(Float4{0.3f, 1.0f, -0.2f, 0.0f});
            const auto F0 = Float4{0.04f, 0.04f, 0.04f, 0.0f};

            const auto brdf = evaluateGGX(N, V, L, F0, 0.5f);

            Assert::IsTrue(x(brdf) >= 0.0f, L"BRDF R >= 0", LINE_INFO());
            Assert::IsTrue(y(brdf) >= 0.0f, L"BRDF G >= 0", LINE_INFO());
            Assert::IsTrue(z(brdf) >= 0.0f, L"BRDF B >= 0", LINE_INFO());
        }

        TEST_METHOD(evaluateGGXZeroBelowHorizon)
        {
            const auto N = Float4{0.0f, 1.0f, 0.0f, 0.0f};
            const auto V = normalize3(Float4{0.0f, 1.0f, 0.0f, 0.0f});
            const auto L = normalize3(Float4{0.0f, -1.0f, 0.0f, 0.0f});
            const auto F0 = Float4{0.04f, 0.04f, 0.04f, 0.0f};

            const auto brdf = evaluateGGX(N, V, L, F0, 0.5f);

            Assert::AreEqual(0.0f, x(brdf), 1e-7f, L"zero below horizon", LINE_INFO());
        }

        TEST_METHOD(ggxEnergyConservation)
        {
            const auto N = Float4{0.0f, 1.0f, 0.0f, 0.0f};
            const auto V = normalize3(Float4{0.0f, 1.0f, 0.3f, 0.0f});
            const auto F0 = Float4{1.0f, 1.0f, 1.0f, 0.0f};
            const Float alpha = 0.5f;

            std::mt19937 rng(42);
            std::uniform_real_distribution<Float> dist(0.0f, 1.0f);

            Float sum = 0.0f;
            constexpr int N_SAMPLES = 4096;
            for (int i = 0; i < N_SAMPLES; ++i)
            {
                const auto u1 = dist(rng);
                const auto u2 = dist(rng);
                const auto theta = std::acos(std::sqrt(1.0f - u1));
                const auto phi = 2.0f * PI * u2;
                const auto L = Float4{
                    std::sin(theta) * std::cos(phi), std::cos(theta), std::sin(theta) * std::sin(phi), 0.0f
                };
                const auto NoL = std::max(dot3(N, L), 0.0f);
                if (NoL <= 0.0f)
                    continue;

                const auto brdf = evaluateGGX(N, V, L, F0, alpha);
                sum += x(brdf) * NoL * 2.0f * PI;
            }
            sum /= static_cast<Float>(N_SAMPLES);

            Assert::IsTrue(sum <= 1.0f + 0.05f, L"energy conservation", LINE_INFO());
        }

        TEST_METHOD(sampleGGXProducesValidDirections)
        {
            const auto N = Float4{0.0f, 1.0f, 0.0f, 0.0f};
            const auto V = normalize3(Float4{0.0f, 1.0f, 0.3f, 0.0f});

            std::mt19937 rng(123);
            std::uniform_real_distribution<Float> dist(0.0f, 1.0f);

            for (int i = 0; i < 100; ++i)
            {
                const auto xi = Float4{dist(rng), dist(rng), 0.0f, 0.0f};
                const auto L = sampleGGX(N, V, xi, 0.5f);
                const auto len = length3(L);

                Assert::AreEqual(1.0f, len, 1e-5f, L"sampled direction is unit length", LINE_INFO());
            }
        }
    };
}
