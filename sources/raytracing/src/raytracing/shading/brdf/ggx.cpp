#include "raytracing/shading/brdf/ggx.h"

namespace raytracer::brdf
{
    using vectorization::abs;
    using vectorization::max;
    using vectorization::sqrt;

    Float ggxD(const Float NoH, const Float alpha) noexcept
    {
        const auto a2 = sqr(alpha);
        const auto denominator = multiplyAdd(sqr(NoH), a2 - One<Float>(), One<Float>());
        return divide(a2, Pi<Float>() * sqr(denominator));
    }

    Float ggxG_smith(const Float NoV, const Float NoL, const Float alpha) noexcept
    {
        const auto a2 = sqr(alpha);
        const auto oneMinusA2 = One<Float>() - a2;
        const auto ggxV = NoL * sqrt(multiplyAdd(sqr(NoV), oneMinusA2, a2));
        const auto ggxL = NoV * sqrt(multiplyAdd(sqr(NoL), oneMinusA2, a2));
        const auto denominator = ggxV + ggxL;
        if (denominator < Epsilon<Float>())
            return Zero<Float>();
        return divide(Two<Float>() * NoV * NoL, denominator);
    }

    Float ggxV_smith(const Float NoV, const Float NoL, const Float alpha) noexcept
    {
        const auto a2 = sqr(alpha);
        const auto oneMinusA2 = One<Float>() - a2;
        const auto ggxV = NoL * sqrt(multiplyAdd(sqr(NoV), oneMinusA2, a2));
        const auto ggxL = NoV * sqrt(multiplyAdd(sqr(NoL), oneMinusA2, a2));
        return divide(Half<Float>(), ggxV + ggxL);
    }

    static Float4 schlickFresnel(const Float4& F0, const Float4& cosTheta) noexcept
    {
        const auto oneMinusCos = One<Float4>() - cosTheta;
        const auto pow5 = sqr(sqr(oneMinusCos)) * oneMinusCos;
        return multiplyAdd(One<Float4>() - F0, pow5, F0);
    }

    Float4 evaluateGGX(
        const Float4& N, const Float4& V, const Float4& L,
        const Float4& F0, const Float alpha
    ) noexcept
    {
        const auto NoV = max(dot3(N, V), Zero<Float>());
        const auto NoL = max(dot3(N, L), Zero<Float>());
        if (NoV < Epsilon<Float>() || NoL < Epsilon<Float>())
            return Zero<Float4>();

        const auto H = normalize3(V + L);
        const auto NoH = max(dot3(N, H), Zero<Float>());
        const auto F = schlickFresnel(F0, max(dot3v(V, H), Zero<Float4>()));

        return F * (ggxD(NoH, alpha) * ggxV_smith(NoV, NoL, alpha));
    }

    Float evaluateGGX_DG(const Float NoV, const Float NoL, const Float NoH, const Float alpha) noexcept
    {
        if (NoV < Epsilon<Float>() || NoL < Epsilon<Float>())
            return Zero<Float>();

        return ggxD(NoH, alpha) * ggxV_smith(NoV, NoL, alpha);
    }

    spectral::SpectralVector schlickFresnelSpectral(const spectral::SpectralVector& F0,
                                                    const spectral::SpectralVector& cosTheta) noexcept
    {
        const auto oneMinusCos = One<spectral::SpectralVector>() - cosTheta;
        const auto pow5 = sqr(sqr(oneMinusCos)) * oneMinusCos;
        return multiplyAdd(One<spectral::SpectralVector>() - F0, pow5, F0);
    }

    Float4 sampleGGX(const Float4& N, const Float4& V, const Float4& xi, const Float alpha) noexcept
    {
        return reflect(V * -One<Float>(), sampleGGXNormal(N, V, xi, alpha));
    }

    Float4 sampleGGXNormal(const Float4& N, const Float4& V, const Float4& xi, const Float alpha) noexcept
    {
        // the tangent frame around N as the rows of a matrix: frame * v takes v into it (dot products with the rows),
        // v * frame back out (the rows weighed by the components)
        const auto upCandidate = select(abs(yyyy(N)) < Float4(0.999f), OneY<Float4>(), OneX<Float4>());
        const auto T = normalize3(cross3(upCandidate, N));
        const auto frame = MFloat4x4(T, cross3(N, T), N, Zero<Float4>());

        // the view direction in the frame, stretched by alpha in the tangent plane: (alpha, alpha, 1, 0)
        const auto alphaStretch = xy_zw(Float4(alpha), OneZ<Float4>());
        const auto Vh = normalize3((frame * V) * alphaStretch);

        // the frame around Vh: T1 = (-y, x, 0, 0) / length in the tangent plane, or x where Vh is the normal
        const auto VhSquared = sqr(Vh);
        const auto lengthSquared = xxxx(VhSquared) + yyyy(VhSquared);
        const auto perpendicular = yxwz(Vh) * (OneY<Float4>() - OneX<Float4>());
        const auto T1 = select(lengthSquared > Epsilon<Float4>(), perpendicular / sqrt(lengthSquared), OneX<Float4>());
        const auto hemisphereFrame = MFloat4x4(T1, cross3(Vh, T1), Vh, Zero<Float4>());

        // a point of the unit disk (r cos phi, r sin phi), its y warped towards the hemisphere's projection
        Float4 sinPhi, cosPhi;
        sincos(yyyy(xi) * (Two<Float4>() * Pi<Float4>()), sinPhi, cosPhi);
        const auto disk = sqrt(xxxx(xi)) * blend<false, true, false, false>(cosPhi, sinPhi);
        const auto s = multiplyAdd(zzzz(Vh), Half<Float4>(), Half<Float4>());
        const auto lower = sqrt(max(Zero<Float4>(), xxxx(negativeMultiplyAdd(disk, disk, One<Float4>()))));
        const auto warped = blend<false, true, false, false>(
            disk, multiplyAdd(s, disk, negativeMultiplyAdd(s, lower, lower)));

        // lifted onto the hemisphere: (t1, t2, sqrt(1 - t1^2 - t2^2), 0) in the frame around Vh
        const auto warpedSquared = sqr(warped);
        const auto lift = sqrt(max(Zero<Float4>(), One<Float4>() - (xxxx(warpedSquared) + yyyy(warpedSquared))));
        const auto Nh = blend<false, false, true, true>(warped, zeroW(lift)) * hemisphereFrame;

        // unstretched, clamped to the upper half, back out of the tangent frame (orthonormal: the length stays 1)
        const auto unstretched = Nh * alphaStretch;
        const auto Ht = normalize3(blend<false, false, true, false>(unstretched, max(unstretched, Zero<Float4>())));
        return Ht * frame;
    }

    Float smithG1(const Float cosine, const Float alpha) noexcept
    {
        const auto a2 = sqr(alpha);
        const auto root = sqrt(multiplyAdd(One<Float>() - a2, sqr(cosine), a2));
        return Two<Float>() * cosine / (cosine + root);
    }

    Float pdfGGX(const Float NoV, const Float NoH, const Float alpha) noexcept
    {
        return smithG1(NoV, alpha) * ggxD(NoH, alpha) / (Two<Float>() * Two<Float>() * NoV);
    }
}
