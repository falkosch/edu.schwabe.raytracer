#include "raytracing/shading/EmitterSPD.h"
#include "../../stdafx.h"

#include "raytracing/shading/spectral/blackbody.h"
#include "raytracing/shading/spectral/illuminants.h"

namespace raytracer
{
    spectral::Spectrum sampleEmitter(const LightInfo& light, const Float8& wavelengths) noexcept
    {
        switch (light.kind)
        {
        case EmitterKind::Blackbody:
            return spectral::Spectrum{spectral::blackbody(light.temperatureKelvin, wavelengths) * light.intensity};
        case EmitterKind::D65:
            return spectral::Spectrum{spectral::illuminantAt(spectral::Illuminant::D65, wavelengths) * light.intensity};
        case EmitterKind::IlluminantA:
            return spectral::Spectrum{spectral::illuminantAt(spectral::Illuminant::A, wavelengths) * light.intensity};
        case EmitterKind::RGB:
        default:
            return spectral::fromRGB(wavelengths, light.emittance.value);
        }
    }
}
