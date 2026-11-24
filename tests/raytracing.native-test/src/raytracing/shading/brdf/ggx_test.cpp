#include "raytracing_tests.h"
#include "raytracing/shading/brdf/ggx.h"

#include <cmath>
#include <random>

namespace raytracer::test
{
    using namespace raytracer::brdf;

    static constexpr Float PI = 3.14159265358979323846f;

    TEST_CLASS (GGXTest)
    {
        public
        :
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

        TEST_METHOD(ggxG_smithNonNegative)
        {
            for (Float a = 0.1f; a <= 1.0f; a += 0.1f)
            {
                const auto g = ggxG_smith(0.5f, 0.5f, a);
                Assert::IsTrue(g >= 0.0f, L"G always non-negative", LINE_INFO());
            }
        }

        TEST_METHOD(ggxG_smithMaxAtGrazingIsBounded)
        {
            const auto g = ggxG_smith(1.0f, 1.0f, 0.5f);
            Assert::IsTrue(g <= 1.0f + 1e-5f, L"G bounded by 1", LINE_INFO());
        }

        TEST_METHOD(ggxG_smithZeroWhenLBelowHorizon)
        {
            const auto g = ggxG_smith(0.5f, 0.0f, 0.5f);
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

        TEST_METHOD(pdfGGXIsTheDensityOfSampleGGX)
        {
            // the share of sampled directions above the surface is the density's integral over the hemisphere
            const Float4 N{0.0f, 1.0f, 0.0f, 0.0f};
            const auto V = normalize3(Float4{0.0f, 0.5f, 0.866f, 0.0f});
            const auto alpha = 0.5f;
            std::mt19937 rng(11);
            std::uniform_real_distribution<Float> uniform(0.0f, 1.0f);
            constexpr int Samples = 1 << 16;
            auto above = 0;
            for (auto i = 0; i < Samples; ++i)
            {
                const auto L = sampleGGX(N, V, Float4{uniform(rng), uniform(rng), 0.0f, 0.0f}, alpha);
                above += dot3(N, L) > 0.0f ? 1 : 0;
            }

            constexpr int Steps = 600;
            auto integral = 0.0;
            for (auto i = 0; i < Steps; ++i)
            {
                const auto theta = 0.5 * PI * (i + 0.5) / Steps;
                for (auto j = 0; j < 2 * Steps; ++j)
                {
                    const auto phi = PI * (j + 0.5) / Steps;
                    const auto L = Float4{
                        static_cast<Float>(std::sin(theta) * std::cos(phi)), static_cast<Float>(std::cos(theta)),
                        static_cast<Float>(std::sin(theta) * std::sin(phi)), 0.0f
                    };
                    const auto H = normalize3(V + L);
                    integral += static_cast<double>(pdfGGX(dot3(N, V), dot3(N, H), alpha)) * std::sin(theta);
                }
            }
            integral *= (0.5 * PI / Steps) * (PI / Steps);
            Assert::AreEqual(static_cast<double>(above) / Samples, integral, 0.01, L"density of the samples",
                             LINE_INFO());
            Assert::IsTrue(integral < 1.0, L"some directions go below the surface", LINE_INFO());
        }

        TEST_METHOD(sampleGGXReflectsAboutTheSampledNormal)
        {
            const Float4 N{0.0f, 1.0f, 0.0f, 0.0f};
            const auto V = normalize3(Float4{0.3f, 0.8f, 0.2f, 0.0f});
            for (const auto x : {0.1f, 0.5f, 0.9f})
            {
                const auto xi = Float4{x, 1.0f - x, 0.0f, 0.0f};
                const auto H = sampleGGXNormal(N, V, xi, 0.4f);
                Assert::AreEqual(1.0f, length3(H), 1e-5f, L"a unit normal", LINE_INFO());
                Assert::IsTrue(dot3(H, V) > 0.0f, L"visible from V", LINE_INFO());
                const auto L = sampleGGX(N, V, xi, 0.4f);
                const auto mirrored = reflect(-V, H);
                Assert::AreEqual(0.0f, length3(L - mirrored), 1e-5f, L"the mirrored direction", LINE_INFO());
            }
        }

        TEST_METHOD(roughRefractionSpreadsAroundTheMirrorRefraction)
        {
            // glass entered at 45 degrees: through the sampled microfacet normals the refracted directions gather around
            // the smooth surface's, the closer the smoother
            const Float4 N{0.0f, 1.0f, 0.0f, 0.0f};
            const auto incident = normalize3(Float4{1.0f, -1.0f, 0.0f, 0.0f});
            const auto eta = Float4{1.0f / 1.5f};
            const auto smooth = refractEta(incident, N, dotv(N, incident), eta);
            const auto meanCosine = [&](const Float alpha)
            {
                std::mt19937 rng(5);
                std::uniform_real_distribution<Float> uniform(0.0f, 1.0f);
                auto sum = 0.0f;
                for (auto i = 0; i < 256; ++i)
                {
                    const auto H = sampleGGXNormal(N, -incident, Float4{uniform(rng), uniform(rng), 0.0f, 0.0f}, alpha);
                    const auto refracted = refractEta(incident, H, dotv(H, incident), eta);
                    Assert::AreEqual(1.0f, length3(refracted), 1e-4f, L"no total reflection entering glass",
                                     LINE_INFO());
                    sum += dot3(refracted, smooth);
                }
                return sum / 256.0f;
            };
            const auto smoothSpread = meanCosine(0.05f);
            const auto roughSpread = meanCosine(0.5f);
            Assert::IsTrue(smoothSpread > 0.99f, L"close to the mirror refraction", LINE_INFO());
            Assert::IsTrue(roughSpread < smoothSpread - 0.02f, L"spread by the roughness", LINE_INFO());
        }

        TEST_METHOD(smithG1IsOneAtNormalIncidence)
        {
            Assert::AreEqual(1.0f, smithG1(1.0f, 0.5f), 1e-6f, L"no masking straight on", LINE_INFO());
            Assert::IsTrue(smithG1(0.1f, 0.5f) < smithG1(0.5f, 0.5f), L"more masking at grazing angles", LINE_INFO());
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

        TEST_METHOD(evaluateGGX_DGMatchesFullWithoutFresnel)
        {
            const auto N = Float4{0.0f, 1.0f, 0.0f, 0.0f};
            const auto V = normalize3(Float4{0.3f, 1.0f, 0.0f, 0.0f});
            const auto L = normalize3(Float4{-0.3f, 1.0f, 0.0f, 0.0f});
            const auto F0 = Float4{1.0f, 1.0f, 1.0f, 0.0f};

            const auto H = normalize3(V + L);
            const auto full = evaluateGGX(N, V, L, F0, 0.5f);
            const auto dg = evaluateGGX_DG(dot3(N, V), dot3(N, L), dot3(N, H), 0.5f);

            Assert::AreEqual(x(full), dg, 1e-5f, L"DG matches full when F0=1", LINE_INFO());
        }

        TEST_METHOD(ggxVSmithIsGeometryOverFourNoVNoL)
        {
            const Float cosines[] = {0.05f, 0.3f, 0.7f, 1.0f};
            const Float alphas[] = {0.1f, 0.5f, 1.0f};
            for (const auto alpha : alphas)
            {
                for (const auto NoV : cosines)
                {
                    for (const auto NoL : cosines)
                    {
                        const auto expected = ggxG_smith(NoV, NoL, alpha) / (4.0f * NoV * NoL);
                        Assert::AreEqual(expected, ggxV_smith(NoV, NoL, alpha), expected * 1e-5f,
                                         L"V = G / (4 NoV NoL)", LINE_INFO());
                    }
                }
            }
        }

        TEST_METHOD(evaluateGGX_DGZeroBelowHorizon)
        {
            const auto N = Float4{0.0f, 1.0f, 0.0f, 0.0f};
            const auto V = normalize3(Float4{0.0f, 1.0f, 0.0f, 0.0f});
            const auto L = normalize3(Float4{0.0f, -1.0f, 0.0f, 0.0f});
            const auto H = N; // V + L is zero here, so the half vector is undefined; it is unused below the horizon
            const auto NoL = max(dot3(N, L), 0.0f);

            Assert::AreEqual(0.0f, evaluateGGX_DG(dot3(N, V), NoL, dot3(N, H), 0.5f), L"zero below horizon",
                             LINE_INFO());
        }

        TEST_METHOD(schlickFresnelSpectralAtNormalIncidence)
        {
            const auto F0 = Float8(0.04f);
            const auto result = schlickFresnelSpectral(F0, One<Float8>());

            Assert::AreEqual(0.04f, x1(result), 1e-5f, L"F(0°) = F0", LINE_INFO());
        }

        TEST_METHOD(schlickFresnelSpectralAtGrazing)
        {
            const auto F0 = Float8(0.04f);
            const auto result = schlickFresnelSpectral(F0, Zero<Float8>());

            Assert::AreEqual(1.0f, x1(result), 1e-5f, L"F(90°) = 1", LINE_INFO());
        }
    };
}
