#pragma once

#include "../common/RGBS.h"
#include "../geometry/ObjectGeometry.h"

namespace raytracer
{
    using namespace vectorization;

    // Spectral power distribution model for a light source.
    // RGB: upsample emittance via Smits 1999 (current default).
    // Blackbody: Planck radiation at temperatureKelvin, scaled by intensity.
    // D65: CIE D65 daylight illuminant SPD, scaled by intensity.
    // IlluminantA: CIE A tungsten illuminant SPD, scaled by intensity.
    enum class EmitterKind
    {
        RGB,
        Blackbody,
        D65,
        IlluminantA
    };

    // Lighting info for emission.
    struct LightInfo
    {
        Float4 position;
        RGBS emittance; // used when kind == RGB; ignored otherwise
        RGBS glare;

        // describes the fall-off of the lighting intensity over distance: x
        // is the constant, y is the linear, and z is the quadratic factor in
        // [(L/|L|) dot N] / (x + y * |L| + z * |L| * |L|), where L is the
        // vector between the light-position and the facet's
        // intersection-point.
        Float4 attenuationFactors;

        EmitterKind kind;
        Float temperatureKelvin; // used when kind == Blackbody
        Float intensity; // raw multiplier on the SPD (pre-normalized by factories)

        LightInfo();

        // `brightness` is calibrated against the same Y convention as spectrumToXYZ:
        // brightness=X produces a spectrum whose Y component is X, equivalent in
        // perceived luminance to an RGB(X, X, X) light under the same upsampling.
        // Two lights with the same brightness look equally bright regardless of color.
        static LightInfo blackbody(Float temperatureKelvin, Float brightness) noexcept;
        static LightInfo d65(Float brightness) noexcept;
        static LightInfo illuminantA(Float brightness) noexcept;
    };
}
