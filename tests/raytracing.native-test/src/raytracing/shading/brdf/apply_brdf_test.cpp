#include "raytracing_tests.h"

#include <raytracing/shading/brdf/apply_brdf.h>
#include <raytracing/shading/spectral/conversion.h>
#include <raytracing/shading/spectral/spectrum.h>
#include <raytracing/shading/spectral/wavelengths.h>

#include <vectorization.h>

namespace raytracer::test
{
    using namespace spectral;

    // brdf::applyBRDF on a hit with the given surface, lighting and coefficients
    static Spectrum applyBRDFFormula(const SurfaceShading& surface, const LightShading& lighting,
                                     const Spectrum& reflectanceCoefficient, const Spectrum& fractionTransmitted)
    {
        BRDFParameters parameters;
        parameters.surface = surface;
        parameters.lighting = lighting;
        parameters.reflectanceCoefficient = reflectanceCoefficient.data;
        parameters.fractionTransmitted = fractionTransmitted.data;
        return brdf::applyBRDF(parameters);
    }

    TEST_CLASS (ApplyBRDFTest)
    {
        public
        :
        TEST_METHOD(emittancePassesThrough)
        {
            SurfaceShading surface;
            surface.emittance = Spectrum::constant(0.7f);
            LightShading lighting;
            const auto result = applyBRDFFormula(surface, lighting, Spectrum::zero(), Spectrum::zero());

            Assert::AreEqual(0.7f, x1(result.data), 1e-6f, L"emittance lane 0", LINE_INFO());
            Assert::AreEqual(0.7f, x4(result.data), 1e-6f, L"emittance lane 3", LINE_INFO());
        }

        TEST_METHOD(diffusionScalesAmbient)
        {
            SurfaceShading surface;
            surface.diffusion = Spectrum::constant(0.5f);
            LightShading lighting{Spectrum::constant(0.4f)};
            const auto result = applyBRDFFormula(surface, lighting, Spectrum::zero(), Spectrum::zero());

            Assert::AreEqual(0.2f, x1(result.data), 1e-6f, L"0.5 * 0.4 = 0.2", LINE_INFO());
        }

        TEST_METHOD(diffusionHalfProducesHalfOutput)
        {
            const auto halfSurface = SurfaceShading{
                Spectrum::constant(0.5f), {}, {}, {}, {}, {}, {}
            };
            const auto fullSurface = SurfaceShading{
                Spectrum::constant(1.0f), {}, {}, {}, {}, {}, {}
            };
            const auto light = LightShading{Spectrum::constant(1.0f)};

            const auto halfResult = applyBRDFFormula(halfSurface, light, Spectrum::zero(), Spectrum::zero());
            const auto fullResult = applyBRDFFormula(fullSurface, light, Spectrum::zero(), Spectrum::zero());

            Assert::AreEqual(x1(fullResult.data) * 0.5f, x1(halfResult.data), 1e-6f,
                             L"half diffusion = half output", LINE_INFO());
        }

        TEST_METHOD(reflectionCoefficientOnePicksReflection)
        {
            SurfaceShading surface;
            surface.reflectance = Spectrum::constant(1.0f);
            LightShading lighting;
            lighting.reflected = Spectrum::constant(0.5f);
            lighting.transmitted = Spectrum::constant(0.9f);
            const auto result = applyBRDFFormula(surface, lighting,
                                                 Spectrum::constant(1.0f), Spectrum::constant(1.0f));

            Assert::AreEqual(0.5f, x1(result.data), 1e-5f, L"reflection = reflectance * reflected", LINE_INFO());
        }

        TEST_METHOD(reflectionCoefficientZeroPicksTransmission)
        {
            SurfaceShading surface;
            LightShading lighting;
            lighting.reflected = Spectrum::constant(0.5f);
            lighting.transmitted = Spectrum::constant(0.9f);
            const auto result = applyBRDFFormula(surface, lighting,
                                                 Spectrum::zero(), Spectrum::constant(1.0f));

            Assert::AreEqual(0.9f, x1(result.data), 1e-5f, L"transmission at coef=0", LINE_INFO());
        }

        TEST_METHOD(diffuseLightIsScaledByDiffusion)
        {
            SurfaceShading surface;
            surface.diffusion = Spectrum::constant(0.5f);
            LightShading lighting;
            lighting.diffuse = Spectrum::constant(0.6f);
            const auto result = applyBRDFFormula(surface, lighting, Spectrum::zero(), Spectrum::zero());

            Assert::AreEqual(0.3f, x1(result.data), 1e-6f, L"0.5 * 0.6 = 0.3", LINE_INFO());
        }

        TEST_METHOD(specularIsAddedUnscaled)
        {
            SurfaceShading surface;
            surface.diffusion = Spectrum::constant(0.5f);
            surface.reflectance = Spectrum::constant(0.5f);
            LightShading lighting;
            lighting.specular = Spectrum::constant(0.25f);
            const auto result = applyBRDFFormula(surface, lighting, Spectrum::zero(), Spectrum::zero());

            Assert::AreEqual(0.25f, x1(result.data), 1e-6f, L"highlights carry their Fresnel already", LINE_INFO());
        }

        TEST_METHOD(fractionTransmittedAbsorbsTransmission)
        {
            SurfaceShading surface;
            LightShading lighting;
            lighting.transmitted = Spectrum::constant(0.8f);
            const auto result = applyBRDFFormula(surface, lighting, Spectrum::zero(), Spectrum::constant(0.25f));

            Assert::AreEqual(0.2f, x1(result.data), 1e-6f, L"0.8 * 0.25 = 0.2", LINE_INFO());
        }

        TEST_METHOD(reflectanceCoefficientMixesPerWavelength)
        {
            SurfaceShading surface;
            surface.reflectance = Spectrum::constant(1.0f);
            LightShading lighting;
            lighting.reflected = Spectrum::constant(1.0f);
            lighting.transmitted = Spectrum::constant(0.0f);
            const auto coefficient = Spectrum{Float8{0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 0.1f, 0.2f, 0.3f}};
            const auto result = applyBRDFFormula(surface, lighting, coefficient, Spectrum::constant(1.0f));

            Assert::AreEqual(0.0f, x1(result.data), 1e-6f, L"lane 0", LINE_INFO());
            Assert::AreEqual(0.5f, x3(result.data), 1e-6f, L"lane 2", LINE_INFO());
            Assert::AreEqual(1.0f, x5(result.data), 1e-6f, L"lane 4", LINE_INFO());
            Assert::AreEqual(0.3f, x8(result.data), 1e-6f, L"lane 7", LINE_INFO());
        }

        TEST_METHOD(allZeroProducesZero)
        {
            SurfaceShading surface;
            LightShading lighting;
            const auto result = applyBRDFFormula(surface, lighting, Spectrum::zero(), Spectrum::zero());

            Assert::AreEqual(0.0f, x1(result.data), L"zero lane 0", LINE_INFO());
            Assert::AreEqual(0.0f, x4(result.data), L"zero lane 3", LINE_INFO());
        }
    };
}
