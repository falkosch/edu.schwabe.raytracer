#include "raytracing/shading/SpectralContext.h"
#include "../../stdafx.h"

#include "raytracing/shading/EmitterSPD.h"
#include "raytracing/shading/LightInfo.h"
#include "raytracing/shading/spectral/wavelengths.h"

namespace raytracer
{
    SpectralContext::SpectralContext(const Float heroLambdaIn)
        : heroLambda(heroLambdaIn), wavelengths(spectral::wavelengthsAt(heroLambdaIn)),
          smitsBasis(spectral::smitsBasisAt(wavelengths)), xyz(spectral::makeXYZContext(heroLambdaIn)),
          ambient(spectral::Spectrum::zero()), lightEmittance()
    {
    }

    SpectralContext::SpectralContext(
        const Float heroLambdaIn, const RGBS& ambientLight, const std::vector<std::unique_ptr<LightInfo>>& lights
    )
        : SpectralContext(heroLambdaIn)
    {
        ambient = fromRGB(ambientLight.value);
        lightEmittance.reserve(lights.size());
        for (const auto& light : lights)
        {
            lightEmittance.push_back(sampleEmitter(*light, wavelengths));
        }
    }

    spectral::Spectrum SpectralContext::fromRGB(const Float4& rgb) const noexcept
    {
        return spectral::Spectrum{spectral::rgbToSpectrum(rgb, smitsBasis)};
    }

    spectral::Spectrum SpectralContext::reflectanceFromRGB(const Float4& rgb) const noexcept
    {
        return spectral::Spectrum{spectral::rgbToReflectance(rgb, smitsBasis)};
    }
}
