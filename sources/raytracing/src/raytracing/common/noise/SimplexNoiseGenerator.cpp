#include "raytracing/common/noise/SimplexNoiseGenerator.h"
#include "../../../stdafx.h"

#include <vectorization/functions/staticAverage.h>

namespace raytracer {
  SimplexNoiseGenerator::SimplexNoiseGenerator(const unsigned int seed) : simplexGrid() {
    srand(seed);
    for (ASizeT i{0}; i < N; ++i) {
      simplexGrid[i] = rand() % static_cast<Int>(B);
    }
  }

  const Float SimplexNoiseGenerator::noise(const Float v) const {
    return noise3(Float4(v));
  }

  const Float SimplexNoiseGenerator::noise(const Float4 &v) const {
    return noise3(v);
  }

  const Float SimplexNoiseGenerator::noise2(const Float4 &v) const {
    return noise3(v);
  }

  const Float SimplexNoiseGenerator::noise3(const Float4 &v) const {
    auto ijk = floor(v + staticAverage<3, Float4::ValueType>(&v[0]));
    auto uvw = v - ijk + Half<Float>() * staticAverage<3>(&ijk[0]);
    auto hi1 = select(x(uvw) >= y(uvw), Zero<Int>(), One<Int>());
    auto hi2 = select(y(uvw) >= z(uvw), One<Int>(), Two<Int>());
    auto hi = select(x(uvw) >= z(uvw), hi1, hi2);
    auto lo1 = select(x(uvw) < y(uvw), Zero<Int>(), One<Int>());
    auto lo2 = select(y(uvw) < z(uvw), One<Int>(), Two<Int>());
    auto lo = select(x(uvw) < z(uvw), lo1, lo2);
    auto A = Zero<Float3>();
    return K(hi, ijk, uvw, A) + K(3 - hi - lo, ijk, uvw, A) + K(lo, ijk, uvw, A) + K(Zero<Int>(), ijk, uvw, A);
  }

  const Float SimplexNoiseGenerator::noise4(const Float4 &v) const {
    return noise3(v);
  }

  const Int SimplexNoiseGenerator::shuffle(const Int3 &ijk) const {
    auto jki = Int3(y(ijk), z(ijk), x(ijk));
    auto kij = Int3(z(ijk), x(ijk), y(ijk));
    return simplex(ijk, Zero<Int>()) + simplex(jki, One<Int>()) + simplex(kij, Two<Int>()) + simplex(ijk, 3)
           + simplex(jki, 4) + simplex(kij, 5) + simplex(ijk, 6) + simplex(jki, 7);
  }

  const Int SimplexNoiseGenerator::simplex(const Int3 &ijk, const Int b) const {
    auto skew = ((ijk >> Int3(b)) & One<Int3>()) << Int3(Two<Int>(), One<Int>(), Zero<Int>());
    return simplexGrid[x(skew) | y(skew) | z(skew)];
  }

  const Float SimplexNoiseGenerator::K(const Int a, const Float4 &ijk, const Float4 &uvw, Float4 &A) const {
    auto xyz = uvw - A + Half<Float>() * staticAverage<3, Float4::ValueType>(&A[0]);
    A = replaceComponent(A, A + One<Float4>(), a);

    auto t = Half<Float>() - dot(xyz, xyz);
    if (t < Zero<Float>()) {
      return Zero<Float>();
    }

    auto h = shuffle(convert<Int3>(ijk + A));
    auto b5 = (h >> 5) & One<Int>();
    auto b4 = (h >> 4) & One<Int>();
    auto b3 = (h >> 3) & One<Int>();
    auto b2 = h & 4;
    auto b1 = h & 3;

    auto b1eq0 = b1 == Zero<Int>();
    auto b1eq1 = b1 == One<Int>();
    auto b1eq2 = b1 == Two<Int>();
    auto p = select(b1eq1, x(xyz), select(b1eq2, y(xyz), z(xyz)));
    auto q = select(b1eq1, y(xyz), select(b1eq2, z(xyz), x(xyz)));
    auto r = select(b1eq1, z(xyz), select(b1eq2, x(xyz), y(xyz)));
    p = select(b5 == b3, -p, p);
    q = select(b5 == b4, -q, q);
    r = select(b5 != (b4 ^ b3), -r, r);
    t *= t;
    return static_cast<Float>(8) * t * t * (p + select(b1eq0, q + r, select(b2 == Zero<Int>(), q, r)));
  }
}
