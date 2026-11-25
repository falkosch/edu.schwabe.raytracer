#pragma once

#include "EmitterKind.h"

#include "../common/RGBS.h"
#include "../geometry/ObjectGeometry.h"

namespace raytracer
{
    using namespace vectorization;

    // Lighting info for emission. A point light (the default) at the position emits its emittance as an intensity: a
    // surface at the distance d gets the irradiance emittance * cos / d^2 (inverse square).
    struct LightInfo
    {
        Float4 position;
        RGBS emittance; // used when kind == RGB; ignored otherwise

        // 0 for a point light. Otherwise a spherical area light of this radius around the position: shadow rays to
        // points spread over its cap visible from the intersection give soft shadows. Show it by an emissive sphere of
        // the same center and radius, marked by ObjectShader::setLightEmitter with this light: mirrors reflect that
        // sphere, rough surfaces get the light's highlight from the shadow rays and from the glossy reflection rays
        // that find the sphere, weighed against each other (multiple importance sampling; without shadow rays, from
        // the center with the lobe widened by the light's size, and the glossy rays skip the sphere).
        Float radius;

        // Zero for a point or sphere light. Otherwise a box light: half the size of a box around the position along its
        // axes, the columns of orientation. Its emitting faces (emittingFaces) emit the light's spectrum as their
        // radiance (per area and solid angle, not an intensity): the irradiance is the faces' form factor, their
        // shadows are soft (up to sphereLightSamples shadow rays). A zero component gives a rectangle, emitting from
        // its two sides. Show it by an emissive box of the same placement, marked by ObjectShader::setLightEmitter, as
        // with sphere lights.
        Float4 halfExtents;
        MFloat4x4 orientation;

        // The faces of a box light that emit, faceBit(axis, positive) for each: the face at +halfExtents[axis] (or
        // at -halfExtents[axis]) along the axis-th column of orientation.
        UInt_32 emittingFaces;

        // Zero for a point, sphere or box light. Otherwise a directional light, a distant one like the sun: the unit
        // direction towards it, the same from every point (the position is unused). Its emittance is its irradiance on
        // a surface facing it, without falloff. Its angular radius (in radians; 0 for a delta light) gives soft shadows
        // (up to sphereLightSamples shadow rays into its cone) and widens its highlight (Karis 2013). It has no emitter
        // that mirrors could show.
        Float4 direction;
        Float angularRadius;

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

        // A directional light (direction)
        bool isDirectional() const noexcept;

        // `brightness` is calibrated against the same Y convention as spectrumToXYZ:
        // brightness=X produces a spectrum whose Y component is X, equivalent in
        // perceived luminance to an RGB(X, X, X) light (D65 of luminance X, spectral::rgbToIlluminant).
        // Two lights with the same brightness look equally bright regardless of color.
        static LightInfo blackbody(Float temperatureKelvin, Float brightness) noexcept;
        static LightInfo d65(Float brightness) noexcept;
        static LightInfo illuminantA(Float brightness) noexcept;
    };
}
