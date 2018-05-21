#include "raytracing/common/noise/SimplexNoiseGenerator.h"
#include "../../../stdafx.h"

#include "raytracing/common/Tools.h"

namespace raytracer
{

    SimplexNoiseGenerator::SimplexNoiseGenerator(const unsigned int seed)
    {
        srand(seed);
        for (ASizeT i = Zero<ASizeT>(); i < N; ++i)
            simplexGrid[i] = rand() % static_cast<Int>(B);
    }

    const Float SimplexNoiseGenerator::noise(const Float v) const
    {
        return noise3(Float3(v));
    }

    const Float SimplexNoiseGenerator::noise(const Float4 & v) const
    {
        return noise3(Float3(x(v)));
    }

    const Float SimplexNoiseGenerator::noise2(const Float2 & v) const
    {
        return noise3(convert<Float3>(v));
    }

    const Float SimplexNoiseGenerator::noise2(const Float4 & v) const
    {
        return noise3(convert<Float3>(v));
    }

    const Float SimplexNoiseGenerator::noise3(const Float3 & v) const
    {
        const Float3 ijk = floor(v + average<3>(&v.components[0]));
        const Float3 uvw = v - ijk + Half<Float>() * average<3>(&ijk.components[0]);
        const Int hi1 = select(x(uvw) >= y(uvw), Zero<Int>(), One<Int>());
        const Int hi2 = select(y(uvw) >= z(uvw), One<Int>(), Two<Int>());
        const Int hi = select(x(uvw) >= z(uvw), hi1, hi2);
        const Int lo1 = select(x(uvw) < y(uvw), Zero<Int>(), One<Int>());
        const Int lo2 = select(y(uvw) < z(uvw), One<Int>(), Two<Int>());
        const Int lo = select(x(uvw) < z(uvw), lo1, lo2);
        Float3 A = Zero<Float3>();
        return K(hi, ijk, uvw, A)
            + K(3 - hi - lo, ijk, uvw, A)
            + K(lo, ijk, uvw, A)
            + K(Zero<Int>(), ijk, uvw, A);
    }

    const Float SimplexNoiseGenerator::noise3(const Float4 &v) const
    {
        return noise3(convert<Float3>(v));
    }

    const Float SimplexNoiseGenerator::noise4(const Float4 &v) const
    {
        return noise3(convert<Float3>(v));
    }

    const Int SimplexNoiseGenerator::shuffle(const Int3 &ijk) const
    {
        const Int3 jki = Int3(y(ijk), z(ijk), x(ijk));
        const Int3 kij = Int3(z(ijk), x(ijk), y(ijk));
        return simplex(ijk, Zero<Int>())
            + simplex(jki, One<Int>())
            + simplex(kij, Two<Int>())
            + simplex(ijk, 3)
            + simplex(jki, 4)
            + simplex(kij, 5)
            + simplex(ijk, 6)
            + simplex(jki, 7);
    }

    const Int SimplexNoiseGenerator::simplex(const Int3 &ijk, const Int b) const
    {
        const Int3 skew = ((ijk >> Int3(b)) & One<Int3>()) << Int3(Two<Int>(), One<Int>(), Zero<Int>());
        return simplexGrid[x(skew) | y(skew) | z(skew)];
    }

    const Float SimplexNoiseGenerator::K(const Int a, const Float3 &ijk, const Float3 &uvw, Float3 &A) const
    {
        const Float3 xyz = uvw - A + Half<Float>() * average<3>(&A.components[0]);
        ++(A[a]);

        Float t = Half<Float>() - dot(xyz, xyz);
        if (t < Zero<Float>())
            return Zero<Float>();

        const Int h = shuffle(convert<Int3>(ijk + A));
        const Int b5 = (h >> 5) & One<Int>();
        const Int b4 = (h >> 4) & One<Int>();
        const Int b3 = (h >> 3) & One<Int>();
        const Int b2 = h & 4;
        const Int b1 = h & 3;

        const bool b1eq0 = b1 == Zero<Int>();
        const bool b1eq1 = b1 == One<Int>();
        const bool b1eq2 = b1 == Two<Int>();
        Float p = select(b1eq1, x(xyz), select(b1eq2, y(xyz), z(xyz)));
        Float q = select(b1eq1, y(xyz), select(b1eq2, z(xyz), x(xyz)));
        Float r = select(b1eq1, z(xyz), select(b1eq2, x(xyz), y(xyz)));
        p = select(b5 == b3, -p, p);
        q = select(b5 == b4, -q, q);
        r = select(b5 != (b4 ^ b3), -r, r);
        t *= t;
        return static_cast<Float>(8) * t * t * (p + select(b1eq0, q + r, select(b2 == Zero<Int>(), q, r)));
    }

}
