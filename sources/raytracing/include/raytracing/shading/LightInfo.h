#pragma once

#include "EmitterKind.h"

#include "../common/RGBS.h"
#include "../geometry/ObjectGeometry.h"

namespace raytracer
{
    using namespace vectorization;

    // Lighting info for emission.
    struct LightInfo
    {
        Float4 position;
        RGBS emittance; // used when kind == RGB; ignored otherwise

        // describes the fall-off of the lighting intensity over distance: x
        // is the constant, y is the linear, and z is the quadratic factor in
        // [(L/|L|) dot N] / (x + y * |L| + z * |L| * |L|), where L is the
        // vector between the light-position and the facet's
        // intersection-point. w is unused.
        Float4 attenuationFactors;

        // 0 for a point light. Otherwise a spherical area light of this radius around the position: shadow rays to
        // points spread over its cap visible from the intersection give soft shadows. Show it by an emissive sphere of
        // the same center and radius, marked by ObjectShader::setLightEmitter: mirrors reflect that sphere, rough
        // surfaces get the light's highlight (widened by the light's size), and their glossy reflections skip the
        // sphere.
        Float radius;

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
