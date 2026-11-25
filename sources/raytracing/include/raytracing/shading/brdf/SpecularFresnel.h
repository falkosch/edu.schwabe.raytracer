#pragma once

#include <vectorization.h>

#include "raytracing/shading/spectral/wavelengths.h"

namespace raytracer::brdf
{
    using namespace vectorization;

    // The Fresnel reflectance of a surface's specular lobe, one law for all it reflects (one microfacet BRDF): the
    // highlights of lights, the mirror and glossy reflections of other objects and the highlights that glossy rays find
    // at emitters. An opaque surface takes Schlick's approximation of its specular channel (F0), a transmissive surface
    // (a dielectric interface) the Fresnel reflectance of its refraction indices (fresnelReflectance). The reflectance
    // channel weighs the whole lobe, clamped to [0, 1] (as OpenPBR's specular_weight); a dielectric interface transmits
    // what its lobe doesn't reflect, except where it reflects totally (no light passes there, and the weight's share is
    // lost).
    class SpecularFresnel
    {
    public:
        // A lobe that reflects nothing
        SpecularFresnel() noexcept;

        // An opaque surface: the weight times Schlick's approximation of f0; nothing for an f0 of 0
        static SpecularFresnel opaque(
            const spectral::SpectralVector& weight, const spectral::SpectralVector& f0
        ) noexcept;

        // A dielectric interface: the weight times the Fresnel reflectance of the indices, eta laid out as
        // RefractionIndices::refractionEta(etaI, etaT) gives it, with I the medium on the viewer's side
        static SpecularFresnel dielectricInterface(const spectral::SpectralVector& weight, const Float4& eta) noexcept;

        // The reflectance of a facet (or microfacet) towards the viewer per wavelength, from the cosine between its
        // normal on the viewer's side and the direction to the viewer (or to a light), in all lanes and clamped to
        // [0, 1]: the weight where a dielectric interface reflects totally
        spectral::SpectralVector reflectance(const Float4& cosine) const noexcept;

        // The lobe's directional albedo per wavelength (SpecularAlbedo), multiple scattering compensated: what it
        // reflects of light from the direction at the cosine to the normal (or towards it, by reciprocity), for the
        // roughness (the GGX alpha)
        spectral::SpectralVector directionalAlbedo(Float cosine, Float roughness) const noexcept;

        // The compensation of the multiple scattering between the microfacets that the single-scattering GGX lobe
        // loses (Turquin 2019), a factor on the lobe for the view direction at the cosine to the normal: 1 + F0 (1 -
        // E_1) / E_1 on an opaque surface (exact for F = 1, the white furnace), 1 / (E_R + E_T) for both lobes of a
        // dielectric interface; 1 for a mirror
        spectral::SpectralVector multipleScattering(Float cosine, Float roughness) const noexcept;

        // Its cosine-weighted mean over the hemisphere: what it reflects of light from all directions alike
        spectral::SpectralVector averageAlbedo(Float roughness) const noexcept;

        // Whether the lobe reflects: a weight above 0, and an F0 above 0 (opaque) or indices that differ (dielectric)
        bool reflects() const noexcept;

        // Whether the lobe is a dielectric interface's
        bool isDielectric() const noexcept;

    private:
        spectral::SpectralVector weight;
        spectral::SpectralVector f0;
        Float4 eta;
        bool dielectric;

        SpecularFresnel(
            const spectral::SpectralVector& weight, const spectral::SpectralVector& f0, const Float4& eta,
            bool dielectric
        ) noexcept;
    };
}
