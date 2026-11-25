#pragma once

#include <vectorization.h>

#include <vector>

namespace raytracer::brdf
{
    using namespace vectorization;

    // Tables of the directional albedo of the GGX specular lobe (single scattering, height-correlated Smith masking):
    // the light a lobe of roughness alpha reflects of light from the direction at the cosine mu to the normal,
    //   E(mu) = integral of f(v, l) cos(l) dl over the hemisphere,
    // and its cosine-weighted mean over the hemisphere, E_avg = 2 integral of E(mu) mu dmu (Kelemen and Szirmay-Kalos
    // 2001, Kulla and Conty 2017). Computed once, by VNDF samples (Heitz 2018), on a grid of 32 cosines (the cells'
    // centers) by 32 roughnesses (0 to 1) and, for dielectric interfaces, 16 index ratios; looked up linearly between.
    // - Schlick's Fresnel of F0: E = F0 E_1 + (1 - F0) E_5, with E_1 the albedo of F = 1 and E_5 that of the term
    //   (1 - cos)^5. Compensated for multiple scattering (Turquin 2019, the lobe times 1 + F0 (1 - E_1) / E_1), it is
    //   F0 E_1 + (1 - F0) E_5 + F0^2 (1 - E_1) + F0 (1 - F0) E_5 (1 - E_1) / E_1.
    // - a dielectric interface (the exact Fresnel equations): the reflected albedo E_R and the transmitted one E_T,
    //   from either side (eta laid out as RefractionIndices::refractionEta gives it, I the viewer's side), for index
    //   ratios up to 3 (sqrt(R0) up to 0.5; larger ones take 3). Compensated (both lobes over E_R + E_T), they sum to
    //   1.
    class SpecularAlbedo
    {
    public:
        static constexpr ASizeT CosineSteps = 32;
        static constexpr ASizeT RoughnessSteps = 32;
        static constexpr ASizeT IndexSteps = 16;

        // The tables, computed on the first call (in parallel: call it outside of parallel regions first)
        static const SpecularAlbedo& tables();

        // Schlick's lobe at the cosine and roughness: (E_1, E_5, E_5 (1 - E_1) / E_1, 0)
        Float4 schlick(Float cosine, Float roughness) const noexcept;

        // Their means over the hemisphere
        Float4 averageSchlick(Float roughness) const noexcept;

        // A dielectric interface's lobes at the cosine and roughness, single scattering: (E_R, E_T, 0, 0)
        Float4 dielectric(Float cosine, Float roughness, const Float4& eta) const noexcept;

        // The means over the hemisphere of the compensated lobes, (E_R, E_T) / (E_R + E_T): (E_R avg, E_T avg, 0, 0)
        Float4 averageDielectric(Float roughness, const Float4& eta) const noexcept;

    private:
        // per roughness and cosine: (E_1, E_5, E_5 (1 - E_1) / E_1)
        std::vector<Float4, AlignedAllocator<Float4>> schlickTable;
        // per roughness: the means of schlickTable's
        std::vector<Float4, AlignedAllocator<Float4>> averageSchlickTable;
        // per index ratio, roughness and cosine: (E_R entering the denser medium, E_R leaving it, E_T entering, E_T
        // leaving)
        std::vector<Float4, AlignedAllocator<Float4>> dielectricTable;
        // per index ratio and roughness: the means of dielectricTable's, compensated
        std::vector<Float4, AlignedAllocator<Float4>> averageDielectricTable;

        SpecularAlbedo();
    };
}
