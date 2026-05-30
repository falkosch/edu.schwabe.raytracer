#pragma once

#include "LightInfo.h"
#include "spectral/spectrum.h"

namespace raytracer
{
    using namespace vectorization;

    // Evaluates a light's spectral power distribution at the given hero-sampled wavelengths,
    // dispatching on LightInfo::kind. Returns a Spectrum scaled by LightInfo::intensity for
    // the non-RGB kinds (factories normalize so brightness=X yields Y=X).
    spectral::Spectrum sampleEmitter(const LightInfo& light, const Float8& wavelengths) noexcept;
}
