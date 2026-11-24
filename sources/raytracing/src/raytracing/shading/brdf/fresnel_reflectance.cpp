#include "raytracing/shading/brdf/fresnel_reflectance.h"

namespace raytracer::brdf
{
    using vectorization::max;
    using vectorization::min;
    using vectorization::sqrt;

    namespace
    {
        // ((a - b) / (a + b))^2, 0 where both are 0 (equal indices at a grazing angle, the limit of the interface that
        // reflects nothing)
        Float4 squaredRatio(const Float4& a, const Float4& b)
        {
            const auto sum = a + b;
            return select(sum > Zero<Float4>(), sqr((a - b) / sum), Zero<Float4>());
        }

        // The exact Fresnel equations for unpolarized light, the mean of the s- and p-polarized reflectances
        //   Rs = ((n1 cos_i - n2 cos_t) / (n1 cos_i + n2 cos_t))^2
        //   Rp = ((n1 cos_t - n2 cos_i) / (n1 cos_t + n2 cos_i))^2
        // divided by n1 (eta's x is n1 / n2, y is n2 / n1), cos_t from Snell's law; 1 beyond the critical angle, where
        // cos_t^2 is negative
        Float4 exactFresnel(const Float4& cosI, const Float4& eta)
        {
            const auto etaItoT = xxxx(eta);
            const auto etaTtoI = yyyy(eta);
            const auto etaItoTMulCosI = etaItoT * cosI;
            const auto cosSqrT = One<Float4>() - multiplySub(etaItoT, etaItoT, etaItoTMulCosI * etaItoTMulCosI);
            const auto cosT = sqrt(max(cosSqrT, Zero<Float4>()));
            const auto rs = squaredRatio(cosI, etaTtoI * cosT);
            const auto rp = squaredRatio(cosT, etaTtoI * cosI);
            const auto reflectance = Half<Float4>() * (rs + rp);
            return select(cosSqrT < Zero<Float4>(), One<Float4>(), reflectance);
        }
    }

    Float4 fresnelReflectance(const bool totalInternalReflection, const Float4& negNdotI, const Float4& eta)
    {
        // entering less dense material and TIR is the case
        const auto enteringLessDense = !!isNegative(wwww(eta) - zzzz(eta));
        if (totalInternalReflection & enteringLessDense)
        {
            return One<Float4>();
        }
        return exactFresnel(min(max(negNdotI, Zero<Float4>()), One<Float4>()), eta);
    }

    Float4 fresnelReflectance(const Float4& incident, const Float4& normal, const Float4& eta)
    {
        const auto NdotI = dotv(normal, incident);
        const auto refracted = refractEta(incident, normal, NdotI, xxxx(eta));
        return fresnelReflectance(allTrue3(!refracted), -NdotI, eta);
    }

    Float4 fresnelReflectance(const Float4& negNdotI, const Float4& eta)
    {
        return exactFresnel(min(max(negNdotI, Zero<Float4>()), One<Float4>()), eta);
    }
}
