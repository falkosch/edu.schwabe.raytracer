#include "raytracing/shading/brdf/SpecularFresnel.h"

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
        return {weight, f0, One<Float4>(), false};
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
