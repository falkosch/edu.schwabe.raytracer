#include "raytracing/shading/brdf/SpecularFresnel.h"

#include "raytracing/shading/brdf/SpecularAlbedo.h"
#include "raytracing/shading/brdf/fresnel_reflectance.h"
#include "raytracing/shading/brdf/ggx.h"
#include "raytracing/shading/spectral/spectrum.h"

namespace raytracer::brdf
{
    using vectorization::max;
    using vectorization::min;

    SpecularFresnel::SpecularFresnel() noexcept
        : SpecularFresnel(Zero<spectral::SpectralVector>(), Zero<spectral::SpectralVector>(), One<Float4>(), false)
    {
    }

    SpecularFresnel::SpecularFresnel(
        const spectral::SpectralVector& weight, const spectral::SpectralVector& f0, const Float4& eta,
        const bool dielectric
    ) noexcept
        : weight(min(max(weight, Zero<spectral::SpectralVector>()), One<spectral::SpectralVector>())), f0(f0),
          eta(eta), dielectric(dielectric)
    {
    }

    SpecularFresnel SpecularFresnel::opaque(
        const spectral::SpectralVector& weight, const spectral::SpectralVector& f0
    ) noexcept
    {
        // an F0 of 0 is no specular lobe: Schlick's (1 - cos)^5 would still reflect at grazing angles
        const auto reflecting = spectral::spectralMax(f0) > Zero<Float>();
        return {reflecting ? weight : Zero<spectral::SpectralVector>(), f0, One<Float4>(), false};
    }

    SpecularFresnel SpecularFresnel::dielectricInterface(
        const spectral::SpectralVector& weight, const Float4& eta
    ) noexcept
    {
        return {weight, Zero<spectral::SpectralVector>(), eta, true};
    }

    spectral::SpectralVector SpecularFresnel::reflectance(const Float4& cosine) const noexcept
    {
        const auto clamped = min(max(cosine, Zero<Float4>()), One<Float4>());
        if (dielectric)
        {
            return weight * spectral::SpectralVector(fresnelReflectance(clamped, eta));
        }
        return weight * schlickFresnelSpectral(f0, spectral::SpectralVector(clamped));
    }

    spectral::SpectralVector SpecularFresnel::directionalAlbedo(
        const Float cosine, const Float roughness
    ) const noexcept
    {
        const auto& tables = SpecularAlbedo::tables();
        if (dielectric)
        {
            // E_R / (E_R + E_T)
            const auto lobes = tables.dielectric(cosine, roughness, eta);
            const auto sum = xxxx(lobes) + yyyy(lobes);
            return weight * spectral::SpectralVector(select(sum > Zero<Float4>(), xxxx(lobes) / sum, Zero<Float4>()));
        }
        // (F0 E_1 + (1 - F0) E_5) (1 + F0 (1 - E_1) / E_1)
        const auto schlick = tables.schlick(cosine, roughness);
        const auto schlickTerm = spectral::SpectralVector(yyyy(schlick));
        const auto singleScattering =
            multiplyAdd(f0, spectral::SpectralVector(xxxx(schlick)) - schlickTerm, schlickTerm);
        return weight * singleScattering * multipleScattering(cosine, roughness);
    }

    spectral::SpectralVector SpecularFresnel::multipleScattering(
        const Float cosine, const Float roughness
    ) const noexcept
    {
        const auto& tables = SpecularAlbedo::tables();
        if (dielectric)
        {
            // 1 / (E_R + E_T)
            const auto lobes = tables.dielectric(cosine, roughness, eta);
            const auto sum = xxxx(lobes) + yyyy(lobes);
            return spectral::SpectralVector(select(sum > Zero<Float4>(), One<Float4>() / sum, One<Float4>()));
        }
        // 1 + F0 (1 - E_1) / E_1
        const auto E1 = xxxx(tables.schlick(cosine, roughness));
        const auto lost = select(E1 > Zero<Float4>(), (One<Float4>() - E1) / E1, Zero<Float4>());
        return multiplyAdd(f0, spectral::SpectralVector(lost), One<spectral::SpectralVector>());
    }

    spectral::SpectralVector SpecularFresnel::averageAlbedo(const Float roughness) const noexcept
    {
        const auto& tables = SpecularAlbedo::tables();
        if (dielectric)
        {
            return weight * spectral::SpectralVector(xxxx(tables.averageDielectric(roughness, eta)));
        }
        // the means of F0 E_1 + (1 - F0) E_5 + F0^2 (1 - E_1) + F0 (1 - F0) E_5 (1 - E_1) / E_1
        const auto averages = tables.averageSchlick(roughness);
        const auto E1 = spectral::SpectralVector(xxxx(averages));
        const auto E5 = spectral::SpectralVector(yyyy(averages));
        const auto compensatedTerm = spectral::SpectralVector(zzzz(averages));
        const auto oneMinusF0 = One<spectral::SpectralVector>() - f0;
        const auto linear = multiplyAdd(f0, E1, oneMinusF0 * E5);
        const auto quadratic = f0 * multiplyAdd(f0, One<spectral::SpectralVector>() - E1, oneMinusF0 * compensatedTerm);
        return weight * (linear + quadratic);
    }

    bool SpecularFresnel::reflects() const noexcept
    {
        if (!(spectral::spectralMax(weight) > Zero<Float>()))
        {
            return false;
        }
        return dielectric ? z(eta) != w(eta) : spectral::spectralMax(f0) > Zero<Float>();
    }

    bool SpecularFresnel::isDielectric() const noexcept
    {
        return dielectric;
    }
}
