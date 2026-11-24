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
        // the same center and radius, marked by ObjectShader::setLightEmitter with this light: mirrors reflect that
        // sphere, rough surfaces get the light's highlight from the shadow rays and from the glossy reflection rays
        // that find the sphere, weighed against each other (multiple importance sampling; without shadow rays, from
        // the center with the lobe widened by the light's size, and the glossy rays skip the sphere).
        Float radius;

        // Zero for a point or sphere light. Otherwise a box light: half the size of a box around the position along
        // its axes, the columns of orientation. Its emitting faces (emittingFaces) emit the light's spectrum as their
        // radiance (per area and solid angle, not an intensity), and attenuationFactors don't apply: the irradiance is
        // the faces' form factor, their shadows are soft (up to sphereLightSamples shadow rays). A zero component
        // gives a rectangle, emitting from its two sides. Show it by an emissive box of the same placement, marked by
        // ObjectShader::setLightEmitter, as with sphere lights.
        Float4 halfExtents;
        MFloat4x4 orientation;

        // The faces of a box light that emit, faceBit(axis, positive) for each: the face at +halfExtents[axis] (or
        // at -halfExtents[axis]) along the axis-th column of orientation.
        UInt_32 emittingFaces;

        EmitterKind kind;
        Float temperatureKelvin; // used when kind == Blackbody
        Float intensity; // raw multiplier on the SPD (pre-normalized by factories)

        static constexpr UInt_32 AllFaces = 0x3fu;

        // The bit of a box light's face in emittingFaces: axis 0, 1, 2 of the box, the positive or negative side
        static constexpr UInt_32 faceBit(const ASizeT axis, const bool positive) noexcept
        {
            return 1u << (2u * static_cast<UInt_32>(axis) + (positive ? 1u : 0u));
        }

        LightInfo();

        // A box light (halfExtents), not a point or sphere light
        bool isBox() const noexcept;

        // `brightness` is calibrated against the same Y convention as spectrumToXYZ:
        // brightness=X produces a spectrum whose Y component is X, equivalent in
        // perceived luminance to an RGB(X, X, X) light under the same upsampling.
        // Two lights with the same brightness look equally bright regardless of color.
        static LightInfo blackbody(Float temperatureKelvin, Float brightness) noexcept;
        static LightInfo d65(Float brightness) noexcept;
        static LightInfo illuminantA(Float brightness) noexcept;
    };
}
