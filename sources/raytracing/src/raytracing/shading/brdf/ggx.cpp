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
        const auto D = ggxD(NoH, alpha);
        const auto G = ggxG_smith(NoV, NoL, alpha);
        const auto F = schlickFresnel(F0, max(dot3v(V, H), Zero<Float4>()));

        return F * divide(D * G, 4.0f * NoV * NoL);
    }

    Float evaluateGGX_DG(
        const Float4& N, const Float4& V, const Float4& L, const Float4& H, const Float alpha
    ) noexcept
    {
        const auto NoV = max(dot3(N, V), Zero<Float>());
        const auto NoL = max(dot3(N, L), Zero<Float>());
        if (NoV < Epsilon<Float>() || NoL < Epsilon<Float>())
            return Zero<Float>();

        const auto NoH = max(dot3(N, H), Zero<Float>());
        const auto D = ggxD(NoH, alpha);
        const auto G = ggxG_smith(NoV, NoL, alpha);

        return divide(D * G, 4.0f * NoV * NoL);
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
        const auto upCandidate =
            abs(y(N)) < 0.999f ? OneY<Float4>() : OneX<Float4>();
        const auto T = normalize3(cross3(upCandidate, N));
        const auto B = cross3(N, T);

        const auto Vt = Float4{dot3(V, T), dot3(V, B), dot3(V, N), Zero<Float>()};

        const auto alphaStretch = Float4{alpha, alpha, One<Float>(), Zero<Float>()};
        const auto Vh = normalize3(Vt * alphaStretch);

        const auto lensq = sqr(x(Vh)) + sqr(y(Vh));
        const auto T1 =
            lensq > Epsilon<Float>()
                ? Float4{-y(Vh), x(Vh), Zero<Float>(), Zero<Float>()} * reciprocal(sqrt(lensq))
                : Float4{One<Float>(), Zero<Float>(), Zero<Float>(), Zero<Float>()};
        const auto T2 = cross3(Vh, T1);

        const auto r = sqrt(x(xi));
        const auto phi = Two<Float>() * Pi<Float>() * y(xi);
        Float4 sinPhi, cosPhi;
        sincos(Float4{phi}, sinPhi, cosPhi);
        const auto t1 = r * x(cosPhi);
        auto t2 = r * x(sinPhi);
        const auto s = Half<Float>() * (One<Float>() + z(Vh));
        t2 = (One<Float>() - s) * sqrt(max(Zero<Float>(), One<Float>() - sqr(t1))) + s * t2;

        const auto Nh = T1 * t1 + T2 * t2 + Vh * sqrt(max(Zero<Float>(), One<Float>() - sqr(t1) - sqr(t2)));

        const auto unstretched = Nh * alphaStretch;
        const auto Ht = normalize3(Float4{
            x(unstretched), y(unstretched), max(Zero<Float>(), z(unstretched)), Zero<Float>()
        });
        const auto H = T * x(Ht) + B * y(Ht) + N * z(Ht);

        return reflect(V * -One<Float>(), normalize3(H));
    }
}
