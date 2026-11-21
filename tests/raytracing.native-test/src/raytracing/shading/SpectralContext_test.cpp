#include "raytracing_tests.h"

#include <raytracing/shading/EmitterSPD.h>
#include <raytracing/shading/LightInfo.h>
#include <raytracing/shading/SpectralContext.h>
#include <raytracing/shading/spectral/conversion.h>
#include <raytracing/shading/spectral/spectrum.h>
#include <raytracing/shading/spectral/wavelengths.h>

#include <vectorization.h>

#include <cmath>
#include <memory>
#include <vector>

namespace raytracer::test
{
    // The context and the reference compute the same expressions along different call paths, which a compiler may
    // contract or fold differently (clang-cl: 1 ULP), so compare with a small relative tolerance.
    static void assertSpectraEqual(const Float8& expected, const Float8& actual, const wchar_t* message)
    {
        const Float e[] = {x1(expected), x2(expected), x3(expected), x4(expected),
                           x5(expected), x6(expected), x7(expected), x8(expected)};
        const Float a[] = {x1(actual), x2(actual), x3(actual), x4(actual),
                           x5(actual), x6(actual), x7(actual), x8(actual)};
        for (ASizeT i = 0; i < spectral::SPECTRAL_SAMPLES; ++i)
        {
            Assert::AreEqual(e[i], a[i], 1e-6f * max(1.0f, std::abs(e[i])), message, LINE_INFO());
        }
    }

    TEST_CLASS (SpectralContextTest)
    {
        public
        :
        TEST_METHOD(derivesWavelengthsAndXYZContextFromHeroLambda)
        {
            const SpectralContext context{437.0f};
            const auto expectedXYZ = spectral::makeXYZContext(437.0f);

            Assert::AreEqual(437.0f, context.heroLambda, L"hero wavelength", LINE_INFO());
            assertSpectraEqual(spectral::wavelengthsAt(437.0f), context.wavelengths, L"wavelengths");
            assertSpectraEqual(expectedXYZ.cmf.yBar, context.xyz.cmf.yBar, L"CMF at the wavelengths");
            Assert::AreEqual(expectedXYZ.rcpYBarSum, context.xyz.rcpYBarSum, L"Y normalization", LINE_INFO());
        }

        TEST_METHOD(withoutSceneLightingHasNoAmbientAndNoLights)
        {
            const SpectralContext context{437.0f};
            assertSpectraEqual(Zero<Float8>(), context.ambient.data, L"ambient is zero");
            Assert::IsTrue(context.lightEmittance.empty(), L"no light spectra", LINE_INFO());
        }

        TEST_METHOD(fromRGBMatchesSpectralFromRGB)
        {
            const SpectralContext context{512.0f};
            const auto rgb = Float4{0.8f, 0.3f, 0.1f, 0.0f};
            assertSpectraEqual(spectral::fromRGB(512.0f, rgb).data, context.fromRGB(rgb).data,
                               L"same upsampling as fromRGB");
        }

        TEST_METHOD(precomputesAmbientAndLightSpectra)
        {
            const auto ambientLight = RGBS{Float4{0.1f, 0.2f, 0.3f, 0.0f}};

            std::vector<std::unique_ptr<LightInfo>> lights;
            lights.push_back(std::make_unique<LightInfo>(LightInfo::blackbody(3000.0f, 2.0f)));
            lights.push_back(std::make_unique<LightInfo>(LightInfo::d65(1.0f)));
            lights.push_back(std::make_unique<LightInfo>());
            lights.back()->emittance = RGBS{Float4{0.9f, 0.6f, 0.2f, 0.0f}};

            const SpectralContext context{437.0f, ambientLight, lights};

            assertSpectraEqual(spectral::fromRGB(437.0f, ambientLight.value).data, context.ambient.data,
                               L"upsampled ambient light");
            Assert::AreEqual(lights.size(), context.lightEmittance.size(), L"one spectrum per light", LINE_INFO());
            for (ASizeT i = 0; i < lights.size(); ++i)
            {
                assertSpectraEqual(sampleEmitter(*lights[i], context.wavelengths).data,
                                   context.lightEmittance[i].data, L"light spectrum");
            }
        }
    };
}
