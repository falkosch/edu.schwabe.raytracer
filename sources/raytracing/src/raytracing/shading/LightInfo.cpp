#include "raytracing/shading/LightInfo.h"
#include "../../stdafx.h"

#include "raytracing/shading/spectral/blackbody.h"
#include "raytracing/shading/spectral/cmf.h"
#include "raytracing/shading/spectral/illuminants.h"
#include "raytracing/shading/spectral/wavelengths.h"

#include <cassert>

namespace raytracer
{
    // Photopic Y value matching spectrumToXYZ's convention (Σ(SPD·ȳ)/Σ(ȳ)). Used to
    // normalize SPDs so `brightness=X` lands the spectrum's Y at X — equivalent in
    // perceived luminance to RGB(X, X, X) under the same Smits-upsampled white.
    static Float photopicLuminance(const Float8& spd) noexcept
    {
        const auto wavelengths = spectral::wavelengthsAt(spectral::LAMBDA_MIN);
        const auto cmf = spectral::cmfAt(wavelengths);
        return horizontalSum(spd * cmf.yBar) / horizontalSum(cmf.yBar);
    }

    LightInfo::LightInfo()
        : position(OneW<Float4>()), emittance(), glare(Two<Float4>()),
          attenuationFactors(One<Float>(), Zero<Float>(), Zero<Float>(), One<Float>()),
          kind(EmitterKind::RGB), temperatureKelvin(Zero<Float>()), intensity(One<Float>())
    {
    }

    LightInfo LightInfo::blackbody(const Float temperatureKelvin, const Float brightness) noexcept
    {
        // T <= 0 is unphysical: Planck returns Zero<Float8>(), photopic luminance is 0,
        // and brightness/0 would taint the spectrum with infinities. Caller must pass T > 0.
        assert(temperatureKelvin > Zero<Float>());
        const auto wavelengths = spectral::wavelengthsAt(spectral::LAMBDA_MIN);
        const auto luminance = photopicLuminance(spectral::blackbody(temperatureKelvin, wavelengths));
        LightInfo l;
        l.kind = EmitterKind::Blackbody;
        l.temperatureKelvin = temperatureKelvin;
        l.intensity = brightness / luminance;
        return l;
    }

    LightInfo LightInfo::d65(const Float brightness) noexcept
    {
        const auto wavelengths = spectral::wavelengthsAt(spectral::LAMBDA_MIN);
        const auto luminance = photopicLuminance(spectral::illuminantAt(spectral::Illuminant::D65, wavelengths));
        LightInfo l;
        l.kind = EmitterKind::D65;
        l.intensity = brightness / luminance;
        return l;
    }

    LightInfo LightInfo::illuminantA(const Float brightness) noexcept
    {
        const auto wavelengths = spectral::wavelengthsAt(spectral::LAMBDA_MIN);
        const auto luminance = photopicLuminance(spectral::illuminantAt(spectral::Illuminant::A, wavelengths));
        LightInfo l;
        l.kind = EmitterKind::IlluminantA;
        l.intensity = brightness / luminance;
        return l;
    }
}
