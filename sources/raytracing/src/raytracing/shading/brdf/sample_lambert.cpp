#include "raytracing/shading/brdf/sample_lambert.h"

namespace raytracer::brdf
{
    Float4 sampleLambert(const Float4& N, const Float4& xi) noexcept
    {
        // the tangent frame around N as the rows of a matrix (as in sampleGGXNormal): v * frame takes v out of it
        const auto upCandidate = select(abs(yyyy(N)) < Float4(0.999f), OneY<Float4>(), OneX<Float4>());
        const auto T = normalize3(cross3(upCandidate, N));
        const auto frame = MFloat4x4(T, cross3(N, T), N, Zero<Float4>());

        // the concentric mapping: the square [-1, 1]^2 to the unit disk, its square rings to circles. The coordinate
        // of the larger magnitude is the radius, the other one's ratio to it turns the angle within its quarter.
        const auto square = multiplyAdd(xi, Two<Float4>(), NegativeOne<Float4>());
        const auto a = xxxx(square);
        const auto b = yyyy(square);
        const auto aWider = abs(a) > abs(b);
        const auto radius = select(aWider, a, b);
        const auto quarterPi = Half<Float4>() * Half<Float4>() * Pi<Float4>();
        // the center (a = b = 0) has the radius 0 and any angle
        const auto ratio = select(aWider, b, a) / select(radius == Zero<Float4>(), One<Float4>(), radius);
        const auto halfPi = quarterPi + quarterPi;
        const auto phi = select(aWider, quarterPi * ratio, negativeMultiplyAdd(quarterPi, ratio, halfPi));
        Float4 sinPhi, cosPhi;
        sincos(phi, sinPhi, cosPhi);
        const auto disk = radius * blend<false, true, false, false>(cosPhi, sinPhi);

        // lifted onto the hemisphere: (x, y, sqrt(1 - r^2), 0) in the frame around N
        const auto lift = sqrt(max(Zero<Float4>(), One<Float4>() - sqr(radius)));
        return blend<false, false, true, true>(disk, zeroW(lift)) * frame;
    }
}
