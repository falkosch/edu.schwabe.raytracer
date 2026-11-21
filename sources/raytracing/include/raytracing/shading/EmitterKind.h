#pragma once

namespace raytracer
{
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
}
