#include "raytracing_tests.h"

#include <raytracing/shading/EmitterSPD.h>
#include <raytracing/shading/LightInfo.h>
#include <raytracing/shading/spectral/conversion.h>
#include <raytracing/shading/spectral/spectrum.h>
#include <raytracing/shading/spectral/wavelengths.h>

#include <vectorization.h>

namespace raytracer::test
{
    // Photopic Y matches spectrumToXYZ's Y convention: Σ(SPD·ȳ) / Σ(ȳ).
    static Float computeY(const spectral::Spectrum& spectrum, const Float heroLambda)
    {
        return y(spectral::spectrumToXYZ(spectrum, spectral::makeXYZContext(heroLambda)));
    }

    TEST_CLASS (LightInfoTest)
    {
        public
        :
        TEST_METHOD(defaultConstructsAsRGBKind)
        {
            const LightInfo light;
            Assert::IsTrue(light.kind == EmitterKind::RGB, L"default kind", LINE_INFO());
            Assert::AreEqual(1.0f, light.intensity, L"default intensity", LINE_INFO());
            Assert::AreEqual(0.0f, light.temperatureKelvin, L"default temperatureKelvin", LINE_INFO());
        }

        TEST_METHOD(blackbodyFactorySetsFields)
        {
            const auto light = LightInfo::blackbody(3000.0f, 1.0f);
            Assert::IsTrue(light.kind == EmitterKind::Blackbody, L"kind", LINE_INFO());
            Assert::AreEqual(3000.0f, light.temperatureKelvin, L"temperature", LINE_INFO());
            Assert::IsTrue(light.intensity > 0.0f, L"normalized intensity > 0", LINE_INFO());
        }

        TEST_METHOD(d65FactorySetsKind)
        {
            const auto light = LightInfo::d65(1.0f);
            Assert::IsTrue(light.kind == EmitterKind::D65, L"kind", LINE_INFO());
            Assert::IsTrue(light.intensity > 0.0f, L"intensity > 0", LINE_INFO());
        }

        TEST_METHOD(illuminantAFactorySetsKind)
        {
            const auto light = LightInfo::illuminantA(1.0f);
            Assert::IsTrue(light.kind == EmitterKind::IlluminantA, L"kind", LINE_INFO());
            Assert::IsTrue(light.intensity > 0.0f, L"intensity > 0", LINE_INFO());
        }

        // brightness=1 should yield Y=1 across the visible band, regardless of color.
        TEST_METHOD(blackbody3000KAtBrightness1YieldsUnitY)
        {
            const auto light = LightInfo::blackbody(3000.0f, 1.0f);
            const auto wavelengths = spectral::wavelengthsAt(spectral::LAMBDA_MIN);
            const auto spd = sampleEmitter(light, wavelengths);
            Assert::AreEqual(1.0f, computeY(spd, spectral::LAMBDA_MIN), 1e-3f,
                             L"Y == 1 at brightness=1", LINE_INFO());
        }

        TEST_METHOD(blackbody9500KAtBrightness1YieldsUnitY)
        {
            const auto light = LightInfo::blackbody(9500.0f, 1.0f);
            const auto wavelengths = spectral::wavelengthsAt(spectral::LAMBDA_MIN);
            const auto spd = sampleEmitter(light, wavelengths);
            Assert::AreEqual(1.0f, computeY(spd, spectral::LAMBDA_MIN), 1e-3f,
                             L"Y == 1 at brightness=1", LINE_INFO());
        }

        TEST_METHOD(d65AtBrightness1YieldsUnitY)
        {
            const auto light = LightInfo::d65(1.0f);
            const auto wavelengths = spectral::wavelengthsAt(spectral::LAMBDA_MIN);
            const auto spd = sampleEmitter(light, wavelengths);
            Assert::AreEqual(1.0f, computeY(spd, spectral::LAMBDA_MIN), 1e-3f,
                             L"Y == 1 at brightness=1", LINE_INFO());
        }

        TEST_METHOD(illuminantAAtBrightness1YieldsUnitY)
        {
            const auto light = LightInfo::illuminantA(1.0f);
            const auto wavelengths = spectral::wavelengthsAt(spectral::LAMBDA_MIN);
            const auto spd = sampleEmitter(light, wavelengths);
            Assert::AreEqual(1.0f, computeY(spd, spectral::LAMBDA_MIN), 1e-3f,
                             L"Y == 1 at brightness=1", LINE_INFO());
        }

        // Two different emitter kinds at the same brightness should produce the same Y —
        // that is the calibration's whole point.
        TEST_METHOD(equalBrightnessProducesEqualYAcrossKinds)
        {
            const auto wavelengths = spectral::wavelengthsAt(spectral::LAMBDA_MIN);
            const auto bbCool = LightInfo::blackbody(9500.0f, 0.5f);
            const auto bbWarm = LightInfo::blackbody(3000.0f, 0.5f);
            const auto d65 = LightInfo::d65(0.5f);
            const auto a = LightInfo::illuminantA(0.5f);

            const auto yCool = computeY(sampleEmitter(bbCool, wavelengths), spectral::LAMBDA_MIN);
            const auto yWarm = computeY(sampleEmitter(bbWarm, wavelengths), spectral::LAMBDA_MIN);
            const auto yD65 = computeY(sampleEmitter(d65, wavelengths), spectral::LAMBDA_MIN);
            const auto yA = computeY(sampleEmitter(a, wavelengths), spectral::LAMBDA_MIN);

            Assert::AreEqual(yCool, yWarm, 1e-3f, L"3000K Y == 9500K Y at same brightness", LINE_INFO());
            Assert::AreEqual(yCool, yD65, 1e-3f, L"blackbody Y == D65 Y at same brightness", LINE_INFO());
            Assert::AreEqual(yCool, yA, 1e-3f, L"blackbody Y == illuminantA Y at same brightness", LINE_INFO());
        }

        // brightness scales the SPD linearly: doubling brightness doubles Y.
        TEST_METHOD(brightnessScalesYLinearly)
        {
            const auto wavelengths = spectral::wavelengthsAt(spectral::LAMBDA_MIN);
            const auto light1 = LightInfo::blackbody(5500.0f, 1.0f);
            const auto light2 = LightInfo::blackbody(5500.0f, 2.0f);
            const auto y1 = computeY(sampleEmitter(light1, wavelengths), spectral::LAMBDA_MIN);
            const auto y2 = computeY(sampleEmitter(light2, wavelengths), spectral::LAMBDA_MIN);
            Assert::AreEqual(2.0f * y1, y2, 1e-3f, L"Y(2x) == 2 * Y(1x)", LINE_INFO());
        }

        // Calibration should be hero-lambda-independent for a given factory result —
        // brightness=1 means Y=1 regardless of which 8 wavelengths the renderer samples.
        TEST_METHOD(d65YIsHeroLambdaIndependent)
        {
            const auto light = LightInfo::d65(1.0f);
            const Float hero1 = spectral::LAMBDA_MIN;
            const Float hero2 = 555.0f;
            const auto spd1 = sampleEmitter(light, spectral::wavelengthsAt(hero1));
            const auto spd2 = sampleEmitter(light, spectral::wavelengthsAt(hero2));
            const auto y1 = computeY(spd1, hero1);
            const auto y2 = computeY(spd2, hero2);
            Assert::AreEqual(y1, y2, 0.05f, L"D65 Y stable across hero shifts", LINE_INFO());
        }

        // The blackbody chromaticity should shift with temperature: warmer 3000K should
        // produce a redder (higher X/Y, lower Z/Y) chromaticity than cooler 9500K.
        TEST_METHOD(blackbodyTemperatureShiftsChromaticity)
        {
            const auto wavelengths = spectral::wavelengthsAt(spectral::LAMBDA_MIN);
            const auto warm = sampleEmitter(LightInfo::blackbody(3000.0f, 1.0f), wavelengths);
            const auto cool = sampleEmitter(LightInfo::blackbody(9500.0f, 1.0f), wavelengths);
            const auto warmXYZ = spectral::spectrumToXYZ(warm, spectral::makeXYZContext(spectral::LAMBDA_MIN));
            const auto coolXYZ = spectral::spectrumToXYZ(cool, spectral::makeXYZContext(spectral::LAMBDA_MIN));
            // X/Y warmer (more red), Z/Y colder (less blue) at low T; opposite at high T.
            Assert::IsTrue(x(warmXYZ) / y(warmXYZ) > x(coolXYZ) / y(coolXYZ),
                           L"3000K x-chromaticity > 9500K", LINE_INFO());
            Assert::IsTrue(z(warmXYZ) / y(warmXYZ) < z(coolXYZ) / y(coolXYZ),
                           L"3000K z-chromaticity < 9500K", LINE_INFO());
        }
    };
}
