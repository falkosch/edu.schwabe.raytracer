#include "raytracing_tests.h"

#include <raytracing/shading/brdf/apply_brdf.h>
#include <raytracing/shading/spectral/conversion.h>
#include <raytracing/shading/spectral/spectrum.h>
#include <raytracing/shading/spectral/wavelengths.h>

#include <vectorization.h>

namespace raytracer::test
{
    using namespace spectral;

    // brdf::applyBRDF on a hit with the given surface, lighting and Beer-Lambert fraction
    static Spectrum applyBRDFFormula(const SurfaceShading& surface, const LightShading& lighting,
                                     const Spectrum& fractionTransmitted)
    {
        BRDFParameters parameters;
        parameters.surface = surface;
        parameters.lighting = lighting;
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
            const auto result = applyBRDFFormula(surface, lighting, Spectrum::zero());

            Assert::AreEqual(0.7f, x1(result.data), 1e-6f, L"emittance lane 0", LINE_INFO());
            Assert::AreEqual(0.7f, x4(result.data), 1e-6f, L"emittance lane 3", LINE_INFO());
        }

        TEST_METHOD(diffusionScalesAmbient)
        {
            SurfaceShading surface;
            surface.diffusion = Spectrum::constant(0.5f);
            LightShading lighting{Spectrum::constant(0.4f)};
            const auto result = applyBRDFFormula(surface, lighting, Spectrum::zero());

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

            const auto halfResult = applyBRDFFormula(halfSurface, light, Spectrum::zero());
            const auto fullResult = applyBRDFFormula(fullSurface, light, Spectrum::zero());

            Assert::AreEqual(x1(fullResult.data) * 0.5f, x1(halfResult.data), 1e-6f,
                             L"half diffusion = half output", LINE_INFO());
        }

        TEST_METHOD(reflectedAndTransmittedLightAdd)
        {
            // both carry their Fresnel weights already (the specular lobe's reflectance and what it leaves)
            SurfaceShading surface;
            LightShading lighting;
            lighting.reflected = Spectrum::constant(0.5f);
            lighting.transmitted = Spectrum::constant(0.3f);
            const auto result = applyBRDFFormula(surface, lighting, Spectrum::constant(1.0f));

            Assert::AreEqual(0.8f, x1(result.data), 1e-6f, L"0.5 + 0.3", LINE_INFO());
        }

        TEST_METHOD(reflectanceDoesNotScaleTheReflectedLightAgain)
        {
            // the reflectance channel weighs the specular lobe (brdf::SpecularFresnel), which weighed the reflection
            SurfaceShading surface;
            surface.reflectance = Spectrum::constant(0.5f);
            LightShading lighting;
            lighting.reflected = Spectrum::constant(0.4f);
            const auto result = applyBRDFFormula(surface, lighting, Spectrum::zero());

            Assert::AreEqual(0.4f, x1(result.data), 1e-6f, L"reflected as it is", LINE_INFO());
        }

        TEST_METHOD(diffuseLightIsScaledByDiffusionOverPi)
        {
            // Lambert's BRDF diffusion / pi times the irradiance
            SurfaceShading surface;
            surface.diffusion = Spectrum::constant(0.5f);
            LightShading lighting;
            lighting.diffuse = Spectrum::constant(0.6f);
            const auto result = applyBRDFFormula(surface, lighting, Spectrum::zero());

            Assert::AreEqual(0.3f / Pi<Float>(), x1(result.data), 1e-6f, L"0.5 * 0.6 / pi", LINE_INFO());
        }

        TEST_METHOD(whiteSurfaceReflectsTheRadianceAroundIt)
        {
            // radiance 0.7 from the whole hemisphere: the irradiance 0.7 pi as direct light, or 0.7 as ambient light;
            // a white Lambert surface reflects both as radiance 0.7 (white furnace)
            SurfaceShading surface;
            surface.diffusion = Spectrum::constant(1.0f);
            LightShading direct;
            direct.diffuse = Spectrum::constant(0.7f * Pi<Float>());
            const auto ambient = LightShading{Spectrum::constant(0.7f)};

            const auto fromDirect = applyBRDFFormula(surface, direct, Spectrum::zero());
            const auto fromAmbient = applyBRDFFormula(surface, ambient, Spectrum::zero());
            Assert::AreEqual(0.7f, x1(fromDirect.data), 1e-6f, L"direct light", LINE_INFO());
            Assert::AreEqual(0.7f, x1(fromAmbient.data), 1e-6f, L"ambient light", LINE_INFO());
        }

        TEST_METHOD(specularIsAddedUnscaled)
        {
            SurfaceShading surface;
            surface.diffusion = Spectrum::constant(0.5f);
            surface.reflectance = Spectrum::constant(0.5f);
            LightShading lighting;
            lighting.specular = Spectrum::constant(0.25f);
            const auto result = applyBRDFFormula(surface, lighting, Spectrum::zero());

            Assert::AreEqual(0.25f, x1(result.data), 1e-6f, L"highlights carry their Fresnel already", LINE_INFO());
        }

        TEST_METHOD(fractionTransmittedAbsorbsTransmission)
        {
            SurfaceShading surface;
            LightShading lighting;
            lighting.transmitted = Spectrum::constant(0.8f);
            const auto result = applyBRDFFormula(surface, lighting, Spectrum::constant(0.25f));

            Assert::AreEqual(0.2f, x1(result.data), 1e-6f, L"0.8 * 0.25 = 0.2", LINE_INFO());
        }

        TEST_METHOD(allZeroProducesZero)
        {
            SurfaceShading surface;
            LightShading lighting;
            const auto result = applyBRDFFormula(surface, lighting, Spectrum::zero());

            Assert::AreEqual(0.0f, x1(result.data), L"zero lane 0", LINE_INFO());
            Assert::AreEqual(0.0f, x4(result.data), L"zero lane 3", LINE_INFO());
        }
    };
}
