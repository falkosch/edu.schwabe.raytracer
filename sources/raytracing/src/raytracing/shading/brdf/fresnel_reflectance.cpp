#include "raytracing/shading/brdf/fresnel_reflectance.h"

#include <cassert>

namespace raytracer::brdf
{
    using vectorization::max;
    using vectorization::sqrt;

    namespace
    {
        // Schlick's approximation; entering a less dense medium must not be a total internal reflection
        Float4 schlickFresnel(const bool enteringLessDense, const Float4& negNdotI, const Float4& eta)
        {
            // calculate cosPhiT from Snell's law (TIR is already excluded)
            Float4 cosPhi;
            if (enteringLessDense)
            {
                const auto etaItoT = xxxx(eta);
                const auto etaItoTMulNdotI = etaItoT * negNdotI;
                const auto cosSqrPhiT =
                    One<Float4>() - multiplySub(etaItoT, etaItoT, etaItoTMulNdotI * etaItoTMulNdotI);
                assert(!isNegative(cosSqrPhiT));
                cosPhi = sqrt(cosSqrPhiT);
            }
            else
            {
                cosPhi = max(Zero<Float4>(), negNdotI);
            }

            // Schlick approximation for polarized light refraction/reflection model:
            // R0 = ((eta_i - eta_t) / (eta_i + eta_t))²
            const auto refractions = addSubtract(zzzz(eta), wwww(eta));
            const auto sqrtR0 = xxxx(refractions) / yyyy(refractions);
            const auto R0 = sqrtR0 * sqrtR0;
            const auto oneSubCosPhi = One<Float4>() - cosPhi;
            const auto sqrOneSubCosPhi = oneSubCosPhi * oneSubCosPhi;
            const auto reflectance =
                multiplyAdd(One<Float4>() - R0, oneSubCosPhi * sqrOneSubCosPhi * sqrOneSubCosPhi, R0);
            return reflectance;
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
        return schlickFresnel(enteringLessDense, negNdotI, eta);
    }
}
